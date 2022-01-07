#include "stdafx.h"
#include "AgarioFood.h"
#include "AgarioData.h"

const float AgarioFood::m_Radius{ 1.f };
using namespace Elite;

AgarioFood::AgarioFood(Elite::Vector2 pos)
	: m_Position(pos)
{
	m_Color = Color(Elite::randomFloat(), Elite::randomFloat(), Elite::randomFloat());


	//Create Rigidbody
	const Elite::RigidBodyDefine define = Elite::RigidBodyDefine(0.01f, 0.1f, Elite::eStatic, false);
	const Transform transform = Transform(pos, Elite::ZeroVector2);
	m_pRigidBody = new RigidBody(define, transform);

	//Add shape
	Elite::EPhysicsCircleShape shape;
	shape.radius = m_Radius;
	m_pRigidBody->AddShape(&shape);

	m_pRigidBody->SetUserData({ static_cast<int>(AgarioObjectTypes::Food), this });
}

AgarioFood::~AgarioFood()
{
	SAFE_DELETE(m_pRigidBody);
}

void AgarioFood::Update(float dt)
{
}

void AgarioFood::Render(float dt)
{
	DEBUGRENDERER2D->DrawSolidCircle(m_Position, m_Radius, Elite::Vector2{}, m_Color);
}

void AgarioFood::MarkForDestroy()
{
	m_ToDestroy = true;
}

bool AgarioFood::CanBeDestroyed()
{
	return m_ToDestroy;
}
