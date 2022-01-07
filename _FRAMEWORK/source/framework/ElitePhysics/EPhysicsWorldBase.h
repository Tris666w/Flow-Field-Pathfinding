/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EPhysicsWorldBase.h: Physics World base for Engine.
/*=============================================================================*/
#ifndef ELITE_PHYSICS_WORLD_BASE
#define	ELITE_PHYSICS_WORLD_BASE

#include "../EliteGeometry/EGeometry2DTypes.h"
class Renderer;

namespace Elite
{
	template<typename physicsWorldType>
	class EPhysicsWorld : public ESingleton< EPhysicsWorld<physicsWorldType>>
	{
	public:
		//=== Constructors & Destructors ===
		EPhysicsWorld() { Initialize(); }
		~EPhysicsWorld();

		//=== World Functions ===
		void Simulate(float elapsedTime = 0.f);
		void RenderDebug() const;

		physicsWorldType GetWorld() const { return m_pPhysicsWorld; }
		std::vector<Elite::Polygon> GetAllStaticShapesInWorld(PhysicsFlags userFlags) const;

		template<typename raycastbackType, typename positionType>
		void Raycast(raycastbackType* callback, const positionType& point1, const positionType& point2)
		{ m_pPhysicsWorld->RayCast(callback, point1, point2); }

	private:
		//=== Datamembers ===
		physicsWorldType m_pPhysicsWorld;
		void* m_pDebugRenderer = nullptr;
		float m_FrameTimeAccumulator = 0.f;

		//=== Internal Functions ===
		void Initialize();
	};
}
#endif