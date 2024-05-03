//
// Created by ianpo on 17/06/2023.
//

#pragma once

#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <memory>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Voxymore::Core {

    class Log {
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

#ifdef VXM_LOG

#define VXM_CORE_TRACE(...)       ::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::trace, __VA_ARGS__)
#define VXM_CORE_INFO(...)        ::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::info, __VA_ARGS__)
#define VXM_CORE_WARNING(...)     ::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::warn, __VA_ARGS__)
#define VXM_CORE_WARN(...)     ::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::warn, __VA_ARGS__)
#define VXM_CORE_ERROR(...)       ::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::err, __VA_ARGS__)
#define VXM_CORE_CRITICAL(...)    ::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::critical, __VA_ARGS__)


#define VXM_TRACE(...)       ::Voxymore::Core::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::trace, __VA_ARGS__)
#define VXM_INFO(...)        ::Voxymore::Core::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::info, __VA_ARGS__)
#define VXM_WARNING(...)     ::Voxymore::Core::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::warn, __VA_ARGS__)
#define VXM_WARN(...)     ::Voxymore::Core::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::warn, __VA_ARGS__)
#define VXM_ERROR(...)       ::Voxymore::Core::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::err, __VA_ARGS__)
#define VXM_CRITICAL(...)    ::Voxymore::Core::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::critical, __VA_ARGS__)

#else

#define VXM_CORE_TRACE(...)
#define VXM_CORE_INFO(...)
#define VXM_CORE_WARNING(...)
#define VXM_CORE_WARN(...)
#define VXM_CORE_ERROR(...)
#define VXM_CORE_CRITICAL(...)


#define VXM_TRACE(...)
#define VXM_INFO(...)
#define VXM_WARNING(...)
#define VXM_WARN(...)
#define VXM_ERROR(...)
#define VXM_CRITICAL(...)

#endif
