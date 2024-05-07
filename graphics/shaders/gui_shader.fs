#version 330 core

out vec4 FragColor;
in vec2 tex_coords;

uniform sampler2D texture1; 

void main()
{
    vec4 texColor = texture(texture1, tex_coords);
    
    if(texColor.a == 0.0)
        discard;

    FragColor = texColor;
}
