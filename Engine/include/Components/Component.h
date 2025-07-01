#pragma once
#include <memory>

#include "GameObject.fwd.h"

class Component {
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

protected:
    friend class GameObject;
    std::weak_ptr<GameObject> m_GameObject;
};
