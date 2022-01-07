// Steering behavior obstacle

class Obstacle final
{
public:
	Obstacle(Elite::Vector2 center, float radius);
	~Obstacle();

	Elite::Vector2 GetCenter() const { return m_Center; }
	float GetRadius() const { return m_Radius; }

private:
	Elite::Vector2 m_Center;
	float m_Radius;
	RigidBody* m_pRigidBody = nullptr;
};
