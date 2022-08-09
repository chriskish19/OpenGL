#pragma once
#include <iostream>
#include "glew.h"
#include "glfw3.h"
#include "App.h"

//////////////////////////////////////////
// Learning OpenGL using the book 
// Computer Grpahics Programming in OpenGL 
// with C++
//
// I try to rewrite the examples in a new way, 
// that way I actually learn something instead 
// of copying the code line for line.


int main(void)
{
    // Class objects 
    // app OpenGL: starts the application with initializing a window.
    // shader shader: Instantiate shader class
    class app OpenGL(640, 480, "My Window");
    class shader shader;
    
    // Initialize shader with vertices, vao, vbo
    shader.init();

    // Run the main game loop
    OpenGL.Run(shader);
    
    return 0;
}