 #pragma once
#include "glew.h"
#include "glfw3.h"
#include <fstream>
#include "log.h"

// This class handles shader creation and initialization
class shader
{
public:
    // Initialization function
    // Creates the shader program
    void init()
    {
        renderingProgram = CreateShaderProgram("src/vertexShader.glsl","src/fragmentShader.glsl");
        
        // camera position in 3d space
        cameraX = 0.0f; cameraY = 0.0f; cameraZ = 6.0f;
        
        // cube position in 3d space
        cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;
        
        // Vertex Data
        // 3D Cube made up of triangles
        float vertexPositions[108] = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
        1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
        1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
        };


        // 
        glGenVertexArrays(1, vao);
        glBindVertexArray(vao[0]);


        glGenBuffers(numVBOs, vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

        // Creates and initializes a buffer object's data store
        // 
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

    }
private:
    GLuint CreateShaderProgram(std::string VertexShader, std::string FragmentShader)
	{
        GLuint vShader = PrepareShader(GL_VERTEX_SHADER, VertexShader);
        GLuint fShader = PrepareShader(GL_FRAGMENT_SHADER, FragmentShader);
       
        GLuint vfProgram = glCreateProgram();
        glAttachShader(vfProgram, vShader);
        glAttachShader(vfProgram, fShader);

        LinkProgram(vfProgram);
        return vfProgram;
	}
    GLuint CreateShaderProgram(std::string VertexShader, std::string FragmentShader, std::string GeometryShader)
    {
        GLuint vShader = PrepareShader(GL_VERTEX_SHADER, VertexShader);
        GLuint fShader = PrepareShader(GL_FRAGMENT_SHADER, FragmentShader);
        GLuint gShader = PrepareShader(GL_GEOMETRY_SHADER, GeometryShader);

        GLuint vfProgram = glCreateProgram();
        glAttachShader(vfProgram, vShader);
        glAttachShader(vfProgram, fShader);
        glAttachShader(vfProgram, gShader);

        LinkProgram(vfProgram);
        return vfProgram;
    }  
    std::string ReadShaderSource(std::string FilePath)
    {
        std::string content;
        std::string line{ "" };
        std::ifstream FileRead(FilePath, std::ios::in);
        if (!FileRead.is_open())
        {
            log::ManualLogError("Error: Shader not read");
        }
        while (!FileRead.eof())
        {
            std::getline(FileRead, line);
            content.append(line + "\n");
        }

        FileRead.close();
        return content;
    }
    void CompileShader(GLuint shader)
    {
        glCompileShader(shader);
        log::checkOpenGLError();
        log::checkCompileStatus(shader);
    }
    GLuint PrepareShader(int ShaderType, std::string FilePath)
    {
        std::string str_Shader = ReadShaderSource(FilePath);
        const char* char_p_shaderSource = str_Shader.c_str();
        GLuint Uint_Shader = glCreateShader(ShaderType);
        glShaderSource(Uint_Shader, 1, &char_p_shaderSource, NULL);
        CompileShader(Uint_Shader);
        return Uint_Shader;
    }
    void LinkProgram(GLuint Program)
    {
        glLinkProgram(Program);
        log::checkOpenGLError();
        log::checkLinkStatus(Program);
    }

public:
    GLuint renderingProgram;
	static const int numVAOs{ 1 };
    static const int numVBOs{ 2 };
	GLuint vao[numVAOs];
    GLuint vbo[numVBOs];
    

    float cameraX, cameraY, cameraZ;
    float cubeLocX, cubeLocY, cubeLocZ;
};