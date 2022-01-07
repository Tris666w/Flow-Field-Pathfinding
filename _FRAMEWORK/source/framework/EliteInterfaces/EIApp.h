#ifndef BASE_APPLICATION_H
#define BASE_APPLICATION_H
//-----------------------------------------------------------------
// Application Base
//-----------------------------------------------------------------
class IApp
{
public:
	//Constructor & Destructor
	IApp() = default;
	virtual ~IApp() = default;

	//App Functions
	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(float deltaTime) const = 0;

protected:
	//Datamembers

private:
	//C++ make the class non-copyable
	IApp(const IApp&){};
	IApp& operator=(const IApp&){};
};
#endif