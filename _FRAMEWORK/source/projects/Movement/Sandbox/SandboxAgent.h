#ifndef SANDBOX_AGENT_H
#define SANDBOX_AGENT_H
#include "projects/Shared/BaseAgent.h"

class SandboxAgent:public BaseAgent
{
public:
	SandboxAgent();
	~SandboxAgent() = default;

	//Functions
	void Update(float dt) override;
	void Render(float dt) override;

	void SetTarget(const Elite::Vector2& target) { m_Target = target; }

private:

	//Functions
	float Orientation(float currentOrientation, Elite::Vector2 velocity) const;

	//Members
	Elite::Vector2 m_Target = {};
};
#endif