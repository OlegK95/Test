// Test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include "pch.h"

#include <stdexcept>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <cmath>
#include <vector>


#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/register/linestring.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/adapted/c_array.hpp>
#include <boost/geometry/algorithms/within.hpp>

BOOST_GEOMETRY_REGISTER_C_ARRAY_CS(cs::cartesian)

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/exception/all.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

typedef double POINT_TYPE;
typedef int POINT_TIME;
typedef unsigned short int POINT_NOMBER;
typedef unsigned short int POINT_ID;
#define  INPUT_POLYGON_INTERSECT_CONTROL

#include "Polygon_Point.hpp"
#include "My_polygon.hpp"

#include "Polyline_Point_template.hpp"
#include "My_Polyline_template.hpp"

#include "My_Segment_custom.hpp"

#include "Text_polygon_polyline.hpp"

#include "Intersect_Point_template.hpp"

#include  "Intersection_algoritm.hpp"

#include  "Auxiliary_function.hpp"


using namespace boost::property_tree;
using namespace boost::geometry;
using boost::property_tree::ptree;

typedef boost::property_tree::ptree ptree;
typedef double  Point_tuype;

typedef model::d2::point_xy<POINT_TYPE> point_2d;
typedef model::segment<point_2d> segment_2d;
typedef model::linestring<point_2d> linestring_2d;
typedef model::polygon<point_2d> polygon_2d;
typedef model::box<point_2d> box_2d;


typedef  unsigned short int POINT_ID;


#define INPUT_POLYGON_CONTROL
#define MIN_SIZE 25







int main()
{
	ptree _p_root;
	int i = 0;
	setlocale(LC_ALL, "Russian");
	setlocale(LC_NUMERIC, "eng");


	char  name_str[] = "\"name\"";
	char  color_str[] = "\"color\"";

	std::map    <std::string, My_polygon>  poligon_map;
	std::vector <Intersect_point <POINT_TYPE>>  intersect_points;
	My_polyline <POINT_TYPE> p_line;
	My_polyline <POINT_TYPE> Stop_point_line;

	std::cout.precision(14);
	try
	{
		std::string polygon_file_name, polyline_file_name;

		std::ofstream cerrFile("CERR_LOG.txt");
		std::cerr.rdbuf(cerrFile.rdbuf());

		std::ifstream ifs;

		ifs.open("READ.txt");
		if (!ifs.is_open())
		      ifs.open("C:/Test/READ.txt");


		if (!ifs.is_open())
			throw "Error opening input file READ.txt";
		else
		{
			getline(ifs, polygon_file_name);
			getline(ifs, polyline_file_name);
			ifs.close();
		}

		std::cout.precision(16);
		std::cout << "Read polygon" << std::endl;

		boost::posix_time::ptime t1 = print_duration("Begin reading from file : ");

		Text_polygon text_polygon(polygon_file_name);

		boost::posix_time::ptime t2 = print_duration("End reading polygons from file   : ", t1);
		std::string  buff_tmp;
		while (true)
		{
			buff_tmp = text_polygon.get_text_polygon();
			if (buff_tmp.length() < MIN_SIZE) break;
			///----------------------------------------
			//    На этой стадии планирую запускать процесс чтения асинхронно
			///----------------------------------------
			add_polygon(poligon_map, buff_tmp);
		}
		buff_tmp.empty();
		boost::posix_time::ptime t3 = print_duration("End reading polygons from string : ", t2);

		std::cout << "Read polyLine" << std::endl;
		Text_polyline text_polyline(polyline_file_name);
		buff_tmp = text_polyline.get_text_polyline();

		boost::posix_time::ptime t4 = print_duration("End reading polyline from string : ", t3);

		if (buff_tmp.length() > MIN_SIZE)
			p_line.read_polyline(buff_tmp);

		boost::posix_time::ptime t5 = print_duration("End reading polyline from string : ", t4);

		//-------------------------------------

		std::cout << "Find intersection" << std::endl;

		Find_intersection(poligon_map, p_line, intersect_points);

		boost::posix_time::ptime t6 = print_duration("End looking intersection : ", t5);
		std::cout << "Read complite. Press Enter to print";
		std::cin.get();

	    Print_intersect(intersect_points);

		cerrFile.close();
		std::cout << "Press Enter to end";
		std::cin.get();
	}
	catch (M_json_read_failed e)
	{
		std::cerr << e.what() << std::endl;
	}

	catch (const char* e)
	{
		std::cerr << e << std::endl;
		std::cout << e << std::endl;
	}
	catch (std::exception const& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return EXIT_FAILURE;

}

