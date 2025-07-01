#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "Scene.fwd.h"

class SceneManager {
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
    std::string CreateScene();

    template<typename T>
    std::string RegisterScene(const std::shared_ptr<T>& scene);

    template<typename T>
    std::shared_ptr<T> GetScene(const std::string& instanceId) const;

    std::shared_ptr<Scene> GetScene(const std::string& instanceId) const;

    template<typename T>
    std::vector<std::shared_ptr<T>> GetScenes() const;

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

private:
    void ApplyPushScene(const std::string& instanceId);

    void ApplyPopScene();

    void ApplyPopScene(const std::string& instanceId);

    void ApplyPopAllScenes();

    void ApplyReplaceTopScene(const std::string& instanceId);

    void ApplyReplaceScene(const std::string& oldInstanceId, const std::string& newInstanceId);

    template<typename T>
    std::string GetTypeName() const;

    std::string GenerateInstanceID(const std::string& typeName);

    std::map<std::string, std::shared_ptr<Scene>> m_Scenes;
    std::vector<std::weak_ptr<Scene>> m_SceneStack;
    std::vector<SceneChange> m_PendingChanges;

    std::map<std::string, int> m_InstanceCounter;
};
