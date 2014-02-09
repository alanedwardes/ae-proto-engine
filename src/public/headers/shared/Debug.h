#pragma once

#include <stdarg.h>
#include <assert.h>
#include <iostream>
#include <ctime>

enum DebugMessageTypes_t
{
	MESSAGE_LOG,
	MESSAGE_DEBUG,
	MESSAGE_WARNING
};

static auto g_oDebugClockStart = std::clock();

class Debug
{
public:
	template<typename T, typename... Args>
	static void LogMessage(std::string format, T value, Args... args){
		Message(MESSAGE_LOG, format, value, args...);
	};
	static void LogMessage(std::string format){
		Message(MESSAGE_LOG, format, std::string());
	};
	
	template<typename T, typename... Args>
	static void DebugMessage(std::string format, T value, Args... args){
		Message(MESSAGE_DEBUG, format, value, args...);
	};
	static void DebugMessage(std::string format){
		Message(MESSAGE_DEBUG, format, std::string());
	}

	template<typename T, typename... Args>
	static void WarningMessage(std::string format, T value, Args... args){
		Message(MESSAGE_WARNING, format, value, args...);
	};
	static void WarningMessage(std::string format){
		Message(MESSAGE_WARNING, format, std::string());
	};

	template<typename T, typename... Args>
	static void Message(DebugMessageTypes_t messageType, std::string format, T value, Args... args)
	{
		auto oTime = (std::clock() - g_oDebugClockStart) / 1000.0f;

		switch (messageType)
		{
		case MESSAGE_LOG:
			Message("[%] LOG: ", oTime);
			break;
		case MESSAGE_DEBUG:
			Message("[%] DEBUG: ", oTime);
			break;
		case MESSAGE_WARNING:
			Message("[%] WARN: ", oTime);
			break;
		}

		Message(format, value, args...);
		Message("\n");
	}

	template<typename T>
	static void Message(T value) { std::cout << value; }

	template<typename T, typename... Args>
	static void Message(std::string format, T value, Args... args)
	{
		const char *s = format.c_str();
		while (*s)
		{
			if (*s == '%')
			{
				if (*(s + 1) == '%') ++s;
				else
				{
					Message(value);
					Message(s + 1, args...);
					return;
				}
			}
			Message(*s++);
		}
	}
};