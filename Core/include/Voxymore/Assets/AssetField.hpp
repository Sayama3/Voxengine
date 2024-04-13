//
// Created by ianpo on 11/04/2024.
//

#pragma once

#include "Asset.hpp"
#include "AssetManager.hpp"
#include "Voxymore/Core/YamlHelper.hpp"

namespace Voxymore::Core
{
	template<typename T>
	class AssetField
	{
	public:
		inline AssetField() = default;
		inline ~AssetField() = default;
		inline AssetField(AssetHandle handle) : Handle(handle) {}
		inline AssetField(Ref<T> asset) : Handle(asset->Handle) {}
		inline AssetField(const T& asset) : Handle(asset.Handle) {}
	public:

		[[nodiscard]] inline bool HasHandle() const {
			return Handle != 0;
		}

		[[nodiscard]] inline Ref<T> GetAsset() const {
			VXM_CORE_ASSERT(AssetManager::IsAssetHandleValid(Handle), "The handle is not valid.");
			return AssetManager::GetAssetAs<T>(Handle);
		}

		[[nodiscard]] inline AssetHandle GetHandle() const {
			return Handle;
		}

		inline void SetHandle(AssetHandle handle) {
			Handle = handle;
		}

		inline void Reset() {
			Handle = 0;
		}

		[[nodiscard]] inline bool IsValid() const {
			return HasHandle() && AssetManager::IsAssetHandleValid(Handle);
		}

		[[nodiscard]] inline explicit operator Ref<T>() const {
			return GetAsset();
		}

		[[nodiscard]] inline explicit operator AssetHandle() const {
			return GetHandle();
		}

		[[nodiscard]] inline explicit operator bool() const {
			return IsValid();
		}

		inline static AssetType GetStaticType() {return T::GetStaticType(); }
		inline AssetType GetType() const {return GetStaticType(); }

		inline bool operator==(AssetField rhs) const {
			return rhs.Handle == Handle;
		}

		inline bool operator!=(AssetField rhs) const {
			return !(rhs.Handle == Handle);
		}
	private:
		AssetHandle Handle = 0;
	};

	template<typename T>
	YAML::Emitter &operator<<(YAML::Emitter &out, const AssetField<T>& v)
	{
		out << v.GetHandle();
		return out;
	}
}// namespace Voxymore::Core

namespace YAML
{
	template<typename T>
	struct convert<::Voxymore::Core::AssetField<T>>
	{
		inline static Node encode(const ::Voxymore::Core::AssetField<T>& rhs)
		{
			Node node;
			node = rhs.GetHandle();
			return node;
		}
		inline static bool decode(const Node& node, ::Voxymore::Core::AssetField<T>& rhs)
		{
			rhs.SetHandle(node.as<::Voxymore::Core::AssetHandle>());
			return true;
		}
	};
}