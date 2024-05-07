#ifndef SHADER_H
#define SHADER_H 

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class c2Shader
{
    public:
      //Compile shader files given in path and link into shader program
      c2Shader(const char* vertexPath, 
             const char* geometryPath, 
             const char* fragmentPath);

      //If geometry shader is not in use
      c2Shader(const char* vertexPath, 
             const char* fragmentPath);

      //Load shader program into GPU
      void LoadShader();

      //Returns shader program ID
      unsigned int GetShaderID();

      //Sets the projection matrix uniform in the vertex shader
      void SetModelMat(const glm::mat4& model);

      //Sets the projection matrix uniform in the vertex shader
      void SetViewMat(const glm::mat4& view);

      //Sets the projection matrix uniform in the vertex shader
      void SetProjectionMat(const glm::mat4& projection);

      //For setting the integer uniforms 
      void SetIntUniform(const int& val, const char* uniform_name);

    private:
      unsigned int ShaderProgramID;

};


inline void c2Shader::LoadShader()
{
    glUseProgram(ShaderProgramID);
}

inline unsigned int c2Shader::GetShaderID()
{
    return ShaderProgramID;
}

inline void c2Shader::SetModelMat(const glm::mat4& model)
{
    unsigned int modelUniform =
        glGetUniformLocation(ShaderProgramID, "model");

    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
}

inline void c2Shader::SetViewMat(const glm::mat4& view)
{
    unsigned int viewUniform =
        glGetUniformLocation(ShaderProgramID, "view");

    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
}

inline void c2Shader::SetProjectionMat(const glm::mat4& projection)
{
    unsigned int projectionUniform =
        glGetUniformLocation(ShaderProgramID, "projection");

    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
}

inline void c2Shader::SetIntUniform(const int& val, const char* uniform_name)
{
    glUniform1i(glGetUniformLocation(ShaderProgramID, uniform_name), val);
}

#endif /* SHADER_H */
