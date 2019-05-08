#ifndef MY_POLYLINE
#define MY_POLYLINE

#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <cassert>
#include <exception>
#include <sstream>

#include <boost/assert.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include <boost/geometry/geometry.hpp>

#include <boost/geometry/strategies/strategies.hpp>

#include <boost/geometry/algorithms/make.hpp>
#include <boost/geometry/algorithms/distance.hpp>

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/register/linestring.hpp>

#include <boost/geometry/util/add_const_if_c.hpp>

#include <boost/geometry/io/dsv/write.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Polyline_Point_template.hpp"
#include "Gabarit_template.hpp"

using namespace boost::geometry;

#ifndef POINT_TYPE
    typedef double POINT_TYPE;
#endif
#ifndef POINT_ID
    typedef unsigned short int POINT_ID;
#endif


template<class T>
class My_polyline
{
	std::string color = "";
	std::string name = "";
	Gabarit <T> g;

public:
	std::vector < My_point_polyline <T>> points;

#ifndef BOOST_NO_CXX11_DEFAULTED_FUNCTIONS  /// \constructor_default_no_init
	My_polyline() = default;
#else   /// \constructor_default_no_init
	inline My_polyline() {}
#endif

	inline T  get_minX() { return g.min_x; }
	inline T  get_minY() { return g.min_y; }
	inline T  get_maxX() { return g.max_x; }
	inline T  get_maxY() { return g.max_y; }
	inline const Gabarit <T> & get_gabarit() { return g; }
	inline const std::string  & get_name() { return name; }

	bool read_polyline(std::string const& _st)
	{
		boost::property_tree::ptree _p_root;
		std::stringstream ss;
		ss << _st;
		boost::property_tree::read_json(ss, _p_root);
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, _p_root)
			{
  			  assert(v.first.empty());
 			  boost::property_tree::ptree::const_iterator end = v.second.end();
			  for (boost::property_tree::ptree::const_iterator it = v.second.begin(); it != end; ++it)
				 {
				    
					boost::property_tree::ptree::const_iterator begin1 = it->second.begin();
					My_point_polyline <T> tmp;
					tmp.x = begin1->second.get_value<T>();
					tmp.y = (++begin1)->second.get_value<T>();
					tmp.nm = (++begin1)->second.get_value<T>();
					++begin1;
					tmp.tm = (++begin1)->second.get_value<POINT_TIME>();
					add_point(tmp);
				 }
			 }
		return true;
	}

	void add_point(My_point_polyline <T> const& _pt)
	{
		g.Add_point(_pt.x, _pt.y);
		points.push_back(My_point_polyline <T>(_pt, (POINT_ID)points.size()));
	}
	// Const access
	My_point_polyline <T> const& get_point(std::size_t i) const
	{
		BOOST_ASSERT(i < points.size());
		return points[i];
	}

	// Mutable access
	My_point_polyline <T> & get_point(std::size_t i)
	{
		BOOST_ASSERT(i < points.size());
		return points[i];
	}

	int point_count() const { return points.size(); }
	void erase_all() { points.clear();  color.clear();  name.clear(); }

	inline void set_size(int n) { points.resize(n); }


	//friend std::ostream& operator <<(std::ostream& o, const My_point_polyline <T>& in);

};

namespace boost { namespace geometry { namespace traits {
			template<class T>
			struct tag<My_polyline <T>>
			{
				typedef linestring_tag type;
			};
}}} // namespace boost::geometry::traits


namespace boost {
	template <class T>
	struct range_iterator<My_polyline<T>>
	{
		typedef typename std::vector< My_point_polyline <T>>::iterator type;
	};

	template<class T>
	struct range_const_iterator<My_polyline<T>>
	{
		typedef typename std::vector< My_point_polyline <T>>::const_iterator type;
	};
}

template<class T>
inline typename std::vector< My_point_polyline <T>>::iterator range_begin(My_polyline <T>& qls)
  {	return qls.points.begin(); }

template<class T>
inline typename std::vector< My_point_polyline <T>>::iterator range_end(My_polyline <T>& qls)
  {	return qls.points.end(); }

template<class T>
inline typename std::vector< My_point_polyline <T>>::const_iterator range_begin(const My_polyline <T>& qls)
  { return qls.points.begin(); }

template<class T>
inline typename std::vector< My_point_polyline <T>>::const_iterator range_end(const My_polyline <T>& qls)
  {	return qls.points.end(); }


#endif