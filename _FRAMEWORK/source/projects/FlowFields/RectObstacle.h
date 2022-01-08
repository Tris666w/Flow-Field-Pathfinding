#pragma once
class RectObstacle final
{
public:
	RectObstacle(Elite::Vector2 center, Elite::Vector2 dimensions);
	~RectObstacle();

	Elite::Vector2 GetCenter() const { return m_Center; }
	Elite::Vector2 GetDimensions() const { return m_Dimensions; }
private:
	Elite::Vector2 m_Center;
	Elite::Vector2 m_Dimensions = { 5.f,5.f };
	RigidBody* m_pRigidBody = nullptr;
};

