//
// Created by ianpo on 15/08/2024.
//

#pragma once

#include "Voxymore/Layers/Layer.hpp"
#include "Voxymore/Renderer/Mesh.hpp"

#include <map>
#include <thread>

namespace Voxymore::Core
{
	struct Gizmo {
		std::vector<Vertex> Vertices {};
		std::vector<uint32_t> Indices {};
		Mat4 ModelMatrix;
		Vec4 Color {1,1,1,1};
		Ref<Mesh> Mesh {nullptr};
		void EnsureMeshExist();
	};

	class GizmosIterator;
	class ConstGizmosIterator;

	class Gizmos : public Layer
	{
	public:
		static inline constexpr Vec4 DefaultColor {0.10,0.85,0.20,1.0};
		struct Key {
			Real TimeAlive{0};
			bool IsWiremesh{false};
			bool UseDepth{false};
		};
		using Container = std::multimap<Key, Gizmo>;
	public:
		Gizmos();
		virtual ~Gizmos() override;
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStep timeStep) override;
	public:
		static void DrawCube(Vec3 position, Quat rotation, Vec3 size = Vec3{1,1,1},Vec4 color = DefaultColor, Real time = -1, bool useDepth = false, bool isWiremesh = true);
		//static void DrawSphere(Vec3 position, Quat rotation, Vec3 size = Vec3{1,1,1},Vec4 color = DefaultColor, Real time = -1, bool useDepth = false, bool isWiremesh = true);
		static void DrawMesh(Ref<Mesh> mesh, Vec3 position, Quat rotation = Math::Identity<Quat>(), Vec3 size = Vec3{1,1,1},Vec4 color = DefaultColor, Real time = -1, bool useDepth = false, bool isWiremesh = true);
		static void DrawMesh(std::vector<Vertex> vertices, Vec3 position, Quat rotation = Math::Identity<Quat>(), Vec3 size = Vec3{1,1,1},Vec4 color = DefaultColor, Real time = -1, bool useDepth = false, bool isWiremesh = true);
		static void DrawMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices , Vec3 position, Quat rotation = Math::Identity<Quat>(), Vec3 size = Vec3{1,1,1}, Vec4 color = DefaultColor, Real time = -1, bool useDepth = false, bool isWiremesh = true);
		static void DrawCube(Mat4 model, Vec4 color = DefaultColor, Real time = -1, bool useDepth = false, bool isWiremesh = true);
		//static void DrawSphere(Mat4 model, Vec4 color = DefaultColor, Real time = -1, bool useDepth = false, bool isWiremesh = true);
		static void DrawMesh(Ref<Mesh> mesh, Mat4 model = Math::Identity<Mat4>(),Vec4 color = DefaultColor, Real time = -1, bool useDepth = false, bool isWiremesh = true);
		static void DrawMesh(std::vector<Vertex> vertices, Mat4 model = Math::Identity<Mat4>(),Vec4 color = DefaultColor, Real time = -1, bool useDepth = false, bool isWiremesh = true);
		static void DrawMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices , Mat4 model = Math::Identity<Mat4>(), Vec4 color = DefaultColor, Real time = -1, bool useDepth = false, bool isWiremesh = true);
	public:
		static GizmosIterator begin();
		static ConstGizmosIterator cbegin();

		static GizmosIterator end();
		static ConstGizmosIterator cend();
	public:
		static GizmosIterator begin_depth();
		static GizmosIterator end_depth();
		static GizmosIterator begin_non_depth();
		static GizmosIterator end_non_depth();
	public:
		static Container::const_iterator get_cbegin_depth();
		static Container::const_iterator get_cend_depth();
		static Container::const_iterator get_cbegin_non_depth();
		static Container::const_iterator get_cend_non_depth();

		static Container::const_reverse_iterator get_crbegin_depth();
		static Container::const_reverse_iterator get_crend_depth();
		static Container::const_reverse_iterator get_crbegin_non_depth();
		static Container::const_reverse_iterator get_crend_non_depth();
	private:
		static Container::iterator get_begin_depth();
		static Container::iterator get_end_depth();
		static Container::iterator get_begin_non_depth();
		static Container::iterator get_end_non_depth();

		static Container::reverse_iterator get_rbegin_depth();
		static Container::reverse_iterator get_rend_depth();
		static Container::reverse_iterator get_rbegin_non_depth();
		static Container::reverse_iterator get_rend_non_depth();
	private:
		static Ref<Mesh> GetOrCreateCube();
		//static Ref<Mesh> GetOrCreateSphere();
	private:
		static void Clear();
	private:
		static inline Container s_Gizmos {};
		// int64_t just in case of a bug where we would go bellow 0. Not suppose to have that many layer anyway.
		static inline int64_t s_GizmosLayerCount{0};
		static inline Ref<Mesh> s_CubeMesh{nullptr};
		//static inline Ref<Mesh> s_SphereMesh{nullptr};
	private:
		friend class GizmosIterator;
		friend class ConstGizmosIterator;
	};

	class GizmosIterator {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = Gizmo;
		using pointer           = value_type*;
		using reference         = value_type&;
	private:
		using sub_iterator =  Gizmos::Container::iterator;
	public:
		GizmosIterator() = default;
		~GizmosIterator() = default;
		GizmosIterator(sub_iterator iter) : m_GizmoIterator(std::move(iter)) {}
		GizmosIterator(Gizmos::Container& container, int index = 0) : GizmosIterator(GizmosIterator(container.begin()) + index) {}
	public:
		reference operator *() {return m_GizmoIterator->second;}
		pointer operator->() {return &m_GizmoIterator->second;}

		GizmosIterator operator++(){++m_GizmoIterator; return *this;};
		GizmosIterator operator++(int){auto tmp = *this;++m_GizmoIterator;return tmp;};
		GizmosIterator operator+=(int count);
		GizmosIterator operator+ (int count) {auto tmp{*this};tmp+=count;return tmp;}

		GizmosIterator operator--(){--m_GizmoIterator; return *this;};
		GizmosIterator operator--(int){auto tmp = *this;--m_GizmoIterator;return tmp;};
		GizmosIterator operator-=(int count) {*this+=-count; return *this;};
		GizmosIterator operator- (int count) {auto tmp{*this};tmp-=count;return tmp;}

		GizmosIterator operator[](int index) {return *this + index;}

		friend bool operator ==(const GizmosIterator& lft, const GizmosIterator& rht) {return lft.m_GizmoIterator == rht.m_GizmoIterator;}
		friend bool operator !=(const GizmosIterator& lft, const GizmosIterator& rht) {return !(lft == rht);}
	private:
		sub_iterator m_GizmoIterator{};
	};

	class ConstGizmosIterator {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = Gizmo;
		using pointer           = const value_type*;
		using reference         = const value_type&;
	private:
		using sub_iterator =  Gizmos::Container::const_iterator;
	public:
		ConstGizmosIterator() = default;
		~ConstGizmosIterator() = default;
		ConstGizmosIterator(sub_iterator iter) : m_GizmoIterator(std::move(iter)) {}
		ConstGizmosIterator(const Gizmos::Container& container, int index = 0) : ConstGizmosIterator(ConstGizmosIterator(container.cbegin()) + index) {}
	public:
		reference operator *() const {return m_GizmoIterator->second;}
		pointer operator->() const {return &m_GizmoIterator->second;}

		ConstGizmosIterator operator++(){++m_GizmoIterator; return *this;};
		ConstGizmosIterator operator++(int){auto tmp = *this;++m_GizmoIterator;return tmp;};
		ConstGizmosIterator operator+=(int count);
		ConstGizmosIterator operator+ (int count) {auto tmp{*this};tmp+=count;return tmp;}

		ConstGizmosIterator operator--(){--m_GizmoIterator; return *this;};
		ConstGizmosIterator operator--(int){auto tmp = *this;--m_GizmoIterator;return tmp;};
		ConstGizmosIterator operator-=(int count) {*this+=-count; return *this;};
		ConstGizmosIterator operator- (int count) {auto tmp{*this};tmp-=count;return tmp;}

		ConstGizmosIterator operator[](int index) {return *this + index;}

		friend bool operator ==(const ConstGizmosIterator& lft, const ConstGizmosIterator& rht) {return lft.m_GizmoIterator == rht.m_GizmoIterator;}
		friend bool operator !=(const ConstGizmosIterator& lft, const ConstGizmosIterator& rht) {return !(lft == rht);}
	private:
		sub_iterator m_GizmoIterator{};
	};

	// =============== Gizmos::Key Comparison Operators =============== //
			bool operator<(const Gizmos::Key& lft, const Gizmos::Key& rht);
	inline	bool operator> (const Gizmos::Key& lhs, const Gizmos::Key& rhs) { return rhs < lhs; }
	inline	bool operator<=(const Gizmos::Key& lhs, const Gizmos::Key& rhs) { return !(lhs > rhs); }
	inline	bool operator>=(const Gizmos::Key& lhs, const Gizmos::Key& rhs) { return !(lhs < rhs); }\
			bool operator==(const Gizmos::Key& lhs, const Gizmos::Key& rhs);
	inline	bool operator!=(const Gizmos::Key& lhs, const Gizmos::Key& rhs) { return !(lhs == rhs); }

} // namespace Voxymore::Core

