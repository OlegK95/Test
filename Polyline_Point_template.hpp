#ifndef POLYLINE_POINT
#define POLYLINE_POINT

#include <boost/range.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

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

using namespace boost::geometry;
template<class T>
struct My_point_polyline
{
	T x, y;
	POINT_TIME tm = 0;
	POINT_NOMBER nm = 0;
	POINT_ID id = 0;
public:
#ifndef BOOST_NO_CXX11_DEFAULTED_FUNCTIONS
	/// \constructor_default_no_init
	My_point_polyline() = default;
#else
	/// \constructor_default_no_init
	inline My_point_polyline() {}
#endif

	My_point_polyline(T _x, T _y)                                                                    : x(_x), y(_y), tm(0), id(0) {}
	My_point_polyline(T _x, T _y, POINT_TIME _tm, POINT_NOMBER _nm, POINT_ID _id)                    : x(_x), y(_y), tm(_tm), nm(_nm), id(_id) {}
	My_point_polyline(model::d2::point_xy <T> _pt)                                                   : x(_pt.x()), y(_pt.y()) {}
	My_point_polyline(model::d2::point_xy <T> _pt, POINT_TIME _tm, POINT_NOMBER _nm, POINT_ID _id)   : x(_pt.x()), y(_pt.y()), tm(_tm), nm(_nm), id(_id) {}
	My_point_polyline(const My_point_polyline &_p, POINT_ID _id) : x(_p.x), y(_p.y), tm(_p.tm), nm(_p.nm), id(_id) {}
};


namespace boost { namespace geometry 	{ namespace traits {   // Adapt QPoint to Boost.Geometry
  template<class T> struct tag               <My_point_polyline<T>> 	{ typedef point_tag type;  };
  template<class T> struct coordinate_type   <My_point_polyline<T>> 	{ typedef T type; };
  template<class T> struct coordinate_system <My_point_polyline<T>>	{ typedef cs::cartesian type; };
  template<class T> struct dimension         <My_point_polyline<T>> : boost::mpl::int_<2> {};

  template<class T>
  		struct access<My_point_polyline<T>, 0>
			{
				static T     get(My_point_polyline<T> const& p)             	{ return p.x;  }
				static void  set(My_point_polyline<T>& p, T const& value)       { p.x = value; }
			};
  template<class T>
       struct access<My_point_polyline<T>, 1>
		    {
		       	static T     get(My_point_polyline<T> const& p)                 { return p.y;  }
				static void  set(My_point_polyline<T>& p, T const& value)      	{ p.y = value; }
			};
}}} // namespace boost::geometry::traits

#endif