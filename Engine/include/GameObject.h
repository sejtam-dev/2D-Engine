#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "Scene.fwd.h"
#include "Components/Transform.h"

class GameObject : public std::enable_shared_from_this<GameObject> {
public:
    static std::shared_ptr<GameObject> Create();

    template<typename T, typename... Args>
    std::shared_ptr<T> AddComponent(Args&&... args);

    template<typename T>
    std::shared_ptr<T> GetComponent();

    template<typename T>
    bool HasComponent() const;

    template<typename T>
    bool RemoveComponent();

    void OnAwake();

    void OnStart();

    void OnUpdate();

    void OnFixedUpdate();

    void OnDraw();

    void OnDestroy();

    void OnEnable();

    void OnDisable();

    void SetWorld(const std::weak_ptr<Scene>& world) {
        m_World = world;
    }

    void Destroy();

    Transform& GetTransform();

private:
    GameObject() = default;

    void Initialize();

    std::unordered_map<std::type_index, std::shared_ptr<Component>> m_Components;
    std::weak_ptr<Scene> m_World;
};
