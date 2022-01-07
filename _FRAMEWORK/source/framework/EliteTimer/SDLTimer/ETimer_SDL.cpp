/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Thomas Goussaert, Matthieu Delaere
/*=============================================================================*/
// ETimer_SDL.cpp: SDL implementatoin of the ETimer. 
// Works on Windows, Mac OS X, Linux, iOS, and Android.
/*=============================================================================*/
#include "stdafx.h"
#include "../ETimer.h"

template<>
Elite::ETimer<PLATFORM_WINDOWS>::ETimer()
{
	long long countsPerSecond = SDL_GetPerformanceFrequency();
	m_SecondsPerCount = 1.0f / (float)countsPerSecond;
}

template<>
void Elite::ETimer<PLATFORM_WINDOWS>::Reset()
{
	long long currentTime = SDL_GetPerformanceCounter();

	m_BaseTime = currentTime;
	m_PreviousTime = currentTime;
	m_StopTime = 0;
	m_FPSTimer = 0.0f;
	m_FPSCount = 0;
	m_IsStopped = false;
}

template<>
void Elite::ETimer<PLATFORM_WINDOWS>::Start()
{
	long long startTime = SDL_GetPerformanceCounter();

	if (m_IsStopped)
	{
		m_PausedTime += (startTime - m_StopTime);

		m_PreviousTime = startTime;
		m_StopTime = 0;
		m_IsStopped = false;
	}
}

template<>
void Elite::ETimer<PLATFORM_WINDOWS>::Update()
{
	if (m_IsStopped)
	{
		m_FPS = 0;
		m_ElapsedTime = 0.0f;
		m_TotalTime = (float)(((m_StopTime - m_PausedTime) - m_BaseTime) * m_BaseTime);
		return;
	}

	long long currentTime = SDL_GetPerformanceCounter();
	m_CurrentTime = currentTime;

	m_ElapsedTime = (float)((m_CurrentTime - m_PreviousTime) * m_SecondsPerCount);
	m_PreviousTime = m_CurrentTime;

	if (m_ElapsedTime < 0.0f)
		m_ElapsedTime = 0.0f;

	if (m_ForceElapsedUpperBound && m_ElapsedTime > m_ElapsedUpperBound)
	{
		m_ElapsedTime = m_ElapsedUpperBound;
	}

	m_TotalTime = (float)(((m_CurrentTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);

	//FPS LOGIC
	m_FPSTimer += m_ElapsedTime;
	++m_FPSCount;
	if (m_FPSTimer >= 1.0f)
	{
		m_FPS = m_FPSCount;
		m_FPSCount = 0;
		m_FPSTimer -= 1.0f;
	}
}

template<>
void Elite::ETimer<PLATFORM_WINDOWS>::Stop()
{
	if (!m_IsStopped)
	{
		long long currentTime = SDL_GetPerformanceCounter();

		m_StopTime = currentTime;
		m_IsStopped = true;
	}
}