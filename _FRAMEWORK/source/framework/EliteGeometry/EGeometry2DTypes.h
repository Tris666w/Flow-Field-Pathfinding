/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EGeometryTypes.h: Common 2D Geometry Types used in the engine.
/*=============================================================================*/
#ifndef ELITE_GEOMETRY_TYPES
#define	ELITE_GEOMETRY_TYPES

#include "EGeometry2DUtilities.h"
#include <array>


namespace Elite 
{
	//=== Options ===
	#define USE_TRIANGLE_METADATA

	//=== Types ===
#pragma region Line
	struct Line final
	{
		//=== Datamembers ===
		Vector2 p1 = {};
		Vector2 p2 = {};
		int index = -1; //Line index, used for optimized intersecting calculations

		//=== Constructors ===
		Line() : p1(ZeroVector2), p2(ZeroVector2) {}
		Line(const Vector2& _p1, const Vector2& _p2, int i = -1) : p1(_p1), p2(_p2), index(i) {}

		//=== Relational Operators ===
		bool operator==(const Line& l) const
		{ return (l.p1 == p1 && l.p2 == p2); }
		bool operator!=(const Line& l) const
		{ return !(l == *this);	}

		//=== Member Access Operators ===
		Vector2& operator[](int index)
		{ return (index == 0) ? p1 : p2; }
	};
#pragma endregion //Line

#pragma region Triangle
	//Triangle MetaData is used for optimized intersecting and shared edges calculations
	struct TriangleMetaData final
	{
		std::array<int, 3> IndexLines{ {-1, -1, -1} };
	};

	struct Triangle final
	{
		//=== Datamembers ===
#ifdef USE_TRIANGLE_METADATA
		TriangleMetaData metaData = {};
#endif
		Vector2 p1 = {};
		Vector2 p2 = {};
		Vector2 p3 = {};

		//=== Constructors ===
		Triangle() : p1(ZeroVector2), p2(ZeroVector2), p3(ZeroVector2) {}
		Triangle(const Vector2& _p1, const Vector2& _p2, const Vector2& _p3) : p1(_p1), p2(_p2), p3(_p3){}

		//=== Internal Triangle Functions ===
		Vector2 GetCenter() const
		{
			auto p = p1 + p2 + p3;
			return p / 3.0f;
		}

		//=== Relational Operators ===
		bool operator==(const Triangle& t) const
		{ return (t.p1 == p1 && t.p2 == p2 && t.p3 == p3); }
		bool operator!=(const Triangle& t) const
		{ return !(t == *this);	}
	};
#pragma endregion //Triangle

#pragma region Polygon
	class Polygon final
	{
	public:
		//=== Constructors ===
		Polygon() = default;
		explicit Polygon(std::list<Vector2>& vertices);
		explicit Polygon(const std::vector<Vector2>& vertices);
		explicit Polygon(const std::vector<Vector2>& outerShape, const std::vector<std::vector<Vector2>> &innerShapes);
		explicit Polygon(const Vector2* vertices, int count);
		~Polygon();

		//=== Functions ===
		//Child functionality
		Polygon* AddChild(std::list<Vector2>& vertices);
		void AddChild(const Polygon& p);
		void RemoveChild(const Polygon& p);

		//General functions
		Vector2 GetCenterPoint() const;

		//Member access
		bool IsTriangulated() const;
		int GetAmountVertices() const;
		const std::list<Vector2>& GetPoints() const;
		const std::vector<Polygon>& GetChildren() const;
		const std::vector<Triangle*>& GetTriangles() const;
		const std::vector<Line*>& GetLines() const;

		//Getters information
		float GetPosVertMaxXPos() const;
		float GetPosVertMaxYPos() const;
		float GetPosVertMinXPos() const;
		float GetPosVertMinYPos() const;
		bool OverlappingXAxis(const Polygon& poly) const;
		bool OverlappingYAxis(const Polygon& poly) const;
		std::vector<Triangle*> GetAdjacentTriangles(const Triangle* t) const;
		std::vector<Triangle*> GetAdjacentTrianglesOnLine(const Triangle* t, const Line& l) const;

		const Triangle* GetTriangleFromPosition(const Vector2& position, bool onLineAllowed = false) const;
#ifdef USE_TRIANGLE_METADATA
		const std::vector<const Triangle*> GetTrianglesFromLineIndex(unsigned int lineIndex) const;
#endif



		//Triangulation functions
		const std::vector<Triangle*>& Triangulate();
		void OrientateWithChildren(Winding winding);
		void ExpandShape(float amount);

		//=== Operators ===
		bool operator ==(const Polygon& b) const
		{ return this->m_vChildren == b.m_vChildren && this->m_vPoints == b.m_vPoints; }

	private:
		//=== Datamembers ===
		std::vector<Polygon> m_vChildren; //Inner shapes of this polygon
		std::list<Vector2> m_vPoints; //Points that define this polygon
		std::vector<Triangle*> m_vpTriangles; //Triangles create for this polygon, used for rendering
		std::vector<Line*> m_vpLines; //Lines constructing this polygon!
		bool m_isTriangulated = false;

		//=== Functions ===
		//Private General Functions
		void GetTriangle(const list<Vector2>& l, const list<Vector2>::const_iterator p, Vector2& currentTip, Vector2& previous, Vector2& next) const;
		bool IsConvexInPolygon(const list<Vector2>& l, const list<Vector2>::const_iterator p) const;
		bool IsEar(const list<Vector2>& l, const list<Vector2>::const_iterator p) const;
		void GenerateLineMatrix();

		//Private Triangulation Functions
		void FindMutualVisibleVertices(const Polygon& outer, const Polygon& inner, Vector2& pOuter, Vector2& pInner);
		void Split();
	};
#pragma endregion //Polygon

#pragma region Rect
	struct Rect final
	{
		Rect();
		Rect(Vector2 bottomLeft, float width, float height);

		Vector2 bottomLeft;
		float width;
		float height;
	};

	inline bool IsOverlapping(const Rect& a, const Rect& b)
	{
		// If one rectangle is on left side of the other
		if (a.bottomLeft.x + a.width < b.bottomLeft.x|| b.bottomLeft.x + b.width < a.bottomLeft.x)
		{
			return false;
		}

		// If one rectangle is under the other
		if (a.bottomLeft.y > b.bottomLeft.y + b.height || b.bottomLeft.y > a.bottomLeft.y + a.height)
		{
			return false;
		}

		return true;
	}
#pragma endregion //Rect


#pragma region Circle
	struct Circle final
	{
		Circle();
		Circle(float radius, Vector2 center);

		Vector2 center;
		float radius;
	};


#pragma endregion //Circle
}
#endif