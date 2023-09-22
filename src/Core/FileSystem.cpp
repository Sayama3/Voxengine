#include "Voxymore/Core/FileSystem.hpp"

namespace Voxymore::Core
{
        std::filesystem::path FileSystem::s_RootPath = "./";
        std::filesystem::path FileSystem::GetRootPath(FileSource source)
        {
            std::filesystem::path folder;
            switch (source)
            {
            case FileSource::None:
                {
                    folder = "./";
                    break;
                }
            
            case FileSource::EditorAsset:
                {
                    folder = "EditorAsset/";
                    break;
                }
            
            case FileSource::Asset:
                {
                    folder = "Asset/";
                    break;
                }
            
            case FileSource::EditorShader:
                {
                    folder = "EditorShader/";
                    break;
                }
            
            case FileSource::Shader:
                {
                    folder = "Shader/";
                    break;
                }
            
            case FileSource::System:
                {
                    folder = "System/";
                    break;
                }
            default:
                break;
            }
            return s_RootPath / folder;
        }
        std::ifstream FileSystem::ReadFileAsByte(const Path &path)
        {
            std::ifstream ifstream(GetPath(path));
            return ifstream;
        }
        std::stringstream FileSystem::ReadFileAsString(const Path& path)
        {
		    std::stringstream  stringstream;
            std::ifstream ifstream(GetPath(path));
		    stringstream << ifstream.rdbuf();
            return stringstream;
        }
        
        void FileSystem::WriteYamlFile(const Path& path, YAML::Emitter& emitter)
        {
            std::ofstream fileOut(GetPath(path));
            fileOut << emitter.c_str();
        }

        YAML::Node FileSystem::ReadFileAsYAML(const Path& path)
        {
            YAML::Node data;
            std::ifstream ifstream(GetPath(path));
            std::stringstream  stringstream;
            stringstream << ifstream.rdbuf();

            data = YAML::Load(stringstream.str());
            return data;
        }
        // bool FileSystem::WriteFile(const Path& path, const std::string& content)
        // {
        // }
        // bool FileSystem::WriteFile(const Path& path, std::vector<uint8_t> content)
        // {
        // }
        std::filesystem::path FileSystem::GetPath(const Path& path)
        {
            return GetRootPath(path.Source) / path.Path;
        }
        
        bool FileSystem::Exist(const Path& path)
        {
            return std::filesystem::exists(GetPath(path));
        }

        std::filesystem::path Path::GetFullPath() const
        {
            return FileSystem::GetPath(*this);
        }
} // namespace Voxymore::Core
