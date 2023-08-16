//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session 
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//
#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

#include "Voxymore/Core/Macros.hpp"

namespace Voxymore::Core {

    struct ProfileResult {
        std::string Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    struct InstrumentationSession {
        std::string Name;
    };

    class Instrumentor {
    private:
        InstrumentationSession *m_CurrentSession;
        std::ofstream m_OutputStream;
        int m_ProfileCount;
    public:
        inline Instrumentor()
                : m_CurrentSession(nullptr), m_ProfileCount(0) {
        }

        inline void BeginSession(const std::string &name, const std::string &filepath = "results.json") {
            m_OutputStream.open(filepath);
            WriteHeader();
            m_CurrentSession = new InstrumentationSession{name};
        }

        inline void EndSession() {
            WriteFooter();
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
            m_ProfileCount = 0;
        }

        inline void WriteProfile(const ProfileResult &result) {
            if (m_ProfileCount++ > 0)
                m_OutputStream << ",";

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_OutputStream << "{";
            m_OutputStream << "\"cat\":\"function\",";
            m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << result.ThreadID << ",";
            m_OutputStream << "\"ts\":" << result.Start;
            m_OutputStream << "}";

            m_OutputStream.flush();
        }

        inline void WriteHeader() {
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
            m_OutputStream.flush();
        }

        inline void WriteFooter() {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        inline static Instrumentor &Get() {
            static Instrumentor instance;
            return instance;
        }
    };

    class InstrumentationTimer {
    public:
        inline InstrumentationTimer(const char *name)
                : m_Name(name), m_Stopped(false) {
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        inline ~InstrumentationTimer() {
            if (!m_Stopped)
                Stop();
        }

        inline void Stop() {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(
                    m_StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(
                    endTimepoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile({m_Name, start, end, threadID});

            m_Stopped = true;
        }

    private:
        const char *m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
    };
}

#if VXM_PROFILING
    #define VXM_PROFILE_CONCAT(x, y) x ## y
    #define VXM_PROFILE_C(x, y) VXM_PROFILE_CONCAT(x, y)

    #define VXM_PROFILE_BEGIN_SESSION(name, filepath) ::Voxymore::Core::Instrumentor::Get().BeginSession(name, filepath)
    #define VXM_PROFILE_END_SESSION() ::Voxymore::Core::Instrumentor::Get().EndSession()
    #define VXM_PROFILE_SCOPE(name) ::Voxymore::Core::InstrumentationTimer VXM_PROFILE_C(timer, __LINE__)(name)
    #define VXM_PROFILE_FUNCTION() VXM_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define VXM_PROFILE_BEGIN_SESSION(name, filepath)
    #define VXM_PROFILE_END_SESSION()
    #define VXM_PROFILE_SCOPE(name)
    #define VXM_PROFILE_FUNCTION()
#endif