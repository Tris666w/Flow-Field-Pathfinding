/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EFrameBase.h: base class of the renderer used in the Engine.
/*=============================================================================*/
#ifndef ELITE_FRAME_BASE
#define	ELITE_FRAME_BASE
namespace Elite
{
	template<typename Impl>
	class EFrameBase
	{
	public:
		//=== Constructors & Destructors ===
		EFrameBase() = default;
		~EFrameBase() = default;

		//=== Functions ===
		void CreateFrame(EliteWindow* pWindow);
		void SubmitAndFlipFrame();

	protected:
		EliteWindow* m_pWindow = nullptr;
		EliteRenderingContext m_Context;

		Color m_ClearColor = { 0.3f, 0.3f, 0.3f, 1.0f };
	};
}
#endif