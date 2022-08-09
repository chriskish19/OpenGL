#pragma once
#include "glew.h"
#include "glfw3.h"
#include "shader.h"
#include "log.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/matrix_transform.hpp"



// This class handles the application creation and management
class app
{
public:
	app(int width, int height, const char* name)
		:m_WindowWidth{ width }, m_WindowHeight{ height }, m_WindowTitle{ name }
	{
        /* Initialize the library */
        if (!glfwInit())
        {
            log::ManualLogError("GLFW failed to Initialize");
        }


        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, m_WindowTitle, NULL, NULL);

        if (!m_window)
        {
            log::ManualLogError("Failed to create Window");
            glfwTerminate();
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);


        /* Initialize glew library */
        if (glewInit() != GLEW_OK)
        {
            log::ManualLogError("Glew Failed to Inititalize");
        }
	}
    void Run(shader& shader)
    {
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(m_window))
        {
            
            /* Render here */
            Render(shader);

            /* Swap front and back buffers */
            glfwSwapBuffers(m_window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    void Render(shader& shader)
    {
        // Returns the current time 
        double CurrentTime = glfwGetTime();

        // Clears buffers to preset values. It is ussually necessary to clear the depth buffer
        // before rendering a frame so that hidden surface removal occurs properly. 
        // (not clearing the depth buffer can sometimes result in every surface being removed, 
        // resulting in a completely black screen)
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);

        // Installs a program object as part of current rendering state 
        glUseProgram(shader.renderingProgram);

        // Returns the location of a uniform variable. Which I am guessing is in the shader.glsl.
        mvLoc = glGetUniformLocation(shader.renderingProgram, "mv_matrix");
        projLoc = glGetUniformLocation(shader.renderingProgram, "proj_matrix");

        // Changing screen size 
        // This will have its own function in the future
        glfwGetFramebufferSize(m_window, &m_WindowWidth, &m_WindowHeight);
        aspect = (float)m_WindowWidth / (float)m_WindowHeight;
        pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

        // Builds a translation matrix
        // glm::mat4(1.0f) : is the identity matrix
        // glm::vec3(-shader.cameraX, -shader.cameraY, -shader.cameraZ)) : is the camera position
        vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-shader.cameraX, -shader.cameraY, -shader.cameraZ));
        mMat = glm::translate(glm::mat4(1.0f), glm::vec3(shader.cubeLocX, shader.cubeLocY, shader.cubeLocZ));
        mvMat = vMat * mMat;

        // 
        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

        glBindBuffer(GL_ARRAY_BUFFER, shader.vbo[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(0);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    ~app()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    	
private:
	//Variables for creating a window
	GLFWwindow* m_window;
	int m_WindowWidth;
	int m_WindowHeight;
	const char* m_WindowTitle;

 
    GLuint mvLoc, projLoc;
    float aspect;
    glm::mat4 pMat, vMat, mMat, mvMat;



    


	
};
