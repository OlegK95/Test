#ifndef GABARIT_POINT
#define GABARIT_POINT

#include <limits>
#include <algorithm>


#ifndef POINT_TYPE
    typedef double POINT_TYPE;
#endif
#ifndef POINT_TIME
	typedef  int POINT_TIME;
#endif
#ifndef POINT_NOMBER
	typedef unsigned short int POINT_NOMBER;
#endif
#ifndef POINT_ID
    typedef unsigned short int POINT_ID;
#endif

template  <typename T>
struct Gabarit
{
public:
	T min_x;
	T min_y;
	T max_x;
	T max_y;

	Gabarit()
	{
		min_x =  std::numeric_limits<T>::max();
		min_y =  std::numeric_limits<T>::max();
		max_x = -std::numeric_limits<T>::max();
		max_y = -std::numeric_limits<T>::max();
	}

	Gabarit(T const _min_x , T const _min_y ,T const _max_x , T const _max_y)
	{
		min_x = (_min_x < _max_x ? _min_x : _max_x);
		max_x = (_min_x < _max_x ? _max_x : _min_x);
		min_y = (_min_y < _max_y ? _min_y : _max_y);
		max_y = (_min_y < _max_y ? _max_y : _min_y);
	}

void Add_point(T x, T y)
	{
		min_x = (x < min_x ? x : min_x);
		max_x = (x > max_x ? x : max_x);
		min_y = (y < min_y ? y : min_y);;
		max_y = (y > max_y ? y : max_y);
	}
};

#endif