//
// Created by ianpo on 17/06/2023.
//

#include "Logger.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Voxymore::Core {
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init()
    {
        // Setting Pattern
        spdlog::set_pattern("%^[%T] [%l] %n (%g:%#->%!): %v%$");

        // Creating core logger
        s_CoreLogger = spdlog::stdout_color_mt("VOXYMORE");
        s_CoreLogger->set_level(spdlog::level::trace);
        // Creating Client logger
        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
    }
}