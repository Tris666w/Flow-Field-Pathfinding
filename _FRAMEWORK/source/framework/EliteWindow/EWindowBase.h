/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EWindowBase.h: base class representing a window (or screen) for Engine.
/*=============================================================================*/
#ifndef ELITE_WINDOW_BASE
#define	ELITE_WINDOW_BASE

//=== Includes & Forward Declarations ===
//struct Vector2;

namespace Elite
{
	//=== Window Parameters ===
	struct WindowParams
	{
		std::string windowTitle = "AI Elite Framework";
		unsigned int width = 901;
		unsigned int height = 451;
		bool isResizable = false;
	};

	template<typename Impl>
	class EWindowBase
	{
	public:
		//=== Constructors & Destructors ===
		EWindowBase() {};
		~EWindowBase() {};

		//=== Window Functions ===
		void CreateEWindow(const WindowParams& params);
		void DestroyEWindow();
		void ProcedureEWindow();
		void ResizeEWindow(unsigned int width, unsigned int height);
		void RequestShutdown();

		bool ShutdownRequested() { return m_ShutdownRequested; }
		WindowParams GetCurrentWindowParameters() { return m_WindowParameters; }
		EliteRawWindow GetRawWindowHandle();

	protected:
		//=== Datamembers ===
		WindowParams m_WindowParameters = {};
		bool m_ShutdownRequested = false;
	};
}
#endif