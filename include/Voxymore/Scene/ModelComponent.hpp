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
        inline const Path& GetPath() const {return m_Path;}
        inline const std::filesystem::path& GetLocalPath() const {return m_Path.path;}
        inline void SetPath(std::filesystem::path p) {m_Path.path = std::move(p);}
        inline const Ref<Model>& GetModel() const {return m_Model;}
        void LoadModel();
    private:
    	Path m_Path;
        Ref<Model> m_Model;
    };
}