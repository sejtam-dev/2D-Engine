module;

#include <memory>
#include <vector>
#include <algorithm>
#include <string>

export module Engine.Objects:Scene;

import :Forward;

export class Scene : public std::enable_shared_from_this<Scene> {
public:
    virtual ~Scene() = default;

    static std::shared_ptr<Scene> Create() {
        std::shared_ptr<Scene> scene = std::make_shared<Scene>();

        scene->Initialize();

        return scene;
    }

    std::shared_ptr<GameObject> CreateGameObject();

    void DestroyGameObject(const std::shared_ptr<GameObject>& gameObject);

    void OnAwake(); // TODO: Implement call

    void OnStart();

    void OnUpdate();

    void OnFixedUpdate(); // TODO: Implement call

    void OnDraw();

    void OnDestroy();

    void OnEnable(); // TODO: Implement call

    void OnDisable(); // TODO: Implement call

    template<typename T>
    std::vector<std::shared_ptr<GameObject>> FindGameObjectsWithComponent() const;

    const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const {
        return m_GameObjects;
    }

    void SetInstanceID(const std::string& name) {
        m_InstanceName = name;
    }

    const std::string& GetInstanceID() const {
        return m_InstanceName;
    }

    virtual void Initialize() {
    }

protected:
    std::vector<std::shared_ptr<GameObject>> m_GameObjects;

private:
    std::string m_InstanceName;

    std::vector<std::shared_ptr<GameObject>> m_GameObjectsToDestroy;
};
