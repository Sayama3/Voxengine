//
// Created by ianpo on 06/09/2023.
//

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Application.hpp"
#include "Voxymore/Utils/Platform.hpp"
#include "portable-file-dialogs.h"
#include <GLFW/glfw3.h>

namespace Voxymore::Core {

	std::string FileDialogs::OpenFile(const std::vector<std::string>& filter)
	{
		std::vector<std::string> result = pfd::open_file("Select a file", ".", filter).result();
		if(result.empty()) return {};
		else return result[0];
	}

	std::string FileDialogs::SaveFile(const std::vector<std::string>& filter)
	{
		std::string result = pfd::save_file("Select a file", ".", filter).result();
		return result;
	}

	void Clipboard::Set(const std::string& str)
	{
		auto* nativeWindow = reinterpret_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetClipboardString(nativeWindow, str.c_str());
	}

	std::string Clipboard::Get()
	{
		auto* nativeWindow = reinterpret_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetClipboardString(nativeWindow);
	}

	void Clipboard::Clear()
	{
		auto* nativeWindow = reinterpret_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetClipboardString(nativeWindow, "");
	}
}