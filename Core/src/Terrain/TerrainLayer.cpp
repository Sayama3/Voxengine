//
// Created by ianpo on 12/02/2025.
//

#include "Voxymore/Terrain/TerrainLayer.hpp"
#include "Voxymore/Terrain/Terrain.hpp"

using namespace std::string_literals;

namespace Voxymore::Core {
    TerrainLayer::TerrainLayer() : Layer("Terrain Layer"s) {}

    TerrainLayer::~TerrainLayer() {}

    void TerrainLayer::OnAttach() {
        Terrain::Initialize();
    }

    void TerrainLayer::OnDetach() {
        Terrain::Shutdown();
    }

    void TerrainLayer::OnUpdate(const TimeStep ts) {
        Terrain::Update(ts);
    }

    void TerrainLayer::OnEvent(Event &event) {
        // EventDispatcher dispatcher(event);
    }

    void TerrainLayer::OnImGuiRender() {
    }
}
