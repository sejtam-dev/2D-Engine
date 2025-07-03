module;
#include "Utils.h"

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <ranges>
#include <algorithm>

export module Engine.Utils:SceneManager;

import Engine.Objects;

export class SceneManager {
    enum class ChangeType {
        PUSH,
        POP,
        POP_SPECIFIC,
        POP_ALL,
        REPLACE_TOP,
        REPLACE_SPECIFIC,
    };

    struct SceneChange {
        ChangeType type;
        std::string instanceId;
        std::string targetInstanceId;
    };

public:
    SceneManager() = default;

    ~SceneManager() = default;

    template<typename T>
    std::string CreateScene() {
        static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");

        const std::string typeName = GetTypeName<T>();
        const std::string instanceId = GenerateInstanceID(typeName);

        std::shared_ptr<T> world = T::Create();
        m_Scenes[instanceId] = world;
        return instanceId;
    }

    template<typename T>
    std::string RegisterScene(const std::shared_ptr<T>& scene) {
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
    std::shared_ptr<T> GetScene(const std::string& instanceId) const {
        static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");

        return std::dynamic_pointer_cast<T>(GetScene(instanceId));
    }

    std::shared_ptr<Scene> GetScene(const std::string& instanceId) const;

    template<typename T>
    std::vector<std::shared_ptr<T>> GetScenes() const {
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

    std::vector<std::shared_ptr<Scene>> GetScenes() const;

    bool PushScene(const std::string& instanceId);

    bool PushScene(const std::shared_ptr<Scene>& scene);

    void PopScene();

    bool PopScene(const std::string& instanceId);

    bool PopScene(const std::shared_ptr<Scene>& scene);

    void PopAllScenes();

    bool ReplaceTopScene(const std::string& instanceId);

    bool ReplaceTopScene(const std::shared_ptr<Scene>& scene);

    bool ReplaceScene(const std::string& oldInstanceId, const std::string& newInstanceId);

    bool ReplaceScene(const std::string& oldInstanceId, const std::shared_ptr<Scene>& newScene);

    void ProcessPendingChanges();

    void HandleUpdate() const;

    void HandleFixedUpdate() const;

    void HandleDraw() const;

private:
    void ApplyPushScene(const std::string& instanceId);

    void ApplyPopScene();

    void ApplyPopScene(const std::string& instanceId);

    void ApplyPopAllScenes();

    void ApplyReplaceTopScene(const std::string& instanceId);

    void ApplyReplaceScene(const std::string& oldInstanceId, const std::string& newInstanceId);

    template<typename T>
    std::string GetTypeName() const {
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

    std::string GenerateInstanceID(const std::string& typeName);

    std::map<std::string, std::shared_ptr<Scene>> m_Scenes;
    std::vector<std::weak_ptr<Scene>> m_SceneStack;
    std::vector<SceneChange> m_PendingChanges;

    std::map<std::string, int> m_InstanceCounter;
};
