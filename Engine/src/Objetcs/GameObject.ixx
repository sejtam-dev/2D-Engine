module;
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <ranges>

export module Engine.Objects:GameObject;

import :Forward;

export class GameObject : public std::enable_shared_from_this<GameObject> {
public:
    static std::shared_ptr<GameObject> Create() {
        auto gameObject = std::shared_ptr<GameObject>(new GameObject());

        gameObject->Initialize();

        return gameObject;
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> AddComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        component->m_GameObject = weak_from_this();

        m_Components[std::type_index(typeid(T))] = component;
        return component;
    }

    template<typename T>
    std::shared_ptr<T> GetComponent() {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

        auto it = m_Components.find(std::type_index(typeid(T)));
        if (it != m_Components.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    template<typename T>
    bool HasComponent() const {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

        return m_Components.contains(std::type_index(typeid(T)));
    }

    template<typename T>
    bool RemoveComponent() {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

        auto it = m_Components.find(std::type_index(typeid(T)));
        if (it != m_Components.end()) {
            m_Components.erase(it);
            return true;
        }

        return false;
    }

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

    std::unordered_map<std::type_index, std::shared_ptr<Component>> m_Components{};
    std::weak_ptr<Scene> m_World;
};
