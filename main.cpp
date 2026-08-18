#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OrbitCamera.h"
#include "Cube.h"
#include "RubiksCube.h"
#include "Ray.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

OrbitCamera camera = OrbitCamera(10.0f);

float lastX;
float lastY;
float cameraSpeed = 5.0f;
bool mouseInitialized = false;
bool bMiddleClicked = false;
bool bLeftClicked = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float inputTimeout = 0.2f;
float rotateCounter, axisCounter, sectionCounter, clockwiseCounter = 0.0f;
bool canRotate, canChangeAxis, canChangeSection, canChangeClockwise = true;
bool highlightCubesThisFrame = true;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath);
void loadShaderCode(const char* shaderPath, std::string& code);
void mouseCallback(GLFWwindow* window, double xPosInput, double yPosInput);
void processDeltaTime();
void processTimeouts();

void processInput(GLFWwindow* window);
void processPlayerRotate();
void processPlayerChangeAxis();
void processPlayerChangeSection(int direction);
void processPlayerSwitchClockwise();

void onRotationCompleted();
void onScrambleCompleted();

glm::vec3 getNearCursorWorldPosition(glm::mat4 projection, glm::mat4 viewMatrix);
glm::vec3 getFarCursorWorldPosition(glm::mat4 projection, glm::mat4 viewMatrix);
Ray getCursorWorldRay(glm::mat4 projection, glm::mat4 viewMatrix, float length = 10.0f);

// Temp Game Settings
int selectedAxis = 1;
RubiksSection selectedSection = RubiksSection::MIDDLE;
bool counterClockwise = false;
bool playerRotationActive = false;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Simple Rubrix", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	unsigned int colorShader = createShaderProgram("colorShader.vert", "colorShader.frag");

	float cubeVertices[] = {
		// Front Face
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		 // Right Face
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,

		 // Left Face
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,

		// Top Face
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,

		 // Bottom Face
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,

		 // Back Face
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	RubiksCube rubiksCube = RubiksCube(1.1f, 0.15f, 1.0f);

	rubiksCube.setOnScrambleComplete(onScrambleCompleted);
	rubiksCube.setRotationCompleteCallback(onRotationCompleted);
	rubiksCube.startScrambleSmooth(5);

	while (!glfwWindowShouldClose(window))
	{
		processDeltaTime();
		processInput(window);
		processTimeouts();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Camera View
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 model;
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		if (!bMiddleClicked && bLeftClicked && !rubiksCube.isPlayerSelecting())
		{
			Ray ray = getCursorWorldRay(projection, view, 200.0f);
			rubiksCube.processCubeSelection(ray, camera.getYaw());
		}
		else if (!bMiddleClicked && bLeftClicked && rubiksCube.isPlayerSelecting())
		{

		}
		else
		{
			rubiksCube.processCubeReleased();
		}

		glUseProgram(colorShader);
		glUniformMatrix4fv(glGetUniformLocation(colorShader, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(colorShader, "projection"), 1, GL_FALSE, &projection[0][0]);

		glBindVertexArray(VAO);

		rubiksCube.update(deltaTime);
		rubiksCube.renderCubes(VAO, colorShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(colorShader);

	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Create shader program and return its ID
unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	loadShaderCode(vertexPath, vertexCode);
	loadShaderCode(fragmentPath, fragmentCode);

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILTION_FAILED:\n" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILTION_FAILED:\n" << infoLog << std::endl;
	}

	unsigned int id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILURE:\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return id;
}

void loadShaderCode(const char* shaderPath, std::string& code)
{
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		code = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
}

void mouseCallback(GLFWwindow* window, double xPosInput, double yPosInput)
{
	float xPos = static_cast<float>(xPosInput);
	float yPos = static_cast<float>(yPosInput);

	if (!mouseInitialized)
	{
		lastX = xPos;
		lastY = yPos;
		mouseInitialized = true;
	}

	float xOffset = (xPos - lastX) * cameraSpeed * deltaTime;
	float yOffset = (lastY - yPos) * cameraSpeed * deltaTime;

	lastX = xPos;
	lastY = yPos;

	// Only rotate camera is middle mouse clicked
	if (bMiddleClicked)
		camera.processRotationInput(xOffset, yOffset);
}

void processDeltaTime()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void processTimeouts()
{
	if (!canRotate)
	{
		rotateCounter += deltaTime;
		if (rotateCounter > inputTimeout)
		{
			rotateCounter = 0.0f;
			canRotate = true;
		}
	}

	if (!canChangeAxis)
	{
		axisCounter += deltaTime;
		if (axisCounter > inputTimeout)
		{
			axisCounter = 0.0f;
			canChangeAxis = true;
		}
	}

	if (!canChangeSection)
	{
		sectionCounter += deltaTime;
		if (sectionCounter > inputTimeout)
		{
			sectionCounter = 0.0f;
			canChangeSection = true;
		}
	}

	if (!canChangeClockwise)
	{
		clockwiseCounter += deltaTime;
		if (clockwiseCounter > inputTimeout)
		{
			clockwiseCounter = 0.0f;
			canChangeClockwise = true;
		}
	}
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		processPlayerRotate();
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		processPlayerChangeAxis();
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		processPlayerChangeSection(1);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		processPlayerChangeSection(3);
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		processPlayerSwitchClockwise();

	// Can probably do this better :/
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		bMiddleClicked = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		bMiddleClicked = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		bLeftClicked = true;
	}
	else
	{
		bLeftClicked = false;
	}
}

// TODO: Start Rubik's Rotation Only When Allowable
void processPlayerRotate()
{
	if (playerRotationActive || !canRotate)
		return;
	playerRotationActive = true;
	canRotate = false;
}

void processPlayerChangeAxis()
{
	if (playerRotationActive || !canChangeAxis)
		return;
	selectedAxis = ((selectedAxis + 3) % 3) + 1;
	canChangeAxis = false;
	highlightCubesThisFrame = true;
}

void processPlayerChangeSection(int direction)
{
	if (playerRotationActive || !canChangeSection)
		return;

	int sectionInt = (((int)selectedSection + direction) % 3) + 1;
	selectedSection = static_cast<RubiksSection>(sectionInt);
	canChangeSection = false;
	highlightCubesThisFrame = true;
}

void processPlayerSwitchClockwise()
{
	if (playerRotationActive || !canChangeClockwise)
		return;
	counterClockwise = !counterClockwise;
	canChangeClockwise = false;
}


void onRotationCompleted()
{
	playerRotationActive = false;
	canRotate = true;
}

void onScrambleCompleted()
{

}

glm::vec3 getNearCursorWorldPosition(glm::mat4 projection, glm::mat4 viewMatrix)
{
	float x = (2.0f * lastX) / SCR_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * lastY) / SCR_HEIGHT;

	glm::vec4 rayStartNDC = glm::vec4(x, y, -1.0f, 1.0f);
	glm::vec4 rayStartCamera = glm::inverse(projection) * rayStartNDC;
	rayStartCamera /= rayStartCamera.w;
	glm::vec4 rayStartWorld = glm::inverse(viewMatrix) * rayStartCamera;
	rayStartWorld /= rayStartWorld.w;
	glm::vec3 rayStartWorldPosition = glm::vec3(rayStartWorld.x, rayStartWorld.y, rayStartWorld.z);

	return rayStartWorldPosition;
}

glm::vec3 getFarCursorWorldPosition(glm::mat4 projection, glm::mat4 viewMatrix)
{
	float x = (2.0f * lastX) / SCR_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * lastY) / SCR_HEIGHT;

	glm::vec4 rayEndNDC = glm::vec4(x, y, 0.0f, 1.0f);
	glm::vec4 rayEndCamera = glm::inverse(projection) * rayEndNDC;
	rayEndCamera /= rayEndCamera.w;
	glm::vec4 rayEndWorld = glm::inverse(viewMatrix) * rayEndCamera;
	rayEndWorld /= rayEndWorld.w;
	glm::vec3 rayEndWorldPosition = glm::vec3(rayEndWorld.x, rayEndWorld.y, rayEndWorld.z);

	return rayEndWorldPosition;
}

// Create a Ray in world space from the mouse cursor
Ray getCursorWorldRay(glm::mat4 projection, glm::mat4 viewMatrix, float length)
{
	glm::vec3 rayStartWorldPosition = getNearCursorWorldPosition(projection, viewMatrix);
	glm::vec3 rayEndWorldPosition = getFarCursorWorldPosition(projection, viewMatrix);

	glm::vec3 rayDirection = rayEndWorldPosition - rayStartWorldPosition;
	rayDirection = glm::normalize(rayDirection);

	return Ray(rayStartWorldPosition, rayDirection, length);
}