//
// Created by ianpo on 17/06/2023.
//

#ifndef LEARNOPENGL_LOGGER_HPP
#define LEARNOPENGL_LOGGER_HPP

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

namespace Voxymore::Core {

    class VXM_API Log {
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

}

#define VXM_CORE_TRACE(...)       SPDLOG_LOGGER_TRACE(::Voxymore::Core::Log::GetCoreLogger(), __VA_ARGS__)
#define VXM_CORE_INFO(...)        SPDLOG_LOGGER_INFO(::Voxymore::Core::Log::GetCoreLogger(), __VA_ARGS__)
#define VXM_CORE_WARNING(...)     SPDLOG_LOGGER_WARNING(::Voxymore::Core::Log::GetCoreLogger(), __VA_ARGS__)
#define VXM_CORE_ERROR(...)       SPDLOG_LOGGER_ERROR(::Voxymore::Core::Log::GetCoreLogger(), __VA_ARGS__)
#define VXM_CORE_CRITICAL(...)    SPDLOG_LOGGER_CRITICAL(::Voxymore::Core::Log::GetCoreLogger(), __VA_ARGS__)


#define VXM_TRACE(...)       SPDLOG_LOGGER_TRACE(::Voxymore::Core::Log::GetClientLogger(), __VA_ARGS__)
#define VXM_INFO(...)        SPDLOG_LOGGER_INFO(::Voxymore::Core::Log::GetClientLogger(), __VA_ARGS__)
#define VXM_WARNING(...)     SPDLOG_LOGGER_WARNING(::Voxymore::Core::Log::GetClientLogger(), __VA_ARGS__)
#define VXM_ERROR(...)       SPDLOG_LOGGER_ERROR(::Voxymore::Core::Log::GetClientLogger(), __VA_ARGS__)
#define VXM_CRITICAL(...)    SPDLOG_LOGGER_CRITICAL(::Voxymore::Core::Log::GetClientLogger(), __VA_ARGS__)

#endif //LEARNOPENGL_LOGGER_HPP
