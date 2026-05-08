#version 430 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform bool useTexture;

void main() {
    vec4 texColor = vec4(1.0); // Blanco por defecto
    
    if (useTexture) {
        texColor = texture(ourTexture, TexCoord);
    }

    // Mezclamos el color del objeto con la textura
    vec4 finalColor = vec4(ourColor, 1.0) * texColor;

    // Si el objeto es muy oscuro o transparente, lo forzamos para debug
    if (finalColor.a < 0.1) discard;

    FragColor = finalColor;
}