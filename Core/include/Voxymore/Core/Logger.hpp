//
// Created by ianpo on 17/06/2023.
//

#pragma once

#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <spdlog/spdlog.h>
#include "LoopBackBuffer.hpp"

namespace Voxymore::Core {

    class Log {
    public:
        enum Level {
            Debug,
            Trace,
            Info,
            Warning,
            Error,
            Critical,
        };

        struct Message {
            std::string message;
            Level level;
        };

        inline static constexpr uint64_t MessageBufferCount = 100;
        using MessageBuffer = LoopBackBuffer<Message, MessageBufferCount>;
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    public:
        inline static MessageBuffer::const_iterator begin() {return s_LastLogs.cbegin();}
        inline static MessageBuffer::const_iterator end() {return s_LastLogs.cend();}
        inline static uint64_t buffer_log_count() {return s_LastLogs.size();}
    private:
        static void OnSpdlogCallback(const spdlog::details::log_msg& msg);
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
        inline static MessageBuffer s_LastLogs{};
    };
}

#ifdef VXM_LOG

#define VXM_CORE_LOG_DEBUG(...)       ::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::debug, __VA_ARGS__)
#define VXM_CORE_TRACE(...)       ::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::trace, __VA_ARGS__)
#define VXM_CORE_INFO(...)        ::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::info, __VA_ARGS__)
#define VXM_CORE_WARNING(...)     ::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::warn, __VA_ARGS__)
#define VXM_CORE_WARN(...)     ::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::warn, __VA_ARGS__)
#define VXM_CORE_ERROR(...)       ::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::err, __VA_ARGS__)
#define VXM_CORE_CRITICAL(...)    ::Voxymore::Core::Log::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::critical, __VA_ARGS__)


#define VXM_LOG_DEBUG(...)       ::Voxymore::Core::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::debug, __VA_ARGS__)
#define VXM_TRACE(...)       ::Voxymore::Core::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::trace, __VA_ARGS__)
#define VXM_INFO(...)        ::Voxymore::Core::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::info, __VA_ARGS__)
#define VXM_WARNING(...)     ::Voxymore::Core::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::warn, __VA_ARGS__)
#define VXM_WARN(...)     ::Voxymore::Core::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::warn, __VA_ARGS__)
#define VXM_ERROR(...)       ::Voxymore::Core::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::err, __VA_ARGS__)
#define VXM_CRITICAL(...)    ::Voxymore::Core::Log::GetClientLogger()->log(spdlog::source_loc{__FILE__, __LINE__, VXM_FUNC}, spdlog::level::critical, __VA_ARGS__)

#else

#define VXM_CORE_DEBUG(...)
#define VXM_CORE_TRACE(...)
#define VXM_CORE_INFO(...)
#define VXM_CORE_WARNING(...)
#define VXM_CORE_WARN(...)
#define VXM_CORE_ERROR(...)
#define VXM_CORE_CRITICAL(...)


#define VXM_DEBUG(...)
#define VXM_TRACE(...)
#define VXM_INFO(...)
#define VXM_WARNING(...)
#define VXM_WARN(...)
#define VXM_ERROR(...)
#define VXM_CRITICAL(...)

#endif
