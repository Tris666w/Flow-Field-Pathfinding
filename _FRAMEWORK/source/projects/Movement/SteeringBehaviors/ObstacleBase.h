#pragma once
class ObstacleBase
{
public:
	ObstacleBase(Elite::Vector2 center, float radius);
	virtual ~ObstacleBase() = default;
	Elite::Vector2 GetCenter() const { return m_Center; }
	float GetRadius() const { return m_Radius; }

private:
	Elite::Vector2 m_Center;
	float m_Radius;
};

