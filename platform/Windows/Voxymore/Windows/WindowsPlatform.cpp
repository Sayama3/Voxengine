//
// Created by ianpo on 29/08/2023.
//

//
// Created by ianpo on 29/08/2023.
//

#include "Voxymore/Utils/Platform.hpp"
#include "portable-file-dialogs.h"

namespace Voxymore::Core {

	std::string FileDialogs::OpenFile(const std::vector<std::string>& filter)
	{
		std::vector<std::string> result = pfd::open_file("Select a file", ".", filter).result();
		if(result.empty()) return std::string();
		else return result[0];
	}

	std::string FileDialogs::SaveFile(const std::vector<std::string>& filter)
	{
		std::vector<std::string> result = pfd::open_file("Select a file", ".", filter).result();
		if(result.empty()) return std::string();
		else return result[0];
	}
}

//TODO: use the native windows API

//#include "Voxymore/Core/Application.hpp"
//#include "Voxymore/Core/Core.hpp"
//#include "Voxymore/Utils/Platform.hpp"
//#include <GLFW/glfw3.h>
//#include <commdlg.h>
//#define GLFW_EXPOSE_NATIVE_WIN32
//#include <GLFW/glfw3native.h>
//#include <numeric>
//
//namespace Voxymore::Core {
//
//		std::string FileDialogs::OpenFile(const std::vector<std::string>& filters)
//		{
//			std::string filter = std::accumulate(filters.begin(), filters.end(), std::string("\0"));
//			OPENFILENAMEA ofn;
//			CHAR szFile[260] = { 0 };
//			CHAR currentDir[256] = { 0 };
//			ZeroMemory(&ofn, sizeof(OPENFILENAME));
//			ofn.lStructSize = sizeof(OPENFILENAME);
//			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
//			ofn.lpstrFile = szFile;
//			ofn.nMaxFile = sizeof(szFile);
//			if (GetCurrentDirectoryA(256, currentDir))
//				ofn.lpstrInitialDir = currentDir;
//			ofn.lpstrFilter = filter.c_str();
//			ofn.nFilterIndex = 1;
//			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
//
//			if (GetOpenFileNameA(&ofn) == TRUE)
//				return ofn.lpstrFile;
//
//			return std::string();
//		}
//
//		std::string FileDialogs::SaveFile(const std::vector<std::string>& filters)
//		{
//			std::string filter = std::accumulate(filters.begin(), filters.end(), std::string("\0"));
//			OPENFILENAMEA ofn;
//			CHAR szFile[260] = { 0 };
//			CHAR currentDir[256] = { 0 };
//			ZeroMemory(&ofn, sizeof(OPENFILENAME));
//			ofn.lStructSize = sizeof(OPENFILENAME);
//			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
//			ofn.lpstrFile = szFile;
//			ofn.nMaxFile = sizeof(szFile);
//			if (GetCurrentDirectoryA(256, currentDir))
//				ofn.lpstrInitialDir = currentDir;
//			ofn.lpstrFilter = filter.c_str();
//			ofn.nFilterIndex = 1;
//			ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
//
//			// Sets the default extension by extracting it from the filter
//			ofn.lpstrDefExt = strchr(filter.c_str(), '\0') + 1;
//
//			if (GetSaveFileNameA(&ofn) == TRUE)
//				return ofn.lpstrFile;
//
//			return std::string();
//		}
//}