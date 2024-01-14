//
// Created by ianpo on 25/07/2023.
//

#pragma once

#include <string>
#include <vector>

namespace Voxymore::Core {
    class Platform {
    public:
        /**
         * Get the time in seconds.
         * @return The time in seconds.
         */
        static double GetTime();
    };

	class FileDialogs
	{
	public:
		static std::string OpenFile(const std::vector<std::string>& filter);
		static std::string SaveFile(const std::vector<std::string>& filter);
	};

	class Clipboard
	{
	public:

		static void Set(const std::string& str);
		static std::string Get();
		static void Clear();
	};
}
