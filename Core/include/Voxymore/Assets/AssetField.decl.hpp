//
// Created by Sayama on 05/12/2024.
//

#pragma once

#include "AssetField.forward.hpp"
#include "AssetHandle.hpp"

namespace Voxymore::Core {
    template<typename T>
    class AssetField {
    public:
        AssetField();
        ~AssetField();
        AssetField(AssetHandle handle);
        AssetField(Ref<T> asset);
        AssetField(const T &asset);
        AssetField(const T *asset);

    public:
        [[nodiscard]] bool HasHandle() const;
        [[nodiscard]] Ref<T> GetAsset() const;
        [[nodiscard]] AssetHandle GetHandle() const;
        void SetHandle(AssetHandle handle);
        void Reset();
        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] explicit operator Ref<T>() const;
        [[nodiscard]] explicit operator AssetHandle() const;
        [[nodiscard]] explicit operator bool() const;
        static AssetType GetStaticType();
        AssetType GetType() const;
        bool operator==(AssetField rhs) const;
        bool operator!=(AssetField rhs) const;
    private:
        AssetHandle Handle = NullAssetHandle;
    };

    template<typename T>
    YAML::Emitter &operator<<(YAML::Emitter &out, const AssetField<T> &v) {
        out << v.GetHandle();
        return out;
    }
}

namespace YAML {
    template<typename T>
    struct convert<::Voxymore::Core::AssetField<T> > {
        static Node encode(const ::Voxymore::Core::AssetField<T> &rhs);
        static bool decode(const Node &node, ::Voxymore::Core::AssetField<T> &rhs);
    };
}
