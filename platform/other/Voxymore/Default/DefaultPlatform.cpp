//
// Created by ianpo on 06/09/2023.
//

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Utils/Platform.hpp"
#include "portable-file-dialogs.h"
#include <clip.h>

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
		if(!clip::set_text(str))
		{
			VXM_CORE_ERROR("Could not Set data to the Clipboard.");
		}
	}

	std::string Clipboard::Get()
	{
		std::string str;
		if(!clip::get_text(str))
		{
			VXM_CORE_ERROR("Could not Get data from the Clipboard.");
		}
		return str;
	}

	void Clipboard::Clear()
	{
		if(!clip::clear())
		{
			VXM_CORE_ERROR("Could not Clear the Clipboard.");
		}
	}
}