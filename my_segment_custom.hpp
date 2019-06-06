#ifndef MY_SEGMENT_CUSTOM
#define MY_SEGMENT_CUSTOM

#include  "polyline_point_template.hpp"
#include  "polygon_point.hpp"
#include  "intersect_point_template.hpp"
#include  "intersection_algoritm.hpp"

#include <boost/assert.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/util/add_const_if_c.hpp>
#include <boost/geometry/geometries/register/segment.hpp>

using namespace boost::geometry;

#ifndef POINT_TYPE
typedef double POINT_TYPE;
#endif
#ifndef POINT_ID
typedef unsigned short int POINT_ID;
#endif

//-------------------------------------------------------------------
class My_polygon_point_segment
{
public:
	My_point_polygon one, two;

#ifndef BOOST_NO_CXX11_DEFAULTED_FUNCTIONS  /// \constructor_default_no_init
	My_polygon_point_segment() = default;
#else   /// \constructor_default_no_init
	inline My_polygon_point_segment() {}
#endif

	My_polygon_point_segment(My_point_polygon _one, My_point_polygon _two)   :  one(_one), two(_two) {}
	inline My_point_polygon const& get_one() const  { return one; } // Const access
    inline My_point_polygon const& get_two() const  { return two; } // Const access
	inline My_point_polygon      & get_one()        { return one; } // Mutable access
	inline My_point_polygon      & get_two()        { return two; } // Mutable access

};

BOOST_GEOMETRY_REGISTER_SEGMENT(My_polygon_point_segment, My_point_polygon, one, two)

template<class T>
class My_polyline_point_segment
{
public:
	My_point_polyline <T>  one, two;

#ifndef BOOST_NO_CXX11_DEFAULTED_FUNCTIONS  /// \constructor_default_no_init
	My_polyline_point_segment() = default;
#else   /// \constructor_default_no_init
	inline My_polyline_point_segment() {}
#endif
	My_polyline_point_segment(My_point_polyline <T> const& _one, My_point_polyline <T> const& _two) : one(_one), two(_two) {}

	inline My_point_polyline <T> const&  get_one() const { return one; }
	inline My_point_polyline <T> const&  get_two() const { return two; }
	inline My_point_polyline <T>&        get_one() { return one; }
	inline My_point_polyline <T>&        get_two() { return two; }
};

namespace boost {
	namespace geometry {
		namespace traits {
			template<class T>
			struct tag<My_polyline_point_segment <T>>
			{
				typedef segment_tag type;
			};
		}
	}
} // namespace boost::geometry::traits
#endif