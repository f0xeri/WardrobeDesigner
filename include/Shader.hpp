//
// Created by Yaroslav on 28.07.2020.
//

#ifndef OPENGLTEST_PROGRAM_HPP
#define OPENGLTEST_PROGRAM_HPP

#include "GL/glew.h"
#include "GL/gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader
{
private:

    GLuint mVertexShader;
    GLuint mFragmentShader;

    GLuint loadShader(const std::string &path, GLenum shaderType);

public:
    GLuint mProgram;

    Shader(const std::string &vert, const std::string &frag);
    ~Shader();
    void link();
    void bindAttribute(GLuint index, const std::string &name);
    void use();
    void uniformMatrix(glm::mat4 matrix, const std::string &name);
    void setFloat(float number, const std::string &name);
    void setInt(int number, const std::string &name);
};

#endif //OPENGLTEST_PROGRAM_HPP