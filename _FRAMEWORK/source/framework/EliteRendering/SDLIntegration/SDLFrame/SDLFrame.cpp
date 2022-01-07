//=== General Includes ===
#include "stdafx.h"
#include "SDLFrame.h"
#include "../../ERendering.h"
#include "../../../EliteUI/EImmediateUI.h"
using namespace Elite;

//=== Constructors & Destructors ===
SDLFrame::~SDLFrame()
{
	//Destroy Context
	SDL_GL_DeleteContext(m_Context);
}

//=== Window Functions ===
void SDLFrame::CreateFrame(EliteWindow* pWindow)
{
	//Store EliteWindow Handle
	m_pWindow = pWindow;

	//Create OpenGL context based, with raw window handle (only works with single window)
	m_Context = SDL_GL_CreateContext(m_pWindow->GetRawWindowHandle());

	//Initialize OpenGL Wrapper after Context Creation
	gl3wInit();

	//Enable VSync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{ printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError()); }

	//Check for errors
	GLenum errCode = glGetError();
	if (errCode != GL_NO_ERROR)
	{
		fprintf(stderr, "OpenGL error = %d\n", errCode);
		assert(false);
	}
}

void SDLFrame::SubmitAndFlipFrame(EImmediateUI* pImmediateUI)
{
	//Set Viewport Size
	auto windowParams = m_pWindow->GetCurrentWindowParameters();
	glViewport(0, 0, windowParams.width, windowParams.height);

	//Set clear color and clear current render target
	glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render all current scenegraph elements
	DEBUGRENDERER2D->Render();
	if (pImmediateUI)
		pImmediateUI->Render();

	//Swap buffers (aka Flip)
	SDL_GL_SwapWindow(m_pWindow->GetRawWindowHandle());
}