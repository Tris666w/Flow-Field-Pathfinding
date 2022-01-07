/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// ERendering.h: General Rendering header
/*=============================================================================*/
#ifndef ELITE_RENDERING
#define ELITE_RENDERING

/* --- FORWARD DECLARED TYPES ---*/
typedef void* EliteRenderingContext;

/* --- INCLUDES --- */
#include "ERenderingTypes.h"
#include "EFrameBase.h"
#include "2DCamera/ECamera2D.h"
#include "EDebugRenderer2D.h"

/* --- PLATFORM-SPECIFIC DEFINES & INCLUDES --- */
#if (PLATFORM_ID == PLATFORM_WINDOWS)
	#include "SDLIntegration/SDLDebugRenderer2D/SDLDebugRenderer2D.h"
	typedef Elite::SDLDebugRenderer2D EliteDebugRenderer2D;
	#include "SDLIntegration/SDLFrame/SDLFrame.h"
	typedef Elite::SDLFrame EliteFrame;
#elif (PLATFORM_ID == PLATFORM_PS4)
	//Nothing
#endif
#endif