//
// Created by Sayama on 27/09/2023.
//

#include "Voxymore/Renderer/Model.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Math.hpp"

// PRIVATE USE ONLY
#include "GLTFHelper.hpp"
#include <tiny_gltf.h>

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

		for (auto& mesh : model.meshes)
		{
			for (auto& primitive : mesh.primitives)
			{
				//TODO: Add other render mode.
				VXM_CORE_ASSERT(primitive.mode == GLTF::MeshRenderMode::TRIANGLES, "The Render Mode {0} cannot be used for the moment.", primitive.mode);
				for (auto&&[name, value] : primitive.attributes)
				{
					GLTF::PrimitiveAttribute attribute = GLTFHelper::GetPrimitiveAttribute(name);
					switch (attribute)
					{
						case GLTF::POSITION: break;
						case GLTF::NORMAL: break;
						case GLTF::TANGENT: break;
						case GLTF::TEXCOORD: break;
						case GLTF::COLOR: break;
						case GLTF::JOINTS: break;
						case GLTF::WEIGHT: break;
						case GLTF::COUNT: break;
					}
				}
			}
		}
	}
} // namespace Voxymore::Core
