#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "Anvil.h"
#include <vector>
#include <fstream>
#include <sstream>
#include "EXAMPLE/RENDIMIENTO.h"

float vertices[] = {
    -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};

void error_callback(int error, const char* description) {
    std::cerr << "Error de GLFW (" << error << "): " << description << std::endl;
}

std::string loadFile(const char* path) {
    std::ifstream file(path);
    std::stringstream buffer;
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el shader: " << path << std::endl;
        return "";
    }
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    int elementosAntes = 0;
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit()) {
        std::cout << "Error: No se pudo inicializar GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1200, 650, "Ventana de ForgeEngine", nullptr, nullptr);
    if (!window) {
        std::cout << "Error: No se pudo crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error: No se pudo inicializar GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    std::string vertexSource = loadFile("./SHADERS/vertexNEW.glsl");
    std::string fragmentSource = loadFile("./SHADERS/fragmentNEW.glsl");

    if (vertexSource.empty() || fragmentSource.empty()) {
        std::cerr << "Error cargando shaders" << std::endl;
        glfwTerminate();
        return -1;
    }

    const char* vertexShaderSource = vertexSource.c_str();
    const char* fragmentShaderSource = fragmentSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR VERTEX SHADER: " << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    glUseProgram(shaderProgram);

    // Inicializar SSBO vacío
    unsigned int ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    float aspect = 1200.0f / 800.0f;
    float matrizProye[] = {
        1.0f / aspect, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    float matrizRotacion[] = {
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float matrizTraslacion[] = {
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float matrizEscala[] = {
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float matrizEscalaPrivada[] = {
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float colorfloat[] = { 1.0f, 1.0f, 1.0f };

    int idUniform = glGetUniformLocation(shaderProgram, "proyeccion");
    int texturaID = glGetUniformLocation(shaderProgram, "ourTexture");
    glUniformMatrix4fv(idUniform, 1, GL_FALSE, matrizProye);

    // Inicializamos el script del juego (Crea los 10,000 objetos)
    Start();


    auto& lista = InternalPassVerticesList();
    auto& objetosLista = InternalPassReferenceObjects();

    float ultimoframe = 0.0f;
    std::vector<float> matricesParaEnviar;

    // ==================== Bucle principal ====================
    while (!glfwWindowShouldClose(window)) {
        glClearColor(GetBackgroundColor().r, GetBackgroundColor().g, GetBackgroundColor().b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        Update(); 

        // 1. Redimensionar el SSBO solo si la cantidad de objetos cambia
        if (objetosLista.size() != elementosAntes) {
            matricesParaEnviar.resize(objetosLista.size() * 68);
            elementosAntes = objetosLista.size();
            
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
            glBufferData(GL_SHADER_STORAGE_BUFFER, matricesParaEnviar.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        }

        if (elementosAntes > 0) {
            // 2. PROCESAMIENTO EN CPU (Llenamos el vector local a toda velocidad)
            for (size_t i = 0; i < objetosLista.size(); i++) {
                

                matrizEscalaPrivada[0] = lista[i][0];
                matrizEscalaPrivada[5] = lista[i][1]; 
                
                matrizTraslacion[12] = objetosLista[i]->Position.x / 100.0f;
                matrizTraslacion[13] = objetosLista[i]->Position.y / 100.0f;
                
                if (objetosLista[i]->Rotation.z >= 360.0f) {
                    objetosLista[i]->Rotation.z = 0.0f;
                }
                
                float rad = (objetosLista[i]->Rotation.z) * (M_PI / 180.0f);
                float cosVal = cos(rad);
                float sinVal = sin(rad);
                
                matrizRotacion[0] = cosVal;
                matrizRotacion[1] = -sinVal;
                matrizRotacion[4] = sinVal;
                matrizRotacion[5] = cosVal;
                
                matrizEscala[0] = objetosLista[i]->Scale.x / 100.0f;
                matrizEscala[5] = objetosLista[i]->Scale.y / 100.0f;
                
                colorfloat[0] = objetosLista[i]->color.r;
                colorfloat[1] = objetosLista[i]->color.g;
                colorfloat[2] = objetosLista[i]->color.b;
                
                int base = i * 68;
                for (int j = 0; j < 16; j++) {
                    matricesParaEnviar[base + j]      = matrizTraslacion[j];
                    matricesParaEnviar[base + j + 16] = matrizRotacion[j];
                    matricesParaEnviar[base + j + 32] = matrizEscala[j];
                    matricesParaEnviar[base + j + 48] = matrizEscalaPrivada[j];
                }
                for (int j = 0; j < 3; j++) {
                    matricesParaEnviar[base + 64 + j] = colorfloat[j];
                }
                matricesParaEnviar[base + 67] = 0.0f; // Padding
            }

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, matricesParaEnviar.size() * sizeof(float), matricesParaEnviar.data());

            bool hasTexture = (objetosLista[0]->textureID != 0);
            glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), hasTexture);
            if (hasTexture) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, objetosLista[0]->textureID);
                glUniform1i(texturaID, 0);
            } else {
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            
        }
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, objetosLista.size());
        float deltatime = glfwGetTime() - ultimoframe;
        ultimoframe = glfwGetTime();
        InternalPassDontAsk(deltatime);
        
        glfwSwapBuffers(window);
        glfwPollEvents(); // <--- Esto mantiene la ventana con vida
    }

    glfwTerminate();
    return 0;
}