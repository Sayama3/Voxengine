#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>

#include "Voxymore/Core/Macros.hpp"

#define TracyFunction VXM_FUNC

#include <tracy/Tracy.hpp>

#if VXM_PROFILING

#define VXM_PROFILE_BEGIN_SESSION(name, filepath) //::Voxymore::Core::Instrumentor::Get().BeginSession(name, filepath)
#define VXM_PROFILE_END_SESSION() //::Voxymore::Core::Instrumentor::Get().EndSession()
#define VXM_FRAME_START() FrameMark//FrameMarkStart(s_MainFrame)
#define VXM_FRAME_END() //FrameMarkEnd(s_MainFrame)
#define VXM_PROFILE_SCOPE(name) ZoneScopedN(name)
#define VXM_PROFILE_FUNCTION() ZoneScoped

#else

#define VXM_PROFILE_BEGIN_SESSION(name, filepath)
#define VXM_PROFILE_END_SESSION()
#define VXM_FRAME_START()
#define VXM_FRAME_END()
#define VXM_PROFILE_SCOPE(name)
#define VXM_PROFILE_FUNCTION()

#endif