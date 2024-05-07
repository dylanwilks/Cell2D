#version 330 core
layout (location = 0) in vec3 coords_in;
layout (location = 1) in vec3 color_in;

//Try using uniform buffer objects
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int px;

out vec3 color;

void main()
{
    //CUDA can access OpenGL VBO: use for flooring
    gl_Position = projection * view * model * vec4(coords_in, 1.0);
    gl_PointSize = px;
    color = color_in;
}
