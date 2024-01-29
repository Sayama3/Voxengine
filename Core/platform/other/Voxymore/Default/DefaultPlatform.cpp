//
// Created by ianpo on 06/09/2023.
//

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Utils/Platform.hpp"
#include "portable-file-dialogs.h"

namespace Voxymore::Core {

	std::string Clipboard::s_StrBuffer = "";

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
		s_StrBuffer = str;
	}

	std::string Clipboard::Get()
	{
		return s_StrBuffer;
	}

	void Clipboard::Clear()
	{
		s_StrBuffer.clear();
	}
}