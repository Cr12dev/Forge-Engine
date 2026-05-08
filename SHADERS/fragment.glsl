#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform bool useTexture;

void main() {
    if (useTexture) {
        // Mezcla el color del objeto con la textura
        FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
    } else {
        // Solo usa el color definido
        FragColor = vec4(ourColor, 1.0);
    }
}