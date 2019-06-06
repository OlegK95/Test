#ifndef TEXT_POLYLINE
#define TEXT_POLYLINE

#include <string>

class Text_polyline
{
private:
  std::string buff;
public:

  Text_polyline(std::string _file_name);
  ~Text_polyline();
  std::string get_text_polyline();
};
#endif