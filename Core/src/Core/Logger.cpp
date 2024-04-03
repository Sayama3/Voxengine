//
// Created by ianpo on 17/06/2023.
//

#include "Voxymore/Core/Logger.hpp"
#include "spdlog/sinks/basic_file_sink.h"
//#include "spdlog/sinks/daily_file_sink.h"


namespace Voxymore::Core {
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init()
    {
        // Setting Pattern
        spdlog::set_pattern("%^[%T] [%l] %n (%s:%#->%!): %v%$");

        // Creating core logger
		std::array<spdlog::sink_ptr, 2> core_sinks;
		core_sinks[0] = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		core_sinks[1] = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Voxymore.log");

        s_CoreLogger = std::make_shared<spdlog::logger>("VOXYMORE", core_sinks.begin(), core_sinks.end());
        s_CoreLogger->set_level(spdlog::level::trace);

        // Creating Client logger
		std::array<spdlog::sink_ptr, 2> client_sinks;
		core_sinks[0] = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		core_sinks[1] = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Application.log");

        s_ClientLogger = std::make_shared<spdlog::logger>("APP", client_sinks.begin(), client_sinks.end());
        s_ClientLogger->set_level(spdlog::level::trace);
    }
}