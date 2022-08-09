#pragma once
#include "color.hpp"
#include <fstream>
#include <chrono>
#include <time.h>
#include <stdlib.h>

// This class logs errors to the console and a txt file
// its a static class its not meant to be objected
class log
{
public:
	// Blank Constructor:
	log()
	{
		
	}
	
	// This function: 
	// Outputs error to console and txt file
	// Also gets the current time and logs it with the error
	// Accepts any type: Some errors are integers, some char*...
	template<class t>
	static void ManualLogError(t ErrorOut)
	{
		//output errors and logs to a txt file
		std::ofstream FileLog;
		
		// opening file for writing log errors to
		FileLog.open("logging/log.txt", std::ios::app);

		//Geting Current time
		auto clock = std::chrono::system_clock::now();
		std::time_t CurrentTime = std::chrono::system_clock::to_time_t(clock);
		char TimeBuff[30];
		ctime_s(TimeBuff, sizeof(TimeBuff), &CurrentTime);

		// The dye::red is defined in header color.hpp and is a header 
		// for changing console output text color
		std::cout << dye::red(ErrorOut) << std::endl;
		FileLog << dye::red(ErrorOut) << std::endl;
		std::cout << dye::yellow(TimeBuff) << std::endl;
		FileLog << dye::yellow(TimeBuff) << std::endl;
		


	}
	
	// Gets log info for a shader
	// Errors and such
	static void printShaderLog(GLuint shader)
	{
		int length{ 0 };
		int chLength{ 0 };
		char* log;
		
		// Returns the length of the log in int length
		// Check the length of the log
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		
		// if length has a length get the data from log then send it 
		// to the ManualLogError function to be displayed and saved
		// then delete log
		if (length > 0)
		{
			// log is a char array and this allocates it from the size of length. Why not do this instead log[length];?
			// malloc : Memory allocation
			// (char*) : cast to type char*
			log = (char*)malloc(length);
			
			// glGetShaderInfoLog parameters
			// GLuint shader: Specifies the shader object whose information log is to be queried.
			// GLsizei maxLength: Specifies the size of the character buffer for storing the returned information log
			// GLsizei *length: Returns the length of the string returned in infoLog (excluding the null terminator).
			// GLchar *infoLog: Specifies an array of characters that is used to return the information log.
			// : Returns the information log for a shader object
			glGetShaderInfoLog(shader, length, &chLength, log);
			ManualLogError(log);
			
			// deallocates log
			free(log);
		}
	}
	
	// Gets program info
	// Whether it was sucessfully created or not
	static void printProgramLog(int prog)
	{
		int length{ 0 };
		int chLength{ 0 };
		char* log;

		// Returns a parameter from a program object
		// Returns the length of the log
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);
		if (length > 0)
		{
			log = (char*)malloc(length);

			// Returns the log in log
			glGetProgramInfoLog(prog, length, &chLength, log);
			ManualLogError(log);
			free(log);
		}
	}
	
	// Checks for errors with glGetError() function
	// if one is found it gets logged
	static bool checkOpenGLError()
	{
		bool foundError = false;

		// return error information
		// Each detectable error is assigned a numeric code and symbolic name. 
		// When an error occurs, the error flag is set to the appropriate error code value.
		// If a call to glGetError returns GL_NO_ERROR, there has been 
		// no detectable error since the last call to glGetError.
		int glError = glGetError();
		while (glError != GL_NO_ERROR)
		{
			ManualLogError(glError);
			foundError = true;
			glError = glGetError();
		}
		return foundError;
	}
	
	// Checks if compiling of a shader was successful or not
	// if it failed the output is logged with ManualLogError
	// and printShaderLog is called
	static void checkCompileStatus(GLuint shader)
	{
		//Return Variables used in the functions below
		GLint Compiled{ 0 };
		GLint Type{ 0 };
		
		// Returns compile status whether it was succesful or not
		glGetShaderiv(shader, GL_COMPILE_STATUS, &Compiled);

		// Returns type of shader ex. Vertex, fragment ...
		glGetShaderiv(shader, GL_SHADER_TYPE, &Type);
		if (Compiled != 1)
		{
			switch (Type)
			{
			case GL_VERTEX_SHADER:
				ManualLogError("GL_VERTEX_SHADER Compilation Failed");
				break;

			case GL_GEOMETRY_SHADER:
				ManualLogError("GL_GEOMETRY_SHADER Compilation Failed");
				break;

			case GL_FRAGMENT_SHADER:
				ManualLogError("GL_FRAGMENT_SHADER Compilation Failed");
				break;
			}
			
			printShaderLog(shader);
		}
		
	}
	
	// Checks if a program linked successfully or not
	static void checkLinkStatus(GLuint program)
	{
		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (linked != 1)
		{
			ManualLogError("Linking Failed");
			printProgramLog(program);
		}
	}

	
};