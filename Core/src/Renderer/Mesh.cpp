//
// Created by Sayama on 27/09/2023.
//

#include <utility>

#include "Voxymore/Renderer/Mesh.hpp"


namespace Voxymore::Core
{
	static PrimitiveMesh * s_Primitive = nullptr;

	std::string PrimitiveMesh::GetTypeString(PrimitiveMesh::Type type)
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
		//TODO: Call the **REAL** AssetManager to create a memory asset.
		m_Meshes.emplace_back(Project::GetActive()->GetEditorAssetManager()->CreateMemoryAsset<Mesh>(vertices, indexes));
	}

	void MeshGroup::AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const MaterialField& material)
	{
		VXM_PROFILE_FUNCTION();
		//TODO: Call the **REAL** AssetManager to create a memory asset.
		m_Meshes.emplace_back(Project::GetActive()->GetEditorAssetManager()->CreateMemoryAsset<Mesh>(vertices, indexes));
		m_Meshes[m_Meshes.size() - 1].GetAsset()->SetMaterial(material);
	}

	void MeshGroup::AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const BoundingBox& aabb)
	{
		VXM_PROFILE_FUNCTION();
		//TODO: Call the **REAL** AssetManager to create a memory asset.
		m_Meshes.emplace_back(Project::GetActive()->GetEditorAssetManager()->CreateMemoryAsset<Mesh>(vertices, indexes, aabb));
	}

	void MeshGroup::AddSubMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, const BoundingBox& aabb, const MaterialField& material)
	{
		VXM_PROFILE_FUNCTION();
		//TODO: Call the **REAL** AssetManager to create a memory asset.
		m_Meshes.emplace_back(Project::GetActive()->GetEditorAssetManager()->CreateMemoryAsset<Mesh>(vertices, indexes, aabb));
		m_Meshes[m_Meshes.size() - 1].GetAsset()->SetMaterial(material);
	}
	void MeshGroup::AddSubMesh(MeshField mesh)
	{
		VXM_PROFILE_FUNCTION();
		m_Meshes.push_back(mesh);
	}

	Mesh::Mesh(const std::vector<Vertex> &vertices)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(vertices.size() % 3 == 0, "Invalid vertices count. The number of vertices should be divisible by 3 to form triangles.")

		std::vector<uint32_t> indexes(vertices.size());
		for (int i = 0; i < indexes.size(); ++i) {
			indexes[i] = i;
		}

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

	Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indexes, BoundingBox aabb) : m_BoundingBox(aabb)
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
		m_VertexArray->Bind();
	}

	void Mesh::Unbind() const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_VertexArray != nullptr, "The VertexArray doesn't exist.");
		m_VertexArray->Unbind();
	}

	void Mesh::SetMaterial(MaterialField material)
	{
		VXM_PROFILE_FUNCTION();
		m_Material = material;
	}

	MaterialField Mesh::GetMaterial() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Material;
	}

	Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord, glm::vec4 color) : Position(position), Normal(normal), TexCoord(texCoord), Color(color)
	{
	}

	PrimitiveMesh *PrimitiveMesh::GetInstance()
	{
		VXM_PROFILE_FUNCTION();
		return s_Primitive;
	}

	void PrimitiveMesh::InitPrimitives()
	{
		VXM_PROFILE_FUNCTION();
		if(IsInit())
		{
			VXM_CORE_WARNING("The primitive is already initialized.");
			return;
		}

		s_Primitive = new PrimitiveMesh();
	}

	void PrimitiveMesh::DestroyPrimitives()
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

	bool PrimitiveMesh::IsInit()
	{
		VXM_PROFILE_FUNCTION();
		return s_Primitive != nullptr;
	}

	Ref<Mesh> PrimitiveMesh::GetMesh(PrimitiveMesh::Type type)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(IsInit(), "The Primitive is not initialized.");
		if(!IsInit())
		{
			return nullptr;
		}

		switch (type)
		{
			case PrimitiveMesh::Square: return GetInstance()->GetOrCreateSquare();
			case PrimitiveMesh::Cube: return GetInstance()->GetOrCreateCube();
		}

		VXM_CORE_ASSERT(false, "The Primitive ({0}) is not implemented", (uint32_t)type);
		return nullptr;
	}

	Ref<Mesh> PrimitiveMesh::CreateOrphan(PrimitiveMesh::Type type, const MaterialField& material)
	{
		VXM_PROFILE_FUNCTION();

		Ref<Mesh> mesh = nullptr;

		switch (type)
		{
			case PrimitiveMesh::Square: mesh = CreateSquare(); break;
			case PrimitiveMesh::Cube: mesh = CreateCube(); break;
		}

		if(mesh) {
			mesh->SetMaterial(material);
		}

		VXM_CORE_ASSERT(mesh, "The Primitive ({0}) is not implemented", (uint32_t)type);
		return mesh;
	}

	Ref<Mesh> PrimitiveMesh::GetOrCreateSquare()
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

	Ref<Mesh> PrimitiveMesh::GetOrCreateCube()
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

	Ref<Mesh> PrimitiveMesh::CreateSquare()
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
//		mesh->SetMaterial(material);
		return mesh;
	}

	Ref<Mesh> PrimitiveMesh::CreateCube()
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
//		mesh->SetMaterial(material);
		return mesh;
	}
} // namespace Voxymore::Core
