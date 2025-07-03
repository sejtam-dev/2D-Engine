module;
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <ranges>

module Engine.Objects;
import :Scene;
import :Component;

import Engine.Components;

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

Transform& GameObject::GetTransform() {
    const std::shared_ptr<Transform> transform = GetComponent<Transform>();
    if (!transform) {
        throw std::runtime_error("Transform component not found. Please add a Transform component to the GameObject.");
    }

    return *transform;
}

void GameObject::Initialize() {
    AddComponent<Transform>();
}

