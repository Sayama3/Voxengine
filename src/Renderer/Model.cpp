//
// Created by Sayama on 27/09/2023.
//

#include "Voxymore/Renderer/Model.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Math.hpp"

// PRIVATE USE ONLY
#include "GLTFHelper.hpp"
#include <tiny_gltf.h>
#include <unordered_map>

namespace Voxymore::Core
{
	namespace Utils
	{
		bool IsValid(int value) {return value > -1;}
	}

	Ref<Model> Model::CreateModel(const Path &path)
	{
		return CreateRef<Model>(path.GetFullPath());
	}

	Model::Model(const std::filesystem::path &path)
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		bool loaded = false;
		if(path.extension() == EXTENSION_GLTF) loaded = loader.LoadASCIIFromFile(&model, &err, &warn, path.string().c_str());
		else loaded = loader.LoadBinaryFromFile(&model, &err, &warn, path.string().c_str()); // for binary glTF(.glb)

		if (!warn.empty()) {
			VXM_CORE_WARNING(warn);
		}

		if (!err.empty()) {
			VXM_CORE_ERROR(err);
		}

		if (!loaded) {
			VXM_CORE_ERROR("Failed to parse glTF {0}", path.string());
			return;
		}

		m_Meshes.reserve(model.meshes.size());
		for (auto& mesh : model.meshes)
		{
			Ref<Mesh> m = CreateRef<Mesh>();
			m->m_VertexArrays.reserve(mesh.primitives.size());
			for (auto& primitive : mesh.primitives)
			{
				//TODO: Add other render mode.
				VXM_CORE_ASSERT(primitive.mode == GLTF::MeshRenderMode::TRIANGLES, "The Render Mode {0} cannot be used for the moment.", primitive.mode);
				VXM_CORE_ASSERT(primitive.indices > 0, "The indices are currently required to load a 3d model.");

				//std::unordered_map<GLTF::PrimitiveAttribute, std::vector<uint8_t>>;
				Ref<VertexArray> va = VertexArray::Create();
				std::vector<Ref<VertexBuffer>> vbs;
				vbs.reserve(primitive.attributes.size());
				for (auto&&[name, value] : primitive.attributes)
				{
					auto& accessor = model.accessors[value];
					auto& bufferView = model.bufferViews[accessor.bufferView];
					auto& buffer = model.buffers[bufferView.buffer];

					GLTF::PrimitiveAttribute attribute = GLTFHelper::GetPrimitiveAttribute(name);
					Ref<VertexBuffer> vb = VertexBuffer::Create(bufferView.byteLength, static_cast<void*>(&buffer.data.at(0) + bufferView.byteOffset));
					BufferLayout bl = {{GLTFHelper::GetShaderDataType(attribute), name}};
					vb->SetLayout(bl);
					vbs.push_back(vb);
					va->AddVertexBuffer(vb);
				}

				{
					auto& accessor = model.accessors[primitive.indices];
					auto& bufferView = model.bufferViews[accessor.bufferView];
					auto& buffer = model.buffers[bufferView.buffer];
					Ref<IndexBuffer> ib = IndexBuffer::Create(bufferView.byteLength / sizeof(uint32_t), static_cast<uint32_t*>(static_cast<void*>(&buffer.data.at(0) + bufferView.byteOffset)));
					va->SetIndexBuffer(ib);
				}

				m->m_VertexArrays.push_back(va);
			}
			m_Meshes.push_back(m);
		}

		m_Nodes.reserve(model.nodes.size());
		for(auto& node : model.nodes)
		{
			m_Nodes.emplace_back(node.mesh > -1 ? m_Meshes[node.mesh] : nullptr, node.children, GLTFHelper::GetMatrix(node));
		}

		m_Scenes.reserve(model.scenes.size());
		for (auto& scene : model.scenes)
		{
			m_Scenes.push_back(scene.nodes);
		}
	}

	Node& Model::GetNode(int index)
	{
		VXM_CORE_ASSERT(index >= 0 && index < m_Nodes.size(), "Index {0} is invalid.", index);
		return m_Nodes[index];
	}

	void Model::Bind() const
	{
	}

	void Model::Unbind() const
	{
	}

	void Model::Draw(const glm::mat4 &transform) const
	{
	}
} // namespace Voxymore::Core
