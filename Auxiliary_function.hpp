#ifndef AUXILIARY_FUNCTION
#define AUXILIARY_FUNCTION

#include  "Polyline_Point_template.hpp"
#include  "Polygon_Point.hpp"
#include  "Intersect_Point_template.hpp"
#include  "Intersection_algoritm.hpp"
#include  "My_segment_custom.hpp"

#include <boost/assert.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/util/add_const_if_c.hpp>
#include <boost/geometry/geometries/register/segment.hpp>


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

using namespace boost::geometry;

#ifndef POINT_TYPE
typedef double POINT_TYPE;
#endif
#ifndef POINT_ID
typedef unsigned short int POINT_ID;
#endif


// Печатает на экран результат поиска пересечений полигонов и полилинии
void Print_intersect(std::vector <Intersect_point <POINT_TYPE>> const & intersect_points)
{


	boost::property_tree::ptree root;
	std::cout << std::endl;
	std::cout.precision(14);
	for (auto it = intersect_points.begin(); it != intersect_points.end(); ++it)
	{
		boost::property_tree::ptree tree;
		std::cout << "Polyline " << it->pl_nm << " segment from "
			<< it->pl_id_b << " ( " << it->pl_x_b << " , " << it->pl_y_b << " ) to "
			<< it->pl_id_e << " ( " << it->pl_x_e << " , " << it->pl_y_e << " )"
			<< " is goes " << (it->is_in == 0 ? "IN " : "OUT") << " polygon " << it->pg_nm << std::endl 
			<< " in point (" << it->x << " : " << it->y << " ) in time " << it->tm //	<< " after line " << it->pl_nm  
			<< " is intersect segment of polygon" << std::endl
			<< "from point " << it->pg_id_b << " ( " << it->pg_x_b << " , " << it->pg_y_b << " ) to "
			<< it->pg_id_e << " ( " << it->pg_x_e << " , " << it->pg_y_e << " )" << std::endl;

		tree.add("Intersect point x", it->x);
		tree.add("Intersect point y", it->y);
		tree.add("Time of intersection", it->tm);

		tree.add("Polyline name", it->pl_nm);
		tree.add("Ferst polyline point id", it->pl_id_b);
		tree.add("Ferst polyline point X", it->pl_x_b);
		tree.add("Ferst polyline point Y", it->pl_y_b);
		tree.add("Second polyline point id", it->pl_id_e);
		tree.add("Second polyline point X", it->pl_x_e);
		tree.add("Second polyline point Y", it->pl_y_e);
		tree.add("IN_OUT", it->is_in);

		tree.add("Polygon name", it->pg_nm);
		tree.add("Ferst polygon point id", it->pg_id_b);
		tree.add("Ferst polygon point X", it->pg_x_b);
		tree.add("Ferst polygon point Y", it->pg_y_b);
		tree.add("Second polygon point id", it->pg_id_e);
		tree.add("Second polygon point X", it->pg_x_e);
		tree.add("Second polygon point Y", it->pg_y_e);

		root.add_child("points.point", tree);
	}
	boost::property_tree::write_json("Intersection_points.json", root);
}

bool My_intersects(My_polygon const & ferst, My_polygon const & second);

bool add_polygon(std::map  <std::string, My_polygon> & poligon_map, std::string const& buff)
{
	My_polygon curr_polygon;
	if (curr_polygon.read_polygon(buff))
	{

#ifdef INPUT_POLYGON_INTERSECT_CONTROL
		if (poligon_map.size() > 0)
		{
			for (const auto& kv : poligon_map)
			{
				auto pg = kv.second;
				if (! out_range(pg.get_gabarit(), curr_polygon.get_gabarit()))
				{
					if (My_intersects(kv.second, curr_polygon))
					{
						std::cerr << "Polygon " << curr_polygon.get_name() << " is intersect with polygon " << kv.first << std::endl;
//						std::cout << "Polygon " << curr_polygon.get_name() << " is intersect with polygon " << kv.first << std::endl;
						return false;
					}
				}
			}
		}
#endif
		poligon_map.insert(std::pair <std::string, My_polygon>(curr_polygon.get_name(), curr_polygon));
		return true;
	}
	else
		return false;
}




// Определяет, пересекаются ли два полигона
bool My_intersects(My_polygon const & ferst, My_polygon const & second)
{
	int i = 0;
	auto end1 = boost::end(ferst);
	My_point_polygon  cur_ferst_point, old_ferst_point, cur_second_point, old_second_point;
	POINT_TYPE x, y;
	for (auto it_f = boost::begin(ferst); it_f != end1; ++it_f, i++)
	{
		cur_ferst_point = *it_f;
		if (i > 0)
		{
			int j = 0;
			auto end1 = boost::end(second);
			My_point_polygon   old_second_point;

			for (auto it_s = boost::begin(second); it_s != end1; ++it_s, j++)
			{
				cur_second_point = *it_s;
				if (j > 0)
				{
					int intersection_result = intersection(old_ferst_point.x, old_ferst_point.y, cur_ferst_point.x, cur_ferst_point.y,
						old_second_point.x, old_second_point.y, cur_second_point.x, cur_second_point.y,
						x, y, true);
					if (intersection_result == 1)
						return true;
					if (intersection_result == -1)
						return true;
					if (intersection_result == -2)
						return true;
				}
				old_second_point = *it_s;
			}
		}
		old_ferst_point = *it_f;
	}
	return false;
}



// Ищет, последовательным перебором пересечение отрезков из точек  полигона с отрезками из точек полилинии
bool Find_intersection(const std::map<std::string, My_polygon> & poligon_map, My_polyline <POINT_TYPE> & p_line, std::vector <Intersect_point <POINT_TYPE>>  &intersect_points)
{
	for (const auto& kv : poligon_map)
	{
		My_polygon *pg = (My_polygon*)&kv.second;
		if (!out_range(pg->get_gabarit(), p_line.get_gabarit(), false))
		{
			std::size_t i = 0;
			My_point_polygon old_pg_point;
			//	if (boost::geometry::intersects(pg, p_line))  Предварительный контроль пересечения полигона и полилинии средствами boost::geometry
			{
				BOOST_FOREACH(My_point_polygon const& pg_point, kv.second)
				{
					if (i > 0)
					{
						std::size_t j = 0;
						My_point_polyline<POINT_TYPE> old_pl_point, cur_pl_point;
						for (auto it = boost::begin(p_line); it != boost::end(p_line); ++it, j++)
						{
							if (j > 0)
							{
								struct Intersect_point 	<POINT_TYPE> tmp;
								cur_pl_point = *it;
								if (intersection(old_pl_point.x, old_pl_point.y, cur_pl_point.x, cur_pl_point.y,
									old_pg_point.x, old_pg_point.y, pg_point.x, pg_point.y,
									tmp.x, tmp.y) != 0)
								{
									tmp.tm = (POINT_NOMBER)(old_pl_point.tm + (it->tm - old_pl_point.tm)*((old_pg_point.x - tmp.x) / (old_pg_point.x - it->x)));
									tmp.is_in = boost::geometry::within(old_pl_point, kv.second) ? 1 : 0;

									tmp.pl_nm = p_line.get_name();
									tmp.pl_x_b = old_pl_point.x; tmp.pl_y_b = old_pl_point.y; tmp.pl_tm_b = old_pl_point.tm; tmp.pl_id_b = old_pl_point.id;
									tmp.pl_x_e = it->x;		  tmp.pl_y_e = it->y;			tmp.pl_tm_e = it->tm; 		   tmp.pl_id_e = it->id;

									tmp.pg_nm = pg->get_name();
									tmp.pg_x_b = old_pg_point.x; tmp.pg_y_b = old_pg_point.y; tmp.pg_id_b = old_pg_point.id;
									tmp.pg_x_e = pg_point.x; 	  tmp.pg_y_e = pg_point.y;     tmp.pg_id_e = pg_point.id;

									intersect_points.push_back(tmp);
								}
							}
							old_pl_point = *it;
						}
					}
					old_pg_point = pg_point;
					i++;
				}
			}
		}
	}
	return true;
}









/*

void print(std::map < std::string, My_polygon> &poligon_map)
{
	auto end = poligon_map.end();

	for (auto it = poligon_map.begin(); it != end; ++it)
	{

		std::cout << std::endl << "name = " << it->first << it->second << std::endl; ///вывод на экран
	}

}
*/





#endif