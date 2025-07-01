#include "Components/Transform.h"

glm::mat4 Transform::GetTransform() {
    if (!m_TransformDirty) {
        return m_Transform;
    }

    m_Transform = glm::mat4(1.0f);
    m_Transform = glm::translate(m_Transform, m_Position);
    m_Transform = glm::rotate(m_Transform, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    m_Transform = glm::scale(m_Transform, glm::vec3(m_Scale, 1.0f));

    m_TransformDirty = false;
    return m_Transform;
}
