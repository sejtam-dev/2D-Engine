#include "Scene.h"

#include <algorithm>

#include "GameObject.h"

std::shared_ptr<Scene> Scene::Create() {
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    scene->Initialize();

    return scene;
}


std::shared_ptr<GameObject> Scene::CreateGameObject() {
    std::shared_ptr<GameObject> gameObject = GameObject::Create();

    gameObject->SetWorld(shared_from_this());
    m_GameObjects.push_back(gameObject);

    return gameObject;
}

void Scene::DestroyGameObject(const std::shared_ptr<GameObject>& gameObject) {
    m_GameObjectsToDestroy.push_back(gameObject);
}

void Scene::OnAwake() {
    for (auto const& value: m_GameObjects) {
        value->OnAwake();
    }
}

void Scene::OnStart() {
    for (auto const& value: m_GameObjects) {
        value->OnStart();
    }
}

void Scene::OnUpdate() {
    for (auto const& value: m_GameObjectsToDestroy) {
        m_GameObjects.erase(
            std::ranges::remove(m_GameObjects, value).begin(),
            m_GameObjects.end()
        );
    }
    m_GameObjectsToDestroy.clear();


    for (auto const& value: m_GameObjects) {
        value->OnUpdate();
    }
}

void Scene::OnFixedUpdate() {
    for (auto const& value: m_GameObjects) {
        value->OnFixedUpdate();
    }
}

void Scene::OnDraw() {
    for (auto const& value: m_GameObjects) {
        value->OnDraw();
    }
}

void Scene::OnDestroy() {
    for (auto const& value: m_GameObjects) {
        value->OnDestroy();
    }
}

void Scene::OnEnable() {
    for (auto const& value: m_GameObjects) {
        value->OnEnable();
    }
}

void Scene::OnDisable() {
    for (auto const& value: m_GameObjects) {
        value->OnDisable();
    }
}

template<typename T>
std::vector<std::shared_ptr<GameObject>> Scene::FindGameObjectsWithComponent() const {
    std::vector<std::shared_ptr<GameObject>> gameObjects;

    for (auto const& value: m_GameObjects) {
        if (value->HasComponent<T>()) {
            gameObjects.push_back(value);
        }
    }

    return gameObjects;
}
