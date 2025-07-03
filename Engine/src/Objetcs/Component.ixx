module;
#include <memory>

export module Engine.Objects:Component;

import :Forward;

export class Component {
public:
    Component() = default;

    virtual ~Component() = default;

    virtual void OnAwake() {
    }

    virtual void OnStart() {
    }

    virtual void OnUpdate() {
    }

    virtual void OnFixedUpdate() {
    }

    virtual void OnDraw() {
    }

    virtual void OnDestroy() {
    }

    virtual void OnEnable() {
    }

    virtual void OnDisable() {
    }

    std::shared_ptr<GameObject> GetGameObject() const {
        return m_GameObject.lock();
    }

    std::weak_ptr<GameObject> m_GameObject;
};
