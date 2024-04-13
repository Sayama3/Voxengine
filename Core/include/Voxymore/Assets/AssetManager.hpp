//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Project/Project.hpp"

namespace Voxymore::Core
{

	class AssetManager
	{
	public:
		inline static Ref<Asset> GetAsset(AssetHandle handle)
		{
			VXM_PROFILE_FUNCTION();
			return Project::GetActive()->GetAssetManager()->GetAsset(handle);
		}

		template<typename T>
		inline static Ref<T> GetAssetAs(AssetHandle handle)
		{
			VXM_PROFILE_FUNCTION();
			Ref<Asset> asset = GetAsset(handle);
			return CastPtr<T>(asset);
		}

		inline static bool IsAssetHandleValid(AssetHandle handle)
		{
			VXM_PROFILE_FUNCTION();
			return Project::GetActive()->GetAssetManager()->IsAssetHandleValid(handle);
		}
	};

} // namespace Voxymore::Core

