//
// Created by ianpo on 20/07/2023.
//

#include "OpenGLShader.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include <filesystem>
#include <glad/glad.h>
#include <regex>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <string>
#include <sha256.h>

#include "Voxymore/Renderer/ShaderDataType.hpp"

#ifndef NEWLINE
#define NEWLINE "\n"
#endif



namespace Voxymore::Core
{
	namespace Test
	{
		// Retrieved from spirv_common.hpp
		enum class ShaderDataType
		{
			Unknown,
			Void,
			Boolean,
			SByte,
			UByte,
			Short,
			UShort,
			Int,
			UInt,
			Int64,
			UInt64,
			AtomicCounter,
			Half,
			Float,
			Double,
			Struct,
			Image,
			SampledImage,
			Sampler,
			AccelerationStructure,
			RayQuery,

			// Keep internal types at the end.
			ControlPointArray,
			Interpolant,
			Char

		};

		ShaderDataType GetShaderDataType(spirv_cross::SPIRType::BaseType spirvType)
		{
			VXM_PROFILE_FUNCTION();
			switch (spirvType) {
				case spirv_cross::SPIRType::Unknown: return ShaderDataType::Unknown;
				case spirv_cross::SPIRType::Void: return ShaderDataType::Void;
				case spirv_cross::SPIRType::Boolean: return ShaderDataType::Boolean;
				case spirv_cross::SPIRType::SByte: return ShaderDataType::SByte;
				case spirv_cross::SPIRType::UByte: return ShaderDataType::UByte;
				case spirv_cross::SPIRType::Short: return ShaderDataType::Short;
				case spirv_cross::SPIRType::UShort: return ShaderDataType::UShort;
				case spirv_cross::SPIRType::Int: return ShaderDataType::Int;
				case spirv_cross::SPIRType::UInt: return ShaderDataType::UInt;
				case spirv_cross::SPIRType::Int64: return ShaderDataType::Int64;
				case spirv_cross::SPIRType::UInt64: return ShaderDataType::UInt64;
				case spirv_cross::SPIRType::AtomicCounter: return ShaderDataType::AtomicCounter;
				case spirv_cross::SPIRType::Half: return ShaderDataType::Half;
				case spirv_cross::SPIRType::Float: return ShaderDataType::Float;
				case spirv_cross::SPIRType::Double: return ShaderDataType::Double;
				case spirv_cross::SPIRType::Struct: return ShaderDataType::Struct;
				case spirv_cross::SPIRType::Image: return ShaderDataType::Image;
				case spirv_cross::SPIRType::SampledImage: return ShaderDataType::SampledImage;
				case spirv_cross::SPIRType::Sampler: return ShaderDataType::Sampler;
				case spirv_cross::SPIRType::AccelerationStructure: return ShaderDataType::AccelerationStructure;
				case spirv_cross::SPIRType::RayQuery: return ShaderDataType::RayQuery;
				case spirv_cross::SPIRType::ControlPointArray: return ShaderDataType::ControlPointArray;
				case spirv_cross::SPIRType::Interpolant: return ShaderDataType::Interpolant;
				case spirv_cross::SPIRType::Char: return ShaderDataType::Char;
				default: return ShaderDataType::Unknown;
			}
			return ShaderDataType::Unknown;
		}
		const char *GetShaderDataTypeName(ShaderDataType sdt)
		{
			VXM_PROFILE_FUNCTION();
			switch (sdt) {
				case ShaderDataType::Unknown: return "Unknown";
				case ShaderDataType::Void: return "Void";
				case ShaderDataType::Boolean: return "Boolean";
				case ShaderDataType::SByte: return "SByte";
				case ShaderDataType::UByte: return "UByte";
				case ShaderDataType::Short: return "Short";
				case ShaderDataType::UShort: return "UShort";
				case ShaderDataType::Int: return "Int";
				case ShaderDataType::UInt: return "UInt";
				case ShaderDataType::Int64: return "Int64";
				case ShaderDataType::UInt64: return "UInt64";
				case ShaderDataType::AtomicCounter: return "AtomicCounter";
				case ShaderDataType::Half: return "Half";
				case ShaderDataType::Float: return "Float";
				case ShaderDataType::Double: return "Double";
				case ShaderDataType::Struct: return "Struct";
				case ShaderDataType::Image: return "Image";
				case ShaderDataType::SampledImage: return "SampledImage";
				case ShaderDataType::Sampler: return "Sampler";
				case ShaderDataType::AccelerationStructure: return "AccelerationStructure";
				case ShaderDataType::RayQuery: return "RayQuery";
				case ShaderDataType::ControlPointArray: return "ControlPointArray";
				case ShaderDataType::Interpolant: return "Interpolant";
				case ShaderDataType::Char: return "Char";
				default: return "Unknown";
			}
			return "Unknown";
		}

		struct ShaderImage {
		};

		struct ShaderDataMember {
			ShaderDataType m_Type = ShaderDataType::Unknown;
			std::string m_Name;
			uint32_t m_Size = 0;
			// If column = 1, it's a vector, > 1 it's a matrix.
			uint32_t m_ColumnNumber = 0;
			// If VectorSize = 1 it's a value, > 1 it's a vector.
			uint32_t m_VectorSize = 0;
			std::vector<ShaderDataMember> m_Children;
			inline bool IsLeaf() { return m_Type != ShaderDataType::Struct; }
			inline uint32_t GetTypeSize()
			{
				return m_Size / (m_ColumnNumber * m_VectorSize);
			}
		};

		class ShaderData
		{
		public:
			std::vector<ShaderDataMember> m_Input;
			std::vector<ShaderDataMember> m_Uniform;
			std::vector<ShaderDataMember> m_Constants;
		};
	}// namespace Test

	namespace Utils
	{
		static ShaderDataType GetShaderDataType(GLenum type)
		{
			VXM_PROFILE_FUNCTION();
			switch (type) {
				case (GL_FLOAT): return ShaderDataType::Float;		// float
				case (GL_FLOAT_VEC2): return ShaderDataType::Float2;// vec2
				case (GL_FLOAT_VEC3): return ShaderDataType::Float3;// vec3
				case (GL_FLOAT_VEC4):
					return ShaderDataType::Float4;						  // vec4
																		  //            case (GL_DOUBLE): return ShaderDataType::Double; // double
																		  //            case (GL_DOUBLE_VEC2): return ShaderDataType::Double2; // dvec2
																		  //            case (GL_DOUBLE_VEC3): return ShaderDataType::Double3; // dvec3
																		  //            case (GL_DOUBLE_VEC4): return ShaderDataType::Double4; // dvec4
				case (GL_INT): return ShaderDataType::Int;				  // int
				case (GL_INT_VEC2): return ShaderDataType::Int2;		  // ivec2
				case (GL_INT_VEC3): return ShaderDataType::Int3;		  // ivec3
				case (GL_INT_VEC4): return ShaderDataType::Int4;		  // ivec4
				case (GL_UNSIGNED_INT): return ShaderDataType::UInt;	  // unsigned int
				case (GL_UNSIGNED_INT_VEC2): return ShaderDataType::UInt2;// uvec2
				case (GL_UNSIGNED_INT_VEC3): return ShaderDataType::UInt3;// uvec3
				case (GL_UNSIGNED_INT_VEC4): return ShaderDataType::UInt4;// uvec4
				case (GL_BOOL): return ShaderDataType::Bool;			  // bool
				case (GL_BOOL_VEC2): return ShaderDataType::Bool2;		  // bvec2
				case (GL_BOOL_VEC3): return ShaderDataType::Bool3;		  // bvec3
				case (GL_BOOL_VEC4): return ShaderDataType::Bool4;		  // bvec4
				case (GL_FLOAT_MAT2): return ShaderDataType::Mat2;		  // mat2
				case (GL_FLOAT_MAT3): return ShaderDataType::Mat3;		  // mat3
				case (GL_FLOAT_MAT4):
					return ShaderDataType::Mat4;					   // mat4
																	   //            case (GL_FLOAT_MAT2x3): return ShaderDataType::Mat2x3; // mat2x3
																	   //            case (GL_FLOAT_MAT2x4): return ShaderDataType::Mat2x4; // mat2x4
																	   //            case (GL_FLOAT_MAT3x2): return ShaderDataType::Mat3x2; // mat3x2
																	   //            case (GL_FLOAT_MAT3x4): return ShaderDataType::Mat3x4; // mat3x4
																	   //            case (GL_FLOAT_MAT4x2): return ShaderDataType::Mat4x2; // mat4x2
																	   //            case (GL_FLOAT_MAT4x3): return ShaderDataType::Mat4x3; // mat4x3
																	   //            case (GL_DOUBLE_MAT2): return ShaderDataType::DMat2; // dmat2
																	   //            case (GL_DOUBLE_MAT3): return ShaderDataType::DMat3; // dmat3
																	   //            case (GL_DOUBLE_MAT4): return ShaderDataType::DMat4; // dmat4
																	   //            case (GL_DOUBLE_MAT2x3): return ShaderDataType::DMat2x3; // dmat2x3
																	   //            case (GL_DOUBLE_MAT2x4): return ShaderDataType::DMat2x4; // dmat2x4
																	   //            case (GL_DOUBLE_MAT3x2): return ShaderDataType::DMat3x2; // dmat3x2
																	   //            case (GL_DOUBLE_MAT3x4): return ShaderDataType::DMat3x4; // dmat3x4
																	   //            case (GL_DOUBLE_MAT4x2): return ShaderDataType::DMat4x2; // dmat4x2
																	   //            case (GL_DOUBLE_MAT4x3): return ShaderDataType::DMat4x3; // dmat4x3
				case (GL_SAMPLER_1D): return ShaderDataType::Sampler1D;// sampler1D
				case (GL_SAMPLER_2D): return ShaderDataType::Sampler2D;// sampler2D
				case (GL_SAMPLER_3D):
					return ShaderDataType::Sampler3D;// sampler3D
													 //            case (GL_SAMPLER_CUBE): return ShaderDataType::Sampler_Cube; // samplerCube
													 //            case (GL_SAMPLER_1D_SHADOW): return ShaderDataType::SAMPLER_1D_SHADOW; // sampler1DShadow
													 //            case (GL_SAMPLER_2D_SHADOW): return ShaderDataType::SAMPLER_2D_SHADOW; // sampler2DShadow
													 //            case (GL_SAMPLER_1D_ARRAY): return ShaderDataType::SAMPLER_1D_ARRAY; // sampler1DArray
													 //            case (GL_SAMPLER_2D_ARRAY): return ShaderDataType::SAMPLER_2D_ARRAY; // sampler2DArray
													 //            case (GL_SAMPLER_1D_ARRAY_SHADOW): return ShaderDataType::SAMPLER_1D_ARRAY_SHADOW; // sampler1DArrayShadow
													 //            case (GL_SAMPLER_2D_ARRAY_SHADOW): return ShaderDataType::SAMPLER_2D_ARRAY_SHADOW; // sampler2DArrayShadow
													 //            case (GL_SAMPLER_2D_MULTISAMPLE): return ShaderDataType::SAMPLER_2D_MULTISAMPLE; // sampler2DMS
													 //            case (GL_SAMPLER_2D_MULTISAMPLE_ARRAY): return ShaderDataType::SAMPLER_2D_MULTISAMPLE_ARRAY; // sampler2DMSArray
													 //            case (GL_SAMPLER_CUBE_SHADOW): return ShaderDataType::SAMPLER_CUBE_SHADOW; // samplerCubeShadow
													 //            case (GL_SAMPLER_BUFFER): return ShaderDataType::SAMPLER_BUFFER; // samplerBuffer
													 //            case (GL_SAMPLER_2D_RECT): return ShaderDataType::SAMPLER_2D_RECT; // sampler2DRect
													 //            case (GL_SAMPLER_2D_RECT_SHADOW): return ShaderDataType::SAMPLER_2D_RECT_SHADOW; // sampler2DRectShadow
													 //            case (GL_INT_SAMPLER_1D): return ShaderDataType::INT_SAMPLER_1D; // isampler1D
													 //            case (GL_INT_SAMPLER_2D): return ShaderDataType::INT_SAMPLER_2D; // isampler2D
													 //            case (GL_INT_SAMPLER_3D): return ShaderDataType::INT_SAMPLER_3D; // isampler3D
													 //            case (GL_INT_SAMPLER_CUBE): return ShaderDataType::INT_SAMPLER_CUBE; // isamplerCube
													 //            case (GL_INT_SAMPLER_1D_ARRAY): return ShaderDataType::INT_SAMPLER_1D_ARRAY; // isampler1DArray
													 //            case (GL_INT_SAMPLER_2D_ARRAY): return ShaderDataType::INT_SAMPLER_2D_ARRAY; // isampler2DArray
													 //            case (GL_INT_SAMPLER_2D_MULTISAMPLE): return ShaderDataType::INT_SAMPLER_2D_MULTISAMPLE; // isampler2DMS
													 //            case (GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY): return ShaderDataType::INT_SAMPLER_2D_MULTISAMPLE_ARRAY; // isampler2DMSArray
													 //            case (GL_INT_SAMPLER_BUFFER): return ShaderDataType::INT_SAMPLER_BUFFER; // isamplerBuffer
													 //            case (GL_INT_SAMPLER_2D_RECT): return ShaderDataType::INT_SAMPLER_2D_RECT; // isampler2DRect
													 //            case (GL_UNSIGNED_INT_SAMPLER_1D): return ShaderDataType::UNSIGNED_INT_SAMPLER_1D; // usampler1D
													 //            case (GL_UNSIGNED_INT_SAMPLER_2D): return ShaderDataType::UNSIGNED_INT_SAMPLER_2D; // usampler2D
													 //            case (GL_UNSIGNED_INT_SAMPLER_3D): return ShaderDataType::UNSIGNED_INT_SAMPLER_3D; // usampler3D
													 //            case (GL_UNSIGNED_INT_SAMPLER_CUBE): return ShaderDataType::UNSIGNED_INT_SAMPLER_CUBE; // usamplerCube
													 //            case (GL_UNSIGNED_INT_SAMPLER_1D_ARRAY): return ShaderDataType::UNSIGNED_INT_SAMPLER_1D_ARRAY; // usampler2DArray
													 //            case (GL_UNSIGNED_INT_SAMPLER_2D_ARRAY): return ShaderDataType::UNSIGNED_INT_SAMPLER_2D_ARRAY; // usampler2DArray
													 //            case (GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE): return ShaderDataType::UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE; // usampler2DMS
													 //            case (GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY): return ShaderDataType::UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY; // usampler2DMSArray
													 //            case (GL_UNSIGNED_INT_SAMPLER_BUFFER): return ShaderDataType::UNSIGNED_INT_SAMPLER_BUFFER; // usamplerBuffer
													 //            case (GL_UNSIGNED_INT_SAMPLER_2D_RECT): return ShaderDataType::UNSIGNED_INT_SAMPLER_2D_RECT; // usampler2DRect
													 //            case (GL_IMAGE_1D): return ShaderDataType::IMAGE_1D; // image1D
													 //            case (GL_IMAGE_2D): return ShaderDataType::IMAGE_2D; // image2D
													 //            case (GL_IMAGE_3D): return ShaderDataType::IMAGE_3D; // image3D
													 //            case (GL_IMAGE_2D_RECT): return ShaderDataType::IMAGE_2D_RECT; // image2DRect
													 //            case (GL_IMAGE_CUBE): return ShaderDataType::IMAGE_CUBE; // imageCube
													 //            case (GL_IMAGE_BUFFER): return ShaderDataType::IMAGE_BUFFER; // imageBuffer
													 //            case (GL_IMAGE_1D_ARRAY): return ShaderDataType::IMAGE_1D_ARRAY; // image1DArray
													 //            case (GL_IMAGE_2D_ARRAY): return ShaderDataType::IMAGE_2D_ARRAY; // image2DArray
													 //            case (GL_IMAGE_2D_MULTISAMPLE): return ShaderDataType::IMAGE_2D_MULTISAMPLE; // image2DMS
													 //            case (GL_IMAGE_2D_MULTISAMPLE_ARRAY): return ShaderDataType::IMAGE_2D_MULTISAMPLE_ARRAY; // image2DMSArray
													 //            case (GL_INT_IMAGE_1D): return ShaderDataType::INT_IMAGE_1D; // iimage1D
													 //            case (GL_INT_IMAGE_2D): return ShaderDataType::INT_IMAGE_2D; // iimage2D
													 //            case (GL_INT_IMAGE_3D): return ShaderDataType::INT_IMAGE_3D; // iimage3D
													 //            case (GL_INT_IMAGE_2D_RECT): return ShaderDataType::INT_IMAGE_2D_RECT; // iimage2DRect
													 //            case (GL_INT_IMAGE_CUBE): return ShaderDataType::INT_IMAGE_CUBE; // iimageCube
													 //            case (GL_INT_IMAGE_BUFFER): return ShaderDataType::INT_IMAGE_BUFFER; // iimageBuffer
													 //            case (GL_INT_IMAGE_1D_ARRAY): return ShaderDataType::INT_IMAGE_1D_ARRAY; // iimage1DArray
													 //            case (GL_INT_IMAGE_2D_ARRAY): return ShaderDataType::INT_IMAGE_2D_ARRAY; // iimage2DArray
													 //            case (GL_INT_IMAGE_2D_MULTISAMPLE): return ShaderDataType::INT_IMAGE_2D_MULTISAMPLE; // iimage2DMS
													 //            case (GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY): return ShaderDataType::INT_IMAGE_2D_MULTISAMPLE_ARRAY; // iimage2DMSArray
													 //            case (GL_UNSIGNED_INT_IMAGE_1D): return ShaderDataType::UNSIGNED_INT_IMAGE_1D; // uimage1D
													 //            case (GL_UNSIGNED_INT_IMAGE_2D): return ShaderDataType::UNSIGNED_INT_IMAGE_2D; // uimage2D
													 //            case (GL_UNSIGNED_INT_IMAGE_3D): return ShaderDataType::UNSIGNED_INT_IMAGE_3D; // uimage3D
													 //            case (GL_UNSIGNED_INT_IMAGE_2D_RECT): return ShaderDataType::UNSIGNED_INT_IMAGE_2D_RECT; // uimage2DRect
													 //            case (GL_UNSIGNED_INT_IMAGE_CUBE): return ShaderDataType::UNSIGNED_INT_IMAGE_CUBE; // uimageCube
													 //            case (GL_UNSIGNED_INT_IMAGE_BUFFER): return ShaderDataType::UNSIGNED_INT_IMAGE_BUFFER; // uimageBuffer
													 //            case (GL_UNSIGNED_INT_IMAGE_1D_ARRAY): return ShaderDataType::UNSIGNED_INT_IMAGE_1D_ARRAY; // uimage1DArray
													 //            case (GL_UNSIGNED_INT_IMAGE_2D_ARRAY): return ShaderDataType::UNSIGNED_INT_IMAGE_2D_ARRAY; // uimage2DArray
													 //            case (GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE): return ShaderDataType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE; // uimage2DMS
													 //            case (GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY): return ShaderDataType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY; // uimage2DMSArray
													 //            case (GL_UNSIGNED_INT_ATOMIC_COUNTER): return ShaderDataType::UNSIGNED_INT_ATOMIC_COUNTER; // atomic_uint
				default: return ShaderDataType::None;
			}
		}

		static GLenum ShaderTypeToOpenGL(ShaderType shaderType)
		{
			VXM_PROFILE_FUNCTION();
			switch (shaderType) {
				case ShaderType::COMPUTE_SHADER:
					return GL_COMPUTE_SHADER;
					break;
				case ShaderType::VERTEX_SHADER:
					return GL_VERTEX_SHADER;
					break;
				case ShaderType::TESS_CONTROL_SHADER:
					return GL_TESS_CONTROL_SHADER;
					break;
				case ShaderType::TESS_EVALUATION_SHADER:
					return GL_TESS_EVALUATION_SHADER;
					break;
				case ShaderType::GEOMETRY_SHADER:
					return GL_GEOMETRY_SHADER;
					break;
				case ShaderType::FRAGMENT_SHADER:
					return GL_FRAGMENT_SHADER;
					break;
			}

			VXM_CORE_ASSERT(false, "ShaderType {0} not supported in OpenGL.", (int) shaderType);
			return 0;
		}

		static shaderc_shader_kind ShaderTypeToShaderC(ShaderType shaderType)
		{
			VXM_PROFILE_FUNCTION();
			switch (shaderType) {
				case ShaderType::COMPUTE_SHADER:
					return shaderc_glsl_compute_shader;
					break;
				case ShaderType::VERTEX_SHADER:
					return shaderc_glsl_vertex_shader;
					break;
				case ShaderType::TESS_CONTROL_SHADER:
					return shaderc_glsl_tess_control_shader;
					break;
				case ShaderType::TESS_EVALUATION_SHADER:
					return shaderc_glsl_tess_evaluation_shader;
					break;
				case ShaderType::GEOMETRY_SHADER:
					return shaderc_glsl_geometry_shader;
					break;
				case ShaderType::FRAGMENT_SHADER:
					return shaderc_glsl_fragment_shader;
					break;
			}
			VXM_CORE_ASSERT(false, "ShaderType {0} not supported...", ShaderTypeToString(shaderType));
			return (shaderc_shader_kind) -1;
		}

		static std::string ShaderTypeToFileExtension(ShaderType type)
		{
			VXM_PROFILE_FUNCTION();
			switch (type) {
				case ShaderType::COMPUTE_SHADER: return ".compute";
				case ShaderType::VERTEX_SHADER: return ".vert";
				case ShaderType::TESS_CONTROL_SHADER: return ".tessco";
				case ShaderType::TESS_EVALUATION_SHADER: return ".tessev";
				case ShaderType::GEOMETRY_SHADER: return ".geom";
				case ShaderType::FRAGMENT_SHADER: return ".frag";
				default: break;
			}
			VXM_CORE_WARNING("The ShaderType {0} is not handled.", ShaderTypeToString(type));
			return ".glsl";
		}

		static std::string HashSrc(const std::string& str)
		{
			SHA256 sha;
			return sha(str);
		}

		template<typename T>
		static std::string HashSrc(const std::vector<T>& src)
		{
			SHA256 sha;
			sha.add(src.data(), src.size() * sizeof(T));
			return sha.getHash();
		}
	}// namespace Utils


	OpenGLShader::OpenGLShader() = default;

	OpenGLShader::~OpenGLShader()
	{
		DeleteProgram();
	}

	OpenGLShader::OpenGLShader(OpenGLShader&& other) noexcept
	{
		swap(other);
	}

	OpenGLShader& OpenGLShader::operator=(OpenGLShader&& other) noexcept
	{
		swap(other);
		return *this;
	}

	void OpenGLShader::swap(OpenGLShader& other)
	{
		std::swap(m_Sources, other.m_Sources);
		std::swap(m_TimeSource, other.m_TimeSource);
		std::swap(m_VulkanSPIRV, other.m_VulkanSPIRV);
		std::swap(m_OpenGLSPIRV, other.m_OpenGLSPIRV);
		std::swap(m_OpenGLSourceCode, other.m_OpenGLSourceCode);
		std::swap(m_Name, other.m_Name);
		std::swap(m_RendererID, other.m_RendererID);
	}
	void OpenGLShader::SetName(std::string name) {m_Name = std::move(name);}

	const std::string& OpenGLShader::GetName() const {return m_Name;}
	Path OpenGLShader::GetCachePath(ShaderType shaderType, OpenGLShader::Target target) const
	{
		Path p = {FileSource::Cache, std::filesystem::path("shader/opengl") / m_Name / m_Name};

		switch (target) {
			case Vulkan:
				p.path += (".vulkan");
				break;
			case OpenGl:
				p.path += (".opengl");
				break;
			case HashVulkan:
				p.path += (".hash.vulkan");
				break;
			case HashOpenGl:
				p.path += (".hash.opengl");
				break;
		}
		p.path += (Utils::ShaderTypeToFileExtension(shaderType));
		return p;
	}

	void OpenGLShader::AddSource(ShaderType type, ShaderSourceField shaderSource)
	{
		m_Sources[type] = shaderSource;
	}

	void OpenGLShader::RemoveSource(ShaderType type)
	{
		m_Sources.erase(type);
	}

	void OpenGLShader::ClearSources()
	{
		m_Sources.clear();
	}

	void OpenGLShader::CompileOrGetVulkanBinaries()
	{
		//Profiling
		VXM_PROFILE_FUNCTION();
		const std::unordered_map<ShaderType, ShaderSourceField> &shaders = m_Sources;
		// Setup of the shader compiler to compile the glsl to spirv
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		options.AddMacroDefinition("VXM_OPENGL", "1");
		const bool optimize = true;
		if (optimize) {
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		auto &shaderData = m_VulkanSPIRV;
		for (auto &&[stage, sourceAssetField]: shaders) {
			if(!sourceAssetField) continue;

			// Fetch the filename we can give.
			std::string filename;
			Ref<ShaderSource> shaderSource = sourceAssetField.GetAsset();

			if (const Ref<FileShaderSource> fileSource = DynamicCastPtr<FileShaderSource>(shaderSource)) {
				filename = fileSource->SourcePath.path.filename().string();
				m_TimeSource[stage] = std::filesystem::last_write_time(fileSource->SourcePath);
			} else {
				filename = GetName() + Utils::ShaderTypeToFileExtension(stage);
			}

			// Fetch the hash of the source.
			const std::string& sourceStr = *shaderSource->GetString();
			std::string sourceHash = Utils::HashSrc(sourceStr);

			Path hashPath = GetCachePath(stage, Target::HashVulkan);
			Path cachedPath = GetCachePath(stage, Target::Vulkan);

			std::string storedHash = FileSystem::ReadFileAsString(hashPath);
			bool sameHash = (storedHash == sourceHash);
			shaderData[stage] = sameHash ? FileSystem::ReadFile<uint32_t>(cachedPath) : std::vector<uint32_t>();

			if (shaderData[stage].empty()) {
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(sourceStr, Utils::ShaderTypeToShaderC(stage), filename.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					VXM_CORE_ERROR("Shader ({0}) - Vulkan Pass {1}\n{2}", m_Name, Utils::ShaderTypeToString(stage), module.GetErrorMessage());
					if(FileSystem::Exist(cachedPath))
					{
						VXM_CORE_INFO("Reusing previous version.");
						shaderData[stage] = FileSystem::ReadFile<uint32_t>(cachedPath);
						// It doesn't correspond anymore, but we will load the same one if the same failed shader is started.
						// Better for the hot reload too.
						FileSystem::Write(hashPath, sourceHash);
					}
					continue;
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				FileSystem::Write(cachedPath, shaderData[stage]);
				FileSystem::Write(hashPath, sourceHash);
			}
		}
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		VXM_PROFILE_FUNCTION();
		auto &shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = true;
		if (optimize) {
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		for (auto &&[stage, spirv]: m_VulkanSPIRV) {

			if(spirv.empty())
			{
				VXM_CORE_ERROR("Shader ({0}) - Vulkan Pass {1} failed. Skipping this pass in OpenGl.", m_Name, Utils::ShaderTypeToString(stage));
				continue;
			}

			// Fetch the hash of the source.
			std::string sourceHash = Utils::HashSrc(spirv);

			Path hashPath = GetCachePath(stage, Target::HashOpenGl);
			Path cachedPath = GetCachePath(stage, Target::OpenGl);

			std::string storedHash = FileSystem::ReadFileAsString(hashPath);

			bool sameHash = (storedHash == sourceHash);
			shaderData[stage] = sameHash ? FileSystem::ReadFile<uint32_t>(cachedPath) : std::vector<uint32_t>();

			if (shaderData[stage].empty()) {
				spirv_cross::CompilerGLSL compilerGlsl(spirv);
				m_OpenGLSourceCode[stage] = compilerGlsl.compile();
				std::string &source = m_OpenGLSourceCode[stage];
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::ShaderTypeToShaderC(stage), m_Name.c_str(), options);

				if(module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					VXM_CORE_ERROR("Shader ({0}) - Vulkan Pass {1}\n{2}", m_Name, Utils::ShaderTypeToString(stage), module.GetErrorMessage());
					if(FileSystem::Exist(cachedPath))
					{
						VXM_CORE_INFO("Reusing previous version.");
						shaderData[stage] = FileSystem::ReadFile<uint32_t>(cachedPath);
						// It doesn't correspond anymore, but we will load the same one if the same failed shader is started.
						// Better for the hot reload too.
						FileSystem::Write(hashPath, sourceHash);
					}
					continue;
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				FileSystem::Write(cachedPath, shaderData[stage]);
				FileSystem::Write(hashPath, sourceHash);
			}
		}
	}

	void OpenGLShader::CreateProgram()
	{
		VXM_PROFILE_FUNCTION();
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto &&[stage, spirv]: m_OpenGLSPIRV) {
			if (spirv.empty())
			{
				VXM_CORE_ERROR("Shader ({0}) - Vulkan Pass {1} failed. Skipping this stage during the shader creation.", m_Name, Utils::ShaderTypeToString(stage));
				continue;
			}
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(Utils::ShaderTypeToOpenGL(stage)));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			if (maxLength > 0) {
				VXM_CORE_ERROR("Shader linking failed ({0}): {1}", m_Name, infoLog.data());
			} else {
				VXM_CORE_ERROR("Shader linking failed ({0}): Unknown reasons.", m_Name);
			}

			glDeleteProgram(program);
			for (auto shaderID: shaderIDs) {
				glDeleteShader(shaderID);
			}
			return;
		}

		for (auto id: shaderIDs) {
			glDetachShader(program, id);
			glDeleteShader(id);
		}
		VXM_CORE_INFO("Shader '{0}' Successfully created.", m_Name);

		m_RendererID = program;
	}

	void OpenGLShader::DeleteProgram()
	{
		VXM_PROFILE_FUNCTION();
		if(m_RendererID) {
			glDeleteProgram(m_RendererID);
			m_RendererID = 0;
		}
	}

	void OpenGLShader::Reload()
	{
		DeleteProgram();

		CompileOrGetVulkanBinaries();
		CompileOrGetOpenGLBinaries();
		CreateProgram();
	}

	const std::unordered_map<ShaderType, ShaderSourceField>& OpenGLShader::GetSources() const
	{
		return m_Sources;
	}

	const std::unordered_map<ShaderType, std::filesystem::file_time_type> & OpenGLShader::GetSourcesTimes() const {
		return m_TimeSource;
	}

	OpenGLGraphicsShader::OpenGLGraphicsShader(const std::string& name, const std::unordered_map<ShaderType, ShaderSourceField>& sources)
	{
		m_OpenGLShader.SetName(name);
		for(const auto&[type,src] : sources)
		{
			m_OpenGLShader.AddSource(type, src);
		}
		m_OpenGLShader.Reload();
	}

	OpenGLGraphicsShader::~OpenGLGraphicsShader()
	{
	}

	void OpenGLGraphicsShader::Bind() const
	{
		VXM_PROFILE_FUNCTION();
		glUseProgram(m_OpenGLShader.GetRendererID());
	}

	void OpenGLGraphicsShader::Unbind() const
	{
		VXM_PROFILE_FUNCTION();
		glUseProgram(0);
	}

	void OpenGLGraphicsShader::SetSources(const std::vector<ShaderSourceField>& sources)
	{
		VXM_PROFILE_FUNCTION();
		m_OpenGLShader.ClearSources();
		for(auto& src : sources)
		{
			if(src.HasHandle()) {
				m_OpenGLShader.AddSource(src.GetAsset()->GetShaderType(), src);
			}
		}

		Reload();
	}

	void OpenGLGraphicsShader::SetInt(const std::string &name, int value) {
		glUniform1i(glGetUniformLocation(m_OpenGLShader.GetRendererID(), name.c_str()), value);
	}

	std::vector<ShaderSourceField> OpenGLGraphicsShader::GetSources() const
	{
		VXM_PROFILE_FUNCTION();
		std::vector<ShaderSourceField> sources;
		const auto& sourcesMap = m_OpenGLShader.GetSources();
		sources.reserve(sourcesMap.size());
		for(auto& source : sourcesMap)
		{
			sources.push_back(source.second);
		}
		return sources;
	}

	OpenGLComputeShader::OpenGLComputeShader(const std::string &name, ShaderSourceField source)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(!source || source.GetAsset()->GetShaderType() == ShaderType::COMPUTE_SHADER, "The Shader Source is not a valid Compute Shader asset.");
		m_OpenGLShader.SetName(name);
		m_OpenGLShader.AddSource(ShaderType::COMPUTE_SHADER, source);
		m_OpenGLShader.Reload();
	}

	void OpenGLGraphicsShader::Reload()
	{
		VXM_PROFILE_FUNCTION();
		m_OpenGLShader.Reload();
	}

	void OpenGLGraphicsShader::ReloadIfAnyChanges() {
		bool hasChanged = false;
		for (auto&&[stage, time] : m_OpenGLShader.GetSourcesTimes()) {
			if (!m_OpenGLShader.GetSources().contains(stage)) continue;
			auto assetField = m_OpenGLShader.GetSources().at(stage);
			if (!assetField) continue;
			const Ref<FileShaderSource> source = DynamicCastPtr<FileShaderSource>(assetField.GetAsset());
			if (source) {
				if (source->HasChanged()) {
					source->TryReloadSource();
				}

				if (source->SourcePathTime > time) {
					hasChanged = true;
					break;
				}
			}
		}

		if (hasChanged) {
			Reload();
		}
	}

	std::string OpenGLGraphicsShader::GetName() const
	{
		return m_OpenGLShader.GetName();
	}

	void OpenGLGraphicsShader::SetName(const std::string& name)
	{
		m_OpenGLShader.SetName(name);
	}

	void OpenGLComputeShader::Bind() const
	{
		glUseProgram(m_OpenGLShader.GetRendererID());
	}
	void OpenGLComputeShader::Unbind() const
	{
		glUseProgram(0);
	}
	void OpenGLComputeShader::Reload()
	{
		m_OpenGLShader.Reload();
	}

	void OpenGLComputeShader::ReloadIfAnyChanges() {
		bool hasChanged = false;
		for (auto&&[stage, time] : m_OpenGLShader.GetSourcesTimes()) {
			if (!m_OpenGLShader.GetSources().contains(stage)) continue;
			auto assetField = m_OpenGLShader.GetSources().at(stage);
			if (!assetField) continue;
			const Ref<FileShaderSource> source = DynamicCastPtr<FileShaderSource>(assetField.GetAsset());
			if (source) {
				if (source->HasChanged()) {
					source->TryReloadSource();
				}

				if (source->SourcePathTime > time) {
					hasChanged = true;
					break;
				}
			}
		}

		if (hasChanged) {
			Reload();
		}
	}

	std::string OpenGLComputeShader::GetName() const
	{
		return m_OpenGLShader.GetName();
	}
	void OpenGLComputeShader::SetName(const std::string &name)
	{
		m_OpenGLShader.SetName(name);
	}

	ShaderSourceField OpenGLComputeShader::GetSource() const
	{
		const auto& srcs = m_OpenGLShader.GetSources();
		auto it = srcs.find(ShaderType::COMPUTE_SHADER);
		if(it != srcs.end()) return it->second;
		return {};
	}
	void OpenGLComputeShader::SetSource(ShaderSourceField source)
	{
		VXM_CORE_ASSERT(source && source.GetAsset()->GetShaderType() == ShaderType::COMPUTE_SHADER, "The Shader Source is not a valid Compute Shader asset.");
		m_OpenGLShader.AddSource(ShaderType::COMPUTE_SHADER, source);
	}

	void OpenGLComputeShader::Dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ)
	{
		VXM_PROFILE_FUNCTION();
		Bind();
		glDispatchCompute(groupX, groupY, groupZ);
		Unbind();
	}
}// namespace Voxymore::Core