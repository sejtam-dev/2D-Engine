module;
#include "Utils.h"
#include "GL/glew.h"
#include <glm/vec4.hpp>

export module Game.Components:TestComponent;

import Engine;
import Engine.Components;

export class TestComponent : public Component {

    void OnStart() override {
        constexpr GLfloat vertices[] = {
            100.0f, 100.0f,
            250.0f, 100.0f,
            250.0f, 200.0f,
            100.0f, 200.0f
        };

        constexpr GLuint indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLuint vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        GLuint bufferId;
        GLCall(glGenBuffers(1, &bufferId));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, bufferId));
        GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW));

        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, nullptr));

        GLuint ibo;
        GLCall(glGenBuffers(1, &ibo));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &indicies[0], GL_STATIC_DRAW));
    }

    void OnDraw() override {
        Shader::LinkShader("default");

        Shader* shader = Shader::GetLinkedShader();
        shader->SetUniformMatrix4fv("u_Projection", Engine::Camera->GetProjectionMatrix());
        shader->SetUniformMatrix4fv("u_View", Engine::Camera->GetViewMatrix());
        shader->SetUniform4f("u_Color", glm::vec4(r, 0.4f, 0.4f, 1.0f));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
    }

    void OnUpdate() override {
        r += value;
        if (r > 1) {
            value = -0.5f * Engine::deltaTime;
        } else if (r < 0) {
            value = 0.5f * Engine::deltaTime;
        }
    }

    float r = 0.0f;
    float value = 0.5f;
};