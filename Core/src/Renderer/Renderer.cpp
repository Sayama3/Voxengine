//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/OpenGL/OpenGLShader.hpp"

namespace Voxymore::Core {
	static RendererData s_Data;
	static AssetHandle s_BindedShader = 0;

	RendererData::ModelData::ModelData(glm::mat4 transformMatrix, glm::mat4 normalMatrix, int entityId) : TransformMatrix(transformMatrix), NormalMatrix(normalMatrix), EntityId(entityId) {}

	void Renderer::Init() {
		VXM_PROFILE_FUNCTION();
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);
		s_Data.ModelUniformBuffer = UniformBuffer::Create(sizeof(RendererData::ModelData), 1);
		s_Data.LightUniformBuffer = UniformBuffer::Create(sizeof(RendererData::LightData), 2);
		s_Data.MaterialUniformBuffer = UniformBuffer::Create(sizeof(MaterialParameters), 3);
		s_Data.CurveParametersBuffer = UniformBuffer::Create(sizeof(RendererData::CurveParameters), 4);

		RenderCommand::Init();
	}

	void Renderer::Shutdown() {
		VXM_PROFILE_FUNCTION();


		RenderCommand::Shutdown();
	}

	void Renderer::BeginScene(const EditorCamera &camera, std::vector<Light> lights)
	{
		VXM_PROFILE_FUNCTION();

		s_BindedShader = 0;
		s_Data.CameraBuffer.ViewProjectionMatrix = camera.GetViewProjection();
		s_Data.CameraBuffer.CameraPosition = glm::vec4(camera.GetPosition(), 1);
		s_Data.CameraBuffer.CameraDirection = glm::vec4(camera.GetForwardDirection(), 0);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));
		s_Data.LightBuffer.lightCount = std::min((int)lights.size(), MAX_LIGHT_COUNT);
		for (size_t i = 0; i < s_Data.LightBuffer.lightCount; ++i)
		{
			s_Data.LightBuffer.lights[i] = lights[i];
		}
		s_Data.LightUniformBuffer->SetData(&s_Data.LightBuffer, sizeof(RendererData::LightData));

		s_Data.AlphaMeshes.clear();
		s_Data.OpaqueMeshes.clear();
	}

	void Renderer::BeginScene(const Camera &camera, const glm::mat4 &transform, std::vector<Light> lights)
	{
		VXM_PROFILE_FUNCTION();
		s_Data.CameraBuffer.ViewProjectionMatrix = camera.GetProjectionMatrix() * glm::inverse(transform);
		auto p = transform * glm::vec4{0,0,0,1};
		s_Data.CameraBuffer.CameraPosition = glm::vec4(glm::vec3(p) / p.w, 1);
		s_Data.CameraBuffer.CameraDirection = transform * glm::vec4{0,0,1,0};
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));
		s_Data.LightBuffer.lightCount = std::min((int)lights.size(), MAX_LIGHT_COUNT);
		for (size_t i = 0; i < s_Data.LightBuffer.lightCount; ++i)
		{
			s_Data.LightBuffer.lights[i] = lights[i];
		}
		s_Data.LightUniformBuffer->SetData(&s_Data.LightBuffer, sizeof(RendererData::LightData));

		s_Data.AlphaMeshes.clear();
		s_Data.OpaqueMeshes.clear();
	}

	void Renderer::EndScene() {
		VXM_PROFILE_FUNCTION();

		for(const auto& mesh : s_Data.OpaqueMeshes)
		{
			s_Data.ModelBuffer.TransformMatrix = std::get<1>(mesh);
			s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(std::get<1>(mesh)));
			s_Data.ModelBuffer.EntityId = std::get<2>(mesh);

			s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));
			s_Data.MaterialUniformBuffer->SetData(&std::get<0>(mesh)->GetMaterial().GetAsset()->GetMaterialsParameters(), sizeof(MaterialParameters));
			std::get<0>(mesh)->Bind();
			auto shaderName = std::get<0>(mesh)->GetMaterial().GetHandle();
			if(shaderName != s_BindedShader)
			{
				std::get<0>(mesh)->GetMaterial().GetAsset()->Bind();
				s_BindedShader = shaderName;
			}
			RenderCommand::DrawIndexed(std::get<0>(mesh)->GetVertexArray());
		}

		for(auto it = s_Data.AlphaMeshes.rbegin(); it != s_Data.AlphaMeshes.rend(); ++it)
		{
			auto& mesh = it->second;

			s_Data.ModelBuffer.TransformMatrix = std::get<1>(mesh);
			s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(std::get<1>(mesh)));
			s_Data.ModelBuffer.EntityId = std::get<2>(mesh);

			s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));
			s_Data.MaterialUniformBuffer->SetData(&std::get<0>(mesh)->GetMaterial().GetAsset()->GetMaterialsParameters(), sizeof(MaterialParameters));
			std::get<0>(mesh)->Bind();
			auto shaderName = std::get<0>(mesh)->GetMaterial().GetHandle();
			if(shaderName != s_BindedShader)
			{
				std::get<0>(mesh)->GetMaterial().GetAsset()->Bind();
				s_BindedShader = shaderName;
			}
			RenderCommand::DrawIndexed(std::get<0>(mesh)->GetVertexArray());
		}

		RenderCommand::ClearBinding();
		s_BindedShader = 0;
	}

	void Renderer::Submit(Ref<Shader>& shader, const Ref<VertexArray> &vertexArray, const glm::mat4& transform, int entityId) {
		VXM_PROFILE_FUNCTION();
		//        VXM_CORE_ASSERT(s_Data.CameraBuffer.ViewProjectionMatrix != glm::zero<glm::mat4>(), "A valid View Projection Matrix is required to submit data to the renderer.");


		s_Data.ModelBuffer.TransformMatrix = transform;
		s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ModelBuffer.EntityId = entityId;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		auto& shaderName = shader->Handle;
		if(shaderName != s_BindedShader)
		{
			shader->Bind();
			s_BindedShader = shaderName;
		}
		//TODO: Set the view projection matrix once per frame not once per model drawn.
		//        std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_ViewProjectionMatrix", s_Data.ViewProjectionMatrix);
		//        std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_Transform", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(Ref<Material> &material, const Ref<VertexArray> &vertexArray, const glm::mat4 &transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		//		VXM_CORE_ASSERT(s_Data.ViewProjectionMatrix != glm::zero<glm::mat4>(), "A valid View Projection Matrix is required to submit data to the renderer.");


		s_Data.ModelBuffer.TransformMatrix = transform;
		s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ModelBuffer.EntityId = entityId;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		s_Data.MaterialUniformBuffer->SetData(&material->GetMaterialsParameters(), sizeof(MaterialParameters));
		auto& shaderName = material->Handle;
		if(shaderName != s_BindedShader)
		{
			material->Bind();
			s_BindedShader = shaderName;
		}
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const MeshGroup& meshGroup, const glm::mat4& transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		// s_Data.ModelBuffer.TransformMatrix = transform;
		// s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(transform));
		// s_Data.ModelBuffer.EntityId = entityId;
		// s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		for (const auto& mesh : meshGroup)
		{
			if(mesh) {
				Submit(mesh.GetAsset(), transform, entityId);
			}
//			s_Data.MaterialUniformBuffer->SetData(&mesh->GetMaterial()->GetMaterialsParameters(), sizeof(MaterialParameters));
//			mesh->Bind();
//			auto& shaderName = mesh->GetMaterial()->GetShaderName();
//			if(shaderName != s_BindedShader)
//			{
//				mesh->GetMaterial()->Bind();
//				s_BindedShader = shaderName;
//			}
//			RenderCommand::DrawIndexed(mesh->GetVertexArray());
		}
	}

	void Renderer::Submit(Ref<Mesh> mesh, const glm::mat4& transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		Vec4 center = transform*Vec4(mesh->GetBoundingBox().GetCenter(),1);
		center /= center.w;
		Real distance = Math::SqrMagnitude(Vec3(center) - Vec3(s_Data.CameraBuffer.CameraPosition));

		if(mesh->GetMaterial().GetAsset()->GetMaterialsParameters().AlphaMode == AlphaMode::Blend)
		{
			s_Data.AlphaMeshes.insert(std::make_pair(distance, std::tuple(mesh, transform, entityId)));
		}
		else
		{
			s_Data.OpaqueMeshes.emplace_back(std::tuple(mesh, transform, entityId));
		}
	}
/*
	void Renderer::Submit(const Mesh& mesh, const glm::mat4& transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();

		s_Data.ModelBuffer.TransformMatrix = transform;
		s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ModelBuffer.EntityId = entityId;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));
		s_Data.MaterialUniformBuffer->SetData(&mesh.GetMaterial()->GetMaterialsParameters(), sizeof(MaterialParameters));
		mesh.Bind();
		auto& shaderName = mesh.GetMaterial()->GetShaderName();
		if(shaderName != s_BindedShader)
		{
			mesh.GetMaterial()->Bind();
			s_BindedShader = shaderName;
		}
		RenderCommand::DrawIndexed(mesh.GetVertexArray());
	}
*/
	void Renderer::Submit(const Ref<Model>& model, const glm::mat4& transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		//model->Bind();
		for (int nodeIndex : model->GetDefaultScene())
		{
			Submit(model, model->GetNode(nodeIndex), transform, entityId);
		}
	}

	void Renderer::Submit(const Ref<Model>& model, const Node& node, const glm::mat4& transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		glm::mat4 currentTransform = transform * node.transform;
		if(node.HasMesh())
		{
			Submit(model->GetMeshGroup(node.GetMeshIndex()), currentTransform, entityId);
		}

		if(node.HasChildren())
		{
			for(const int i : node.children)
			{
				Submit(model, model->GetNode(i), currentTransform, entityId);
			}
		}
	}

	void Renderer::Submit(Ref<Material> material, const std::vector<glm::vec3>& bezierControlPoints, int lineDefinition, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(bezierControlPoints.size() <= s_Data.CurveBuffer.ControlPoints.size(), "The shader might won't support more than a {0} control point...", s_Data.CurveBuffer.ControlPoints.size());


		uint32_t count = std::min(s_Data.CurveBuffer.ControlPoints.size(), bezierControlPoints.size());

		std::vector<Vertex> vertices((count/3) + (count%3 ? 1 : 0));

		s_Data.CurveBuffer.NumberOfSegment = lineDefinition;
		s_Data.CurveBuffer.NumberOfControlPoint = static_cast<int>(bezierControlPoints.size());
		for (int i = 0; i < count; ++i) {
			s_Data.CurveBuffer.ControlPoints[i] = glm::vec4(bezierControlPoints[i],1);
			vertices[i/3].Position[i%3] = float(i);
		}
		std::vector<uint32_t> indices;
		indices.reserve((bezierControlPoints.size()-1)*2);
		for (uint32_t i = 0; i < bezierControlPoints.size() - 1; ++i) {
			indices.push_back(i);
			indices.push_back(i+1);
		}
		Ref<Mesh> mesh = CreateRef<Mesh>(vertices, indices);

		mesh->SetMaterial(material);
		mesh->SetDrawMode(DrawMode::Lines);

		s_Data.ModelBuffer.TransformMatrix = Math::Identity<Mat4>();
		s_Data.ModelBuffer.NormalMatrix = Math::Identity<Mat4>();
		s_Data.ModelBuffer.EntityId = entityId;

		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));
		s_Data.MaterialUniformBuffer->SetData(&material->GetMaterialsParameters(), sizeof(MaterialParameters));
		s_Data.CurveParametersBuffer->SetData(&s_Data.CurveBuffer, sizeof(RendererData::CurveParameters));

		mesh->Bind();
		if(material->GetShaderHandle() != s_BindedShader)
		{
			material->Bind();
			s_BindedShader = material->GetShaderHandle();
		}
		RenderCommand::DrawPatches(vertices.size());
	}

	void Renderer::Submit(Ref<Material> material, const glm::vec3& controlPoint0, const glm::vec3& controlPoint1, const glm::vec3& controlPoint2, const glm::vec3& controlPoint3, int lineDefinition, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		Submit(material, {controlPoint0, controlPoint1, controlPoint2, controlPoint3}, lineDefinition, entityId);
	}

	void Renderer::Submit(Ref<Material> material, int degree, const std::vector<glm::vec3>& points, const std::vector<float>& nodes, const std::vector<float>& weights, int lineDefinition, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(points.size() <= s_Data.CurveBuffer.ControlPoints.size(), "The shader might won't support more than a {0} control point...", s_Data.CurveBuffer.ControlPoints.size());
		VXM_CORE_ASSERT(nodes.size() <= s_Data.CurveBuffer.ControlPoints.size(), "The shader might won't support more than a {0} nodes...", s_Data.CurveBuffer.ControlPoints.size());
		VXM_CORE_ASSERT(points.size() == weights.size(), "The number of weight and the number of points should be equal.");

		uint32_t controlPointCount = std::min(s_Data.CurveBuffer.ControlPoints.size(), points.size());
		uint32_t nodeCount = std::min(s_Data.CurveBuffer.ControlPoints.size(), nodes.size());

		std::vector<Vertex> vertices((nodeCount /3) + (nodeCount %3 ? 1 : 0));

		s_Data.CurveBuffer.Degree = degree;
		s_Data.CurveBuffer.NumberOfSegment = lineDefinition;
		s_Data.CurveBuffer.NumberOfControlPoint = static_cast<int>(points.size());
		s_Data.CurveBuffer.NumberOfKnot = static_cast<int>(nodeCount);
		for (int i = 0; i < controlPointCount; ++i) {
			s_Data.CurveBuffer.ControlPoints[i] = glm::vec4(points[i],1);
			s_Data.CurveBuffer.Weigths[i] = weights[i];
		}

		for (int i = 0; i < nodeCount; ++i) {
			vertices[i/3].Position[i%3] = float(nodes[i]);
		}

		std::vector<uint32_t> indices;
		indices.reserve((nodeCount-1)*2);
		for (uint32_t i = 0; i < nodeCount - 1; ++i) {
			indices.push_back(i);
			indices.push_back(i+1);
		}
		Ref<Mesh> mesh = CreateRef<Mesh>(vertices, indices);

		mesh->SetMaterial(material);
		mesh->SetDrawMode(DrawMode::Lines);

		s_Data.ModelBuffer.TransformMatrix = Math::Identity<Mat4>();
		s_Data.ModelBuffer.NormalMatrix = Math::Identity<Mat4>();
		s_Data.ModelBuffer.EntityId = entityId;

		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));
		s_Data.MaterialUniformBuffer->SetData(&material->GetMaterialsParameters(), sizeof(MaterialParameters));
		s_Data.CurveParametersBuffer->SetData(&s_Data.CurveBuffer, sizeof(RendererData::CurveParameters));

		mesh->Bind();
		if(material->GetShaderHandle() != s_BindedShader)
		{
			material->Bind();
			s_BindedShader = material->GetShaderHandle();
		}
		RenderCommand::DrawPatches(vertices.size());
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		VXM_PROFILE_FUNCTION();
		RenderCommand::SetViewport(0,0,width,height);
	}

} // Core