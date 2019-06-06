#ifndef INTERSECTION_ALGORITM
#define INTERSECTION_ALGORITM

#include <cmath>
#include <algorithm>
#include <vector>

#include "gabarit_template.hpp"

template < typename T >
bool out_range(const  Gabarit <T> &a, const Gabarit <T> &b, bool equal = false)
{
	if (equal)
	{
		if (b.max_x <= a.min_x || a.max_x <= b.min_x || b.max_y <= a.min_y || a.max_y <= b.min_y)
			return true;
	}
	else
	{
		if (b.max_x < a.min_x || a.max_x < b.min_x || b.max_y < a.min_y || a.max_y < b.min_y)
			return true;
	}
	return false;
}


int intersection(POINT_TYPE const x11, POINT_TYPE const y11, POINT_TYPE const x12, POINT_TYPE const y12,
	             POINT_TYPE const x21, POINT_TYPE const y21, POINT_TYPE const x22, POINT_TYPE const y22,
	             POINT_TYPE & xrz, POINT_TYPE & yrz, bool equal = false)
{
	//Gabarit <POINT_TYPE> g1(POINT_TYPE x11, POINT_TYPE  y11, POINT_TYPE x12, POINT_TYPE y12);
	//Gabarit <POINT_TYPE> g2(POINT_TYPE x21, POINT_TYPE  y21, POINT_TYPE x22, POINT_TYPE  y22);


	Gabarit <POINT_TYPE> g1; 	g1.Add_point( x11, y11);	g1.Add_point( x12, y12);
	Gabarit <POINT_TYPE> g2;	g2.Add_point(x21, y21);  	g2.Add_point(x22, y22);

	if (out_range <POINT_TYPE> (g1, g2, equal))	return 0;

	POINT_TYPE dir1_x = x12 - x11;
	POINT_TYPE dir1_y = y12 - y11;
	POINT_TYPE dir2_x = x22 - x21;
	POINT_TYPE dir2_y = y22 - y21;

	POINT_TYPE d1 = -(dir1_x * y11 - dir1_y * x11);
	POINT_TYPE d2 = -(dir2_x * y21 - dir2_y * x21);

	//подставляем концы отрезков, для выяснения в каких полуплоскотях они
	POINT_TYPE seg1_line2_start = dir2_x * y11 - dir2_y * x11 + d2;
	POINT_TYPE seg1_line2_end = dir2_x * y12 - dir2_y * x12 + d2;
	POINT_TYPE seg2_line1_start = dir1_x * y21 - dir1_y * x21 + d1;
	POINT_TYPE seg2_line1_end = dir1_x * y22 - dir1_y * x22 + d1;

	//если концы одного отрезка имеют один знак, значит он в одной полуплоскости и пересечения нет.
	if (seg1_line2_start * seg1_line2_end > 0 || seg2_line1_start * seg2_line1_end > 0)
		return 0;

	//если совпадают, значит отрезки частично лежат на одной прямой
	if (seg1_line2_start == seg1_line2_end)
	{
		if (x11 == x21 && x12 == x22)    // параллельны оси Y
		{
            xrz = x11;
			if ((y11 == y21) && (! ((y11 - y12) * (y11 - y22) > 0)))  { yrz = y11; return -1; }  // одинаковый знак, значит по одну сторону и перекрываются 
			if ((y11 == y22) && (! ((y11 - y12) * (y11 - y21) > 0)))  { yrz = y11; return -1; }  // одинаковый знак, значит по одну сторону и перекрываются 
			if ((y12 == y21) && (! ((y12 - y11) * (y12 - y22) > 0)))  { yrz = y12; return -1; }   // одинаковый знак, значит по одну сторону и перекрываются 
			if ((y12 == y22) && (! ((y12 - y11) * (y12 - y21) > 0)))  { yrz = y12; return -1; }   // одинаковый знак, значит по одну сторону и перекрываются 
			return -2;
		}
		else
		{
			yrz = y11;
			if ((x11 == x21) && (!((x11 - x12) * (x11 - x22) > 0))) { xrz = x11; return -1; }  // одинаковый знак, значит по одну сторону и перекрываются 
			if ((x11 == x22) && (!((x11 - x12) * (x11 - x21) > 0))) { xrz = x11; return -1; }  // одинаковый знак, значит по одну сторону и перекрываются 
			if ((x12 == x21) && (!((x12 - x11) * (x12 - x22) > 0))) { xrz = x12; return -1; }   // одинаковый знак, значит по одну сторону и перекрываются 
			if ((x12 == x22) && (!((x12 - x11) * (x12 - x21) > 0))) { xrz = x12; return -1; }   // одинаковый знак, значит по одну сторону и перекрываются 
			return -2;
		}
	}

	POINT_TYPE u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);
	xrz = x11 + u * dir1_x;
	yrz = y11 + u * dir1_y;

	if ((xrz == x21 && yrz == y21) || (xrz == x22 && yrz == y22) || (xrz == x11 && yrz == y11) || (xrz == x12 && yrz == y12))
		return -1;
	return 1;
}
#endif