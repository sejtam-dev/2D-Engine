#include "Utils/SceneManager.h"

#include <algorithm>
#include <ranges>

#include "Scene.h"
#include "Utils.h"

template<typename T>
std::string SceneManager::CreateScene() {
    static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");

    const std::string typeName = GetTypeName<T>();
    const std::string instanceId = GenerateInstanceID(typeName);

    std::shared_ptr<T> world = T::Create();
    m_Scenes[instanceId] = world;
    return instanceId;
}

template<typename T>
std::string SceneManager::RegisterScene(const std::shared_ptr<T>& scene) {
    static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");

    const std::string& sceneInstanceId = scene->GetInstanceID();
    if (!sceneInstanceId.empty()) {
        WARNING("Scene is already registered with an instance ID: {}", scene->GetInstanceID());

        return sceneInstanceId;
    }

    for (const auto& [id, registeredScene]: m_Scenes) {
        if (registeredScene.get() == scene.get()) {
            WARNING("Scene already registered with different ID: {}", id);
            return id;
        }
    }

    const std::string typeName = GetTypeName<T>();
    const std::string instanceId = GenerateInstanceID(typeName);

    scene->SetInstanceID(instanceId);
    m_Scenes[instanceId] = scene;
    return instanceId;
}

template<typename T>
std::shared_ptr<T> SceneManager::GetScene(const std::string& instanceId) const {
    static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");

    return std::dynamic_pointer_cast<T>(GetScene(instanceId));
}

std::shared_ptr<Scene> SceneManager::GetScene(const std::string& instanceId) const {
    return m_Scenes.at(instanceId);
}

template<typename T>
std::vector<std::shared_ptr<T>> SceneManager::GetScenes() const {
    static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");

    const std::string typeName = GetTypeName<T>();
    auto filtered = m_Scenes
                    | std::views::filter([&typeName](const auto& pair) {
                        return pair.first.starts_with(typeName);
                    })
                    | std::views::values
                    | std::views::transform([](const auto& scene) {
                        return std::dynamic_pointer_cast<T>(scene);
                    })
                    | std::views::filter([](const auto& scene) {
                        return scene != nullptr;
                    });

    return {filtered.begin(), filtered.end()};
}

std::vector<std::shared_ptr<Scene>> SceneManager::GetScenes() const {
    auto values = std::views::values(m_Scenes);
    return {values.begin(), values.end()};
}

bool SceneManager::PushScene(const std::string& instanceId) {
    if (!m_Scenes.contains(instanceId)) {
        ERROR("Scene with instance ID '{}' does not exist!", instanceId);
        return false;
    }

    m_PendingChanges.emplace_back(
        ChangeType::PUSH,
        instanceId,
        ""
    );

    return true;
}

bool SceneManager::PushScene(const std::shared_ptr<Scene>& scene) {
    std::string instanceId = scene->GetInstanceID();

    if (instanceId.empty()) {
        instanceId = RegisterScene(scene);

        WARNING("Scene instance ID was empty, generated: {}", instanceId);
    }

    return PushScene(instanceId);
}

void SceneManager::PopScene() {
    m_PendingChanges.emplace_back(
        ChangeType::POP,
        "",
        ""
    );
}

bool SceneManager::PopScene(const std::string& instanceId) {
    if (!m_Scenes.contains(instanceId)) {
        ERROR("Scene with instance ID '{}' does not exist!", instanceId);
        return false;
    }

    m_PendingChanges.emplace_back(
        ChangeType::POP_SPECIFIC,
        instanceId,
        ""
    );

    return true;
}

bool SceneManager::PopScene(const std::shared_ptr<Scene>& scene) {
    std::string instanceId = scene->GetInstanceID();

    if (instanceId.empty()) {
        ERROR("Scene with instance ID '{}' does not exist!", instanceId);
        return false;
    }

    return PopScene(instanceId);
}

void SceneManager::PopAllScenes() {
    m_PendingChanges.emplace_back(
        ChangeType::POP_ALL,
        "",
        ""
    );
}

bool SceneManager::ReplaceTopScene(const std::string& instanceId) {
    if (!m_Scenes.contains(instanceId)) {
        ERROR("Scene with instance ID '{}' does not exist!", instanceId);
        return false;
    }

    m_PendingChanges.emplace_back(
        ChangeType::REPLACE_TOP,
        instanceId,
        ""
    );

    return true;
}

bool SceneManager::ReplaceTopScene(const std::shared_ptr<Scene>& scene) {
    std::string instanceId = scene->GetInstanceID();

    if (instanceId.empty()) {
        instanceId = RegisterScene(scene);

        WARNING("Scene instance ID was empty, generated: {}", instanceId);
    }

    return ReplaceTopScene(instanceId);
}

bool SceneManager::ReplaceScene(const std::string& oldInstanceId, const std::string& newInstanceId) {
    if (!m_Scenes.contains(oldInstanceId)) {
        ERROR("Scene with instance ID '{}' does not exist!", oldInstanceId);
        return false;
    }

    if (!m_Scenes.contains(newInstanceId)) {
        ERROR("Scene with instance ID '{}' does not exist!", newInstanceId);
        return false;
    }

    m_PendingChanges.emplace_back(
        ChangeType::REPLACE_SPECIFIC,
        oldInstanceId,
        newInstanceId
    );

    return true;
}

bool SceneManager::ReplaceScene(const std::string& oldInstanceId, const std::shared_ptr<Scene>& newScene) {
    std::string instanceId = newScene->GetInstanceID();

    if (instanceId.empty()) {
        ERROR("Scene with instance ID '{}' does not exist!", instanceId);
        return false;
    }

    return ReplaceScene(oldInstanceId, instanceId);
}

void SceneManager::ProcessPendingChanges() {
    if (m_PendingChanges.empty()) {
        return;
    }

    for (const auto& [type, instanceId, targetInstanceId]: m_PendingChanges) {
        switch (type) {
            case ChangeType::PUSH: {
                ApplyPushScene(instanceId);
                break;
            }
            case ChangeType::POP: {
                ApplyPopScene(instanceId);
                break;
            }
            case ChangeType::POP_SPECIFIC: {
                ApplyPopScene(instanceId);
                break;
            }
            case ChangeType::POP_ALL: {
                ApplyPopAllScenes();
                break;
            }
            case ChangeType::REPLACE_TOP: {
                ApplyReplaceTopScene(instanceId);
                break;
            }
            case ChangeType::REPLACE_SPECIFIC: {
                ApplyReplaceScene(instanceId, targetInstanceId);
                break;
            }
        }
    }

    m_PendingChanges.clear();
}

void SceneManager::ApplyPushScene(const std::string& instanceId) {
    if (!m_Scenes.contains(instanceId)) {
        ERROR("Scene with instance ID '{}' does not exist!", instanceId);
        return;
    }

    const auto scene = m_Scenes.at(instanceId);
    m_SceneStack.push_back(scene->weak_from_this());

    scene->OnEnable();
    scene->OnStart();
}

void SceneManager::ApplyPopScene() {
    if (m_SceneStack.empty()) {
        WARNING("Cannot pop scene - stack is empty!");
        return;
    }

    if (const auto topScene = m_SceneStack.back().lock()) {
        topScene->OnDisable();
        topScene->OnDestroy();
    }

    m_SceneStack.pop_back();
}

void SceneManager::ApplyPopScene(const std::string& instanceId) {
    const auto it = std::ranges::find_if(m_SceneStack,
                                         [&instanceId](const std::weak_ptr<Scene>& weakScene) {
                                             if (const auto scene = weakScene.lock()) {
                                                 return scene->GetInstanceID() == instanceId;
                                             }
                                             return false;
                                         });

    if (it == m_SceneStack.end()) {
        WARNING("Scene with instance ID '{}' not found in stack!", instanceId);
        return;
    }

    if (const auto scene = it->lock()) {
        scene->OnDisable();
        scene->OnDestroy();
    }

    m_SceneStack.erase(it);
}

void SceneManager::ApplyPopAllScenes() {
    for (auto& weakScene: m_SceneStack) {
        if (const auto scene = weakScene.lock()) {
            scene->OnDisable();
            scene->OnDestroy();
        }
    }

    m_SceneStack.clear();
}

void SceneManager::ApplyReplaceTopScene(const std::string& instanceId) {
    if (!m_Scenes.contains(instanceId)) {
        ERROR("Scene with instance ID '{}' does not exist!", instanceId);
        return;
    }

    const auto newScene = m_Scenes.at(instanceId);
    if (!m_SceneStack.empty()) {
        if (const auto currentTop = m_SceneStack.back().lock()) {
            currentTop->OnDisable();
            currentTop->OnDestroy();
        }

        m_SceneStack.back() = newScene->weak_from_this();
    } else {
        m_SceneStack.push_back(newScene->weak_from_this());
    }

    newScene->OnEnable();
    newScene->OnStart();
}

void SceneManager::ApplyReplaceScene(const std::string& oldInstanceId, const std::string& newInstanceId) {
    if (!m_Scenes.contains(newInstanceId)) {
        ERROR("Scene with instance ID '{}' does not exist!", newInstanceId);
        return;
    }

    const auto newScene = m_Scenes.at(newInstanceId);

    const auto it = std::ranges::find_if(m_SceneStack,
                                         [&oldInstanceId](const std::weak_ptr<Scene>& weakScene) {
                                             if (const auto scene = weakScene.lock()) {
                                                 return scene->GetInstanceID() == oldInstanceId;
                                             }
                                             return false;
                                         });

    if (it == m_SceneStack.end()) {
        WARNING("Scene with instance ID '{}' not found in stack!", oldInstanceId);
        return;
    }

    if (const auto oldScene = it->lock()) {
        oldScene->OnDisable();
        oldScene->OnDestroy();
    }

    *it = newScene->weak_from_this();

    newScene->OnEnable();
    newScene->OnStart();
}

template<typename T>
std::string SceneManager::GetTypeName() const {
    std::string name = typeid(T).name();

    if (name.starts_with("class ")) {
        name = name.substr(6);
    }

    const size_t lastColonIndex = name.find_last_of(':');
    if (lastColonIndex != std::string::npos) {
        name = name.substr(lastColonIndex + 1);
    }

    return name;
}

std::string SceneManager::GenerateInstanceID(const std::string& typeName) {
    int& counter = m_InstanceCounter[typeName];
    return typeName + "_" + std::to_string(counter++);
}
