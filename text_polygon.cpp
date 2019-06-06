#include "text_polygon.hpp"

#include "my_errors.hpp"

#include <stdexcept>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>

#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/thread/thread.hpp>

//-------------------------

Text_polygon::Text_polygon(std::string _file_name)
{

	  std::ifstream ifs;
	  ifs.open(_file_name);
	  if (!ifs)
	  {
		  throw	"File READ.txt not found";
	  }
	  else
	  {
		  std::string tm((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		  ifs.close();

		  buff = std::move(tm);

		  bool is_color_find = buff.find("\"color\"");
		  bool is_coords_find = buff.find("\"coords\"");
		  bool is_name_find = buff.find("\"name\"");

		  if (!((is_color_find != std::string::npos) && (is_coords_find != std::string::npos) && (is_name_find != std::string::npos)))
		  {
			  buff.clear();
			  throw ("Error input file");
		  }
		  correct = true;
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
    return buff;
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

