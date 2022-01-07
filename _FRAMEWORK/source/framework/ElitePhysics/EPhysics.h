/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EPhysics.h: General Physics header that includes all physics types and utilities
/*=============================================================================*/
#ifndef ELITE_PHYSICS
#define ELITE_PHYSICS

/* --- FLAGS --- */
enum PhysicsFlags
{
	Default = 0,
	NavigationCollider = 1
};

/* --- INCLUDES --- */
#include "ERigidBodyBase.h"
#include "EPhysicsWorldBase.h"
#include "EPhysicsTypes.h"

/* --- PLATFORM-SPECIFIC DEFINES --- */
#ifdef USE_BOX2D
	//=== Third-Party Includes ===
#pragma warning(push)
#pragma warning(disable: 26495)
	#include <Box2D/Box2D.h>
#pragma warning(pop)
	
	typedef Elite::ETransform<Elite::Vector2, Elite::Vector2> Transform;
	typedef Elite::RigidBodyBase<Elite::Vector2, Elite::Vector2> RigidBody;
	typedef Elite::EPhysicsWorld<b2World*> PhysicsWorld;
	typedef Elite::ERaycastHitPoint<RigidBody, Elite::Vector2> RaycastHitPoint;
	
	//=== Raycast callback specialized classes ===
	//Closest Object
	class SpecializedClosestRaycastCallback : public b2RayCastCallback
	{
	public:
		//Functions
		float32 ReportFixture(b2Fixture* pFixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) override
		{
			std::vector<Elite::Vector2> vertices = {};
			auto pRigidBody = static_cast<RigidBody*>(pFixture->GetUserData());
			if (!pRigidBody)
				return -1.0f;

			Elite::Vector2 _point = Elite::Vector2(point.x, point.y);
			Elite::Vector2 _normal = Elite::Vector2(normal.x, normal.y);
			float _fraction = fraction;

			//Store shape points [Only polygon supported at this point]
			auto shapeType = pFixture->GetType();
			if (shapeType == b2Shape::e_polygon)
			{
				auto pPolygonShape = static_cast<b2PolygonShape*>(pFixture->GetShape());
				if (!pPolygonShape)
					return -1.0f;

				for (auto i = 0; i < pPolygonShape->GetVertexCount(); ++i)
				{
					auto vert = pFixture->GetBody()->GetWorldPoint(pPolygonShape->GetVertex(i));
					vertices.push_back(Elite::Vector2(vert.x, vert.y));
				}
			}
			auto raycastHitPoint = RaycastHitPoint(pRigidBody, _point, _fraction);
			raycastHitPoint.vHitShapePoints = vertices;
			m_closestPoint = raycastHitPoint;
			return fraction;
		}

		const RaycastHitPoint& GetClosestPoint() const { return m_closestPoint; }

	private:
		//Datamembers
		RaycastHitPoint m_closestPoint = {};
	};
	//All Objects
	class SpecializedAllRaycastCallback : public b2RayCastCallback
	{
	public:
		//Functions
		float32 ReportFixture(b2Fixture* pFixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) override
		{
			std::vector<Elite::Vector2> vertices = {};
			auto pRigidBody = static_cast<RigidBody*>(pFixture->GetUserData());
			if (!pRigidBody)
				return -1.0f;

			Elite::Vector2 _point = Elite::Vector2(point.x, point.y);
			Elite::Vector2 _normal = Elite::Vector2(normal.x, normal.y);
			float _fraction = fraction;

			//Store shape points [Only polygon supported at this point]
			auto shapeType = pFixture->GetType();
			if (shapeType == b2Shape::e_polygon)
			{
				auto pPolygonShape = static_cast<b2PolygonShape*>(pFixture->GetShape());
				if (!pPolygonShape)
					return -1.0f;

				for (auto i = 0; i < pPolygonShape->GetVertexCount(); ++i)
				{
					auto vert = pFixture->GetBody()->GetWorldPoint(pPolygonShape->GetVertex(i));
					vertices.push_back(Elite::Vector2(vert.x, vert.y));
				}
			}
			auto raycastHitPoint = RaycastHitPoint(pRigidBody, _point, _fraction);
			raycastHitPoint.vHitShapePoints = vertices;
			m_vAllPoints.push_back(raycastHitPoint);

			return 1.0f;
		}
		const std::vector<RaycastHitPoint>& GetAllPoints() const { return m_vAllPoints; }
		void ClearPoints() { m_vAllPoints.clear(); }

	private:
		std::vector<RaycastHitPoint> m_vAllPoints = {};
	};

	typedef SpecializedClosestRaycastCallback ClosestRaycastCallback;
	typedef SpecializedAllRaycastCallback AllRaycastCallback;
#endif
#endif