//
// Created by Sayama on 27/09/2023.
//

#include <utility>

#include "Voxymore/Renderer/Mesh.hpp"


namespace Voxymore::Core
{
	static Primitive* s_Primitive = nullptr;

	std::string Primitive::GetTypeString(Primitive::Type type)
	{
		VXM_PROFILE_FUNCTION();

		switch (type) {
			case Square: return "Square";
			case Cube: return "Cube";
		}

		return "None";
	}

	void MeshGroup::AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes)
	{
		VXM_PROFILE_FUNCTION();
		auto &sm = m_Meshes.emplace_back(vertices, indexes);
	}

	void MeshGroup::AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const Ref<Material> &material)
	{
		VXM_PROFILE_FUNCTION();
		auto &sm = m_Meshes.emplace_back(vertices, indexes);
		sm.SetMaterial(material);
	}

	void MeshGroup::AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const BoundingBox& aabb)
	{
		VXM_PROFILE_FUNCTION();
		auto &sm = m_Meshes.emplace_back(vertices, indexes, aabb);
	}

	void MeshGroup::AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const BoundingBox& aabb, const Ref<Material> &material)
	{
		VXM_PROFILE_FUNCTION();
		auto &sm = m_Meshes.emplace_back(vertices, indexes, aabb);
		sm.SetMaterial(material);
	}

	Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes)
	{
		VXM_PROFILE_FUNCTION();
		m_VertexArray = VertexArray::Create();

		m_BufferLayout = Vertex::Layout();

		m_VertexBuffer = VertexBuffer::Create(vertices.size() * sizeof(Vertex), vertices.data());
		m_VertexBuffer->SetLayout(m_BufferLayout);

		m_IndexBuffer = IndexBuffer::Create(indexes.size(), indexes.data());

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		if (!vertices.empty())
		{
			for (const auto& v : vertices) {
				m_BoundingBox.Grow(v.Position);
			}
		}
	}

	Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, BoundingBox aabb) : m_BoundingBox(std::move(aabb))
	{
		VXM_PROFILE_FUNCTION();
		m_VertexArray = VertexArray::Create();

		m_BufferLayout = Vertex::Layout();

		m_VertexBuffer = VertexBuffer::Create(vertices.size() * sizeof(Vertex), vertices.data());
		m_VertexBuffer->SetLayout(m_BufferLayout);

		m_IndexBuffer = IndexBuffer::Create(indexes.size(), indexes.data());

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		if (!m_BoundingBox && !vertices.empty())
		{
			for (const auto& v : vertices) {
				m_BoundingBox.Grow(v.Position);
			}
		}
	}

	void Mesh::Bind() const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_VertexArray != nullptr, "The VertexArray doesn't exist.");
//		VXM_CORE_ASSERT(m_Material != nullptr, "The Material must be set.");
//		m_Material->Bind();
		m_VertexArray->Bind();
	}

	void Mesh::Unbind() const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_VertexArray != nullptr, "The VertexArray doesn't exist.");
		VXM_CORE_ASSERT(m_Material != nullptr, "The Material must be set.");
		m_VertexArray->Unbind();
		m_Material->Unbind();
	}

	void Mesh::SetMaterial(Ref<Material> material)
	{
		VXM_PROFILE_FUNCTION();
		m_Material = std::move(material);
	}

	const Ref<Material>&Mesh::GetMaterial() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Material;
	}

	void Mesh::SetShader(const std::string& shaderName)
	{
		VXM_PROFILE_FUNCTION();
		m_Material->ChangeShader(shaderName);
	}

	void Mesh::SetShader(Ref<Shader>& shader)
	{
		VXM_PROFILE_FUNCTION();
		m_Material->ChangeShader(shader);
	}

	Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord, glm::vec4 color) : Position(position), Normal(normal), TexCoord(texCoord), Color(color)
	{
	}

	Primitive* Primitive::GetInstance()
	{
		VXM_PROFILE_FUNCTION();
		return s_Primitive;
	}

	void Primitive::InitPrimitives()
	{
		VXM_PROFILE_FUNCTION();
		if(IsInit())
		{
			VXM_CORE_WARNING("The primitive is already initialized.");
			return;
		}

		s_Primitive = new Primitive();
	}

	void Primitive::DestroyPrimitives()
	{
		VXM_PROFILE_FUNCTION();
		if(!IsInit())
		{
			VXM_CORE_WARNING("The primitive is not initialized.");
			return;
		}

		delete s_Primitive;
		s_Primitive = nullptr;
	}

	bool Primitive::IsInit()
	{
		VXM_PROFILE_FUNCTION();
		return s_Primitive != nullptr;
	}

	Ref<Mesh> Primitive::GetMesh(Primitive::Type type)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(IsInit(), "The Primitive is not initialized.");
		if(!IsInit())
		{
			return nullptr;
		}

		switch (type)
		{
			case Primitive::Square: return GetInstance()->GetOrCreateSquare();
			case Primitive::Cube: return GetInstance()->GetOrCreateCube();
		}

		VXM_CORE_ASSERT(false, "The Primitive ({0}) is not implemented", (uint32_t)type);
		return nullptr;
	}

	Ref<Mesh> Primitive::CreateOrphan(Primitive::Type type)
	{
		VXM_PROFILE_FUNCTION();

		switch (type)
		{
			case Primitive::Square: return CreateSquare();
			case Primitive::Cube: return CreateCube();
		}

		VXM_CORE_ASSERT(false, "The Primitive ({0}) is not implemented", (uint32_t)type);
		return nullptr;
	}

	Ref<Mesh> Primitive::GetOrCreateSquare()
	{
		VXM_PROFILE_FUNCTION();
		Ref<Mesh> mesh;
		Type currentType = Type::Square;

		if (m_Meshes.contains(currentType))
		{
			mesh = m_Meshes.at(currentType);
		}
		else
		{
			mesh = CreateSquare();
			m_Meshes[currentType] = mesh;
		}

		return mesh;
	}

	Ref<Mesh> Primitive::GetOrCreateCube()
	{
		VXM_PROFILE_FUNCTION();
		Ref<Mesh> mesh;
		Type currentType = Type::Cube;

		if (m_Meshes.contains(currentType))
		{
			mesh = m_Meshes.at(currentType);
		}
		else
		{
			mesh = CreateCube();
			m_Meshes[currentType] = mesh;
		}

		return mesh;
	}

	Ref<Mesh> Primitive::CreateSquare()
	{
		std::vector<Vertex> square {
				Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0,0,1), glm::vec2(0,0)), // 0
				Vertex(glm::vec3(+0.5f, -0.5f, 0.0f), glm::vec3(0,0,1), glm::vec2(1,0)), // 1
				Vertex(glm::vec3(+0.5f, +0.5f, 0.0f), glm::vec3(0,0,1), glm::vec2(1,1)), // 2
				Vertex(glm::vec3(-0.5f, +0.5f, 0.0f), glm::vec3(0,0,1), glm::vec2(0,1)), // 3
		};

		std::vector<uint32_t> vertices {
				0,1,2,
				0,2,3
		};

		Ref<Mesh >mesh = CreateRef<Mesh>(square, vertices);
		mesh->SetMaterial(MaterialLibrary::GetInstance().GetOrCreate(std::to_string(mesh->id()), "Default"));
		return mesh;
	}

	Ref<Mesh> Primitive::CreateCube()
	{
		std::vector<Vertex> cube {
				//Front Face
				Vertex(glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec3(0,0,+1), glm::vec2(0,0)), // 0
				Vertex(glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec3(0,0,+1), glm::vec2(1,0)), // 1
				Vertex(glm::vec3(+0.5f, +0.5f, +0.5f), glm::vec3(0,0,+1), glm::vec2(1,1)), // 2
				Vertex(glm::vec3(-0.5f, +0.5f, +0.5f), glm::vec3(0,0,+1), glm::vec2(0,1)), // 3
				//Front Face
				Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,0,-1), glm::vec2(0,0)), // 4
				Vertex(glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec3(0,0,-1), glm::vec2(1,0)), // 5
				Vertex(glm::vec3(+0.5f, +0.5f, -0.5f), glm::vec3(0,0,-1), glm::vec2(1,1)), // 6
				Vertex(glm::vec3(-0.5f, +0.5f, -0.5f), glm::vec3(0,0,-1), glm::vec2(0,1)), // 7
				//Up Face
				Vertex(glm::vec3(-0.5f, +0.5f, -0.5f), glm::vec3(0,+1, 0), glm::vec2(0,0)), // 8
				Vertex(glm::vec3(+0.5f, +0.5f, -0.5f), glm::vec3(0,+1, 0), glm::vec2(1,0)), // 9
				Vertex(glm::vec3(+0.5f, +0.5f, +0.5f), glm::vec3(0,+1, 0), glm::vec2(1,1)), // 10
				Vertex(glm::vec3(-0.5f, +0.5f, +0.5f), glm::vec3(0,+1, 0), glm::vec2(0,1)), // 11
				//Down Face
				Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,-1, 0), glm::vec2(0,0)), // 12
				Vertex(glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec3(0,-1, 0), glm::vec2(1,0)), // 13
				Vertex(glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec3(0,-1, 0), glm::vec2(1,1)), // 14
				Vertex(glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec3(0,-1, 0), glm::vec2(0,1)), // 15
				//Right Face
				Vertex(glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec3(+1, 0, 0), glm::vec2(0,0)), // 16
				Vertex(glm::vec3(+0.5f, +0.5f, -0.5f), glm::vec3(+1, 0, 0), glm::vec2(1,0)), // 17
				Vertex(glm::vec3(+0.5f, +0.5f, +0.5f), glm::vec3(+1, 0, 0), glm::vec2(1,1)), // 18
				Vertex(glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec3(+1, 0, 0), glm::vec2(0,1)), // 19
				//Left Face
				Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1, 0, 0), glm::vec2(0,0)), // 20
				Vertex(glm::vec3(-0.5f, +0.5f, -0.5f), glm::vec3(-1, 0, 0), glm::vec2(1,0)), // 21
				Vertex(glm::vec3(-0.5f, +0.5f, +0.5f), glm::vec3(-1, 0, 0), glm::vec2(1,1)), // 22
				Vertex(glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec3(-1, 0, 0), glm::vec2(0,1)), // 23

		};

		std::vector<uint32_t> vertices {
				// Front
				0,1,2,
				0,2,3,
				// Back
				4,6,5,
				4,7,6,
				// Up
				8,10,9,
				8,11,10,
				// Down
				12,13,14,
				12,14,15,
				// Front
				16,17,18,
				16,18,19,
				// Back
				20,22,21,
				20,23,22,
		};

		Ref<Mesh> mesh = CreateRef<Mesh>(cube, vertices);
		mesh->SetMaterial(MaterialLibrary::GetInstance().GetOrCreate(std::to_string(mesh->id()), "Default"));
		return mesh;
	}
} // namespace Voxymore::Core
