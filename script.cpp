#include <iostream>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "Anvil.h"

AnvilObject holamundo;
AnvilObject holamundo2;
GLFWwindow* Window;
Collision2D colision;

void Start() {
    holamundo.CreateQuad(30,30);
    holamundo2.CreateQuad(30,10);
    holamundo2.Position.x = -90;
    holamundo2.Position.y = 0;
    holamundo2.Rotation.z = 90;
    Window = glfwGetCurrentContext();
}

void Update() {
    if (glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        holamundo2.Position += Vector2(1,0);
    }
    colision = holamundo2.CheckCollision();
    if(colision.Bool)
    {
        holamundo2.Position.x -= 2;
    }
    std::cout << deltaTime() << std::endl;
}