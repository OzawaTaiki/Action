#include "EnemyManager.h"
#include <Math/Random/RandomGenerator.h>
#include <Features/Camera/Camera/Camera.h>
#include <Features/Model/Manager/ModelManager.h>
#include "Enemy.h"


EnemyManager* EnemyManager::GetInstance()
{
    static EnemyManager instance;
    return &instance;
}

void EnemyManager::Initialize()
{
    spawnRange_.x = 8.0f;
    spawnRange_.y = 13.0f;

    Model* model = new Model();
    model->CreateFromObj(enemyModelPath_);
    delete model;

    spawnCount_ = 0;
    clearFlag_ = false;
}

void EnemyManager::Update()
{
#ifdef _DEBUG
    ImGui();
#endif // _DEBUG

    if (enemies_.size() < 1)
    {
        if (phaseSpawnNum_.size() != spawnCount_)
            SpawnEnemy(phaseSpawnNum_[spawnCount_]);
    }

    for (auto it = enemies_.begin(); it != enemies_.end();)
    {
        if (!(*it)->IsAlive())
            it = enemies_.erase(it);
        else
        {
            (*it)->Update();
            ++it;
        }
    }
}

void EnemyManager::Draw(const Camera* _camera)
{
    for (auto& enemy : enemies_)
    {
        enemy->Draw(_camera);
    }
}

void EnemyManager::DrawFront(const Camera* _camera)
{
    for (auto& enemy : enemies_)
    {
        enemy->DrawFront(_camera);
    }
}

void EnemyManager::SpawnEnemy(const uint32_t _spawnNum)
{
    if (enemySpawnNum_ < spawnCount_++)
        return;

    Vector3 playerPos = playerWT_->GetWorldPosition();
    auto random = RandomGenerator::GetInstance();

    // playerから半径n外に円を置く
    // その中にランダムで出現
    // playerからどの方向へスポーンするか
    Vector3 spawnDirectionFromPlayer = random->GetUniformVec3({ -1,0,-1 }, { 1,0,1 });
    spawnDirectionFromPlayer.y = 0;
    spawnDirectionFromPlayer = spawnDirectionFromPlayer.Normalize();

    // playerからどの距離にスポーンするか
    float spawnDistance = random->GetUniformFloat(spawnRange_.x, spawnRange_.y);

    // スポーン範囲の円の座標
    Vector3 spawnCirclePos = playerPos + spawnDirectionFromPlayer * spawnDistance;

    for (uint32_t i = 0; i < _spawnNum; ++i)
    {
        auto& enemy = enemies_.emplace_back(std::make_unique<Enemy>());
        enemy->Initialize();


        // スポーン範囲
        Vector3 spawnDirection = random->GetUniformVec3({ -1,0,-1 }, { 1,0,1 });
        spawnDirection.y = 0;
        float randomRadius = random->GetUniformFloat(0, spawnRadius);
        Vector3 spawnPos = spawnCirclePos + spawnDirection * randomRadius;
        spawnPos.y = 0;

        enemy->SetPosition(spawnPos);
        enemy->SetPlayerPosition(playerWT_);

    }
}

bool EnemyManager::Clear()
{
    return ClearCheak();
}

bool EnemyManager::ClearCheak()
{
    if (enemySpawnNum_ <= spawnCount_ && enemies_.empty())
    {
        GameTime::GetChannel("default").SetGameSpeed(.3f);
        GameTime::GetChannel("Effects").SetGameSpeed(.3f);
        slowTimeCount_ += GameTime::GetChannel("default").GetDeltaTime<float>();

        if (slowTimeCount_ >= slowTime_)
        {
            GameTime::GetChannel("default").SetGameSpeed(1.0f);
            slowTimeCount_ = 0;
            return true;
        }

    }
    return false;

}

#ifdef _DEBUG
void EnemyManager::ImGui()
{
#ifdef _DEBUG
    ImGui::Begin("Enemy");
    ImGui::SeparatorText("EnemyManager");
    ImGui::DragFloatRange2("SpawnRange", &spawnRange_.x, &spawnRange_.y, 0.1f);
    if (ImGui::Button("SpawnEnemy##1"))
        SpawnEnemy(1);
    static int spawnNum_ = 1;
    ImGui::DragInt("SpawnNum", &spawnNum_, 1, 1, 100);
    if (ImGui::Button("SpawnEnemy##2"))
    {
        Vector3 playerPos = playerWT_->GetWorldPosition();
        auto random = RandomGenerator::GetInstance();

        // playerから半径n外に円を置く
        // その中にランダムで出現
        // playerからどの方向へスポーンするか
        Vector3 spawnDirectionFromPlayer = random->GetUniformVec3({ -1,0,-1 }, { 1,0,1 });
        spawnDirectionFromPlayer.y = 0;
        spawnDirectionFromPlayer = spawnDirectionFromPlayer.Normalize();

        // playerからどの距離にスポーンするか
        float spawnDistance = random->GetUniformFloat(spawnRange_.x, spawnRange_.y);

        // スポーン範囲の円の座標
        Vector3 spawnCirclePos = playerPos + spawnDirectionFromPlayer * spawnDistance;

        for (uint32_t i = 0; i < spawnNum_; ++i)
        {
            auto& enemy = enemies_.emplace_back(std::make_unique<Enemy>());
            enemy->Initialize();


            // スポーン範囲
            Vector3 spawnDirection = random->GetUniformVec3({ -1,0,-1 }, { 1,0,1 });
            spawnDirection.y = 0;
            float randomRadius = random->GetUniformFloat(0, spawnRadius);
            Vector3 spawnPos = spawnCirclePos + spawnDirection * randomRadius;
            spawnPos.y = 0;

            enemy->SetPosition(spawnPos);
            enemy->SetPlayerPosition(playerWT_);

        }
        spawnNum_ = 1;
    }

    if (ImGui::Button("ChangeState"))
    {
        idle_ = !idle_;
        for (auto& enemy : enemies_)
        {
            enemy->SetIdleFlag(idle_);
        }
    }

    if (ImGui::Button("Clear"))
    {
        enemies_.clear();
    }
    ImGui::End();

#endif // _DEBUG
}
#endif // _DEBUG

