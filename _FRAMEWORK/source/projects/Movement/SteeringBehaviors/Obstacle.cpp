#pragma once
#include "stdafx.h"
#include "Obstacle.h"

Obstacle::Obstacle(Elite::Vector2 center, float radius)
	:m_Center(center), m_Radius(radius)
{
	//Create Rigidbody
	const Elite::RigidBodyDefine define = Elite::RigidBodyDefine(0.01f, 0.1f, Elite::eStatic, false);
	const Transform transform = Transform(center, Elite::ZeroVector2);
	m_pRigidBody = new RigidBody(define, transform);

	//Add shape
	Elite::EPhysicsCircleShape shape;
	shape.radius = m_Radius;
	m_pRigidBody->AddShape(&shape);
}

Obstacle::~Obstacle()
{
	SAFE_DELETE(m_pRigidBody);
}