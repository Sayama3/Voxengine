//
// Created by Sayama on 28/09/2023.
//

// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.

#include <tiny_gltf.h>
#include "GLTFHelper.hpp"
#include "Voxymore/Core/Logger.hpp"

namespace Voxymore
{
	namespace Core
	{
		glm::mat4 GLTFHelper::GetMatrix(const tinygltf::Node& node)
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
	}// namespace Core
}// namespace Voxymore