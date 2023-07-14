//
// Created by ianpo on 16/06/2023.
//

#ifndef LEARNOPENGL_MODEL_HPP
#define LEARNOPENGL_MODEL_HPP

#include "Mesh.hpp"
#include "Texture2D.hpp"
#include "ShaderProgram.hpp"
#include "MathHelper.hpp"

namespace Voxymore::Core::Renderer {
// TODO: Have a hierarchy and a node system so that we can easily navigate and draw it.
    class Model {
    public:
        Model(const std::string &path, glm::mat4 model = glm::identity<glm::mat4>(), bool flipUVs = false);

        Model(const Model &) = delete;

        Model &operator=(const Model &) = delete;

        void Draw(ShaderProgram &shader);

    private:
        std::vector<std::shared_ptr<Mesh>> m_Meshes;
        std::string m_Directory;
        std::vector<std::shared_ptr<Texture2D>> textures_loaded;
        // In private for now as the change won't influence the sub-meshes.
        glm::mat4 m_Model;

        void loadModel(const std::string &path, bool flipUVs);

        void processNode(aiNode *node, const aiScene *scene, const glm::mat4 &parentMatrix);

        std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<std::shared_ptr<Texture2D>>
        loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureUsage textureUsage);
    };

}
#endif //LEARNOPENGL_MODEL_HPP
