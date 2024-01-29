//
// Created by ianpo on 25/05/2023.
//

#ifndef LEARNOPENGL_SYSTEMHELPER_HPP
#define LEARNOPENGL_SYSTEMHELPER_HPP

#ifndef BIT
#define BIT(X) (1 << X)
#endif

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/FileSystem.hpp"

namespace Voxymore::Core {
    class SystemHelper {
    public:
        inline static std::string ReadFile(const Path &path) { return ReadFile(path.GetFullPath()); }
        inline static std::string ReadFile(const std::filesystem::path &path) {
            VXM_PROFILE_FUNCTION();
            std::string result;
            std::ifstream fileStream(path, std::ios::in | std::ios::binary);
            if(fileStream)
            {
                fileStream.seekg(0, std::ios::end);
                result.resize(fileStream.tellg());
                fileStream.seekg(0, std::ios::beg);
                fileStream.read(result.data(), result.size());
                fileStream.close();
            }
            else
            {
                VXM_CORE_ERROR("Could not open file '{0}'.", path.string());
            }
            return result;
        }
    };
}
#endif //LEARNOPENGL_SYSTEMHELPER_HPP