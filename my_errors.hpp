#ifndef MY_ERRORS
#define MY_ERRORS

#include <string>

class M_json_read_failed
{
private:
	std::string e;
public:
	M_json_read_failed(const std::string &what) { e = what; };
	std::string& what() { return e; }
};
#endif