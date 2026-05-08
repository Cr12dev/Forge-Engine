#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 proyeccion;

layout(std430, binding = 0) buffer ObjetoBuffer {
    float datos[]; 
} objetoBuffer;

void main() {
    // CAMBIO CLAVE: Usamos 68 como multiplicador para mantener la alineación de 16 bytes
    int base = gl_InstanceID * 68; 

    // Extraer matrices (16 floats cada una)
    // El constructor mat4(vec4, vec4, vec4, vec4) es lo más seguro en GLSL
    mat4 mTras = mat4(
        vec4(objetoBuffer.datos[base + 0],  objetoBuffer.datos[base + 1],  objetoBuffer.datos[base + 2],  objetoBuffer.datos[base + 3]),
        vec4(objetoBuffer.datos[base + 4],  objetoBuffer.datos[base + 5],  objetoBuffer.datos[base + 6],  objetoBuffer.datos[base + 7]),
        vec4(objetoBuffer.datos[base + 8],  objetoBuffer.datos[base + 9],  objetoBuffer.datos[base + 10], objetoBuffer.datos[base + 11]),
        vec4(objetoBuffer.datos[base + 12], objetoBuffer.datos[base + 13], objetoBuffer.datos[base + 14], objetoBuffer.datos[base + 15])
    );

    mat4 mRot = mat4(
        vec4(objetoBuffer.datos[base + 16], objetoBuffer.datos[base + 17], objetoBuffer.datos[base + 18], objetoBuffer.datos[base + 19]),
        vec4(objetoBuffer.datos[base + 20], objetoBuffer.datos[base + 21], objetoBuffer.datos[base + 22], objetoBuffer.datos[base + 23]),
        vec4(objetoBuffer.datos[base + 24], objetoBuffer.datos[base + 25], objetoBuffer.datos[base + 26], objetoBuffer.datos[base + 27]),
        vec4(objetoBuffer.datos[base + 28], objetoBuffer.datos[base + 29], objetoBuffer.datos[base + 30], objetoBuffer.datos[base + 31])
    );

    mat4 mEsc = mat4(
        vec4(objetoBuffer.datos[base + 32], objetoBuffer.datos[base + 33], objetoBuffer.datos[base + 34], objetoBuffer.datos[base + 35]),
        vec4(objetoBuffer.datos[base + 36], objetoBuffer.datos[base + 37], objetoBuffer.datos[base + 38], objetoBuffer.datos[base + 39]),
        vec4(objetoBuffer.datos[base + 40], objetoBuffer.datos[base + 41], objetoBuffer.datos[base + 42], objetoBuffer.datos[base + 43]),
        vec4(objetoBuffer.datos[base + 44], objetoBuffer.datos[base + 45], objetoBuffer.datos[base + 46], objetoBuffer.datos[base + 47])
    );

    mat4 mPriv = mat4(
        vec4(objetoBuffer.datos[base + 48], objetoBuffer.datos[base + 49], objetoBuffer.datos[base + 50], objetoBuffer.datos[base + 51]),
        vec4(objetoBuffer.datos[base + 52], objetoBuffer.datos[base + 53], objetoBuffer.datos[base + 54], objetoBuffer.datos[base + 55]),
        vec4(objetoBuffer.datos[base + 56], objetoBuffer.datos[base + 57], objetoBuffer.datos[base + 58], objetoBuffer.datos[base + 59]),
        vec4(objetoBuffer.datos[base + 60], objetoBuffer.datos[base + 61], objetoBuffer.datos[base + 62], objetoBuffer.datos[base + 63])
    );

    ourColor = vec3(objetoBuffer.datos[base + 64], objetoBuffer.datos[base + 65], objetoBuffer.datos[base + 66]);
    TexCoord = aTexCoord;

    // Combinar matrices y aplicar proyección
    gl_Position = proyeccion * mTras * mRot * mEsc * mPriv * vec4(aPos, 1.0);
}