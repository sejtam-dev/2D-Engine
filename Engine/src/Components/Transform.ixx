module;
#include "glm/fwd.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <glm/gtc/matrix_transform.hpp>

export module Engine.Components:Transform;

import Engine.Objects;

export class Transform : public Component {
public:
    Transform() = default;

    void SetPosition(const glm::vec3& position) {
        m_Position = position;
        m_TransformDirty = true;
    }

    glm::vec3 GetPosition() const {
        return m_Position;
    }

    glm::vec2 GetPosition2D() const {
        return m_Position;
    }

    void SetRotation(const float rotation) {
        m_Rotation = rotation;
        m_TransformDirty = true;
    }

    float GetRotation() const {
        return m_Rotation;
    }

    void SetScale(const glm::vec2& scale) {
        m_Scale = scale;
        m_TransformDirty = true;
    }

    glm::vec2 GetScale() const {
        return m_Scale;
    }

    glm::mat4 GetTransform();

private:
    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    float m_Rotation = 0.0f;
    glm::vec2 m_Scale = {1.0f, 1.0f};

    glm::mat4 m_Transform = glm::mat4(1.0f);
    bool m_TransformDirty = true;
};
