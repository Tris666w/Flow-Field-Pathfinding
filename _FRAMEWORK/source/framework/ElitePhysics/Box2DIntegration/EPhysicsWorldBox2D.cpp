//=== General Includes ===
#include "stdafx.h"
#include "../Box2DIntegration/Box2DRenderer.h"

#ifdef USE_BOX2D
//=== Extra Information ===
struct Box2DPhysicsSettings
{
	Box2DPhysicsSettings()
	{
		hz = 60.0f;
		velocityIterations = 8;
		positionIterations = 3;
	}

	float32 hz;
	int32 velocityIterations;
	int32 positionIterations;
};

//=== Constructors & Destructors ===
template<>
PhysicsWorld::~EPhysicsWorld()
{
	SAFE_DELETE(m_pPhysicsWorld);
	auto pDebugRenderer = static_cast<Box2DRenderer*>(m_pDebugRenderer);
	SAFE_DELETE(pDebugRenderer);
}

//=== Functions ===
template <>
void PhysicsWorld::Initialize()
{
	//Create Physics
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);
	m_pPhysicsWorld = new b2World(gravity);
	m_pPhysicsWorld->SetAllowSleeping(true);
	m_pPhysicsWorld->SetWarmStarting(true);
	m_pPhysicsWorld->SetContinuousPhysics(true);
	m_pPhysicsWorld->SetSubStepping(false);

	auto pDebugRenderer = new Box2DRenderer();
	const auto flags = 1; //Render: shapes
	pDebugRenderer->SetFlags(flags);
	m_pPhysicsWorld->SetDebugDraw(pDebugRenderer);  //Link debugrenderer as debug drawer
	m_pDebugRenderer = pDebugRenderer; //Store pointer in void pointer to delete later
}

template<>
void PhysicsWorld::Simulate(float elapsedTime)
{
	if (!m_pPhysicsWorld)
		return;

	const Box2DPhysicsSettings physicsSettings;
	const float frameTime = physicsSettings.hz > 0.0f ? 1.0f / physicsSettings.hz : float32(1.f / 60.f);

	if (elapsedTime > 0.25f)
		elapsedTime = 0.25f;

	m_FrameTimeAccumulator += elapsedTime;

	while (m_FrameTimeAccumulator >= frameTime)
	{
		m_pPhysicsWorld->Step(frameTime, physicsSettings.velocityIterations, physicsSettings.positionIterations);
		m_FrameTimeAccumulator -= frameTime;
	}
}

template<>
void PhysicsWorld::RenderDebug() const
{
	const auto pDebugRenderer = static_cast<Box2DRenderer*>(m_pDebugRenderer);
	if (pDebugRenderer != nullptr)
		m_pPhysicsWorld->DrawDebugData();
}

template<>
std::vector<Elite::Polygon> PhysicsWorld::GetAllStaticShapesInWorld(PhysicsFlags userFlags) const
{
	//Locals
	std::vector<Elite::Polygon> vShapes = {};

	//Get all the shapes with certain flag, from static rigibodies
	for (b2Body* pB = m_pPhysicsWorld->GetBodyList(); pB; pB = pB->GetNext())
	{
		//Check if flagged
		if (pB->GetUserData() == nullptr)
			continue;
		//Retrieve data
		PhysicsFlags userData = *static_cast<PhysicsFlags*>(pB->GetUserData());
		if (((int)userData & (int)userFlags) == 0)
			continue;

		//Check based on type
		if (pB->GetType() == b2_staticBody)
		{
			//Get fixtures
			std::vector<b2Fixture*> vpFixtures;
			for (b2Fixture* f = pB->GetFixtureList(); f; f = f->GetNext())
				vpFixtures.push_back(f);

			//Foreach fixture
			for (auto pF : vpFixtures)
			{
				std::vector<Vector2> points; //Temp container verts
				b2Shape::Type shapeType = pF->GetType();

				switch (shapeType)
				{
				case b2Shape::e_polygon:
				{
					auto pPolygonShape = static_cast<b2PolygonShape*>(pF->GetShape());
					//Get verts and store as polygon
					for (auto i = 0; i < pPolygonShape->GetVertexCount(); ++i)
					{
						auto vert = pB->GetWorldPoint(pPolygonShape->GetVertex(i));
						points.push_back(Vector2(vert.x, vert.y));
					}
					auto polygon = Polygon(points);
					polygon.OrientateWithChildren(Winding::CW);
					vShapes.push_back(polygon);
				}
				break;
				case b2Shape::e_circle:
				{
					//Clip out bounding box of circle, not the circle itself (too many "bad" triangles!)
					b2AABB aabb;
					aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
					aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
					auto pCircleShape = static_cast<b2CircleShape*>(pF->GetShape());
					auto childCount = pCircleShape->GetChildCount();
					for (auto i = 0; i < childCount; ++i)
						aabb.Combine(aabb, pF->GetAABB(i));

					auto center = aabb.GetCenter();
					auto extents = aabb.GetExtents();
					points.push_back(Vector2(center.x - extents.x, center.y - extents.y));
					points.push_back(Vector2(center.x - extents.x, center.y + extents.y));
					points.push_back(Vector2(center.x + extents.x, center.y + extents.y));
					points.push_back(Vector2(center.x + extents.x, center.y - extents.y));

					auto polygon = Polygon(points);
					polygon.OrientateWithChildren(Winding::CW);
					vShapes.push_back(polygon);
				}
				break;
				case b2Shape::e_edge: break; //Do nothing
				case b2Shape::e_chain: break; //Do nothing
				case b2Shape::e_typeCount: break; //Do nothing
				default: break;
				}
			}
		}
	}
	return vShapes;
}
#endif