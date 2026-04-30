#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;

uniform mat4 proyeccion;
uniform mat4 rotacion;
uniform mat4 translacion;
uniform mat4 escala;

out vec3 color2;

void main()
{
    gl_Position = proyeccion * translacion * rotacion * escala * vec4(aPos, 1.0);
    color2 = color;
}