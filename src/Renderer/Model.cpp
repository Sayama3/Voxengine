//
// Created by Sayama on 27/09/2023.
//

#include "Voxymore/Renderer/Model.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Math.hpp"

#include <tiny_gltf.h>

namespace Voxymore::Core
{
	namespace Utils
	{
		glm::mat4 GetMatrix(const tinygltf::Node& node)
		{
			glm::mat4 matrix(1.0f);
			if(!node.matrix.empty())
			{
				for (int x = 0; x < 4; ++x) {
					for (int y = 0; y < 4; ++y) {
						matrix[x][y] = node.matrix[x + (y*4)];
					}
				}
			}
			else
			{
				glm::vec3 pos = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
				glm::quat rot = glm::quat(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
				glm::vec3 scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
				Math::TRS(matrix, pos, rot, scale);
			}
			return matrix;
		}


		bool IsValid(int value) {return value > -1;}
	}

	Ref<Model> Model::CreateModel(const Path &path)
	{
		const std::filesystem::path p = path.GetFullPath();

		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		bool loaded = false;
		if(p.extension() == "gltf") loaded = loader.LoadASCIIFromFile(&model, &err, &warn, p.string().c_str());
		else loaded = loader.LoadBinaryFromFile(&model, &err, &warn, p.string().c_str()); // for binary glTF(.glb)

		if (!warn.empty()) {
			VXM_CORE_WARNING(warn);
		}

		if (!err.empty()) {
			VXM_CORE_ERROR(err);
		}

		if (!loaded) {
			VXM_CORE_ERROR("Failed to parse glTF {0}", p.string());
			return nullptr;
		}

		for (const auto& scene : model.scenes) {
			VXM_CORE_INFO("Iterating through scene {0}", scene.name);
			for (auto nodeIndex : scene.nodes) {
				auto& node = model.nodes[nodeIndex];
				VXM_CORE_INFO("Iterating on the node {0}", node.name);
				glm::mat4 matrix = Utils::GetMatrix(node);
				if(Utils::IsValid(node.mesh))
				{
					auto& mesh = model.meshes[node.mesh];
					VXM_CORE_INFO("Iterating on the mesh {0}", mesh.name);
					for(auto& primitive : mesh.primitives)
					{
						const tinygltf::Accessor& accessor = model.accessors[primitive.attributes["POSITION"]];
						const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];

						// cast to float type read only. Use accessor and bufview byte offsets to determine where position data
						// is located in the buffer.
						const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
						// bufferView byteoffset + accessor byteoffset tells you where the actual position data is within the buffer. From there
						// you should already know how the data needs to be interpreted.
						const float* positions = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
						// From here, you choose what you wish to do with this position data. In this case, we  will display it out.
						for (size_t i = 0; i < accessor.count; ++i) {
							glm::vec3 position = glm::vec3(positions[i * 3 + 0],positions[i * 3 + 1],positions[i * 3 + 2]);
							// Positions are Vec3 components, so for each vec3 stride, offset for x, y, and z.
						}
					}
				}

			}
		}
	}

} // namespace Voxymore::Core
