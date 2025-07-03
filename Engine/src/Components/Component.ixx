module;
#include <memory>

export module Engine.Components:Component;

// TODO: Module forward declaration

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

protected:
    std::weak_ptr<GameObject> m_GameObject;
};
