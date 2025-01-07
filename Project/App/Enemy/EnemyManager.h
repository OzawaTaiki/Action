#pragma once

#include "BaseEnemy.h"

#include <list>
#include <memory>

class Enemy;
class Camera;
class EnemyManager
{
public:

    static EnemyManager* GetInstance();

    void Initialize();
    void Update();
    void Draw(const Camera* _camera);
    void DrawFront(const Camera* _camera);

    void SetPlayerPosition(const WorldTransform* _playerWT) { playerWT_ = _playerWT; }

    void SpawnEnemy(const uint32_t _spawnNum);

    bool Clear();


private:

    const int enemySpawnNum_ = 2;
    int spawnCount_ = 0;

    std::array<uint32_t, 2> phaseSpawnNum_ = { 10,20 };


    std::list<std::unique_ptr<Enemy>> enemies_ = {};
    std::string enemyModelPath_ = "enemy/enemy.gltf";

    const WorldTransform* playerWT_ = nullptr;

    Vector2 spawnRange_ = {};
    float spawnRadius= 5.0f;

#ifdef _DEBUG
    bool idle_ = true;
    void ImGui();
#endif // _DEBUG



private:
    // コピー禁止
    EnemyManager() = default;
    ~EnemyManager() = default;
    EnemyManager(const EnemyManager&) = delete;
    EnemyManager& operator=(const EnemyManager&) = delete;
};