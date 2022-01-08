#include "stdafx.h"
#include "RectObstacle.h"

RectObstacle::RectObstacle(Elite::Vector2 center, Elite::Vector2 dimensions)
	:m_Center(center), m_Dimensions(dimensions)
{
	//Create Rigidbody
	const Elite::RigidBodyDefine define = Elite::RigidBodyDefine(0.01f, 0.1f, Elite::eStatic, false);
	const Transform transform = Transform(center, Elite::ZeroVector2);
	m_pRigidBody = new RigidBody(define, transform);

	//Add shape
	Elite::EPhysicsBoxShape shape;

	shape.width = m_Dimensions.x;
	shape.height = m_Dimensions.y;

	m_pRigidBody->AddShape(&shape);
}

RectObstacle::~RectObstacle()
{
	SAFE_DELETE(m_pRigidBody)
}
