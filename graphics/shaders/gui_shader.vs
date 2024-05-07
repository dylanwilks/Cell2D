#version 330 core
layout (location = 0) in vec3 coords_in;
layout (location = 1) in vec2 tex_in;

out vec2 tex_coords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(coords_in, 1.0);
    tex_coords = tex_in;
}
