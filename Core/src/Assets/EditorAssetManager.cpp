//
// Created by ianpo on 09/04/2024.
//

#include "Voxymore/Assets/EditorAssetManager.hpp"
#include "Voxymore/Core/Core.hpp"


namespace Voxymore::Core
{
	bool EditorAssetManager::IsAssetHandleValid(AssetHandle handle) const
	{
		VXM_PROFILE_FUNCTION();
		return handle != 0 && m_AssetRegistry.contains(handle);
	}

	bool EditorAssetManager::IsAssetLoaded(AssetHandle handle) const
	{
		VXM_PROFILE_FUNCTION();
		return m_LoadedAssets.contains(handle);
	}

	const AssetMetadata& EditorAssetManager::GetMetadata(AssetHandle handle) const
	{
		VXM_PROFILE_FUNCTION();
		static AssetMetadata s_NullMetadata;
		auto it = m_AssetRegistry.find(handle);
		if(it == m_AssetRegistry.end()) {
			return s_NullMetadata;
		}
		return it->second;
	}

	Ref<Asset> EditorAssetManager::GetAsset(AssetHandle handle)
	{
		VXM_PROFILE_FUNCTION();
		if(IsAssetHandleValid(handle)) {
			return nullptr;
		}

		Ref<Asset> asset = nullptr;

		if(IsAssetLoaded(handle))
		{
			asset = m_LoadedAssets.at(handle);
		}
		else
		{
			const auto& metadata = GetMetadata(handle);
			asset = AssetImporter::ImportAsset(metadata);
			if(asset) {
				m_LoadedAssets.insert({handle, asset});
			} else {
				VXM_CORE_ERROR("Could not load the asset {0}", handle);
				//TODO: use this version :
				// VXM_CORE_ERROR("Could not load the asset {0} ({1})", handle, metadata);
			}
		}

		return asset;
	}
} // namespace Voxymore::Core
