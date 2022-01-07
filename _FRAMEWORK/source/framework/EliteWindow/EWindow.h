/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EWindow.h: General Window Handling header
/*=============================================================================*/
#ifndef ELITE_WINDOW
#define ELITE_WINDOW

/* --- FORWARD DECLARED TYPES ---*/
typedef void* EliteRawWindow;

/* --- INCLUDES --- */
#include "EWindowBase.h"

/* --- PLATFORM-SPECIFIC DEFINES & INCLUDES --- */
#if (PLATFORM_ID == PLATFORM_WINDOWS)
	#include "SDLWindow/SDLWindow.h"
	typedef Elite::SDLWindow EliteWindow;
#elif (PLATFORM_ID == PLATFORM_PS4)
//Nothing
#endif

#endif