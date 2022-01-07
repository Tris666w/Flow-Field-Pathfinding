/*=============================================================================*/
// Copyright 2017-2018 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// ENavigation.h: General Navigation header that includes all navigation pathfinders and utilities
/*=============================================================================*/
#ifndef ELITE_NAVIGATION
#define ELITE_NAVIGATION

/* --- TYPE DEFINES --- */
//Function pointer type to heuristic function
namespace Elite
{
	typedef float(*Heuristic)(float, float);
}

/* --- UTILITIES --- */
//Utilities
#include "EHeuristicFunctions.h"
#endif