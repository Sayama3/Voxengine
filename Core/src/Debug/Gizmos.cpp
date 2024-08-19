//
// Created by ianpo on 15/08/2024.
//

#include "Voxymore/Debug/Gizmos.hpp"
#include <algorithm>


namespace Voxymore::Core
{
	bool operator<(const Gizmos::Key &lft, const Gizmos::Key &rht)
	{
		// Not using depth is "inferior" to using Depth
		if (lft.UseDepth != rht.UseDepth) return rht.UseDepth;
		if (lft.IsWiremesh != rht.IsWiremesh) return rht.IsWiremesh;
		// Then normal comparison.
		return lft.TimeAlive < rht.TimeAlive;
	}

	bool operator==(const Gizmos::Key &lft, const Gizmos::Key &rht)
	{
		return lft.UseDepth == rht.UseDepth &&  lft.IsWiremesh == rht.IsWiremesh && lft.TimeAlive == rht.TimeAlive;
	}

	void Gizmo::EnsureMeshExist() {
		if(Mesh) return;
		if(Indices.empty()) Mesh = CreateRef<Voxymore::Core::Mesh>(Vertices);
		else Mesh = CreateRef<Voxymore::Core::Mesh>(Vertices, Indices);
	}

	Gizmos::Gizmos() : Layer("GizmoLayer") {}
	Gizmos::~Gizmos() = default;

	void Gizmos::OnAttach()
	{
		++s_GizmosLayerCount;
	}

	void Gizmos::OnDetach()
	{
		if(--s_GizmosLayerCount <= 0) Gizmos::Clear();
	}

	void Gizmos::OnUpdate(TimeStep timeStep)
	{
		VXM_PROFILE_FUNCTION();
		if(s_Gizmos.empty()) return;

		Container tmp{};

		for (auto& [key, gizmo] : s_Gizmos) {
			auto&[time, isWiremodel, useDepth] = key;
			if(time-timeStep > 0) {
				tmp.insert({Key{time - timeStep, isWiremodel, useDepth}, std::move(gizmo)});
			}
		}

		std::swap(tmp, s_Gizmos);
	}

	GizmosIterator Gizmos::begin()
	{
		return s_Gizmos.begin();
	}
	ConstGizmosIterator Gizmos::cbegin()
	{
		return s_Gizmos.cbegin();
	}
	GizmosIterator Gizmos::end()
	{
		return s_Gizmos.end();
	}
	ConstGizmosIterator Gizmos::cend()
	{
		return s_Gizmos.cend();
	}
	GizmosIterator Gizmos::begin_depth()
	{
		return get_begin_depth();
	}
	GizmosIterator Gizmos::end_depth()
	{
		return get_end_depth();
	}
	GizmosIterator Gizmos::begin_non_depth()
	{
		return get_begin_non_depth();
	}
	GizmosIterator Gizmos::end_non_depth()
	{
		return get_end_non_depth();
	}

	void Gizmos::Clear()
	{
		s_Gizmos.clear();
		s_CubeMesh = nullptr;
	}

	Gizmos::Container::const_iterator Gizmos::get_cbegin_non_depth()
	{
		return std::find_if(s_Gizmos.cbegin(), s_Gizmos.cend(), [](const Container::value_type& iter) {return !iter.first.UseDepth;});
	}

	Gizmos::Container::const_iterator Gizmos::get_cend_non_depth()
	{
		return std::find_if(get_cbegin_non_depth(), s_Gizmos.cend(), [](const Container::value_type& iter) {return iter.first.UseDepth;});
	}

	Gizmos::Container::const_iterator Gizmos::get_cbegin_depth()
	{
		return std::find_if(s_Gizmos.cbegin(), s_Gizmos.cend(), [](const Container::value_type& iter) {return iter.first.UseDepth;});
	}

	Gizmos::Container::const_iterator Gizmos::get_cend_depth()
	{
		return s_Gizmos.cend();
	}

	Gizmos::Container::const_reverse_iterator Gizmos::get_crbegin_depth()
	{
		return std::find_if(s_Gizmos.crbegin(), s_Gizmos.crend(), [](const Container::value_type& pair) {return pair.first.UseDepth;});
	}

	Gizmos::Container::const_reverse_iterator Gizmos::get_crend_depth()
	{
		return std::find_if(get_crbegin_depth(), s_Gizmos.crend(), [](const Container::value_type& pair) {return !pair.first.UseDepth;});
	}

	Gizmos::Container::const_reverse_iterator Gizmos::get_crbegin_non_depth()
	{
		return std::find_if(s_Gizmos.crbegin(), s_Gizmos.crend(), [](const Container::value_type& pair) {return pair.first.UseDepth;});
	}

	Gizmos::Container::const_reverse_iterator Gizmos::get_crend_non_depth()
	{
		return s_Gizmos.crend();
	}

	Gizmos::Container::iterator Gizmos::get_begin_non_depth()
	{
		return std::find_if(s_Gizmos.begin(), s_Gizmos.end(), [](const Container::value_type& iter) {return !iter.first.UseDepth;});
	}

	Gizmos::Container::iterator Gizmos::get_end_non_depth()
	{
		return std::find_if(get_begin_non_depth(), s_Gizmos.end(), [](const Container::value_type& iter) {return iter.first.UseDepth;});
	}

	Gizmos::Container::iterator Gizmos::get_begin_depth()
	{
		return std::find_if(s_Gizmos.begin(), s_Gizmos.end(), [](const Container::value_type& iter) {return iter.first.UseDepth;});
	}

	Gizmos::Container::iterator Gizmos::get_end_depth()
	{
		return s_Gizmos.end();
	}

	Gizmos::Container::reverse_iterator Gizmos::get_rbegin_depth()
	{
		return std::find_if(s_Gizmos.rbegin(), s_Gizmos.rend(), [](const Container::value_type& pair) {return pair.first.UseDepth;});
	}

	Gizmos::Container::reverse_iterator Gizmos::get_rend_depth()
	{
		return std::find_if(get_rbegin_depth(), s_Gizmos.rend(), [](const Container::value_type& pair) {return !pair.first.UseDepth;});
	}

	Gizmos::Container::reverse_iterator Gizmos::get_rbegin_non_depth()
	{
		return std::find_if(s_Gizmos.rbegin(), s_Gizmos.rend(), [](const Container::value_type& pair) {return pair.first.UseDepth;});
	}

	Gizmos::Container::reverse_iterator Gizmos::get_rend_non_depth()
	{
		return s_Gizmos.rend();
	}
	void Gizmos::DrawCube(Vec3 position, Quat rotation, Vec3 size, Vec4 color, Real time, bool useDepth, bool isWiremesh)
	{
		VXM_PROFILE_FUNCTION();
		if(s_GizmosLayerCount <= 0) return;
		VXM_CORE_ASSERT(RendererAPI::GetAPI() != RendererAPI::API::None, "There is no Renderer API.");
		Gizmo gizmo{{},{},Math::TRS(position,rotation,size),color,GetOrCreateCube()};
		s_Gizmos.insert({{time, isWiremesh, useDepth}, std::move(gizmo)});
	}
	void Gizmos::DrawMesh(Ref<Mesh> mesh, Vec3 position, Quat rotation, Vec3 size, Vec4 color, Real time, bool useDepth, bool isWiremesh)
	{
		VXM_PROFILE_FUNCTION();
		if(s_GizmosLayerCount <= 0) return;
		VXM_CORE_ASSERT(RendererAPI::GetAPI() != RendererAPI::API::None, "There is no Renderer API.");
		Gizmo gizmo{{},{},Math::TRS(position,rotation,size),color,std::move(mesh)};
		s_Gizmos.insert({{time, isWiremesh, useDepth}, std::move(gizmo)});
	}
	void Gizmos::DrawMesh(std::vector<Vertex> vertices, Vec3 position, Quat rotation, Vec3 size, Vec4 color, Real time, bool useDepth, bool isWiremesh)
	{
		VXM_PROFILE_FUNCTION();
		if(s_GizmosLayerCount <= 0) return;
		VXM_CORE_ASSERT(RendererAPI::GetAPI() != RendererAPI::API::None, "There is no Renderer API.");
		Gizmo gizmo{std::move(vertices),{},Math::TRS(position,rotation,size),color,nullptr};
		gizmo.EnsureMeshExist();
		s_Gizmos.insert({{time, isWiremesh, useDepth}, std::move(gizmo)});
	}
	void Gizmos::DrawMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Vec3 position, Quat rotation, Vec3 size, Vec4 color, Real time, bool useDepth, bool isWiremesh)
	{
		VXM_PROFILE_FUNCTION();
		if(s_GizmosLayerCount <= 0) return;
		VXM_CORE_ASSERT(RendererAPI::GetAPI() != RendererAPI::API::None, "There is no Renderer API.");
		Gizmo gizmo{std::move(vertices),std::move(indices),Math::TRS(position,rotation,size),color,nullptr};
		gizmo.EnsureMeshExist();
		s_Gizmos.insert({{time, isWiremesh, useDepth}, std::move(gizmo)});
	}
	void Gizmos::DrawCube(Mat4 model, Vec4 color, Real time, bool useDepth, bool isWiremesh)
	{
		VXM_PROFILE_FUNCTION();
		if(s_GizmosLayerCount <= 0) return;
		VXM_CORE_ASSERT(RendererAPI::GetAPI() != RendererAPI::API::None, "There is no Renderer API.");
		Gizmo gizmo{{}, {} ,model, color, GetOrCreateCube()};
		s_Gizmos.insert({{time, isWiremesh, useDepth}, std::move(gizmo)});
	}
	void Gizmos::DrawMesh(Ref<Mesh> mesh, Mat4 model, Vec4 color, Real time, bool useDepth, bool isWiremesh)
	{
		VXM_PROFILE_FUNCTION();
		if(s_GizmosLayerCount <= 0) return;
		VXM_CORE_ASSERT(RendererAPI::GetAPI() != RendererAPI::API::None, "There is no Renderer API.");
		Gizmo gizmo{{}, {} ,model, color, std::move(mesh)};
		s_Gizmos.insert({{time, isWiremesh, useDepth}, std::move(gizmo)});
	}
	void Gizmos::DrawMesh(std::vector<Vertex> vertices, Mat4 model, Vec4 color, Real time, bool useDepth, bool isWiremesh)
	{
		VXM_PROFILE_FUNCTION();
		if(s_GizmosLayerCount <= 0) return;
		VXM_CORE_ASSERT(RendererAPI::GetAPI() != RendererAPI::API::None, "There is no Renderer API.");
		Gizmo gizmo{std::move(vertices), {} ,model, color, nullptr};
		gizmo.EnsureMeshExist();
		s_Gizmos.insert({{time, isWiremesh, useDepth}, std::move(gizmo)});
	}
	void Gizmos::DrawMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Mat4 model, Vec4 color, Real time, bool useDepth, bool isWiremesh)
	{
		VXM_PROFILE_FUNCTION();
		if(s_GizmosLayerCount <= 0) return;
		VXM_CORE_ASSERT(RendererAPI::GetAPI() != RendererAPI::API::None, "There is no Renderer API.");
		Gizmo gizmo{std::move(vertices), std::move(indices) ,model, color, nullptr};
		gizmo.EnsureMeshExist();
		s_Gizmos.insert({{time, isWiremesh, useDepth}, std::move(gizmo)});
	}
	Ref<Mesh> Gizmos::GetOrCreateCube()
	{
		if(!s_CubeMesh) s_CubeMesh = PrimitiveMesh::CreateOrphan(PrimitiveMesh::Cube, NullAssetHandle);
		return s_CubeMesh;
	}

	GizmosIterator GizmosIterator::operator+=(int count)
	{
		uint16_t amount = std::abs(count);
		bool isNeg = count < 0;
		for(uint16_t i{0}; i < amount;++i) isNeg ? --m_GizmoIterator : ++m_GizmoIterator;
		return *this;
	}

	ConstGizmosIterator ConstGizmosIterator::operator+=(int count)
	{
		uint16_t amount = std::abs(count);
		bool isNeg = count < 0;
		for(uint16_t i{0}; i < amount;++i) isNeg ? --m_GizmoIterator : ++m_GizmoIterator;
		return *this;
	}
} // namespace Voxymore::Core
