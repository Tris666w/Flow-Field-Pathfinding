/*=============================================================================*/
// Copyright 2021-2022
// Authors: Matthieu Delaere
/*=============================================================================*/
// Box2DRenderer.h: Debug renderer for Box2D, which uses the general renderer functionality
/*=============================================================================*/
#ifndef ELITE_BOX2D_RENDERER
#define ELITE_BOX2D_RENDERER

#include "Box2D/Common/b2Draw.h"
#include "../../EliteMath/EMath.h"
#include "../../EliteRendering/ERendering.h"

class Box2DRenderer final : public b2Draw
{
public:
	//=== Constructors & Destructors ===
	explicit Box2DRenderer() = default;
	~Box2DRenderer() = default;

	//=== b2Draw Functions ===
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
	{
		EliteDebugRenderer2D::GetInstance()->DrawPolygon(
			reinterpret_cast<Elite::Vector2*>(const_cast<b2Vec2*>(vertices)), 
			vertexCount, Elite::Color(color.r, color.g, color.b, color.a),
			EliteDebugRenderer2D::GetInstance()->NextDepthSlice());
	};

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
	{
		EliteDebugRenderer2D::GetInstance()->DrawSolidPolygon(
			reinterpret_cast<Elite::Vector2*>(const_cast<b2Vec2*>(vertices)),
			vertexCount, Elite::Color(color.r, color.g, color.b, color.a),
			EliteDebugRenderer2D::GetInstance()->NextDepthSlice());
	}

	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override
	{
		EliteDebugRenderer2D::GetInstance()->DrawCircle(
			reinterpret_cast<Elite::Vector2&>(const_cast<b2Vec2&>(center)), radius, Elite::Color(color.r, color.g, color.b, color.a),
			EliteDebugRenderer2D::GetInstance()->NextDepthSlice());
	}

	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override
	{
		EliteDebugRenderer2D::GetInstance()->DrawSolidCircle(
			reinterpret_cast<Elite::Vector2&>(const_cast<b2Vec2&>(center)), radius,
			reinterpret_cast<Elite::Vector2&>(const_cast<b2Vec2&>(axis)),
			Elite::Color(color.r, color.g, color.b, color.a),
			EliteDebugRenderer2D::GetInstance()->NextDepthSlice());
	}

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override
	{
		EliteDebugRenderer2D::GetInstance()->DrawSegment(
			reinterpret_cast<Elite::Vector2&>(const_cast<b2Vec2&>(p1)),
			reinterpret_cast<Elite::Vector2&>(const_cast<b2Vec2&>(p2)),
			Elite::Color(color.r, color.g, color.b, color.a),
			EliteDebugRenderer2D::GetInstance()->NextDepthSlice());
	}

	void DrawTransform(const b2Transform& xf) override
	{
		auto pos = xf.p;
		auto xAxis = xf.q.GetXAxis(), yAxis = xf.q.GetYAxis();

		EliteDebugRenderer2D::GetInstance()->DrawTransform(
			reinterpret_cast<Elite::Vector2&>(pos), 
			reinterpret_cast<Elite::Vector2&>(xAxis),	reinterpret_cast<Elite::Vector2&>(yAxis),
			EliteDebugRenderer2D::GetInstance()->NextDepthSlice());
	}

	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override
	{
		EliteDebugRenderer2D::GetInstance()->DrawPoint(
			reinterpret_cast<Elite::Vector2&>(const_cast<b2Vec2&>(p)), size,
			Elite::Color(color.r, color.g, color.b, color.a),
			EliteDebugRenderer2D::GetInstance()->NextDepthSlice());
	}
};
#endif