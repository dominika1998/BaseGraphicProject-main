#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>


#include "Application/utils.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/gtc/matrix_transform.hpp>

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
        std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");



    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    std::vector<GLfloat> vertices = {
         -0.7f, 0.2f, 0.0f, 0.0f, 0.8f, 0.8f,
        0.7f, 0.2f, 0.0f, 0.0f, 0.8f, 0.8f,
        0.0f, 0.75f, 0.0f, 0.0f, 0.8f, 0.8f,

        -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.5f,
        -0.5f, 0.2f, 0.0f, 0.0f, 0.5f, 0.5f,

        0.5f, 0.2f, 0.0f, 0.0f, 0.5f, 0.5f
    };

    std::vector<GLushort> indices = { 0, 1, 2, 3, 4, 5, 5, 4, 6 };


    GLuint v_buffer_handle[2];
    glGenBuffers(2, v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_buffer_handle[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLfloat), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v_buffer_handle[1]);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    auto u_modifiers_index = glGetUniformBlockIndex(program, "Modifiers");
    if (u_modifiers_index == GL_INVALID_INDEX) { std::cout << "Cannot find Modifiers uniform block in program" << std::endl; }
    else { glUniformBlockBinding(program, u_modifiers_index, 0); }

    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformations_index == GL_INVALID_INDEX) {
        std::cout << "Cannot find Transformations uniform block in program" << std::endl;
    }
    else {
        glUniformBlockBinding(program, u_transformations_index, 1);
    }


    GLuint ubo_handle[2];
    glGenBuffers(2, ubo_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle[0]);
    float strength = 0.9;
    const int light_color_size = 3;
    float light[light_color_size] = { 1.0f, 0.0f, 1.0f };
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), light);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_handle[0]);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle[1]);

    int w, h;
    std::tie(w, h) = frame_buffer_size();

    auto V = glm::lookAt(glm::vec3{ 0.5f, 0.5f, 3.0f }, glm::vec3{ 0.0f, 0.7f, 0.0f }, glm::vec3{ 10.0f, 1.0f, 0.6f });
    auto P = glm::perspective(glm::half_pi<float>(), (float)w / h, 0.1f, 1000.0f);
    auto PVM = P * V;

    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo_handle[1]);
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);


    glUseProgram(program);


}



void SimpleShapeApplication::frame() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    //glDrawArrays(GL_TRIANGLES, 0, 9);
    glBindVertexArray(0);
}