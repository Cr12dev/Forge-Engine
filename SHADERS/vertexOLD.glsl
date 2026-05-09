#version 330 core

// Atributos de entrada (definidos en tu VAO)
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

// Matrices uniformes que envías en el bucle for
uniform mat4 proyeccion;
uniform mat4 rotacion;
uniform mat4 translacion;
uniform mat4 escala;
uniform mat4 escalaPrivada;

// Color del objeto enviado por glUniform3fv
uniform vec3 objectColor;

out vec3 ourColor;
out vec2 TexCoord;

void main() {
    // multiplicamos las matrices en orden inverso (de derecha a izquierda)
    // escala los vértices brutos (escalaPrivada)
    // aplica la escala del objeto (escala)
    // aplica la rotación (rotacion)
    // aplica la posición (translacion)
    // proyecta al espacio de la pantalla (proyeccion)
    gl_Position = proyeccion * translacion * rotacion * (escala * escalaPrivada) * vec4(aPos, 1.0);
    
    // pasamos el color y las coordenadas de textura al Fragment Shader
    ourColor = aColor * objectColor;
    TexCoord = aTexCoord;
}