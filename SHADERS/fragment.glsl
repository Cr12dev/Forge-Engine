#version 330 core

in vec3 color2;
out vec4 fragColor;

void main()
{
    fragColor = vec4(color2, 1.0);
}