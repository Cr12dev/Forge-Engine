#version 460 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform bool useTexture;

void main() {
    vec4 texColor = vec4(1.0);
    
    if (useTexture) {
        texColor = texture(ourTexture, TexCoord);
    }


    vec4 finalColor = vec4(ourColor, 1.0) * texColor;



    FragColor = finalColor;
}