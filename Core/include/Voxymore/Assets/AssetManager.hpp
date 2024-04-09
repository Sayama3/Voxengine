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

		template<typename T>
		static Ref<T> GetAssetAs(AssetHandle handle)
		{
			VXM_PROFILE_FUNCTION();
			Ref<Asset> asset = Project::GetActive()->GetAssetManager()->GetAsset(handle);
			return CastPtr<T>(asset);
		}
	};

} // namespace Voxymore::Core

