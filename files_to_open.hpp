#ifndef FILES_TO_OPEN
#define FILES_TO_OPEN

#include <string>

class Files_to_open
{
private:
//  std::string file_name;
  std::string polyline_file_name;
  std::string polygon_file_name;
  std::string error_log_file_name = "ERROR_LOG.txt";
  std::string output_file_name;



public:

  Files_to_open(std::string _file_name);
 // ~Files_to_open();
  std::string get_polyline_file_name();
  std::string get_polygon_file_name();
  std::string get_error_log_file_name();
  std::string get_output_file_name();
  std::string get_polyline();
  std::string get_polygon();
};
#endif