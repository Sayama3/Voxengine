//
// Created by ianpo on 18/07/2023.
//

#pragma once

#include "Layer.hpp"
#include "Voxymore/Core/Core.hpp"

namespace Voxymore::Core {

	class LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		inline std::vector<Layer*>::iterator begin() {return m_Layers.begin();}
		inline std::vector<Layer*>::iterator end() {return m_Layers.end();}
		inline std::vector<Layer*>::reverse_iterator rbegin() {return m_Layers.rbegin();}
		inline std::vector<Layer*>::reverse_iterator rend() {return m_Layers.rend();}

		inline std::vector<Layer*>::const_iterator begin() const {return m_Layers.begin();}
		inline std::vector<Layer*>::const_iterator end() const {return m_Layers.end();}
		inline std::vector<Layer*>::const_reverse_iterator rbegin() const {return m_Layers.rbegin();}
		inline std::vector<Layer*>::const_reverse_iterator rend() const {return m_Layers.rend();}

		template<typename T>
		inline T*GetLayer()
		{
			VXM_PROFILE_FUNCTION();
			for (Layer *layer: m_Layers) {
				if (typeid(*layer) == typeid(T))
					return static_cast<T *>(layer);
			}
			return nullptr;
		}

		template<typename T>
		inline const T* GetLayer() const
		{
			VXM_PROFILE_FUNCTION();
			for (Layer* layer: m_Layers) {
				if (typeid(*layer) == typeid(T))
					return static_cast<const T *>(layer);
			}
			return nullptr;
		}

		template<typename T>
		inline bool TryGetLayer(T*& ptr)
		{
			VXM_PROFILE_FUNCTION();
			for (Layer *layer: m_Layers) {
				if (typeid(*layer) == typeid(T))
				{
					ptr = static_cast<T *>(layer);
					return true;
				}
			}
			return false;
		}

		template<typename T>
		inline const bool TryGetLayer(const T*& ptr) const
		{
			VXM_PROFILE_FUNCTION();
			for (Layer* layer: m_Layers) {
				if (typeid(*layer) == typeid(T))
				{
					ptr = static_cast<const T *>(layer);
					return true;
				}
			}
			return false;
		}

		template<typename T>
		inline size_t GetLayerIndex() const
		{
			VXM_PROFILE_FUNCTION();
			for (size_t i = 0; i < m_Layers.size(); ++i) {
				if (typeid(*(m_Layers[i])) == typeid(T))
				{
					return i;
				}
			}
			return -1;
		}

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};

} // Layers
