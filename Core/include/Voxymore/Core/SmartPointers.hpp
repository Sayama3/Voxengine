//
// Created by ianpo on 27/07/2023.
//

#pragma once

#include <memory>

namespace Voxymore::Core {
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

	template<typename T>
	using Weak = std::weak_ptr<T>;

	template<typename TTarget, typename TBasePtr>
	[[nodiscard]] inline Ref<TTarget> CastPtr(TBasePtr ptr)
	{
#ifdef VXM_DEBUG
		return std::dynamic_pointer_cast<TTarget>(ptr);
#else
		return std::static_pointer_cast<TTarget>(ptr);
#endif
	}

	template<typename TTarget, typename TBasePtr>
	[[nodiscard]] inline Ref<TTarget> DynamicCastPtr(TBasePtr ptr)
	{
		return std::dynamic_pointer_cast<TTarget>(ptr);
	}
}
