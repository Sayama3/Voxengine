//
// Created by ianpo on 25/05/2023.
//

#ifndef LEARNOPENGL_SYSTEMHELPER_HPP
#define LEARNOPENGL_SYSTEMHELPER_HPP
#include <string>
#include <sstream>
#include <fstream>

#ifndef BIT
#define BIT(X) (1 << X)
#endif

//TODO: Write some real assert!
#ifdef VXM_ENABLE_ASSERTS
#define VXM_ASSERT(x, ...)
    #define VXM_CORE_ASSERT(x, ...)
#else
#define VXM_ASSERT(x, ...)
#define VXM_CORE_ASSERT(x, ...)
#endif

namespace Voxymore::Core {
    namespace SystemHelper {
        inline std::string ReadFile(const std::string &path) {
            std::ifstream fileStream(path);
            std::stringstream file;
            std::string line;
            while (std::getline(fileStream, line)) {
                file << line << "\n";
            }
            return file.str();
        }
    }
}
#endif //LEARNOPENGL_SYSTEMHELPER_HPP