#pragma once

namespace peach
{
	enum class LogLevel
	{
		Info = 0x1,
		Warning = 0x2,
		Error = 0x4
	};

	class Logger
	{
	public:
		static void Initialize();
		static void Release();
		static void Log(LogLevel level, const std::string& msg);
		static void LogInfo(const std::string& msg);
		static void LogWarning(const std::string& msg);
		static void LogError(const std::string& msg);

	private:
		static std::ostream* m_pOutputSteam;
	};
}
