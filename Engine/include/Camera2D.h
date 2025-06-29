#pragma once
#include "static.h"
#include "glm/fwd.hpp"
#include "glm/vec2.hpp"

class Camera2D {
public:
    Camera2D(const float width, const float height)
        : m_Width(width), m_Height(height) {}

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;

    float GetWidth() const {
        return m_Width;
    }
    float GetHeight() const {
        return m_Height;
    }
    void SetSize(const float width, const float height) {
        DEBUG_LOG("Camera size changed to: {}x{}", width, height);

        m_Width = width;
        m_Height = height;
    }

    glm::vec2 GetPosition() const {
        return m_Position;
    }
    void SetPosition(const glm::vec2 &position) {
        m_Position = position;
    }

    float GetRotation() const {
        return m_Rotation;
    }
    void SetRotation(const float rotation) {
        m_Rotation = rotation;
    }

    float GetZoom() const {
        return m_Zoom;
    }
    void SetZoom(const float zoom) {
        m_Zoom = zoom;
    }

private:
    glm::vec2 m_Position = {0.0f, 0.0f};
    float m_Rotation = 0.0f;

    float m_Width = 0.0f;
    float m_Height = 0.0f;

    float m_Zoom = 1.0f;
};
