//
// Created by Sayama on 27/09/2023.
//

#include "Voxymore/Renderer/Model.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Math/BoundingBox.hpp"
#include "Voxymore/Assets/AssetManager.hpp"
#include "Voxymore/Assets/Importers/TextureImporter.hpp"
#include "Voxymore/Project/Project.hpp"

// PRIVATE USE ONLY
//#include "GLTFHelper.hpp"
//#include <tiny_gltf.h>
#include <unordered_map>
#include <utility>

namespace Voxymore::Core
{
	Model::Model(const std::vector<MeshGroup>& meshes, const std::vector<Node>& nodes, std::vector<ModelRootScene> scenes, int defaultScene)
		: m_Meshes(meshes), m_Nodes(nodes), m_Scenes(scenes), m_DefaultScene(defaultScene)
	{
	}

	Model::~Model()
	{
	}

	const Node& Model::GetNode(int index) const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(index >= 0 && index < m_Nodes.size(), "Index {0} is invalid.", index);
		return m_Nodes[index];
	}

	const std::vector<int> &Model::GetDefaultScene() const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(!m_Scenes.empty(), "No scene on the model...");
		return  m_Scenes[m_DefaultScene];
	}

	void Model::Bind()
	{
		VXM_PROFILE_FUNCTION();
	}

	void Model::Unbind()
	{
        VXM_PROFILE_FUNCTION();
	}

	const MeshGroup& Model::GetMeshGroup(int index) const
	{
        VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(index > -1 && index < m_Meshes.size(), "The index '{0}' is not a valid mesh.", index);
		return m_Meshes[index];
	}
} // namespace Voxymore::Core
