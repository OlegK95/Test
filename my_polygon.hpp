#ifndef MY_POLYGON
#define MY_POLYGON


#include <iostream>
#include <string>
#include <algorithm>
#include <map>

#include "polygon_point.hpp"
#include "gabarit_template.hpp"

#include <boost/assert.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include <boost/geometry/geometry.hpp>

#include <boost/geometry/strategies/strategies.hpp>

#include <boost/geometry/algorithms/make.hpp>
#include <boost/geometry/algorithms/distance.hpp>

#include <boost/geometry/geometries/register/ring.hpp>

#include <boost/geometry/util/add_const_if_c.hpp>

#include <boost/geometry/io/dsv/write.hpp>




#define BOOST_UTF8_BEGIN_NAMESPACE \
namespace boost { namespace detail {
#define BOOST_UTF8_DECL
#define BOOST_UTF8_END_NAMESPACE }}
#include <boost/detail/utf8_codecvt_facet.ipp>
#undef BOOST_UTF8_END_NAMESPACE
#undef BOOST_UTF8_DECL
#undef BOOST_UTF8_BEGIN_NAMESPACE






using namespace boost::geometry;

#ifndef POINT_TYPE
    typedef double POINT_TYPE;
#endif
#ifndef POINT_ID
    typedef unsigned short int POINT_ID;
#endif

//-----------------------------------------------
class My_polygon
{
	std::string color = "";
	std::string name = "";
	Gabarit <POINT_TYPE> g;
public:
	std::vector <My_point_polygon>  points;

#ifndef BOOST_NO_CXX11_DEFAULTED_FUNCTIONS  /// \constructor_default_no_init
	My_polygon() = default;
#else   /// \constructor_default_no_init
	inline My_polygon() {}
#endif

	bool read_polygon(std::string const& _st)
	{
		boost::property_tree::ptree _p_root;
		std::stringstream ss;
		ss << _st;

		std::locale utf8_locale(std::locale(), new boost::detail::utf8_codecvt_facet);

		boost::property_tree::read_json(ss, _p_root);
		if (!_p_root.empty())
		{
			color = _p_root.get<std::string>(".color");
			name = _p_root.get<std::string>(".name");
			boost::property_tree::ptree  _coord = _p_root.get_child(".coords");      //_coord
			if ((!_coord.empty()) && (_coord.begin() != _coord.end()))
			{
#ifdef INPUT_POLYGON_CONTROL
				POINT_TYPE old_x, old_y;
#endif
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v, _coord)
				{
					assert(v.first.empty());
					boost::property_tree::ptree::const_iterator end = v.second.end();
					int i = 0;
					for (boost::property_tree::ptree::const_iterator it = v.second.begin(); it != end; ++it, i++)
					{
						boost::property_tree::ptree::const_iterator begin1 = it->second.begin();
						POINT_TYPE x = begin1->second.get_value<POINT_TYPE>();
						POINT_TYPE y = (++begin1)->second.get_value<POINT_TYPE>();
#ifdef INPUT_POLYGON_CONTROL
						if (i < 4 && boost::geometry::intersects(_ls, segment_2d(make<point_2d>(x, y), make<point_2d>(old_x, old_y))))
							break;  // В полигон добавляем отрезок, только если он не пересекается с остальными
						old_x = x;
						old_y = y;
#endif
						add_point(My_point_polygon (x, y));
					}
				}
			}
			if (!boost::geometry::is_valid(*this))
				boost::geometry::correct(*this);
			return true;
		}
		else return false;
	}

	void add_point(const My_point_polygon &_pt)
	{
		g.Add_point(_pt.x, _pt.y);
	//	_pt.id = (POINT_ID)points.size();
		points.push_back(My_point_polygon(_pt, (POINT_ID)points.size()));
	}


	// Const access
	My_point_polygon const& get_point(std::size_t i) const
	{
		BOOST_ASSERT(i < points.size());
		return points[i];
	}

	// Mutable access
	My_point_polygon & get_point(std::size_t i)
	{
		BOOST_ASSERT(i < points.size());
		return points[i];
	}


	int point_count() const { return points.size(); }
	void erase_all() { points.clear();   color.clear();  name.clear(); }

	inline void set_size(int n) { points.resize(n); }
	friend std::ostream& operator <<(std::ostream& o, const My_point_polygon& in);

	std::string  const& get_name()  { return name; }
	std::string  const& get_color() { return color; }

	inline POINT_TYPE get_minX() { return g.min_x; }
	inline POINT_TYPE get_minY() { return g.min_y; }
	inline POINT_TYPE get_maxX() { return g.max_x; }
	inline POINT_TYPE get_maxY() { return g.max_y; }
	inline const Gabarit <POINT_TYPE> & get_gabarit() { return g; }
};

/*
std::ostream& operator<<(std::ostream& o, const My_polygon& in)
{
//	std::string nam = in.get_color();
	return o << " color = " //<< in.get_color()
//		<< " Dimension = (" << in.minX << ", " << in.minY << " : " << in.maxX << ", " << in.maxY << ')' << std::endl
		<< dsv(in.points) << std::endl;
}
*/

// 1) implement iterator (const and non-const versions)
template<typename MY_PG>
struct custom_iterator : public boost::iterator_facade
	<
	custom_iterator<MY_PG>,
	My_point_polygon,
	boost::random_access_traversal_tag,
	typename boost::mpl::if_
	<
	boost::is_const<MY_PG>,
	My_point_polygon const,
	My_point_polygon
	>::type&
	>
{
	// Constructor for begin()
	explicit custom_iterator(MY_PG& polygon) : m_polygon(&polygon)	, m_index(0) {}

	// Constructor for end()
	explicit custom_iterator(bool, MY_PG& polygon)
		: m_polygon(&polygon)
		, m_index(polygon.point_count())
	{}


	// Default constructor
	explicit custom_iterator()
		: m_polygon(NULL)
		, m_index(-1)
	{}

	typedef typename boost::mpl::if_
		<
		boost::is_const<MY_PG>,
		My_point_polygon const,
		My_point_polygon
		>::type my_point_type;

private:
	friend class boost::iterator_core_access;

	typedef boost::iterator_facade
		<
		custom_iterator<MY_PG>,
		My_point_polygon,
		boost::random_access_traversal_tag,
		my_point_type&
		> facade;

	MY_PG* m_polygon;
	int m_index;

	bool equal(custom_iterator const& other) const
	{	return this->m_index == other.m_index;	}
	typename facade::difference_type distance_to(custom_iterator const& other) const
	{ return other.m_index - this->m_index; }

	void advance(typename facade::difference_type n)
	{
		m_index += n;
		if (m_polygon != NULL  && (m_index >= m_polygon->point_count()  || m_index < 0)	)
		   {  m_index = m_polygon->point_count(); }
	}

	void increment() {  advance(1);  }
	void decrement() {  advance(-1); }

	// const and non-const dereference of this iterator
	my_point_type& dereference() const 	{  return m_polygon->get_point(m_index); }
};



// 2) Implement Boost.Range const functionality
//    using method 2, "provide free-standing functions and specialize metafunctions"
// 2a) meta-functions
namespace boost
{
	template<> struct range_mutable_iterator<My_polygon>
	{
		typedef custom_iterator<My_polygon> type;
	};

	template<> struct range_const_iterator<My_polygon>
	{
		typedef custom_iterator<My_polygon const> type;
	};

	// RangeEx
	template<> struct range_size<My_polygon>
	{
		typedef std::size_t type;
	};

} // namespace 'boost'


// 2b) free-standing function for Boost.Range ADP
inline custom_iterator<My_polygon> range_begin(My_polygon& polygon)
{
	return custom_iterator<My_polygon>(polygon);
}

inline custom_iterator<My_polygon const> range_begin(My_polygon const& polygon)
{
	return custom_iterator<My_polygon const>(polygon);
}

inline custom_iterator<My_polygon> range_end(My_polygon& polygon)
{
	return custom_iterator<My_polygon>(true, polygon);
}

inline custom_iterator<My_polygon const> range_end(My_polygon const& polygon)
{
	return custom_iterator<My_polygon const>(true, polygon);
}



// 3) optional, for writable geometries only, implement push_back/resize/clear
namespace boost {
	namespace geometry {
		namespace traits
		{

			template<> struct push_back<My_polygon>
			{
				static inline void apply(My_polygon& polygon, My_point_polygon const& point)
				{
					polygon.add_point(point);
				}
			};

			template<> struct resize<My_polygon>
			{
				static inline void apply(My_polygon& polygon, std::size_t new_size)
				{
					polygon.set_size(new_size);
				}
			};

			template<> struct clear<My_polygon>
			{
				static inline void apply(My_polygon& polygon)
				{
					polygon.erase_all();
				}
			};

		}
	}
}


// 4) register with Boost.Geometry
//BOOST_GEOMETRY_REGISTER_POINT_2D(my_point, double, cs::cartesian, x, y)

BOOST_GEOMETRY_REGISTER_RING(My_polygon)

// end adaption
// ----------------------------------------------------------------------------

void print_using_iterator(My_polygon const& polygon)
{
	for (custom_iterator<My_polygon const> it = custom_iterator<My_polygon const>(polygon);
		it != custom_iterator<My_polygon const>(true, polygon);
		++it)
	{
		std::cout << boost::geometry::dsv(*it) << std::endl;
	}
	std::cout << std::endl;
}


void print_using_range(My_polygon const& polygon)
{
	for (boost::range_iterator<My_polygon const>::type it
		= boost::begin(polygon);
		it != boost::end(polygon);
		++it)
	{
		std::cout << boost::geometry::dsv(*it) << std::endl;
	}
	std::cout << std::endl;
}

#endif