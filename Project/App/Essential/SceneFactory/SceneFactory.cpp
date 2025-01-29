#include "SceneFactory.h"

#include <App/Scene/GameScene.h>
#include <Features/Scene/ParticleTestScene.h>

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& _name)
{
    if (_name == "Game")
    {
        return std::make_unique<GameScene>();
    }
    else if (_name == "ParticleTest")
    {
        return std::make_unique<ParticleTestScene>();
    }

    assert("Scene Not Found");

    return nullptr;
}

std::string SceneFactory::ShowDebugWindow()
{
#ifdef _DEBUG

    ImGui::SeparatorText("Scene Factory");

    if (ImGui::Button("GameScene"))
    {
        return "Game";
    }
    if (ImGui::Button("ParticleTest"))
    {
        return "ParticleTest";
    }


#endif // _DEBUG
    return "";

}
