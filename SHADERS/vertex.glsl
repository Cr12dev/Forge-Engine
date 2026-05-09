#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 proyeccion;

layout(std430, binding = 0) buffer ObjetoBuffer {
    float datos[]; 
} objetoBuffer;

void main() {
    int base = gl_InstanceID * 12; 

    // extraer matrices (16 floats cada una)
    mat4 mTras = mat4(
        vec4(1.0f, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, 1.0f, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, 1.0f, 0.0f),
        vec4(objetoBuffer.datos[base + 2] / 100.0f, objetoBuffer.datos[base + 3] / 100.0f, 0.0f, 1.0f)
    );
    float rotacionGrados = objetoBuffer.datos[base + 4];
    float c = cos(radians(rotacionGrados));
    float s = sin(radians(rotacionGrados));
    mat4 mRot = mat4(
        vec4(c,s,0.0f,0.0f),
        vec4(-s, c,0.0f,0.0f),
        vec4(0.0f,0.0f,1.0f,0.0f),
        vec4(0.0f,0.0f,0.0f,1.0f)
    );

    mat4 mEsc = mat4(
        vec4(objetoBuffer.datos[base + 5] / 100.0f,0.0f,0.0f,0.0f),
        vec4(0.0f,objetoBuffer.datos[base + 6] / 100.0f,0.0f,0.0f),
        vec4(0.0f,0.0f,1.0f,0.0f),
        vec4(0.0f,0.0f,0.0f,1.0f)
    );

    mat4 mPriv = mat4(
        vec4(objetoBuffer.datos[base],0.0f,0.0f,0.0f),
        vec4(0.0f,objetoBuffer.datos[base + 1],0.0f,0.0f),
        vec4(0.0f,0.0f,1.0f,0.0f),
        vec4(0.0f,0.0f,0.0f,1.0f)
    );

    ourColor = vec3(objetoBuffer.datos[base + 8] / 255.0f, objetoBuffer.datos[base + 9] / 255.0f,objetoBuffer.datos[base + 10] / 255.0f);
    TexCoord = aTexCoord;

    // Combinar matrices y aplicar proyección
    gl_Position = proyeccion * mTras * mRot * mEsc * mPriv * vec4(aPos, 1.0);
}