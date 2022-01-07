#ifndef SANDBOX_APPLICATION_H
#define SANDBOX_APPLICATION_H
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "framework/EliteInterfaces/EIApp.h"

class Renderer;
class SandboxAgent;

//-----------------------------------------------------------------
// Application
//-----------------------------------------------------------------
class App_Sandbox final : public IApp
{
public:
	//Constructor & Destructor
	App_Sandbox() = default;
	virtual ~App_Sandbox() final;

	//App Functions
	void Start() override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) const override;

private:

	//C++ make the class non-copyable
	App_Sandbox(const App_Sandbox&) = delete;
	App_Sandbox& operator=(const App_Sandbox&) = delete;
};
#endif