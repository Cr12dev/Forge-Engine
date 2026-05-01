#include <iostream>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "Anvil.h"

//====================IMPORTANTE=====================   
//Si vas a usar este script renombrar a "script.h" y agregarlo al proyecto
//El motor lo va a compilar y ejecutar automaticamente


//===================================================
//=====================**PONG**======================
//==================EN FORGE ENGINE==================
//===================================================

//=======================Definicion de objetos globales========================
//Inicializamos los objetos y las variables globales que vamos a usar en el script
//No se preocupen por esto, es solo para que el motor pueda acceder a estas variables y objetos desde el script

AnvilObject Suelo;
AnvilObject Perosnaje;
GLFWwindow* Window;
Collision2D colision;

float speedplayer = 80.0f;

void Start() {
    Suelo.CreateQuad(1080, 3);
    Personaje.CreateQuad(10, 10);

    Suelo.Position = Vector2(0.0f, -100.0f);
    Perosnaje.Position = Vector2(0.0f, 0.0f);

    Window = glfwGetCurrentContext();

    //COLORS
    Personaje.color = Color(1.0f, 100.0f, 0.0f);
    Suelo.color = Color(5.0f, 0.0f, 0.0f);
    
}

void Update() {
    std::cout << "Posicion del jugador: (" << Perosnaje.Position.x << ", " << Perosnaje.Position.y << ")" << std::endl;
    if (glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS && Suelo.Position.y < 80.0f) {
        Pala.Position.x += 150.0f * deltaTime();
    }
}