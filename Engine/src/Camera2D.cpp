#include "Camera2D.h"

#include "glm/gtc/matrix_transform.hpp"

glm::mat4 Camera2D::GetViewMatrix() const {
    glm::mat4 view = glm::mat4(1.0f);

    // Translate the view matrix to the camera position
    view = glm::translate(view, glm::vec3(-m_Position, 0.0f));

    // Rotate the view matrix around the camera position
    view = glm::rotate(view, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    // Scale the view matrix to the camera zoom
    view = glm::scale(view, glm::vec3(m_Zoom, m_Zoom, 1.0f));

    return view;
}

glm::mat4 Camera2D::GetProjectionMatrix() const {
    return glm::ortho(
        0.0f, m_Width,
        m_Height, 0.0f,
        -1.0f, 1.0f
    );
}
