//
// Created by Sayama on 05/12/2024.
//

#pragma once

#include "AssetField.decl.hpp"
#include "AssetManager.hpp"
#include "Voxymore/Core/YamlHelper.hpp"


namespace Voxymore::Core
{
    template<typename T>
    AssetField<T>::AssetField() = default;

    template<typename T>
    AssetField<T>::~AssetField() = default;

    template<typename T>
    AssetField<T>::AssetField(AssetHandle handle): Handle(handle) {}

    template<typename T>
    AssetField<T>::AssetField(Ref<T> asset): Handle(asset->Handle) {}

    template<typename T>
    AssetField<T>::AssetField(const T &asset): Handle(asset.Handle) {}

    template<typename T>
    AssetField<T>::AssetField(const T *asset): Handle(asset->Handle) {}

    template<typename T>
    bool AssetField<T>::HasHandle() const {
        return Handle != NullAssetHandle;
    }

    template<typename T>
    Ref<T> AssetField<T>::GetAsset() const {
        VXM_CORE_ASSERT(AssetManager::IsAssetHandleValid(Handle), "The handle is not valid.");
        VXM_PROFILE_FUNCTION();
        return AssetManager::GetAssetAs<T>(Handle);
    }

    template<typename T>
    AssetHandle AssetField<T>::GetHandle() const {
        return Handle;
    }

    template<typename T>
    void AssetField<T>::SetHandle(AssetHandle handle) {
        Handle = handle;
    }

    template<typename T>
    void AssetField<T>::Reset() {
        Handle = NullAssetHandle;
    }

    template<typename T>
    bool AssetField<T>::IsValid() const {
        VXM_PROFILE_FUNCTION();
        return HasHandle() && AssetManager::IsAssetHandleValid(Handle);
    }

    template<typename T>
    AssetField<T>::operator Ref<T>() const {
        return GetAsset();
    }

    template<typename T>
    AssetField<T>::operator UUID() const {
        return GetHandle();
    }

    template<typename T>
    AssetField<T>::operator bool() const {
        return IsValid();
    }

    template<typename T>
    AssetType AssetField<T>::GetStaticType() {return T::GetStaticType(); }

    template<typename T>
    AssetType AssetField<T>::GetType() const {return GetStaticType(); }

    template<typename T>
    bool AssetField<T>::operator==(AssetField rhs) const {
        return rhs.Handle == Handle;
    }

    template<typename T>
    bool AssetField<T>::operator!=(AssetField rhs) const {
        return !(rhs.Handle == Handle);
    }
}


namespace YAML {
    template<typename T>
    Node convert<Voxymore::Core::AssetField<T>>::encode(const Voxymore::Core::AssetField<T> &rhs) {
        Node node;
        node = rhs.GetHandle();
        return node;
    }

    template<typename T>
    bool convert<Voxymore::Core::AssetField<T>>::decode(const Node &node, Voxymore::Core::AssetField<T> &rhs) {
        rhs.SetHandle(node.as<::Voxymore::Core::AssetHandle>());
        return true;
    }
}