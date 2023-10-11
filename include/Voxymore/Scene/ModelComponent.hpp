#include <utility>

#include "Voxymore/Scene/CustomComponent.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Renderer/Model.hpp"

namespace Voxymore::Core
{
    
    // ======== ModelComponent ========
    class ModelComponent
    {
    	VXM_IMPLEMENT_COMPONENT(ModelComponent);
    public:
        inline const Path& GetPath() const {return m_ModelPath;}
        inline const std::filesystem::path& GetLocalPath() const {return m_ModelPath.path;}
		inline void SetPath(const std::filesystem::path& p) { m_ModelPath.path = p;}
		inline void SetPath(const Path& p) { m_ModelPath = p;}
		inline void SetShader(const std::string& shader) { m_ShaderName = shader;}
		inline const std::string& GetShader() const { return m_ShaderName;}
        inline const Ref<Model>& GetModel() const {return m_Model;}
        void LoadModel();
		inline bool IsLoaded() const { return m_IsLoaded; }
    private:
    	Path m_ModelPath;
		std::string m_ShaderName;
        Ref<Model> m_Model;
		bool m_IsLoaded = false;
    };
}