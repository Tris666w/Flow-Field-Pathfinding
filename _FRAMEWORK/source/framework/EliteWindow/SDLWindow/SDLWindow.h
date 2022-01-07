/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// SDLWindow.h: SDL2 Window implementation for Engine.
/*=============================================================================*/
#ifndef ELITE_SDLWINDOW
#define	ELITE_SDLWINDOW

//=== Includes & Forward Declarations ===

namespace Elite
{
	//Override the typedef with the correct type (by default void*)
	typedef SDL_Window* EliteRawWindow;

	//SDL specific window
	class SDLWindow final : public EWindowBase<SDLWindow>
	{
	public:
		//=== Constructors & Destructors ===
		SDLWindow() = default;;
		~SDLWindow();

		//=== Window Functions ===
		void CreateEWindow(const WindowParams& params);
		void SetWindowPosition(int x, int y) { SDL_SetWindowPosition(m_pWindow.get(), x, y); }
		void ProcedureEWindow();
		void ResizeEWindow(unsigned int width, unsigned int height);
		void RequestShutdown();

		//Retrieves the handle to the raw window, handle with caution!
		EliteRawWindow GetRawWindowHandle() const { return m_pWindow.get(); };

	private:
		//=== Private Struct ===
		struct SDL_WindowDeleter //Deleter to be able to use smart pointer
		{ void operator()(SDL_Window* pWindow) { SDL_DestroyWindow(pWindow); } };

		//=== Datamembers ===
		unique_ptr<SDL_Window, SDL_WindowDeleter> m_pWindow = nullptr;

		//=== Internal Functions ===
	};
}
#endif