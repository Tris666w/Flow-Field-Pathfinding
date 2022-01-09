#include "stdafx.h"
#include "SquareObstacle.h"

SquareObstacle::SquareObstacle(Elite::Vector2 center, float radius)
	:ObstacleBase(center,radius)
{
	//Create Rigidbody
	const Elite::RigidBodyDefine define = Elite::RigidBodyDefine(0.01f, 0.1f, Elite::eStatic, false);
	const Transform transform = Transform(center, Elite::ZeroVector2);
	m_pRigidBody = new RigidBody(define, transform);

	//Add shape
	Elite::EPhysicsBoxShape shape;

	shape.width = GetRadius();
	shape.height = GetRadius();

	m_pRigidBody->AddShape(&shape);
}

SquareObstacle::~SquareObstacle()
{
	SAFE_DELETE(m_pRigidBody)
}
