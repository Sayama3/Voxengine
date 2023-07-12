//
// Created by ianpo on 17/06/2023.
//

#ifndef LEARNOPENGL_LOGGER_HPP
#define LEARNOPENGL_LOGGER_HPP

#include <string>
#include "glm/gtx/string_cast.hpp"

#define ERR_LOCATION (std::string(__FILE__).append(":").append(std::to_string(__LINE__)).append(" -> ").append(__FUNCTION__))

#define LOG_FILE_NAME "log_file.log"
#define OLD_LOG_FILE_NAME "log_file.log.old"
namespace Voxymore::Core {

    enum LogType {
        Log,
        Warning,
        Error,
    };

    std::string LogTypeToString(LogType logType);

    class Logger {
    public:
        static void Log(const std::string &log, const std::string &location);

        static void LogWarning(const std::string &log, const std::string &location);

        static void LogError(const std::string &log, const std::string &location);

    private:
        inline static bool init = false;

        static void Log(const std::string &log, LogType logType, const std::string &location);
    };
}
#define LOG(x) Logger::Log((x), ERR_LOCATION)
#define LOG_WARNING(x) Logger::LogWarning((x), ERR_LOCATION)
#define LOG_ERROR(x) Logger::LogError((x), ERR_LOCATION)

#endif //LEARNOPENGL_LOGGER_HPP
