#pragma once

#include <iostream>
#include <chrono>
#include <windows.h> 

namespace Logger {

	static bool showSuccess = true;
	static bool showWarning = true;
	static bool showError = true;
	static bool showInfo = true;
	static unsigned __int64 sectionStartTime;
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	static unsigned __int64 getSysTime() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	static void logSuccess(const char* category, const char* message) {
		if (showSuccess) {
			SetConsoleTextAttribute(hConsole, 10);
			std::cout << "[" << category << "] " << message << " finished sucessfully. Took " << getSysTime() - sectionStartTime << " ms." << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
		}
	}

	static void logWarning(const char* category, const char* message, bool timed, const char* warning = "") {
		if (showWarning) {
			SetConsoleTextAttribute(hConsole, 14);
			if (timed) {
				std::cout << "[" << category << "] " << message << " finished. Took " << getSysTime() - sectionStartTime << " ms." << std::endl;
				std::cout <<  "Warning: " << warning << std::endl;
			}
			else {
				std::cout << "[" << category << "] " << message << std::endl;
			}
			SetConsoleTextAttribute(hConsole, 15);
		}
	}

	static void logMessage(const char* category, const char* message) {
		SetConsoleTextAttribute(hConsole, 15);
		sectionStartTime = getSysTime();
		std::cout << "[" << category << "] " << message << std::endl;
	}

	static void logInfo(const char* category, const char* message) {
		if (showInfo) {
			SetConsoleTextAttribute(hConsole, 9);
			sectionStartTime = getSysTime();
			std::cout << "[" << category << "] " << message << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
		}
	}
	
	static void logError(const char* category, const char* message) {
		if (showError) {
			SetConsoleTextAttribute(hConsole, 12);
			std::cout << "[" << category << "] " << message << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
		}
	}
};
