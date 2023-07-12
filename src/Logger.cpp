//
// Created by ianpo on 17/06/2023.
//

#include "Logger.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace Voxymore::Core {
    void Logger::Log(const std::string &log, const std::string &location) {
        Log(log, LogType::Log, location);
    }

    void Logger::LogWarning(const std::string &log, const std::string &location) {
        Log(log, LogType::Warning, location);
    }

    void Logger::LogError(const std::string &log, const std::string &location) {
        Log(log, LogType::Error, location);
    }

    void Logger::Log(const std::string &log, LogType logType, const std::string &location) {
#ifdef DEBUG
        std::cout << "LOG::" << LogTypeToString(logType) << " " << location << " : " << log << std::endl;
#endif
        if (!init) {
            init = true;
            if (std::filesystem::exists(LOG_FILE_NAME)) {
                std::filesystem::rename(LOG_FILE_NAME, OLD_LOG_FILE_NAME);
            }
        }
        std::fstream logFile;
        logFile.open(LOG_FILE_NAME);
        logFile << "LOG::" << LogTypeToString(logType) << " " << location << " : " << log << std::endl;
        logFile.close();
    }

    std::string LogTypeToString(LogType logType) {
        switch (logType) {
            case LogType::Log:
                return "LOG";
            case LogType::Warning:
                return "WARNING";
            case LogType::Error:
                return "ERROR";
        }
        return "UNKNOWN";
    }
}