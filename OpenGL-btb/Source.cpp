#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>

#include "Utils/Utils.h"

#define numVAOs 1
#define numVBOs 2

struct Camera
{
	Camera(float x, float y, float z) : camera_x{ x }, camera_y{ y }, camera_z{z} {}
	float camera_x;
	float camera_y;
	float camera_z;
};

struct CubeLoc
{
	CubeLoc(float x, float y, float z) : cube_loc_x{ x }, cube_loc_y{ y }, cube_loc_z{ z } {}
	float cube_loc_x;
	float cube_loc_y;
	float cube_loc_z;
};

float camera_x, camera_y, camera_z;
float cube_loc_x, cube_loc_y, cube_loc_z;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint MV_loc, Proj_loc, v_loc, TF_loc;
int width, height;
float aspect, timeFactor;
glm::mat4 p_mat, v_mat, m_mat, mv_mat, r_mat, t_mat;

//Camera* p_camera = new Camera(0.0f, 0.0f, 8.0f);
//CubeLoc* p_cube_loc = new CubeLoc(0.0f, -20.0f, 0.0f);


void setupVertices()
{
	float vertexPositions[108] =
	{
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0
	};

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}


GLuint createShaderProgram()
{
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	std::string	VertexShaderSrc = Utils::readShaderSource("Shaders/VertexShader.glsl");
	std::string FragmentShaderSrc = Utils::readShaderSource("Shaders/FragmentShader.glsl");
	
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	auto VS_src = VertexShaderSrc.c_str();
	auto FS_src = FragmentShaderSrc.c_str();
	
	glShaderSource(vShader, 1, &VS_src, nullptr);
	glShaderSource(fShader, 1, &FS_src, nullptr);
	
	glCompileShader(vShader);

	Utils::checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) 
	{
		std::cout << "vertex compilation failed" << std::endl;
		Utils::printShaderLog(vShader);
	}
	
	glCompileShader(fShader);

	Utils::checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) 
	{
		std::cout << "fragment compilation failed" << std::endl;
		Utils::printShaderLog(fShader);
	}
	
	GLuint vfProgram = glCreateProgram();
	
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);

	Utils::checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) 
	{
		std::cout << "linking failed" << std::endl;
		Utils::printProgramLog(vfProgram);
	}
	
	return vfProgram;
}

void init(GLFWwindow* window)
{
	renderingProgram = createShaderProgram();
	glfwGetFramebufferSize(window, &width, &height);
	camera_x = 0.0f; camera_y = 0.0f; camera_z = 20.0f;
	cube_loc_x = 0.0f; cube_loc_y = 0.0f; cube_loc_z = 0.0f;
	
	//glGenVertexArrays(numVAOs, vao);
	//glBindVertexArray(vao[0]);
	
	aspect = static_cast<float>(width) / static_cast<float>(height);
	p_mat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
	setupVertices();
	
}

void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	v_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-camera_x, -camera_y, -camera_z));

	v_loc = glGetUniformLocation(renderingProgram, "v_matrix");
	Proj_loc = glGetUniformLocation(renderingProgram, "proj_matrix");

	glUniformMatrix4fv(v_loc, 1, GL_FALSE, glm::value_ptr(v_mat));
	glUniformMatrix4fv(Proj_loc, 1, GL_FALSE, glm::value_ptr(p_mat));


	timeFactor = static_cast<float>(currentTime);
	TF_loc = glGetUniformLocation(renderingProgram, "tf");
	glUniform1f(TF_loc, static_cast<float>(timeFactor));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 10);
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
