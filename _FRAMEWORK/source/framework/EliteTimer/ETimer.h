/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Thomas Goussaert, Matthieu Delaere
/*=============================================================================*/
// ETimer.h: base template class that encapsulates the timer for the engine.
// Timing calculations are platform specific
/*=============================================================================*/
#ifndef ELITE_TIMER
#define	ELITE_TIMER
namespace Elite
{
	template <int PlatformID>
	class ETimer final : public ESingleton<ETimer<PlatformID> >
	{
	public:
		//=== Constructors & Destructors ===
		ETimer();
		~ETimer() {};

		//=== Timer Functions ===
		void Reset();
		void Start();
		void Update();
		void Stop();
		
		unsigned int GetFPS() const { return m_FPS; };
		float GetElapsed() const { return m_ElapsedTime; };
		float GetTotal() const { return m_TotalTime; };
		bool IsRunning() const { return !m_IsStopped; };
		
		void ForceElapsedUpperbound(bool force, float upperBound = 0.03f)
		{ m_ForceElapsedUpperBound = force; m_ElapsedUpperBound = upperBound; }
	
	private:
		//=== Datamembers ===
		float m_TotalTime = 0.0f;
		float m_ElapsedTime = 0.0f;
		float m_SecondsPerCount = 0.0f;

		float m_ElapsedUpperBound = 0.03f;
		bool m_ForceElapsedUpperBound = false;

		long long m_BaseTime = 0;
		long long m_PausedTime = 0;
		long long m_StopTime = 0;
		long long m_PreviousTime = 0;
		long long m_CurrentTime = 0;

		bool m_IsStopped = true;

		unsigned int m_FPS = 0;
		unsigned int m_FPSCount = 0;
		float m_FPSTimer = 0.0f;
	};
}
#endif

/* --- PLATFORM-SPECIFIC DEFINES & INCLUDES --- */
#if (PLATFORM_ID == PLATFORM_WINDOWS)
	typedef Elite::ETimer<PLATFORM_ID> EliteTimer;
	#elif (PLATFORM_ID == PLATFORM_PS4)
//Nothing
#endif