#pragma once
#include <cstdlib>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Utils
{
	static void printShaderLog(GLuint shader)
	{
		int len = 0;
		int chWrittn = 0;
		char* log;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		if (len > 0)
		{
			log = (char*)malloc(len);
			glGetShaderInfoLog(shader, len, &chWrittn, log);
			std::cout << "Shader Info Log: " << log << std::endl;
			free(log);
		}
	}

	static void printProgramLog(int prog)
	{
		int len = 0;
		int chWrittn = 0;
		char* log;

		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);

		if (len > 0)
		{
			log = (char*)malloc(len);
			glGetProgramInfoLog(prog, len, &chWrittn, log);
			std::cout << "Program Info Log: " << log << std::endl;
			free(log);
		}
	}

	static bool checkOpenGLError()
	{
		bool foundError = false;
		int glErr = glGetError();

		while (glErr != GL_NO_ERROR)
		{
			std::cout << "glError: " << glErr << std::endl;
			foundError = true;
			glErr = glGetError();
		}

		return foundError;
	}

	static std::string readShaderSource(const char* filePath)
	{
		std::string content;
		std::ifstream fileStream(filePath, std::ios::in);
		std::string line = "";

		while (!fileStream.eof()) {
			getline(fileStream, line);
			content.append(line + "\n");
		}
		fileStream.close();
		return content;

	}

}
