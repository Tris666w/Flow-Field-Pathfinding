#include "stdafx.h"
#include "Logger.h"

std::ostream* peach::Logger::m_pOutputSteam = nullptr;


void peach::Logger::Initialize()
{
	//m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	m_pOutputSteam = &std::cout;

}

void peach::Logger::Release()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void peach::Logger::Log(LogLevel level, const std::string& msg)
{
	std::stringstream sstream;

	switch (level)
	{
	case LogLevel::Info:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		sstream << "[INFO]    ";
		break;
	case LogLevel::Warning:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		sstream << "[WARNING] ";
		break;
	case LogLevel::Error:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		sstream << "[ERROR]   ";
		break;
	}

	sstream << msg;
	sstream << "\n";

	//if error, break
	if (level == LogLevel::Error)
	{
		MessageBox(0, msg.c_str(), "ERROR", 0);
	}


	(*m_pOutputSteam) << sstream.str();
	m_pOutputSteam->flush();

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

}

void peach::Logger::LogInfo(const std::string& msg)
{
	Log(LogLevel::Info, msg);
}

void peach::Logger::LogWarning(const std::string& msg)
{
	Log(LogLevel::Warning, msg);
}

void peach::Logger::LogError(const std::string& msg)
{
	Log(LogLevel::Error, msg);
}
