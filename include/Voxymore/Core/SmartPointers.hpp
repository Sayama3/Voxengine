//
// Created by ianpo on 27/07/2023.
//

#pragma once

#include <memory>

namespace Voxymore::Core {
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;
}
