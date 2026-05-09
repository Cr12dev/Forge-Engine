#include <iostream>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "../Anvil.h"
#include <cstdlib>
#include <ctime>



//===================================================
//=====================**TEST**======================
//==============DE VELOCIDAD DEL MOTOR===============
//===================================================



std::vector<AnvilObject*> cosas;
GLFWwindow* window;



//=======================Funciones del script========================

void Start() {
    std::cout << "Dale a la tecla W" << std::endl;
    window = glfwGetCurrentContext();
    srand(time(0));
    for (int i = 0; i < 1000000; i++)
    {
        AnvilObject nuevoObjeto = new AnvilObject(); 
        nuevoObjeto->CreateQuad(2, 10);
        nuevoObjeto->Position = Vector2((rand() % 60)-30, (rand() % 60) - 30);
        cosas.push_back(nuevoObjeto);
    }
    

}

void Update() {
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        for (int i = 0; i < 1000000; i++)
        {
        
            cosas[i]->Position += Vector2(((rand() % 100) / 30), ((rand() % 100) / 30));

        }
    }
    std::cout << deltaTime() << std::endl;

}