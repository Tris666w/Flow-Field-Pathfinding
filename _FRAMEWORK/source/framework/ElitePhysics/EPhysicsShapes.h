/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EPhysicsShapes.h: Physics Shapes base for Engine.
/*=============================================================================*/
#ifndef ELITE_PHYSICS_SHAPES_BASE
#define	ELITE_PHYSICS_SHAPES_BASE

enum EPhysicsShapeType
{
	UndefinedShape = 0,
	CircleShape = 1,
	BoxShape = 2,
	PolygonShape = 3
};

struct EPhysicsShape
{
	EPhysicsShapeType type = EPhysicsShapeType::UndefinedShape;
};

struct EPhysicsCircleShape : EPhysicsShape
{
	Elite::Vector2 position = Elite::ZeroVector2;
	float radius = 1.f;
	
	EPhysicsCircleShape() { type = EPhysicsShapeType::CircleShape; };
	EPhysicsCircleShape(const Elite::Vector2& _position, float _radius):position(_position),radius(_radius){ type = CircleShape; }
};

struct EPhysicsBoxShape : EPhysicsShape
{
	Elite::Vector2 position = Elite::ZeroVector2;
	float width = 1.f;
	float height = 1.f;
	float angle = 0.f;

	EPhysicsBoxShape() { type = EPhysicsShapeType::BoxShape; };
	EPhysicsBoxShape(const Elite::Vector2& _position, float _width, float _height, float _angle = 0.f)
		:position(_position), width(_width), height(_height), angle(_angle) {
		type = BoxShape;
	}
};

struct EPhysicsPolygonShape : EPhysicsShape
{
	Elite::Vector2 center = Elite::ZeroVector2;
	std::vector<Elite::Vector2> vertices = {};
	std::vector<Elite::Vector2> normals = {};

	EPhysicsPolygonShape() { type = EPhysicsShapeType::PolygonShape; };
	EPhysicsPolygonShape(const Elite::Vector2& _center, const std::vector<Elite::Vector2>& _vertices, const std::vector<Elite::Vector2>& _normals)
		: center(_center), vertices(_vertices), normals(_normals){ type = EPhysicsShapeType::PolygonShape; }
};
#endif