//
// Created by Sayama on 10/10/2024.
//

#pragma once

#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Math/Math.hpp"

#include "Voxymore/Renderer/Buffer.hpp"
#include "Voxymore/Renderer/UniformBuffer.hpp"
#include "Voxymore/Renderer/Texture.hpp"
#include "Voxymore/Renderer/Model.hpp"
#include "Voxymore/Renderer/Mesh.hpp"

#include "Action.hpp"

namespace Voxymore::Core
{
	struct PipelineParameter
	{
	public:

		enum Type : uint8_t
		{
			SSBO,
			UBO,
			ATOMIC_COUNTER,
			TEXTURES,
			MESH,
		};

		enum ValueType : uint8_t
		{
			UINT, INT, FLOAT,
			UINT2, INT2, FLOAT2,
			UINT4, INT4, FLOAT4,
			UINT2x2, INT2x2, FLOAT2x2,
			UINT4x4, INT4x4, FLOAT4x4,
		};

		enum SystemValues : uint8_t
		{
			CAMERA_VIEWPROJ,
			CAMERA_VIEW,
			CAMERA_PROJ,

			CAMERA_POS,
			CAMERA_DIR,

			LIGHT_COLOR,
			LIGHT_POSITION,
			LIGHT_DIRECTION,
			LIGHT_RANGE,
			LIGHT_INTENSITY,
			LIGHT_CUTOFF,
			LIGHT_TYPE,

			MODEL_MATRIX,
			NORMAL_MATRIX,
			ENTITY_ID,
		};

		using Value = std::variant<SystemValues,
								   uint32_t, int32_t, float,
								   glm::uvec2, glm::ivec2, glm::vec2,
								   glm::uvec4, glm::ivec4, glm::vec4,
								   glm::umat2x2, glm::imat2x2, glm::mat2x2,
								   glm::umat4x4, glm::imat4x4, glm::mat4x4
								   >;

		struct Variable
		{
			Value m_Value;
			std::string m_Name;
		};

	public:
		std::vector<Variable> m_Variables;
		std::string m_Name;
		uint32_t m_BindingPoint{~0u};
		Type m_Type;
	};

	class Pipeline
	{
	public:
		struct RendererParameter
		{
			PipelineParameter m_Parameter;
			std::variant<Ref<SSBO>, Ref<Mesh>, Ref<UniformBuffer>> m_RenderData;
		};
	public:
		Pipeline();
		~Pipeline();
		Pipeline(Pipeline&& other) noexcept;
		Pipeline& operator=(Pipeline&& other) noexcept;
		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;
		void swap(Pipeline& other) noexcept;
	public:
		void Execute(Action& action);
	private:
		std::optional<std::pair<UUID, RendererParameter&>> FindParameter(const Action::ParameterKey& parameterKey);
	public:
		std::unordered_map<UUID, RendererParameter> m_Parameters;
		std::vector<Action> m_Actions;
	private:
	};

} // namespace Voxymore::Core

