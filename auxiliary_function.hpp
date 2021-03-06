﻿#ifndef AUXILIARY_FUNCTION
#define AUXILIARY_FUNCTION

#include  "polyline_point_template.hpp"
#include  "polygon_point.hpp"
#include  "intersect_point_template.hpp"
#include  "intersection_algoritm.hpp"
#include  "my_segment_custom.hpp"
#include "my_errors.hpp"

#include <boost/assert.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/util/add_const_if_c.hpp>
#include <boost/geometry/geometries/register/segment.hpp>


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

using namespace boost::geometry;
typedef model::d2::point_xy<POINT_TYPE> point_2d;

#ifndef POINT_TYPE
typedef double POINT_TYPE;
#endif
#ifndef POINT_ID
typedef unsigned short int POINT_ID;
#endif

/*
//---------------------------------
class M_json_read_failed
{
private:
	std::string e;
public:
	M_json_read_failed(const std::string &what) { e = what; };
	std::string& what() { return e; }
};
*/
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


std::string  find_polygon_point(const std::map<std::string, My_polygon> & poligon_map, const My_point_polyline<POINT_TYPE> &cur_pl_point)
{
	Gabarit <POINT_TYPE> g2;	g2.Add_point(cur_pl_point.x, cur_pl_point.y);

	for (const auto& kv : poligon_map)
	{
		My_polygon *pg = (My_polygon*)&kv.second;
	if (!out_range(pg->get_gabarit(), g2, false))
			if (boost::geometry::within(point_2d (cur_pl_point.x, cur_pl_point.y), kv.second))
				return pg->get_name();
	}
	return "";
}


// Ищет, последовательным перебором пересечение отрезков из точек  полигона с отрезками из точек полилинии
bool Find_intersection(const std::map<std::string, My_polygon> & poligon_map, My_polyline <POINT_TYPE> & p_line, std::vector <Intersect_point <POINT_TYPE>>  &intersect_points)
{
	std::string ferst_polygon_name = "";
	std::string cur_polygon_name = "";
	for (const auto& kv : poligon_map)
	{
		My_polygon *pg = (My_polygon*)&kv.second;
		if (!out_range(pg->get_gabarit(), p_line.get_gabarit(), false))
		{
			std::size_t i = 0;
			My_point_polygon old_pg_point;
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
								int intersection_flag = intersection(old_pl_point.x, old_pl_point.y, cur_pl_point.x, cur_pl_point.y,
									                                 old_pg_point.x, old_pg_point.y, pg_point.x, pg_point.y,
									                                 tmp.x, tmp.y);
								if (intersection_flag == 1)
								{
									tmp.tm = (POINT_NOMBER)(cur_pl_point.tm - old_pl_point.tm) * boost::geometry::distance(old_pl_point, tmp) / boost::geometry::distance(old_pl_point, cur_pl_point)  + old_pl_point.tm;
								
									tmp.is_in = (boost::geometry::within(old_pl_point, kv.second) ? true : false);

									tmp.pl_nm = p_line.get_name();
									tmp.pl_x_b = old_pl_point.x; tmp.pl_y_b = old_pl_point.y; tmp.pl_tm_b = old_pl_point.tm; tmp.pl_id_b = old_pl_point.id;
									tmp.pl_x_e = cur_pl_point.x; tmp.pl_y_e = cur_pl_point.y; tmp.pl_tm_e = cur_pl_point.tm; tmp.pl_id_e = cur_pl_point.id;

									tmp.pg_nm = pg->get_name();
									tmp.pg_x_b = old_pg_point.x;  tmp.pg_y_b = old_pg_point.y; tmp.pg_id_b = old_pg_point.id;
									tmp.pg_x_e = pg_point.x; 	  tmp.pg_y_e = pg_point.y;     tmp.pg_id_e = pg_point.id;

									intersect_points.push_back(tmp);
									cur_polygon_name = find_polygon_point(poligon_map, *it);
								}
								if (intersection_flag == -1)
								{
									if (cur_polygon_name != pg->get_name())
									{
										tmp.tm = (POINT_NOMBER)(cur_pl_point.tm - old_pl_point.tm) * boost::geometry::distance(old_pl_point, tmp) / boost::geometry::distance(old_pl_point, cur_pl_point) + old_pl_point.tm;

										tmp.is_in = true;

										tmp.pl_nm = p_line.get_name();
										tmp.pl_x_b = old_pl_point.x; tmp.pl_y_b = old_pl_point.y; tmp.pl_tm_b = old_pl_point.tm; tmp.pl_id_b = old_pl_point.id;
										tmp.pl_x_e = cur_pl_point.x; tmp.pl_y_e = cur_pl_point.y; tmp.pl_tm_e = cur_pl_point.tm; tmp.pl_id_e = cur_pl_point.id;

										tmp.pg_nm = pg->get_name();
										tmp.pg_x_b = old_pg_point.x;  tmp.pg_y_b = old_pg_point.y; tmp.pg_id_b = old_pg_point.id;
										tmp.pg_x_e = pg_point.x; 	  tmp.pg_y_e = pg_point.y;     tmp.pg_id_e = pg_point.id;

										intersect_points.push_back(tmp);
										cur_polygon_name = find_polygon_point(poligon_map, *it);
									}
								}
								if (intersection_flag == -2)
								{

								}
							}
							else
							{
								std::string ferst_polygon_name = find_polygon_point(poligon_map, *it);
								cur_polygon_name = ferst_polygon_name;
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

//----------------------------------------------------------

boost::posix_time::ptime print_duration(std::string const& str, boost::posix_time::ptime old_time = boost::posix_time::microsec_clock::local_time())
{

	boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = t - old_time;
	std::cout << str << to_simple_string(t) << " Duration ms.: " << diff.total_milliseconds() << std::endl;
	return t;
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