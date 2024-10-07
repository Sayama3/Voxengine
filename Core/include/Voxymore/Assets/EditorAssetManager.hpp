//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include "AssetManagerBase.hpp"
#include "AssetMetadata.hpp"
#include "AssetImporter.hpp"
#include <map>

namespace Voxymore::Core
{
	// ordered map is cool for debugging
	using AssetRegistry = std::map<AssetHandle, AssetMetadata>;
	using AssetRegistryIterator = std::map<AssetHandle, AssetMetadata>::iterator;

	class EditorAssetManager : public AssetManagerBase
	{
	public:
		[[nodiscard]] virtual bool IsAssetHandleValid(AssetHandle handle) const override;
		[[nodiscard]] virtual bool IsAssetLoaded(AssetHandle handle) const override;
		[[nodiscard]] virtual AssetType GetAssetType(AssetHandle handle) const override;
		virtual Ref<Asset> GetAsset(AssetHandle handle) override;
	public:
		[[nodiscard]] bool IsAssetImported(const Path& path, AssetHandle* handle = nullptr) const;
		[[nodiscard]] const AssetMetadata& GetMetadata(AssetHandle handle) const;
		Ref<Asset> ImportAsset(const Path& assetPath, AssetType hint = AssetType::None);
		Ref<Asset> GetOrCreateAsset(const Path& assetPath, AssetType hint = AssetType::None);

		[[nodiscard]] Path GetFilePath(AssetHandle) const;
		[[nodiscard]] AssetMetadata GetMetadata(const Path&) const;

		void SetPath(AssetHandle handle, Path newPath);

		bool AddMemoryAsset(Ref<Asset> asset);
		bool AddAsset(Ref<Asset> asset, const Path& path);

		bool SaveMemoryAsset(AssetHandle handle, const Path& path);

		void RemoveAsset(AssetHandle handle);
		void UnloadAsset(AssetHandle handle);

		void SerializeAssetRegistry();
		void DeserializeAssetRegistry();
	public:
		Ref<Asset> GetLoadedAsset(AssetHandle handle) const;

		template<typename T, typename ...Args>
		Ref<T> CreateMemoryAsset(Args&&... args);

		template<typename T, typename ...Args>
		Ref<T> CreateAsset(Path path, Args&&... args);
	private:
		AssetMap m_LoadedAssets;
		AssetMap m_MemoryAssets;
		AssetRegistry m_AssetRegistry;
	};


	template<typename T, typename... Args>
	Ref<T> EditorAssetManager::CreateMemoryAsset(Args &&...args)
	{
		Ref<T> asset = CreateRef<T>(std::forward<Args>(args)...);
		m_MemoryAssets.emplace(asset->Handle, asset);
		return asset;
	}

	template<typename T, typename ...Args>
	inline Ref<T> EditorAssetManager::CreateAsset(Path path, Args&&... args)
	{
		AssetMetadata metadata;
		metadata.FilePath = std::move(path);
		metadata.Type = T::GetStaticType();
		Ref<T> asset = CreateRef<T>(std::forward<Args>(args)...);
		asset->Handle = metadata.Handle;
		m_AssetRegistry.emplace(metadata.Handle, metadata);
		m_LoadedAssets.emplace(metadata.Handle, asset);
		SerializeAssetRegistry();
		return asset;
	}

} // namespace Voxymore::Core

