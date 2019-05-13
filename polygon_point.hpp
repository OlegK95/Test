#ifndef POLYGON_POINT
#define POLYGON_POINT

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/algorithms/make.hpp>
#include <boost/geometry/geometries/register/point.hpp>

using namespace boost::geometry;

#ifndef POINT_TYPE
    typedef double POINT_TYPE;
#endif
#ifndef POINT_ID
    typedef unsigned short int POINT_ID;
#endif
typedef model::d2::point_xy<POINT_TYPE> point_2d;

struct My_point_polygon
{
public:
	POINT_TYPE x, y;
	POINT_ID id;

	My_point_polygon() : x(0), y(0), id(0) {}
    My_point_polygon(POINT_TYPE _x, POINT_TYPE _y) : x(_x), y(_y), id(0) {}
	My_point_polygon(POINT_TYPE _x, POINT_TYPE _y, POINT_ID _id) : x(_x), y(_y), id(_id) {}
	

	My_point_polygon(point_2d pt, POINT_ID _id) : x(pt.x()), y(pt.y()), id(_id) {}
	My_point_polygon(point_2d pt) : x(pt.x()), y(pt.y()), id(0) {}

	My_point_polygon(My_point_polygon pt, POINT_ID _id) : x(pt.x), y(pt.y), id(_id) {}
	
};

BOOST_GEOMETRY_REGISTER_POINT_2D(My_point_polygon, POINT_TYPE, cs::cartesian, x, y)

#endif