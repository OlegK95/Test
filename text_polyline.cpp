#include "text_polyline.hpp"

#include "my_errors.hpp"

#include <stdexcept>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>

#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/thread/thread.hpp>

//------------------

Text_polyline::Text_polyline (std::string _file_name)
{
  std::ifstream ifs(_file_name);
  if (!ifs.is_open())
 	ifs.open("C:/Test/" + _file_name);

  if (!ifs)
	throw M_json_read_failed("Error input file");

  std::string tm((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

  buff = std::move(tm);
}
//---------------
Text_polyline::~Text_polyline()
{
   buff.clear();
}

//--------------

std::string Text_polyline::get_text_polyline()
{
  buff = "{\"\":" + std::move(buff) + "}";
  std::replace(buff.begin(), buff.end(), '\'', '\"');
  return std::move(buff);
}
