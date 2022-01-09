#pragma once
#include "projects/Movement/SteeringBehaviors/ObstacleBase.h"

// Steering behavior obstacle
class CircleObstacle final : public ObstacleBase
{
public:
	CircleObstacle(Elite::Vector2 center, float radius);
	~CircleObstacle()override;
private:
	RigidBody* m_pRigidBody = nullptr;
};
