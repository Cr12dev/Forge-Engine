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
#include "EXAMPLE/PONG.h"

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
    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error: No se pudo inicializar GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    std::string vertexSource = loadFile("./SHADERS/vertex.glsl");
    std::string fragmentSource = loadFile("./SHADERS/fragment.glsl");

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
    glBufferData(GL_ARRAY_BUFFER, 1000, NULL, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
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

    int idUniform = glGetUniformLocation(shaderProgram, "proyeccion");
    int texturaID = glGetUniformLocation(shaderProgram, "ourTexture");
    glUniformMatrix4fv(idUniform, 1, GL_FALSE, matrizProye);

    // Inicializamos el script del juego (Crea los 10,000 objetos)
    Start();


    auto& lista = InternalPassVerticesList();
    auto& objetosLista = InternalPassReferenceObjects();

    float ultimoframe = 0.0f;
    std::vector<float> matricesParaEnviar;
    short floatsAEnviar = 12;
    // ==================== Bucle principal ====================
    while (!glfwWindowShouldClose(window)) {
        glClearColor(GetBackgroundColor().r, GetBackgroundColor().g, GetBackgroundColor().b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Update();
        //bucle principal, las listas que definimos en Anvil.h las recorre agarra los vertices y los renderiza
        //ademas (pendiente por corregir) si los objetos y los vertices estan alineados (en esta version suponemos que si)
        //aplicamos estas propiedades al shader de los objetos en la lista y dibujamos
        //y esto se vuelve a repetir con todos los objetos en las listas creados
        for(size_t i = 0; i < lista.size(); i++) {
            matrizEscalaPrivada[0] = lista[i][0];
            matrizEscalaPrivada[5] = lista[i][1]; 
            matrizTraslacion[12] = objetosLista[i]->Position.x / 100.0f;
            matrizTraslacion[13] = objetosLista[i]->Position.y / 100.0f;
            if (objetosLista[i]->Rotation.z >= 360) {
                objetosLista[i]->Rotation.z = 0;
            }
            matrizRotacion[0] = cos((objetosLista[i]->Rotation.z) * (M_PI / 180));
            matrizRotacion[1] = -sin((objetosLista[i]->Rotation.z) * (M_PI / 180));
            matrizRotacion[4] = sin((objetosLista[i]->Rotation.z) * (M_PI / 180));
            matrizRotacion[5] = cos((objetosLista[i]->Rotation.z) * (M_PI / 180));
            matrizEscala[0] = objetosLista[i]->Scale.x / 100;
            matrizEscala[5] = objetosLista[i]->Scale.y / 100;
            colorfloat[0] = objetosLista[i]->color.r;
            colorfloat[1] = objetosLista[i]->color.g;
            colorfloat[2] = objetosLista[i]->color.b;
            glUniformMatrix4fv(rotacionID, 1, GL_FALSE, matrizRotacion);
            glUniformMatrix4fv(translacionID, 1, GL_FALSE, matrizTraslacion);
            glUniformMatrix4fv(escalaPrivadaID, 1, GL_FALSE, matrizEscalaPrivada);
            glUniformMatrix4fv(escalaID, 1, GL_FALSE, matrizEscala);
            glUniform3fv(objectColorID, 1, colorfloat);
            
            // Bind texture if exists
            bool hasTexture = (objetosLista[i]->textureID != 0);
            glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), hasTexture);
            
            if (hasTexture) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, objetosLista[i]->textureID);
                glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
            } else {
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            
            GLenum drawMode = (objetosLista[i]->primitiveType == TRIANGLE_FAN) ? GL_TRIANGLE_FAN : GL_TRIANGLE_STRIP;
            glDrawArrays(drawMode, 0, 4);
        }   

        float deltatime = glfwGetTime() - ultimoframe;
        ultimoframe = glfwGetTime();
        InternalPassDontAsk(deltatime);
        
        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }

    glfwTerminate();
    return 0;
}