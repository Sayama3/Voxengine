//
// Created by ianpo on 06/09/2023.
//

#include "Voxymore/Utils/Platform.hpp"
#include "portable-file-dialogs.h"

namespace Voxymore::Core {

	std::string FileDialogs::OpenFile(const std::vector<std::string>& filter)
	{
		VXM_PROFILE_FUNCTION();
		std::vector<std::string> result = pfd::open_file("Select a file", ".", filter).result();
		if(result.empty()) return {};
		else return result[0];
	}

	std::string FileDialogs::SaveFile(const std::vector<std::string>& filter)
	{
		VXM_PROFILE_FUNCTION();
		std::string result = pfd::save_file("Select a file", ".", filter).result();
		return result;
	}
}