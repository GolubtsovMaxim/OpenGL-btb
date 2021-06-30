#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

#define numVAOs 1
GLuint renderingProgram;
GLuint vao[numVAOs];

std::string readShaderSource(const char* filePath)
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

void printShaderLog(GLuint shader)
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

void printProgramLog(int prog)
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

bool checkOpenGLError()
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

GLuint createShaderProgram()
{
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	std::string	VertexShaderSrc = readShaderSource("Shaders/VertexShader.glsl");
	std::string FragmentShaderSrc = readShaderSource("Shaders/FragmentShader.glsl");
	
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	auto VS_src = VertexShaderSrc.c_str();
	auto FS_src = FragmentShaderSrc.c_str();
	
	glShaderSource(vShader, 1, &VS_src, nullptr);
	glShaderSource(fShader, 1, &FS_src, nullptr);
	
	glCompileShader(vShader);
	
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) 
	{
		std::cout << "vertex compilation failed" << std::endl;
		printShaderLog(vShader);
	}
	
	glCompileShader(fShader);

	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) 
	{
		std::cout << "fragment compilation failed" << std::endl;
		printShaderLog(fShader);
	}
	
	GLuint vfProgram = glCreateProgram();
	
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);

	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) 
	{
		std::cout << "linking failed" << std::endl;
		printProgramLog(vfProgram);
	}
	
	return vfProgram;
}

void init(GLFWwindow* window)
{
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

float x = 0.0f; // location of triangle on x axis
float inc = 0.01f; // offset for moving the triangle

void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(renderingProgram);
	
	x += inc; // move the triangle along x axis
	if (x > 1.0f) inc = -0.01f; // switch to moving the triangle to the left
	if (x < -1.0f) inc = 0.01f;

	GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset"); // get ptr to "offset"
	glProgramUniform1f(renderingProgram, offsetLoc, x); // send value in "x" to "offset"
	
	glDrawArrays(GL_TRIANGLES, 0, 3);

	
}

int main() {
	
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(600, 600, "init", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}
	
	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window)) 
	{
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
