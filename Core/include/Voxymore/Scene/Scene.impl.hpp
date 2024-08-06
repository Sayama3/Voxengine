//
// Created by ianpo on 27/01/2024.
//

#pragma once

#include "Scene.decl.hpp"
#include "Entity.decl.hpp"

// TODO: find a better way?
namespace Voxymore::Core
{
	/**
		 * @brief Executes a function on each entity in the registry.
		 *
		 * This template function executes a provided function on each entity in the registry.
		 * The specified components are extracted from the registry using the view method.
		 * The function takes the callable object as a parameter.
		 * The callable object is applied to each entity in the registry.
		 *
		 * @tparam Get Variadic template parameter pack for the types of components to retrieve from each entity.
		 * @tparam Func The type of the callable object to apply to each entity.
		 * @param func The callable object to apply to each entity.
		 */
	template<typename... Get, typename Func>
	inline void Scene::each(Func func)
	{
		each<Get...>(MultiThreading::ExecutionPolicy::None, func);
	}

	/**
		 * @brief Executes a function on each entity in the registry that has the specified components.
		 *
		 * This template function executes a provided function on each entity in the registry that
		 * has the specified components. The specified components are extracted from the registry
		 * using the view method. The function takes the callable object as a parameter.
		 * The callable object is applied to each entity that matches the specified components.
		 *
		 * @tparam Get Variadic template parameter pack for the types of components to retrieve from each entity.
		 * @tparam Func The type of the callable object to apply to each entity.
		 * @param func The callable object to apply to each entity.
		 */
	template<typename... Get, typename... Exclude, typename Func>
	inline void Scene::each(entt::exclude_t<Exclude...> ex, Func func)
	{
		each<Get...>(ex, MultiThreading::ExecutionPolicy::None, func);
	}

	/**
		 * @brief Executes a function on each entity in the registry that has the specified components.
		 *
		 * This template function executes a provided function on each entity in the registry that
		 * has the specified components. The specified components are extracted from the registry
		 * using the view method. The function takes an execution policy and a callable object
		 * as parameters. The execution policy determines how the function is executed.
		 * The callable object is applied to each entity that matches the specified components.
		 *
		 * @tparam Get Variadic template parameter pack for the types of components to retrieve from each entity.
		 * @tparam Func The type of the callable object to apply to each entity.
		 * @param exec The execution policy to control how the function is executed.
		 * @param func The callable object to apply to each entity.
		 */
	template<typename... Get, typename Func>
	inline void Scene::each(MultiThreading::ExecutionPolicy exec, Func func)
	{
		VXM_PROFILE_FUNCTION();
		auto view = m_Registry.view<Get...>();
		MultiThreading::for_each(exec, view.begin(), view.end(), [&view, &func](auto e)
								 {
									 std::tuple<Get&...> comps = view.template get<Get...>(e);
									 auto cs = std::tuple_cat(std::tie(e), comps);
									 std::apply(func, cs);
								 });
	}

	/**
		 * @brief Executes a function on each entity in the registry that has the specified components.
		 *
		 * This template function executes a provided function on each entity in the registry that
		 * has the specified components. The specified components are extracted from the registry
		 * using the view method. The function takes an execution policy and a callable object
		 * as parameters. The execution policy determines how the function is executed.
		 * The callable object is applied to each entity that matches the specified components.
		 *
		 * @tparam Get Variadic template parameter pack for the types of components to retrieve from each entity.
		 * @tparam Exclude Variadic template parameter pack for the types of components to exclude from each entity.
		 * @tparam Func The type of the callable object to apply to each entity.
		 * @param ex The components to exclude from each entity.
		 * @param exec The execution policy to control how the function is executed.
		 * @param func The callable object to apply to each entity.
		 */
	template<typename... Get, typename... Exclude, typename Func>
	inline void Scene::each(entt::exclude_t<Exclude...> ex, MultiThreading::ExecutionPolicy exec, Func func)
	{
		VXM_PROFILE_FUNCTION();
		auto view = m_Registry.view<Get...>(ex);
		MultiThreading::for_each(exec, view.begin(), view.end(), [&view, &func](auto e)
								 {
									 std::tuple<Get&...> comps = view.template get<Get...>(e);
									 auto cs = std::tuple_cat(std::tie(e), comps);
									 std::apply(func, cs);
								 });
	}

	/**
		 * @brief Creates a view of entities with specified components.
		 *
		 * This template function creates a view of entities in the registry that have the specified components.
		 * The specified components are retrieved from the registry using the view method.
		 * The returned view can be used to iterate over entities with the specified components.
		 *
		 * @tparam Get Variadic template parameter pack for the types of components to retrieve from each entity.
		 * @return entt::basic_view<entt::get_t<entt::storage_for_t<Get>...>, entt::exclude_t<>>
		 */
	template<typename... Get>
	[[nodiscard]] inline entt::basic_view<entt::get_t<entt::storage_for_t<Get>...>, entt::exclude_t<>> Scene::view()
	{
		VXM_PROFILE_FUNCTION();
		return m_Registry.view<Get ...>();
	}

	/**
		 *@brief Creates a view of entities with specified components.
		 *
		 *This template function creates a view of entities in the registry that have the specified components. The specified components are retrieved from the registry using the view method
		*. The returned view can be used to iterate over entities with the specified components.
		 *
		 *@tparam Get Variadic template parameter pack for the types of components to retrieve from each entity.
		 *@return entt::basic_view<entt::get_t<entt::storage_for_t<Get>...>, entt::exclude_t<>>
		 */
	template<typename... Get, typename... Exclude>
	[[nodiscard]] inline entt::basic_view<entt::get_t<entt::storage_for_t<Get>...>, entt::exclude_t<entt::storage_for_t<Exclude>...>> Scene::view(entt::exclude_t<Exclude...> ex)
	{
		VXM_PROFILE_FUNCTION();
		return m_Registry.view<Get ...>(ex);
	}
}