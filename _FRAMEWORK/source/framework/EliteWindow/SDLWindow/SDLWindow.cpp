//=== General Includes ===
#include "stdafx.h"
#include "SDLWindow.h"
using namespace Elite;

//=== Constructors & Destructors ===
SDLWindow::~SDLWindow()
{
	if (m_pWindow)
		SDL_DestroyWindow(m_pWindow.get());
}

//=== Window Functions ===
void SDLWindow::CreateEWindow(const WindowParams& params)
{
	//Store the current parameters
	m_WindowParameters = params;

	//Initialize SDL Video, which also automatically initializes the events subsystem. Returns 0 on success!
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		throw Elite_Exception("SDL_Init failed! Cannot create window!");

	//Set SDL Attributes for our OpenGl Context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	//Create flags
	unsigned int flags = SDL_WINDOW_OPENGL; //Default, this is a windows using opengl 
	if (params.isResizable)
		flags |= SDL_WINDOW_RESIZABLE;

	//Create window and store pointer (always centered at initialization)
	m_pWindow = unique_ptr<SDL_Window, SDL_WindowDeleter>(SDL_CreateWindow(
		params.windowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		params.width, params.height,flags));

	if(!m_pWindow.get())
		throw Elite_Exception("SDL_CreateWindow! Could not create window!");
}

void SDLWindow::ProcedureEWindow()
{
	SDL_Event e;
	EInputManager::GetInstance()->Flush(); //Flush before refilling again!
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			//If user closes the window
			case SDL_QUIT:
			{
				//Request shutdown by flagging base ShutdownRequested datamember! This will be used in the main for the game loop!
				m_ShutdownRequested = true;
				break;
			}
			case SDL_KEYDOWN:
			{
				auto data = KeyboardData(
					static_cast<int>(e.key.timestamp),
					static_cast<Elite::InputScancode>(e.key.keysym.scancode));

				EInputManager::GetInstance()->AddInputAction(InputAction(
					InputType::eKeyboard, InputState::eDown, InputData(data)));
				break;
			}
			case SDL_KEYUP:
			{
				auto data = KeyboardData(
					static_cast<int>(e.key.timestamp),
					static_cast<Elite::InputScancode>(e.key.keysym.scancode));

				EInputManager::GetInstance()->AddInputAction(InputAction(
					InputType::eKeyboard, InputState::eReleased, InputData(data)));
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				//SDL_GetMouseState -> Relative to Desktop, not Window
				int x, y;
				SDL_GetMouseState(&x, &y);

				auto data = MouseData(
					static_cast<int>(e.key.timestamp),
					static_cast<InputMouseButton>(e.button.button),
					x, y);

				EInputManager::GetInstance()->AddInputAction(InputAction(
					InputType::eMouseButton, InputState::eDown, InputData(data)));
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				int x, y;
				SDL_GetMouseState(&x, &y);

				auto data = MouseData(
					static_cast<int>(e.key.timestamp),
					static_cast<InputMouseButton>(e.button.button),
					x, y);

				EInputManager::GetInstance()->AddInputAction(InputAction(
					InputType::eMouseButton, InputState::eReleased, InputData(data)));
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				auto data = MouseData(
					static_cast<int>(e.key.timestamp),
					InputMouseButton(0), e.wheel.x, e.wheel.y, 0, 0);

				EInputManager::GetInstance()->AddInputAction(InputAction(
					InputType::eMouseWheel, InputState(0), InputData(data)));
				break;
			}
			case SDL_MOUSEMOTION:
			{
				int x, y;
				SDL_GetMouseState(&x, &y);

				auto data = MouseData(
					static_cast<int>(e.key.timestamp),
					InputMouseButton(0), x, y, e.motion.xrel, e.motion.yrel);

				EInputManager::GetInstance()->AddInputAction(InputAction(
					InputType::eMouseMotion, InputState(0), InputData(data)));
				break;
			}
		}
	}
}

void SDLWindow::ResizeEWindow(unsigned int width, unsigned int height)
{
	//https://wiki.libsdl.org/SDL_WindowEventID
	//SDL_WINDOWEVENT_SIZE_CHANGED || SDL_WINDOWEVENT_RESIZED
}

void SDLWindow::RequestShutdown()
{
	SDL_Event e;
	e.type = SDL_QUIT;
	SDL_PushEvent(&e);
}
