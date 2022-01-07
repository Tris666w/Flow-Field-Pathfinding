#ifndef BASE_AGENT_H
#define BASE_AGENT_H
class BaseAgent
{
public:
	BaseAgent(float radius = 1.f);
	virtual ~BaseAgent();

	virtual void Update(float dt);
	virtual void Render(float dt);

	//Functions
	void TrimToWorld(float worldBounds) const { TrimToWorld({ worldBounds,worldBounds }); }
	void TrimToWorld(const Elite::Vector2& bounds) const;
	void TrimToWorld(const Elite::Vector2& bottomLeft, const Elite::Vector2& topRight) const;
	void LimitToWorld(float worldBounds) const { LimitToWorld({ worldBounds,worldBounds }); }
	void LimitToWorld(const Elite::Vector2& bounds) const;

	//Get - Set
	Elite::Vector2 GetPosition() const { return m_pRigidBody->GetPosition(); }
	void SetPosition(const Elite::Vector2& pos) const { m_pRigidBody->SetPosition(pos); }

	float GetRotation() const { return m_pRigidBody->GetRotation().x; }
	void SetRotation(float rot) const { m_pRigidBody->SetRotation({ rot,0.f }); }

	Elite::Vector2 GetLinearVelocity() const { return m_pRigidBody->GetLinearVelocity(); }
	void SetLinearVelocity(const Elite::Vector2& linVel) const { m_pRigidBody->SetLinearVelocity(linVel); }

	float GetAngularVelocity() const { return m_pRigidBody->GetAngularVelocity().x; }
	void SetAngularVelocity(float angVel) const { m_pRigidBody->SetAngularVelocity({ angVel,0.f }); }
	
	float GetMass() const { return m_pRigidBody->GetMass(); }
	void SetMass(float mass) const { m_pRigidBody->SetMass(mass); }

	const Elite::Color& GetBodyColor() const { return m_BodyColor; }
	void SetBodyColor(const Elite::Color& col) { m_BodyColor = col; }

	Elite::RigidBodyUserData GetUserData() const { return m_pRigidBody->GetUserData(); }
	void SetUserData(Elite::RigidBodyUserData userData) { m_pRigidBody->SetUserData(userData); }

	float GetRadius() const { return m_Radius; }

protected:
	RigidBody* m_pRigidBody = nullptr;
	float m_Radius = 1.f;
	Elite::Color m_BodyColor = { 1,1,0,1 };

private:

	//C++ make the class non-copyable
	BaseAgent(const BaseAgent&) {};
	BaseAgent& operator=(const BaseAgent&) {};
};
#endif