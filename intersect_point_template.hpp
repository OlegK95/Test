#ifndef INTERSECT_POINT
#define INTERSECT_POINT

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
struct Intersect_point
{
public:
	T x, y;
	bool is_in;
	POINT_TIME tm;

	T pg_x_b, pg_y_b, pg_x_e, pg_y_e;
	POINT_ID pg_id_b, pg_id_e;
	POINT_TIME pg_tm_b, pg_tm_e;
	std::string pg_nm;

	T pl_x_b, pl_y_b, pl_x_e, pl_y_e;
	POINT_ID pl_id_b, pl_id_e;
	POINT_TIME pl_tm_b, pl_tm_e;
	std::string pl_nm;


#ifndef BOOST_NO_CXX11_DEFAULTED_FUNCTIONS
	/// \constructor_default_no_init
	Intersect_point() = default;
#else
	/// \constructor_default_no_init
	inline Intersect_point() {}
#endif

};


namespace boost { namespace geometry 	{ namespace traits {   // Adapt QPoint to Boost.Geometry
  template<class T> struct tag               <Intersect_point<T>> 	{ typedef point_tag type;  };
  template<class T> struct coordinate_type   <Intersect_point<T>> 	{ typedef T type; };
  template<class T> struct coordinate_system <Intersect_point<T>>	{ typedef cs::cartesian type; };
  template<class T> struct dimension         <Intersect_point<T>> : boost::mpl::int_<2> {};

  template<class T>
  		struct access<Intersect_point<T>, 0>
			{
				static T     get(Intersect_point<T> const& p)             	{ return p.x;  }
				static void  set(Intersect_point<T>& p, T const& value)       { p.x = value; }
			};
  template<class T>
       struct access<Intersect_point<T>, 1>
		    {
		       	static T     get(Intersect_point<T> const& p)                 { return p.y;  }
				static void  set(Intersect_point<T>& p, T const& value)      	{ p.y = value; }
			};
}}} // namespace boost::geometry::traits

#endif