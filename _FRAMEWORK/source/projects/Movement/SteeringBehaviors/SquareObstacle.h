#pragma once
#include "ObstacleBase.h"

class SquareObstacle final : public ObstacleBase
{
public:
	SquareObstacle(Elite::Vector2 center, float radius);
	~SquareObstacle() override;

private:
	RigidBody* m_pRigidBody = nullptr;
};

