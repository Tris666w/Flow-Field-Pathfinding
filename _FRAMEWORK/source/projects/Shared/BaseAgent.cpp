#include "stdafx.h"
#include "BaseAgent.h"

BaseAgent::BaseAgent(float radius):	m_Radius(radius)
{
	//Create Rigidbody
	const Elite::RigidBodyDefine define = Elite::RigidBodyDefine(0.01f, 0.1f, Elite::eDynamic, false);
	const Transform transform = Transform(Elite::ZeroVector2, Elite::ZeroVector2);
	m_pRigidBody = new RigidBody(define, transform);

	//Add shape
	Elite::EPhysicsCircleShape shape;
	shape.radius = m_Radius;
	m_pRigidBody->AddShape(&shape);
}


BaseAgent::~BaseAgent()
{
	SAFE_DELETE(m_pRigidBody);
}

void BaseAgent::Update(float dt)
{
}

void BaseAgent::Render(float dt)
{
	auto o = GetRotation() - E_PI_2;
	auto p = GetPosition();
	auto r = Elite::ToRadians(150.f);

	//EliteDebugRenderer2D::GetInstance()->DrawSolidCircle(GetPosition(), m_Radius, { 0,0 }, m_BodyColor);
	DEBUGRENDERER2D->DrawSolidCircle(GetPosition(), m_Radius, { 0,0 }, m_BodyColor);

	vector<Elite::Vector2> points;
	points.push_back(Elite::Vector2(static_cast<float>(cos(o)), static_cast<float>(sin(o)) * m_Radius) + p);
	points.push_back(Elite::Vector2(static_cast<float>(cos(o + r)), static_cast<float>(sin(o + r))*m_Radius) + p);
	points.push_back(Elite::Vector2(static_cast<float>(cos(o - r)), static_cast<float>(sin(o - r))*m_Radius) + p);

	DEBUGRENDERER2D->DrawSolidPolygon(&points[0], 3, { 0,0,0,1 }, DEBUGRENDERER2D->NextDepthSlice());
}

void BaseAgent::TrimToWorld(const Elite::Vector2& bounds) const
{
	auto pos = GetPosition();

	if (pos.x > bounds.x)
		pos.x = -bounds.x;
	else if (pos.x < -bounds.x)
		pos.x = bounds.x;

	if (pos.y > bounds.y)
		pos.y = -bounds.y;
	else if (pos.y < -bounds.y)
		pos.y = bounds.y;

	SetPosition(pos);
}

void BaseAgent::TrimToWorld(const Elite::Vector2& bottomLeft, const Elite::Vector2& topRight) const
{
	auto pos = GetPosition();

	if (pos.x > topRight.x)
		pos.x = bottomLeft.x;
	else if (pos.x < bottomLeft.x)
		pos.x = topRight.x;

	if (pos.y > topRight.y)
		pos.y = bottomLeft.y;
	else if (pos.y < bottomLeft.y)
		pos.y = topRight.y;

	SetPosition(pos);
}

void BaseAgent::LimitToWorld(const Elite::Vector2& bounds) const
{
	Elite::Vector2 pos = GetPosition();

	pos.x = Elite::Clamp(pos.x, -bounds.x, bounds.x);
	pos.y = Elite::Clamp(pos.y, -bounds.y, bounds.y);	
	SetPosition(pos);
}


