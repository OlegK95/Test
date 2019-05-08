#ifndef TEXT_POLYGON
#define TEXT_POLYGON

#include <stdexcept>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>

#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/thread/thread.hpp>

class M_json_read_failed
{
private:
	std::string e;
public:
	M_json_read_failed(const std::string &what) { e = what; };
	std::string& what() { return e; }
};

class Text_polygon
{
private:
	std::string buff;
public:

	Text_polygon(std::string _file_name)
	{
		std::ifstream ifs(_file_name);
		if (!ifs.is_open())
			ifs.open("C:/Test/" + _file_name);

		if (!ifs)
			throw M_json_read_failed("Error input file");

		std::string tm((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		buff = std::move(tm);

		if (!(((buff.find("\"color\"")) != std::string::npos) &&
			((buff.find("\"coords\"")) != std::string::npos) &&
			((buff.find("\"name\"")) != std::string::npos)))
		{
			buff.clear();
			throw ("Error input file");
		}
	}
	//---------------
	~Text_polygon()
	{
		buff.clear();
	}
	//--------------

	std::string get_text_polygon()
	{
		size_t fnd = buff.find("\"name\"");
		std::string buff_tmp = "";

		if (!(((buff.find("\"color\"")) != std::string::npos) &&
			((buff.find("\"coords\"")) != std::string::npos) &&
			(fnd != std::string::npos)))
		{
			buff.clear();

			return std::move(buff_tmp);
		}

		size_t fnd_2 = buff.find(',', fnd);
		if (fnd_2 != std::string::npos)
		{
			buff_tmp.assign(buff.begin(), buff.begin() + fnd_2);
			buff_tmp += ']';
			buff.assign(buff.begin() + fnd_2, buff.end());
			buff[0] = '[';
			return std::move(buff_tmp);
		}

		else
			return std::move(buff);
	}
};


class Text_polyline
{
private:
	std::string buff;
public:

	Text_polyline (std::string _file_name)
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
	~Text_polyline()
	{
		buff.clear();
	}
	//--------------

	std::string get_text_polyline()
	{
		buff = "{\"\":" + std::move(buff) + "}";
		std::replace(buff.begin(), buff.end(), '\'', '\"');
		return std::move(buff);
	}
};


boost::posix_time::ptime print_duration(std::string const& str, boost::posix_time::ptime old_time = boost::posix_time::microsec_clock::local_time())
{

	boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = t - old_time;
	std::cout << str << to_simple_string(t) << " Duration ms.: " << diff.total_milliseconds() << std::endl;
	return t;
}


#endif