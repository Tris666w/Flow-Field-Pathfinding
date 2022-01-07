/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EGeometryTypes.cpp: Implementation of some of the Common 2D Geometry Types.
/*=============================================================================*/
#include "stdafx.h"
//#include "EGeometry.h"
#include "EGeometry2DTypes.h"
#include "EGeometry2DUtilities.h"

#pragma region Polygon
#pragma region Constructors

//=== Constructors ===
Elite::Polygon::Polygon(std::list<Vector2>& vertices)
{
	//Store vertices
	m_vPoints = vertices;
}

Elite::Polygon::Polygon(const std::vector<Vector2>& vertices)
{
	//Store vertex vector into list
	m_vPoints.assign(vertices.begin(), vertices.end()); //Copy
}

Elite::Polygon::Polygon(const std::vector<Vector2>& outerShape, const std::vector<std::vector<Vector2>> &innerShapes)
{
	//Store outer shape points into list
	m_vPoints.assign(outerShape.begin(), outerShape.end()); //Copy

	//For each child, add child
	for (const auto i : innerShapes)
	{
		const auto polygon = new Polygon(i);
		AddChild(*polygon);
	}
}

Elite::Polygon::Polygon(const Vector2* vertices, int count)
{
	for (auto i = 0; i < count; ++i)
		m_vPoints.push_back(vertices[i]);
}

Elite::Polygon::~Polygon()
{
	m_vPoints.clear();
	m_vChildren.clear();
	for (auto t : m_vpTriangles)
		SAFE_DELETE(t);
	m_vpTriangles.clear();
	for (auto l : m_vpLines)
		SAFE_DELETE(l);
	m_vpLines.clear();
}
#pragma endregion //Constructors
//----------------------------------------------------------
#pragma region ChildFunctionality
//=== Child functionality ===
Elite::Polygon* Elite::Polygon::AddChild(std::list<Vector2>& vertices)
{
	const Polygon p(vertices); 
	m_vChildren.push_back(p); 
	return &m_vChildren[m_vChildren.size() - 1];
};

void Elite::Polygon::AddChild(const Polygon& p)
{ m_vChildren.push_back(p); }

void Elite::Polygon::RemoveChild(const Polygon& p)
{
	//Find if child is present
	const auto child = std::find(m_vChildren.begin(), m_vChildren.end(), p);
	if (child != m_vChildren.end())
		m_vChildren.erase(child);
}
#pragma endregion //ChildFunctionality
//----------------------------------------------------------
#pragma region GeneralFunctions
Elite::Vector2 Elite::Polygon::GetCenterPoint() const
{
	Vector2 cp; 
	for (const auto p : m_vPoints) 
		cp += p; 
	return cp / static_cast<float>(m_vPoints.size());
}
#pragma endregion //GeneralFunctions
//----------------------------------------------------------
#pragma region MemberAccess
bool Elite::Polygon::IsTriangulated() const
{ return m_isTriangulated; }

int Elite::Polygon::GetAmountVertices() const
{ return m_vPoints.size(); }

const std::list<Elite::Vector2>& Elite::Polygon::GetPoints() const
{ return m_vPoints; }

const std::vector<Elite::Polygon >& Elite::Polygon::GetChildren() const
{ return m_vChildren; }

const std::vector<Elite::Triangle*>& Elite::Polygon::GetTriangles() const
{ return m_vpTriangles; }

const std::vector<Elite::Line*>& Elite::Polygon::GetLines() const
{ return m_vpLines; }
#pragma endregion //MemberAccess
//----------------------------------------------------------
#pragma region GettersInformation
float Elite::Polygon::GetPosVertMaxXPos() const
{
	//Go over all the verts of this polygon (not children) and get the position of the most right vertex
	auto maxXPos = std::numeric_limits<float>::lowest();
	for (const auto p : m_vPoints)
		p.x > maxXPos ? maxXPos = p.x : 0;
	return maxXPos;
}

float Elite::Polygon::GetPosVertMaxYPos() const
{
	//Go over all the verts of this polygon (not children) and get the position of the most top vertex
	auto maxYPos = std::numeric_limits<float>::lowest();
	for (const auto p : m_vPoints)
		p.y > maxYPos ? maxYPos = p.y : 0;
	return maxYPos;
}

float Elite::Polygon::GetPosVertMinXPos() const
{
	//Go over all the verts of this polygon (not children) and get the position of the most right vertex
	auto minXPos = (std::numeric_limits<float>::max)();
	for (const auto p : m_vPoints)
		p.x < minXPos ? minXPos = p.x : 0;
	return minXPos;
}

float Elite::Polygon::GetPosVertMinYPos() const
{
	//Go over all the verts of this polygon (not children) and get the position of the most top vertex
	auto minYPos = (std::numeric_limits<float>::max)();
	for (const auto p : m_vPoints)
		p.y < minYPos ? minYPos = p.y : 0;
	return minYPos;
}

bool Elite::Polygon::OverlappingXAxis(const Polygon& poly) const
{
	auto leftOverlap = this->GetPosVertMinXPos() < poly.GetPosVertMinXPos() && this->GetPosVertMaxXPos() > poly.GetPosVertMinXPos();
	auto collinearOverlap = this->GetPosVertMinXPos() >= poly.GetPosVertMinXPos() && this->GetPosVertMaxXPos() <= poly.GetPosVertMaxXPos();
	auto rightOverlap = this->GetPosVertMaxXPos() > poly.GetPosVertMaxXPos() && this->GetPosVertMinXPos() < poly.GetPosVertMaxXPos();

	return leftOverlap || collinearOverlap || rightOverlap;
}

bool Elite::Polygon::OverlappingYAxis(const Polygon& poly) const
{
	auto bottomOverlap = this->GetPosVertMaxYPos() < poly.GetPosVertMaxYPos() && this->GetPosVertMaxYPos() > poly.GetPosVertMinYPos();
	auto collinearOverlap = this->GetPosVertMinYPos() >= poly.GetPosVertMinYPos() && this->GetPosVertMaxYPos() <= poly.GetPosVertMaxYPos();
	auto topOverlap = this->GetPosVertMaxYPos() > poly.GetPosVertMaxYPos() && this->GetPosVertMinYPos() < poly.GetPosVertMaxYPos();

	return bottomOverlap || collinearOverlap || topOverlap;
}

std::vector<Elite::Triangle*> Elite::Polygon::GetAdjacentTriangles(const Triangle* t) const
{
	//For this triangle, go over all triangles and look if any of it's edges matches the edges of a triangle,
	//in other words, two points overlap. If two points match, it's an adjacent triangle
	std::vector<Triangle*> adjTriangles;
	for (auto ct : m_vpTriangles)
	{
		if (t == ct) //If same triangle, ignore
			continue;

		//Check if points match
		auto matchingVerts = 0;
		if (t->p1 == ct->p1 || t->p1 == ct->p2 || t->p1 == ct->p3)
			++matchingVerts;
		if (t->p2 == ct->p1 || t->p2 == ct->p2 || t->p2 == ct->p3)
			++matchingVerts;
		if (t->p3 == ct->p1 || t->p3 == ct->p2 || t->p3 == ct->p3)
			++matchingVerts;

		//Only adjacent of two points match!
		if (matchingVerts == 2)
			adjTriangles.push_back(ct);
	}
	return adjTriangles;
}

std::vector<Elite::Triangle*> Elite::Polygon::GetAdjacentTrianglesOnLine(const Triangle* t, const Line& l) const
{
	//For this triangle, go over all triangles and look if it's edges matches the given line.
	//WARNING: this only works when metadata has been enabled!
	std::vector<Triangle*> adjTriangles;

#ifdef USE_TRIANGLE_METADATA
	//Start by getting index of line in matrix
	auto lRev = Line(l.p2, l.p1);
	const auto it = std::find_if(m_vpLines.begin(), m_vpLines.end(), [&](const Line* rl)
	{ return (*rl == l || *rl == lRev); });
	if (it == m_vpLines.end())
	{
		std::cout << "WARNING: line not found!" << std::endl;
		return adjTriangles;
	}
	const int lineIndex = it - m_vpLines.begin();

	//Go over all the triangles and compare lines
	for (auto ct : m_vpTriangles)
	{
		if (t == ct) //If same triangle, ignore
			continue;

		if (ct->metaData.IndexLines[0] == lineIndex
			|| ct->metaData.IndexLines[1] == lineIndex
			|| ct->metaData.IndexLines[2] == lineIndex)
			adjTriangles.push_back(ct);
	}
#endif
	return adjTriangles;
}


const Elite::Triangle* Elite::Polygon::GetTriangleFromPosition(const Vector2& position, bool onLineAllowed /*= false*/) const
{
	for (size_t i = 0; i < m_vpTriangles.size(); i++)
	{
		if (PointInTriangle(position, m_vpTriangles[i]->p1, m_vpTriangles[i]->p2, m_vpTriangles[i]->p3, onLineAllowed))
			return m_vpTriangles[i];
	}
	return nullptr;
}

#ifdef USE_TRIANGLE_METADATA
const std::vector<const Elite::Triangle*> Elite::Polygon::GetTrianglesFromLineIndex(unsigned int lineIndex) const
{
	std::vector<const Triangle*> vpFoundTriangles = {};
	for (auto pT : m_vpTriangles)
	{
		if (pT->metaData.IndexLines[0] == lineIndex ||
			pT->metaData.IndexLines[1] == lineIndex ||
			pT->metaData.IndexLines[2] == lineIndex)
		{
			vpFoundTriangles.push_back(pT);
		}
	}
	return vpFoundTriangles;
}
#endif


#pragma endregion //GettersInformation
//----------------------------------------------------------
#pragma region TriangulationFunctions
const std::vector<Elite::Triangle*>& Elite::Polygon::Triangulate()
{
	//Check winding
	OrientateWithChildren(Winding::CCW);

	//TODO: CHECK FOR OVERLAPPING POLYGONS AND MERGE WHEN REQUIRED
	for (auto child : m_vChildren)
	{
		//Check for overlap, if so merge, create new child and remove old ones!
		//FILL IN FUNCTIONS!
	}

	//Sort the children. Start by sorting from top to bottom (verices are what matters, not the "center" pos of the polygon!)
	std::sort(m_vChildren.begin(), m_vChildren.end(),
		[](const Polygon& p1, const Polygon& p2)
	{ return p1.GetPosVertMaxYPos() > p2.GetPosVertMaxYPos(); });

	//Copy children as backup after sort
	const auto children = m_vChildren;
	
	//THEN, we check two elements, if the don't overlap horizontally you don't do anything, else you swap them based on right most object
	for (auto i = 0; i < static_cast<int>(m_vChildren.size()) - 1; ++i)
	{
		if (m_vChildren[i].OverlappingYAxis(m_vChildren[i + 1]))
		{
			//Swap if i.y < i+1.y
			if (m_vChildren[i].GetPosVertMaxXPos() < m_vChildren[i + 1].GetPosVertMaxXPos())
			{
				const auto temp = m_vChildren[i];
				m_vChildren[i] = m_vChildren[i + 1];
				m_vChildren[i + 1] = temp;
			}
		}
	}

	//First split polygon
	while (m_vChildren.size() != 0)
		Split();

	//Triangle list - Clear first (if already containing triangles)
	for (auto t : m_vpTriangles)
		SAFE_DELETE(t);
	m_vpTriangles.clear();

	std::list<Vector2> copyPoints;
	copyPoints.assign(m_vPoints.begin(), m_vPoints.end()); //Copy

	//For each ear, remove ear and push verts, recheck earness (including convexness obviously :-))!
	while (copyPoints.size() > 3)
	{
		list<Vector2>::const_iterator earListIt = copyPoints.end();
		for (auto it = copyPoints.begin(); it != copyPoints.end(); ++it)
		{
			if (earListIt != copyPoints.end())
				break;
			if (IsConvexInPolygon(copyPoints, it) && IsEar(copyPoints, it))
				earListIt = it;
		}

		//Push triangle
		Vector2 current, prev, next;
		GetTriangle(copyPoints, earListIt, current, prev, next);
		Triangle* t = new Triangle(prev, current, next);
		m_vpTriangles.push_back(t);

		//Remove current from pointslist
		const auto currentIt = std::find(copyPoints.begin(), copyPoints.end(), current);
		copyPoints.erase(currentIt); //remove
	}
	//Add the remaining 3 vertices to the triangulated polygon
	std::vector<Vector2> tempCopy;
	for (const auto p : copyPoints)
		tempCopy.push_back(p);
	Triangle* lastTriangle = new Triangle(tempCopy[0], tempCopy[1], tempCopy[2]);
	m_vpTriangles.push_back(lastTriangle);

	//Flag as triangulated for later use
	m_isTriangulated = true; 

#ifdef USE_TRIANGLE_METADATA
	GenerateLineMatrix();
#endif

	m_vChildren = children;
	return m_vpTriangles;
}

void Elite::Polygon::OrientateWithChildren(Winding winding)
{
	//Based on the orientation given rewind these points if necessary, change winding of children
	//accordingly! Start with THIS shape.
	const auto currentWinding = GetPolygonWinding(m_vPoints);
	if (currentWinding != winding)
		m_vPoints.reverse();

	//Rewind the children if necessary
	auto windingChildren = abs(winding - 1); //CCW -> CW, CW -> CCW ----- abs(0-1)=1, abs(1-1)=0
	for (auto child : m_vChildren)
		child.OrientateWithChildren(static_cast<Winding>(windingChildren));
}

void Elite::Polygon::ExpandShape(float amount)
{
	//Expand each vertex along it's normal (based on adjacent edges)
	std::list<Vector2> adjustedPoints;
	for (auto it = m_vPoints.begin(); it != m_vPoints.end(); ++it)
	{
		Vector2 current, prev, next;
		GetTriangle(m_vPoints, it, current, prev, next);
		//Calculate directions
		auto dirOne = current - prev;
		auto dirTwo = next - current;
		//Calculate normals + esize normals based on amount
		Vector2 normOne = Vector2(-dirOne.y, dirOne.x);
		normOne.Normalize();
		normOne *= amount;
		Vector2 normTwo = Vector2(-dirTwo.y, dirTwo.x);
		normTwo.Normalize();
		normTwo *= amount;
		//Find size hypotenuse
		auto l1 = normOne.Magnitude();
		auto l2 = normTwo.Magnitude();
		auto size = sqrtf(l1 * l1 + l2 * l2);
		//Resize vertex normal
		Vector2 fnorm = Vector2(normOne + normTwo) / 2.0f;
		fnorm.Normalize();
		fnorm *= size;

		//Displace
		adjustedPoints.push_back(*it + fnorm);
	}
	//Overwrite data
	m_vPoints = adjustedPoints;
}
#pragma endregion //TriangulationFunctions
//----------------------------------------------------------
#pragma region PrivateGeneralFunctions
void Elite::Polygon::GetTriangle(const list<Vector2>& l, const list<Vector2>::const_iterator p, Vector2& currentTip, Vector2& previous, Vector2& next) const
{
	//Look at the point list of this polygon as it where a circular list (end attach begin)
	list<Vector2>::const_iterator prev;
	if (p == l.begin())
		prev = std::prev(l.end());
	else
		prev = std::prev(p);

	auto n = std::next(p);
	if (n == l.end())
		n = l.begin();

	//Output data
	currentTip = *p;
	previous = *prev;
	next = *n;
}

bool Elite::Polygon::IsConvexInPolygon(const list<Vector2>& l, const list<Vector2>::const_iterator p) const
{
	//Look at the point list of this polygon as it where a circular list (end attach begin)
	Vector2 current, prev, next;
	GetTriangle(l, p, current, prev, next);
	return IsConvex(current, prev, next);
}

bool Elite::Polygon::IsEar(const list<Vector2>& l, const list<Vector2>::const_iterator p) const
{
	//Look at the point list of this polygon as it where a circular list (end attach begin)
	Vector2 current, prev, next;
	GetTriangle(l, p, current, prev, next);
	
	//See if there are any vertices (excluding the ones of the triangle) are in the triangle
	for (auto it = l.begin(); it != l.end(); ++it)
	{
		if (*it == current || *it == prev || *it == next)
			continue;

		if (PointInTriangle(*it, current, prev, next))
			return false;
	}
	return true;
}

void Elite::Polygon::GenerateLineMatrix()
{
#ifdef USE_TRIANGLE_METADATA
	//Go over all the triangles
	for (auto t : m_vpTriangles)
	{
		//Go over all the lines of the triangle, search if they are already in the matrix
		//If not add them and store it's index in the triangles meta data
		const Line l1{ t->p1, t->p2 };
		const Line l1rev{ t->p2, t->p1 };
		const Line l2{ t->p2, t->p3 };
		const Line l2rev{ t->p3, t->p2 };
		const Line l3{ t->p3, t->p1 };
		const Line l3rev{ t->p1, t->p3 };
		for (auto i = 0; i < static_cast<int>(m_vpLines.size()); ++i)
		{
			const auto l = m_vpLines[i];
			if (*l == l1 || *l == l1rev)
				t->metaData.IndexLines[0] = i;
			if (*l == l2 || *l == l2rev)
				t->metaData.IndexLines[1] = i;
			if (*l == l3 || *l == l3rev)
				t->metaData.IndexLines[2] = i;
		}
		//Not found, add to matrix
		if (t->metaData.IndexLines[0] == -1)
		{
			const int index = m_vpLines.size();
			m_vpLines.push_back(new Line(t->p1, t->p2, index));
			t->metaData.IndexLines[0] = index;
		}
		if (t->metaData.IndexLines[1] == -1)
		{
			const int index = m_vpLines.size();
			m_vpLines.push_back(new Line(t->p2, t->p3, index));
			t->metaData.IndexLines[1] = index;
		}
		if (t->metaData.IndexLines[2] == -1)
		{
			const int index = m_vpLines.size();
			m_vpLines.push_back(new Line(t->p3, t->p1, index));
			t->metaData.IndexLines[2] = index;
		}
	}
#endif
}
#pragma endregion //PrivateGeneralFunctions
//----------------------------------------------------------
#pragma region PrivateTriangulationFunctions
void Elite::Polygon::FindMutualVisibleVertices(const Polygon& outer, const Polygon& inner, Vector2& pOuter, Vector2& pInner)
{
	//1. Find vertex with the biggest x value of the inner polygon AND biggest x value of outer polygon (used for size ray)
	const auto maxInnerPoint = std::max_element(inner.m_vPoints.begin(), inner.m_vPoints.end(),
		[](const Vector2& p1, const Vector2& p2) { return p1.x < p2.x; });

	const auto maxOuterPoint = std::max_element(outer.m_vPoints.begin(), outer.m_vPoints.end(),
		[](const Vector2& p1, const Vector2& p2) { return p1.x < p2.x; });
	const float rayMaxX = maxOuterPoint->x;

	//Store inner point to output
	pInner = *maxInnerPoint;

	// --- 2. Based on found inner point, find mutually visisble outer point
	//2.1 Intersect ray M + t(1,0) with all directed edges of OUTER (M is left of edges) - Result point I
	//http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	const Vector2 furthestPointRay = Vector2(rayMaxX, maxInnerPoint->y);
	Vector2 p = pInner;
	Vector2 r = furthestPointRay - pInner; //Ray
	Vector2 I = ZeroVector2; //Found intersection
	Vector2 intersectedLine[2] = { ZeroVector2, ZeroVector2 }; //Store to find point P if I is found
	for (auto it = outer.m_vPoints.begin(); it != outer.m_vPoints.end(); ++it)
	{
		const auto next = std::next(it);
		//If on right of innerPoint AND has a next point (to make line)
		if (it->x > maxInnerPoint->x && next != outer.m_vPoints.end())
		{
			//Poly line segment
			const Vector2 q = *it;
			Vector2 s = *next - q;

			//Capture parallel and collinear cases
			if (Cross(r, s) == 0)
			{
				if (Cross(q - p, r) == 0) //== Collinear
				{
					//Get closest vertex (OR the correct way see link :)) - We don't check
					//overlap because the max x of our ray is within the boundaries
					if (it->x < next->x)
						I = *it;
					else
						I = *next;
				}
				continue;
			}

			//If not parallel or collinear, search point, calculate T and U
			float crossRS = Cross(r, s);
			float t = Cross(q - p, s) / crossRS;
			float u = Cross(q - p, r) / crossRS;

			//If U == 0 || u == 1, endpoints touch, else if within range also a hit
			if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
			{
				Vector2 tempI = p + (t * r);
				auto distanceTemp = abs(Distance(p, tempI));
				auto distanceI = abs(Distance(p, I));
				//Check if closer than other intersected lines, if it is, replace stored line!
				if (intersectedLine[0] == ZeroVector2 && intersectedLine[1] == ZeroVector2
					|| distanceTemp < distanceI)
				{
					//Determine point with T and U
					I = p + (t * r);
					intersectedLine[0] = *it;
					intersectedLine[1] = *next;
				}
			}
		}
	}

	//2.2 IF I is vertex of OUTER == mutually visisble so terminate algorithm
	const auto IOuter = std::find(outer.m_vPoints.begin(), outer.m_vPoints.end(), I);
	if (IOuter != outer.m_vPoints.end()) //Point is vertex of outer, so I is correct vertex!
	{
		pOuter = I;
		return;
	}

	//2.3 ELSE I is interior point on edge, select vertex with maximum x value of the hitted edge - Result point P
	Vector2 P = intersectedLine[(intersectedLine[0].x > intersectedLine[1].x ? 0 : 1)];
	
	//2.4 Search for reflex vertices (excluding P if it's a reflex)
	std::list<Vector2> reflexVertices;
	for (auto it = outer.m_vPoints.begin(); it != outer.m_vPoints.end(); ++it)
	{
		if (*it == P)
			continue;

		if (!outer.IsConvexInPolygon(outer.m_vPoints, it))
			reflexVertices.push_back(*it);
	}

	//2.5 IF all reflex vertices are OUTSIDE of triangle (M,I,P), then P is mutually visible == terminate algorithm
	std::vector<Vector2> pointsInTriangle;
	for (auto it = reflexVertices.begin(); it != reflexVertices.end(); ++it)
	{
		if (PointInTriangle(*it, pInner, I, P))
			pointsInTriangle.push_back(*it);
	}

	if (pointsInTriangle.size() == 0)
	{
		pOuter = P;
		return;
	}

	//2.6 ELSE at least one reflex vertex lies in triangle (M,I,P). Search for reflex R that minimizes the angle
	//between (1,0) and the line (M,R). If found more choose one, else use the one found and terminate algorithm
	auto smallestAngle = 2 * M_PI;
	for (const auto point : pointsInTriangle)
	{
		//Angle between r and (M,p)
		Vector2 seg = point - p;
		auto angle = acos((Dot(seg, r)) / (seg.Magnitude() * r.Magnitude()));
		if (angle < smallestAngle)
		{
			smallestAngle = angle;
			pOuter = point;
		}
	}
}

void Elite::Polygon::Split()
{
	//Temporary new children vector
	std::vector<Polygon> newChildren;
	//Split polygon into pieces based on it's children (holes)
	for (auto child : m_vChildren)
	{
		//Find mutually visible vertices
		Vector2 pInner, pOuter;
		FindMutualVisibleVertices(*this, child, pOuter, pInner);
		//Based on mutually visible vertices, merge meshes at the split
		const auto itOuter = std::find(m_vPoints.begin(), m_vPoints.end(), pOuter); //Find where to start inserting
		const auto itInner = std::find(child.m_vPoints.begin(), child.m_vPoints.end(), pInner);
		//insert child polygon vertices to outer starting from the found inner vertex
		std::list<Vector2> mergeableChild;
		for (auto it = itInner; it != child.m_vPoints.end(); ++it)
			mergeableChild.push_back(*it);
		for (auto it = child.m_vPoints.begin(); it != itInner; ++it)
			mergeableChild.push_back(*it);
		//End by "duplicating" both the inner and outer vertex
		mergeableChild.push_back(*itInner);
		mergeableChild.push_back(*itOuter);

		//untill we've reached it again, and link this back to the outer vertex
		m_vPoints.insert(std::next(itOuter), mergeableChild.begin(), mergeableChild.end()); //insert AFTER itOuter (so next)

		//Add it's children as new children of this newly generated polygon. We remove the old ones later
		for (auto newChild : child.m_vChildren)
		{
			newChild.OrientateWithChildren(Winding::CW);
			newChildren.push_back(newChild);
		}
	}
	//Remove all "old" children and replace with new vector
	m_vChildren.clear();
	m_vChildren = newChildren;
}
#pragma endregion //PrivateTriangulationFunctions
//----------------------------------------------------------
#pragma endregion //Polygon

Elite::Rect::Rect()
	: Rect({0.f, 0.f}, 0.f, 0.f)
{
}

Elite::Rect::Rect(Vector2 bottomLeft, float width, float height)
	:bottomLeft(bottomLeft), width(width), height(height)
{
}

Elite::Circle::Circle()
	: Circle({ 0.f,{0.f,0.f} })
{
}

Elite::Circle::Circle(float radius, Vector2 center)
	:radius(radius),center(center)
{
}
