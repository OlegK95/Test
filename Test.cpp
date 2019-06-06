// Test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include "pch.h"

#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>

typedef double POINT_TYPE;
typedef int POINT_TIME;
typedef unsigned short int POINT_NOMBER;
typedef unsigned short int POINT_ID;
#define  INPUT_POLYGON_INTERSECT_CONTROL

#include "files_to_open.hpp"
#include "files_to_open.cpp"

#include "polygon_point.hpp"
#include "my_polygon.hpp"

#include "polyline_point_template.hpp"
#include "my_polyline_template.hpp"

#include "my_segment_custom.hpp"

#include "text_polyline.hpp"
#include "text_polygon.hpp"

#include "text_polyline.cpp"
#include "text_polygon.cpp"

#include "intersect_Point_template.hpp"

#include  "intersection_algoritm.hpp"

#include  "auxiliary_function.hpp"

#define INPUT_POLYGON_CONTROL
#define MIN_SIZE 25

int main()
{
	ptree _p_root;
	int i = 0;
	setlocale(LC_ALL, "Russian");
	setlocale(LC_NUMERIC, "eng");

	std::map    <std::string, My_polygon>  poligon_map;
	My_polyline <POINT_TYPE> p_line;

	std::cout.precision(16);
	try
	{
		Files_to_open read_file ("C:/Test/READ_.txt");

		std::ofstream cerrFile(read_file.get_error_log_file_name());
		std::cerr.rdbuf(cerrFile.rdbuf());

		std::cout << "Read polygon" << std::endl;

		boost::posix_time::ptime t = print_duration("Begin reading from file : ");

		Text_polygon text_polygon (read_file.get_polygon_file_name());

		t = print_duration("End reading polygons from file   : ", t);
		std::string  buff_tmp;
		while (true)
		{
			buff_tmp = text_polygon.get_text_polygon();
			if (buff_tmp.length() < MIN_SIZE) break;
			add_polygon(poligon_map, buff_tmp);
		}
		buff_tmp.empty();
		t = print_duration("End reading polygons from string : ", t);

		std::cout << "Read polyLine" << std::endl;
		Text_polyline text_polyline(read_file.get_polyline_file_name());
		buff_tmp = text_polyline.get_text_polyline();

		t = print_duration("End reading polyline from file : ", t);

		if (buff_tmp.length() > MIN_SIZE)
			p_line.read_polyline(buff_tmp);

		t = print_duration("End reading polyline from string : ", t);

		std::cout << "Find intersection" << std::endl;

		std::vector <Intersect_point <POINT_TYPE>>  intersect_points;
		Find_intersection(poligon_map, p_line, intersect_points);

		t = print_duration("End looking intersection : ", t);
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

