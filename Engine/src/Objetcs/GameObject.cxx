module;
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <ranges>

module Engine.Objects:GameObjectImpl;
import :GameObject;
import :Scene;

import Engine.Components;

std::shared_ptr<GameObject> GameObject::Create() {
    auto gameObject = std::shared_ptr<GameObject>(new GameObject());

    gameObject->Initialize();

    return gameObject;
}

template<typename T, typename... Args>
std::shared_ptr<T> GameObject::AddComponent(Args&&... args) {
    auto component = std::make_shared<T>(std::forward<Args>(args)...);
    component->m_GameObject = weak_from_this();

    m_Components[std::type_index(typeid(T))] = component;
    return component;
}

template<typename T>
std::shared_ptr<T> GameObject::GetComponent() {
    static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

    auto it = m_Components.find(std::type_index(typeid(T)));
    if (it != m_Components.end()) {
        return std::static_pointer_cast<T>(it->second);
    }
    return nullptr;
}

template<typename T>
bool GameObject::HasComponent() const {
    static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

    return m_Components.contains(std::type_index(typeid(T)));
}

template<typename T>
bool GameObject::RemoveComponent() {
    static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

    auto it = m_Components.find(std::type_index(typeid(T)));
    if (it != m_Components.end()) {
        m_Components.erase(it);
        return true;
    }

    return false;
}

void GameObject::OnAwake() {
    for (auto const& value: std::views::values(m_Components)) {
        value->OnAwake();
    }
}

void GameObject::OnStart() {
    for (auto const& value: std::views::values(m_Components)) {
        value->OnStart();
    }
}

void GameObject::OnUpdate() {
    for (auto const& value: std::views::values(m_Components)) {
        value->OnUpdate();
    }
}

void GameObject::OnFixedUpdate() {
    for (auto const& value: std::views::values(m_Components)) {
        value->OnFixedUpdate();
    }
}

void GameObject::OnDraw() {
    for (auto const& value: std::views::values(m_Components)) {
        value->OnDraw();
    }
}

void GameObject::OnDestroy() {
    for (auto const& value: std::views::values(m_Components)) {
        value->OnDestroy();
    }
}

void GameObject::OnEnable() {
    for (auto const& value: std::views::values(m_Components)) {
        value->OnEnable();
    }
}

void GameObject::OnDisable() {
    for (auto const& value: std::views::values(m_Components)) {
        value->OnDisable();
    }
}

void GameObject::Destroy() {
    if (const auto world = m_World.lock()) {
        world->DestroyGameObject(shared_from_this());
    }
}

std::shared_ptr<Transform>& GameObject::GetTransform() {
    std::shared_ptr<Transform> transform = GetComponent<Transform>();
    if (!transform) {
        throw std::runtime_error("Transform component not found. Please add a Transform component to the GameObject.");
    }

    return transform;
}

void GameObject::Initialize() {
    AddComponent<Transform>();
}

