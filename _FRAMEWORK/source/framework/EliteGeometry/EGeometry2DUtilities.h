/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EGeometry2DUtilities.h: Common 2D Geometry Utilities used in the engine.
/*=============================================================================*/
#ifndef ELITE_GEOMETRY_2D_UTILITIES
#define ELITE_GEOMETRY_2D_UTILITIES

namespace Elite 
{
	/* --- TYPES --- */
	enum Winding //OUTER shapes should always be given CCW, INNER shapes as CW
	{
		CCW = 0, //Outer
		CW = 1 //Inner
	};

	/* --- FUNCTIONS --- */
	/*! Checking if a container with points is ordered Clockwise or Counter Clockwise. */
	template<typename container>
	Winding GetPolygonWinding(const container& shape)
	{
		//Formula: (xn+1 - xn)(yn+1 + yn)
		//Not allowed to have "changing" winding per triangle, it's the outer space that counts.
		//If ignoring this, correct winding feedback can not be guaranteed!
		// THIS IS WRONG | THIS IS CORRECT	| THIS IS CORRECT
		//	0-------3		 0-------3			 0-------1
		//	|\		|		 |\		 |			 |\		 |
		//	| \		|		 | \	 |			 | \	 |
		//	|  \	|		 |  \	 |			 |  \	 |
		//	|	\	|		 |	 \	 |			 |	 \	 |
		//	|	 \	| 		 |	  \	 | 			 |	  \	 | 
		//	|	  \	|		 |	   \ |			 |	   \ |
		//	2-------1		 1-------2			 3-------2
		//	   ??				CCW				    CW

		auto signArea = 0;
		for (auto it = shape.begin(); it != shape.end(); ++it)
		{
			auto next = std::next(it);
			if (next != shape.end())
				signArea += static_cast<int>((next->x - it->x) * (next->y + it->y));
		}
		if (signArea >= 0)
			return CW;
		return CCW;
	}
	/*! Based on 3 points see if point is convex or not. */
	constexpr auto IsConvex(const Vector2& tip, const Vector2& prev, const Vector2& next)
	{
		//Formula: d=(x−x1)(y2−y1)−(y−y1)(x2−x1)
		//http://math.stackexchange.com/questions/274712/calculate-on-which-side-of-a-straight-line-is-a-given-point-located
		//    tip
		// 	   /\
		//    /  \
		//   /    \
		// prev   next
		const auto d = (tip.x - prev.x)*(next.y - prev.y) - (tip.y - prev.y)*(next.x - prev.x);
		return d > 0;
	}
	/*! Check if a single point is inside the triangle's bounding box. This is a quick overlap test. */
	constexpr auto PointInTriangleBoundingBox(const Vector2& p, const Vector2& tip, const Vector2& prev, const Vector2& next)
	{
		const auto xMin = min(tip.x, min(prev.x, next.x)) - FLT_EPSILON;
		const auto xMax = max(tip.x, max(prev.x, next.x)) + FLT_EPSILON;
		const auto yMin = min(tip.y, min(prev.y, next.y)) - FLT_EPSILON;
		const auto yMax = max(tip.y, max(prev.y, next.y)) + FLT_EPSILON;
		return !(p.x < xMin || xMax < p.x || p.y < yMin || yMax < p.y);
	}
	/*! Square Distance of a point to a line. Used to deal with floating point errors when checking if point is on a line. */
	inline auto DistanceSquarePointToLine(const Vector2& p1, const Vector2& p2, const Vector2& point)
	{
		//http://totologic.blogspot.be/2014/01/accurate-point-in-triangle-test.html
		auto p1p2_squareDistance = DistanceSquared(p1, p2);
		auto dp = ((point.x - p1.x)*(p2.x - p1.x) + (point.y - p1.y)*(p2.y - p1.y)) / p1p2_squareDistance;
		if (dp < 0)
			return DistanceSquared(p1, point);
		if (dp <= 1)
		{
			auto pp1_squareDistance = DistanceSquared(point, p1);
			return pp1_squareDistance - dp * dp * p1p2_squareDistance;
		}
		return DistanceSquared(p2, point);
	}
	/*! Check if a single point is inside the triangle. This is a more accurate overlap test, first using the quick PointInTriangleBoundingBox test. */
	constexpr auto PointInTriangle(const Vector2& point, const Vector2& tip, const Vector2& prev, const Vector2& next, bool onLineAllowed = false)
	{
		//Do bounding box test first
		if (!PointInTriangleBoundingBox(point, tip, prev, next))
			return false;

		//Reference: http://www.blackpawn.com/texts/pointinpoly/default.html
		//Compute direction vectors
		auto v0 = prev - tip;
		auto v1 = next - tip;
		auto v2 = point - tip;

		//Compute dot products
		auto dot00 = Dot(v0, v0);
		auto dot01 = Dot(v0, v1);
		auto dot02 = Dot(v0, v2);
		auto dot11 = Dot(v1, v1);
		auto dot12 = Dot(v1, v2);

		// Compute barycentric coordinates
		auto invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		auto u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		auto v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is not in triangle (== can be on edge/line)
		if (u < 0 || v < 0 || u > 1 || v > 1 || (u + v) > 1)
		{
			if (onLineAllowed)
			{
				//Check special case where these barycentric coordinates are not enough for on line detection!
				if (DistanceSquarePointToLine(tip, next, point) <= FLT_EPSILON ||
					DistanceSquarePointToLine(next, prev, point) <= FLT_EPSILON ||
					DistanceSquarePointToLine(prev, tip, point) <= FLT_EPSILON)
					return true;
			}
			return false;
		}
		return true;
	}
	/*! Check if point is on a line */
	inline auto IsPointOnLine(const Vector2& lineStart, const Vector2& lineEnd, const Vector2& point)
	{
		//Direction vector
		Vector2 line = lineEnd - lineStart;
		line.Normalize();

		//Projection
		auto w = point - lineStart;
		auto proj = Dot(w, line);
		if (proj < 0) //Not on line
			return false;

		auto vsq = Dot(line, line);
		if (proj > vsq) //Not on line
			return false;

		return true;
	}
	/*! Project point on line segment. If not on line, remap to fit on line segment (with or without extra offset)*/
	inline Vector2 ProjectOnLineSegment(const Vector2& segmentStart, const Vector2& segmentEnd, const Vector2& point, float offset = 0.0f)
	{
		//Shorten segment based on offset, before doing actual calculations
		auto toStartDirection = (segmentStart - segmentEnd).GetNormalized();
		auto vEnd = segmentEnd + (toStartDirection * offset);
		auto toEndDirection = (segmentEnd - segmentStart).GetNormalized();
		auto vStart = segmentStart + (toEndDirection * offset);
		//Create shorten segment
		auto line = vEnd - vStart;

		//Projection
		auto w = point - vStart;
		auto proj = Dot(w, line);
		auto offsetPercentage = offset / line.Magnitude();

		//Not on line or on vertex, get closest point and offset
		if (proj <= 0)
			return vStart;
		else
		{
			auto vsq = Dot(line, line);
			//Again not on line or on vertex, but the other side
			if (proj >= vsq)
				return vEnd;
			else
				return vStart + (proj / vsq) * line;
		}
	}

	inline bool IsSegmentIntersectingWithCircle(Vector2 startSegment, Vector2 endSegment, Vector2 circleCenter, float circleRadius)
	{
		Vector2 closestPoint = ProjectOnLineSegment(startSegment, endSegment, circleCenter);
		Vector2 distanceToCircleCenter = circleCenter - closestPoint;

		if (distanceToCircleCenter.MagnitudeSquared() <= (circleRadius * circleRadius))
		{
			return true;
		}

		return false;
	}
}
#endif