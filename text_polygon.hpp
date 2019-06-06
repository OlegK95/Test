#ifndef TEXT_POLYGON
#define TEXT_POLYGON

#include <string>

class Text_polygon
{
private:
  std::string buff;
  bool correct = false;
public:

  Text_polygon(std::string _file_name);
  ~Text_polygon();
  std::string get_text_polygon();
  bool is_correct();
};
#endif