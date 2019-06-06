#include "files_to_open.hpp"

#include <stdexcept>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
using boost::property_tree::ptree;

typedef boost::property_tree::ptree ptree;



Files_to_open::Files_to_open(std::string _file_name)
{
	try
	{
		std::ifstream ifs;
		ifs.open(_file_name);
		ifs.close();
		if (!ifs)
		{
			throw	"File READ.txt not found";
		}
		else
		{
 			ptree p_root;
			boost::property_tree::read_json(_file_name, p_root);

			if (!p_root.empty())
			{
				polyline_file_name  = p_root.get<std::string>("polyline_file_name");
				polygon_file_name   = p_root.get<std::string>("polygon_file_name");
				error_log_file_name = p_root.get<std::string>("error_log_file_name");
				output_file_name    = p_root.get<std::string>("output_file_name");
				p_root.clear();
			}
			else
			{
				throw	"Error reading input file READ.txt";
			}
		}
	}

	catch (std::exception const& e)
	{
		std::cerr << "In file " << _file_name << " error " << e.what() << std::endl;
	}
}



//Files_to_open::~Files_to_open(){ }



inline std::string Files_to_open::get_polyline_file_name()
{
	return polyline_file_name;
}
std::string Files_to_open::get_polygon_file_name()
{
	return polygon_file_name;
}
std::string Files_to_open::get_error_log_file_name()
{
	return error_log_file_name;
}
std::string Files_to_open::get_output_file_name()
{
	return output_file_name;
}















/*




//-------------------------

Text_polygon::Text_polygon(std::string _file_name)
{
  std::ifstream ifs(_file_name);
  if (!ifs.is_open())
	ifs.open("C:/Test/" + _file_name);

  if (!ifs)
	throw M_json_read_failed("Error input file");

  std::string tm((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  buff = std::move(tm);

  bool is_color_find  = buff.find("\"color\""); 
  bool is_coords_find = buff.find("\"coords\""); 
  bool is_name_find   = buff.find("\"name\"");

  if (!(( is_color_find != std::string::npos) && (is_coords_find != std::string::npos) && (is_name_find != std::string::npos)))
	{
	buff.clear();
	throw ("Error input file");
	}
}

//---------------

Text_polygon::~Text_polygon()
  {
  buff.clear();
  }

//--------------

std::string Text_polygon::get_text_polygon()
{
  size_t fnd = buff.find("\"name\"");
  bool is_color_find = buff.find("\"color\"");
  bool is_coords_find = buff.find("\"coords\"");

  if (!((is_color_find != std::string::npos) && (is_coords_find != std::string::npos) && (fnd != std::string::npos)))
    {
    buff.clear();
    return std::string "";
    }

  size_t fnd_2 = buff.find(',', fnd);
  if (fnd_2 != std::string::npos)
    {
	std::string buff_tmp = "";
	buff_tmp.assign(buff.begin(), buff.begin() + fnd_2);
	buff_tmp += ']';
	buff.assign(buff.begin() + fnd_2, buff.end());
	buff[0] = '[';
	return std::move(buff_tmp);
	}
  else
	return std::move(buff);
}

*/