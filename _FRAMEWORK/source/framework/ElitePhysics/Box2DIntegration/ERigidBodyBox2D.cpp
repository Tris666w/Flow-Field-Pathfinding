//=== General Includes ===
#include "stdafx.h"

#ifdef USE_BOX2D
#include "Box2D/Collision/Shapes/b2CircleShape.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"

//=== Constructors & Destructors ===
template <>
Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::RigidBodyBase(const RigidBodyDefine& define, 
	const internalTransformType& initialTransform, PhysicsFlags userFlags)
{
	//Store define information
	m_RigidBodyInformation = define;

	//Define body
	b2BodyDef bd;
	bd.position.Set(initialTransform.position.x, initialTransform.position.y);
	bd.allowSleep = define.allowSleep;
	bd.linearDamping = define.linearDamping;
	bd.angularDamping = define.angularDamping;
	switch(define.type)
	{
		case eStatic:
			bd.type = b2_staticBody; break;
		case eKinematic: 
			bd.type = b2_kinematicBody; break;
		case eDynamic: 
			bd.type = b2_dynamicBody; break;
		default: 
			bd.type = b2_staticBody; break;
	}

	//Create body
	if(PHYSICSWORLD != nullptr && PHYSICSWORLD->GetWorld() != nullptr)
		m_pBody = PHYSICSWORLD->GetWorld()->CreateBody(&bd);

	//When body is created, store userdata (flags: see EPhysics.h)
	if (m_pBody)
	{
		//Set Userdata
		m_UserDefinedFlags = userFlags;
		auto pBody = static_cast<b2Body*>(m_pBody);
		pBody->SetUserData(&m_UserDefinedFlags);
	}
}

Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::~RigidBodyBase()
{
	if (PHYSICSWORLD != nullptr && PHYSICSWORLD->GetWorld() != nullptr && m_pBody != nullptr)
	{
		auto pBody = static_cast<b2Body*>(m_pBody);
		pBody->SetUserData(nullptr);
		PHYSICSWORLD->GetWorld()->DestroyBody(pBody);
	}
}

template<>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::AddShape(Elite::EPhysicsShape* pShape)
{
	auto pBody = static_cast<b2Body*>(m_pBody);

	if(pShape->type == Elite::CircleShape)
	{
		auto* pCs = static_cast<Elite::EPhysicsCircleShape*>(pShape);
		//Create Box2D shape
		b2CircleShape b2Circle;
		b2Circle.m_radius = pCs->radius;
		//Create fixture
		b2FixtureDef fd;
		fd.shape = &b2Circle;
		fd.density = 1.0f;
		fd.friction = 0.2f;
		auto pFix = pBody->CreateFixture(&fd);
		pFix->SetUserData(this); //Set the this pointer as userdata of this fixture (RigidBody object)
		m_vFixtures.push_back(pFix); //Store fixture for later retrieval/removal
	}
	else if (pShape->type == Elite::BoxShape)
	{
		auto* pBs = static_cast<Elite::EPhysicsBoxShape*>(pShape);
		//Create Box2D shape
		b2PolygonShape b2Box;
		b2Box.SetAsBox(pBs->width/2.0f, pBs->height/2.0f, b2Vec2(0.0f, 0.0f), pBs->angle);
		//Create fixture
		b2FixtureDef fd;
		fd.shape = &b2Box;
		fd.density = 1.0f;
		fd.friction = 0.2f;
		fd.userData = this;
		auto pFix = pBody->CreateFixture(&fd);
		pFix->SetUserData(this); //Set the this pointer as userdata of this fixture (RigidBody object)
		m_vFixtures.push_back(pFix); //Store fixture for later retrieval/removal
	}
	else if(pShape->type == Elite::PolygonShape)
	{
		auto* pPs = static_cast<Elite::EPhysicsPolygonShape*>(pShape);
		//Create Box2D shape
		b2PolygonShape b2Polygon;
		b2Polygon.m_centroid = b2Vec2(pPs->center.x, pPs->center.y);
		std::vector<b2Vec2> rawVertices = {};
		for (auto v : pPs->vertices)
			rawVertices.push_back(b2Vec2(v.x, v.y));
		b2Polygon.Set(&rawVertices[0], rawVertices.size());
		//Create fixture
		b2FixtureDef fd;
		fd.shape = &b2Polygon;
		fd.density = 1.0f;
		fd.friction = 0.2f;
		fd.userData = this;
		auto pFix = pBody->CreateFixture(&fd);
		pFix->SetUserData(this); //Set the this pointer as userdata of this fixture (RigidBody object)
		m_vFixtures.push_back(pFix); //Store fixture for later retrieval/removal
	}
}

template<>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::RemoveAllShapes()
{
	auto pBody = static_cast<b2Body*>(m_pBody);
	for (auto fix : m_vFixtures)
	{
		auto pB2Fix = static_cast<b2Fixture*>(fix);
		pBody->DestroyFixture(pB2Fix);
	}
	m_vFixtures.clear();
}

template <>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::Initialize()
{
	//Do nothing
}

template<>
Elite::ETransform<Elite::Vector2, Elite::Vector2> Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::GetTransform()
{
	const auto pBody = static_cast<b2Body*>(m_pBody);
	const auto trans = pBody->GetTransform();
	return ETransform<Vector2, Vector2>(Vector2(trans.p.x, trans.p.y), Vector2(trans.q.s, trans.q.c));
}

template<>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::SetTransform(const internalTransformType& transform)
{
	auto pBody = static_cast<b2Body*>(m_pBody);
	pBody->SetTransform(b2Vec2(transform.position.x, transform.position.y), transform.rotation.x);
}

template<>
Elite::Vector2 Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::GetPosition()
{
	const auto pBody = static_cast<b2Body*>(m_pBody);
	const auto position = pBody->GetPosition();
	return Vector2(position.x, position.y);
}

template<>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::SetPosition(const Vector2& pos)
{
	auto pBody = static_cast<b2Body*>(m_pBody);
	pBody->SetTransform(b2Vec2(pos.x, pos.y), pBody->GetAngle());
}

template<>
Elite::Vector2 Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::GetRotation()
{
	const auto pBody = static_cast<b2Body*>(m_pBody);
	const auto rotation = pBody->GetAngle();
	return Vector2(rotation, rotation);
}

template<>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::SetRotation(const Vector2& rot)
{
	auto pBody = static_cast<b2Body*>(m_pBody);
	pBody->SetTransform(pBody->GetPosition(), rot.x);
}

template<>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::SetMass(float m)
{
	auto pBody = static_cast<b2Body*>(m_pBody);
	b2MassData massData;
	pBody->GetMassData(&massData);

	massData.mass = m;
	pBody->SetMassData(&massData);
}

template<>
float Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::GetMass()
{
	const auto pBody = static_cast<b2Body*>(m_pBody);
	return pBody->GetMass();
}

template<>
Elite::Vector2 Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::GetLinearVelocity()
{
	const auto pBody = static_cast<b2Body*>(m_pBody);
	const auto linearVelocity = pBody->GetLinearVelocity();
	return Vector2(linearVelocity.x, linearVelocity.y);
}

template<>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::SetLinearVelocity(const Vector2& linearVelocity)
{
	auto pBody = static_cast<b2Body*>(m_pBody);
	pBody->SetLinearVelocity(b2Vec2(linearVelocity.x, linearVelocity.y));
}

template<>
Elite::Vector2 Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::GetAngularVelocity()
{
	const auto pBody = static_cast<b2Body*>(m_pBody);
	const auto linearVelocity = pBody->GetAngularVelocity();
	return Vector2(linearVelocity, linearVelocity);
}

template<>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::SetAngularVelocity(const Vector2& angularVelocity)
{
	auto pBody = static_cast<b2Body*>(m_pBody);
	pBody->SetAngularVelocity(angularVelocity.x);
}

template <>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::SetLinearDamping(float damping)
{
	auto pBody = static_cast<b2Body*>(m_pBody);
	pBody->SetLinearDamping(damping);
}

template <>
float Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::GetLinearDamping()
{
	auto pBody = static_cast<b2Body*>(m_pBody);
	return pBody->GetLinearDamping();
}

template <>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::AddForce(const Vector2& force, EForceMode mode, bool autoWake)
{
	auto pBody = static_cast<b2Body*>(m_pBody);
	switch (mode)
	{
	case eForce: 
		pBody->ApplyForce(b2Vec2(force.x, force.y), b2Vec2_zero, autoWake);
		break;
	case eImpulse: 
		pBody->ApplyLinearImpulse(b2Vec2(force.x, force.y), b2Vec2_zero, autoWake);
		break;
	default: ;
	}
}

/*Apply torque about the z-axis (out of the screen), only X value of the Vector2 is being used!*/
template <>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::AddTorque(const Vector2& torque, EForceMode mode, bool autoWake)
{
	auto pBody = static_cast<b2Body*>(m_pBody);
	switch (mode)
	{
	case eForce:
		pBody->ApplyTorque(torque.x, autoWake);
		break;
	case eImpulse:
		pBody->ApplyAngularImpulse(torque.x, autoWake);
		break;
	default:;
	}
}

template <>
int Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::GetUserDefinedFlags()
{
	return m_UserDefinedFlags;
}

template <>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::SetUserDefinedFlags(PhysicsFlags flags)
{
	m_UserDefinedFlags = flags;
	auto pBody = static_cast<b2Body*>(m_pBody);
	pBody->SetUserData(&m_UserDefinedFlags);
}

template<>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::AddUserDefinedFlags(PhysicsFlags flags)
{
	m_UserDefinedFlags = static_cast<PhysicsFlags>((int)m_UserDefinedFlags | (int)flags);
	auto pBody = static_cast<b2Body*>(m_pBody);
	pBody->SetUserData(&m_UserDefinedFlags);
}

template<>
void Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2>::RemoveUserDefinedFlags(PhysicsFlags flags)
{
	m_UserDefinedFlags = static_cast<PhysicsFlags>(
		(int)m_UserDefinedFlags ^ ((int)m_UserDefinedFlags & (int)flags));
	auto pBody = static_cast<b2Body*>(m_pBody);
	pBody->SetUserData(&m_UserDefinedFlags);
}
#endif
