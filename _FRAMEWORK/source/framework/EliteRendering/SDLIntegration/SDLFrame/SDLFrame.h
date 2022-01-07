/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// SDLFrame.h: SDL2 Frame implementation for Engine.
/*=============================================================================*/
#ifndef ELITE_SDLFRAME
#define	ELITE_SDLFRAME
namespace Elite
{
	//Override the typedef with the correct type (by default void*)
	typedef SDL_GLContext EliteRenderingContext;

	//TODO temporarly Immediate UI implementation in SubmitAndFlipFrame()
	class EImmediateUI;

	//SDL specific renderer
	class SDLFrame final : public EFrameBase<SDLFrame>
	{
	public:
		//=== Constructors & Destructors ===
		SDLFrame() {};
		~SDLFrame();

		//=== Functions ===
		void CreateFrame(EliteWindow* pWindow);
		void SubmitAndFlipFrame(EImmediateUI* pImmediateUI = nullptr);

	private:

	};
}
#endif