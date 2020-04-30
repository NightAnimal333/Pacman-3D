#include <fstream>

#include "window.h"
#include "mesh.h"

GLFWwindow* initializeWindow()
{
	glfwWindowHint(GLFW_SAMPLES, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Starts GLFW
	if (!glfwInit()) {
		GFX_ERROR("Failed to start glfw\n");
	}

	//Size of the window
	int h = 640, w = 800;
	GLFWwindow* window = glfwCreateWindow(w, h, "El Pacmaño 3d", nullptr, nullptr);

	if (window == nullptr)
	{
		GFX_ERROR("Failed to open GLFW window.\n");
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	// Initialize Glad
	if (!gladLoadGL())
	{
		GFX_ERROR("Failed to initialize GLAD\n");
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSwapInterval(1);

	return window;
}

std::string readFile(const char* source)
{
	std::ifstream file{ source };
	file.sync_with_stdio(false);

	std::string out = {};
	std::string line = {};

	while (std::getline(file, line)) {
		out += line + "\n";
	}

	if (!out.length()) {
		std::cout << "Shader file is empty!\n";
		return "";
	}

	return out;
}

GLuint makeShader(const char* fragSource, const char* vertSource)
{
	int success, len;	//Checks whether compiling the shader/program was successful
	const char* text;	//Used to read the content of shader
	GLuint vertex, fragment;

	//Reads and compiles the vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexText = readFile(vertSource);
	text = vertexText.c_str();
	glShaderSource(vertex, 1, &text, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		std::cout << "Failed to load vertex shader!\n";

		glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &len);
		auto log = new char[len];

		/* Get the log text and print it */
		glGetShaderInfoLog(vertex, len, &len, log);
		fprintf(stderr, "Vertex shader compile error:\n%s\n", log);
	}

	//Reads and compiles fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentText = readFile(fragSource);
	text = fragmentText.c_str();
	glShaderSource(fragment, 1, &text, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		std::cout << "Failed to load fragment shader!\n";

		glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &len);
		auto log = new char[len];

		/* Get the log text and print it */
		glGetShaderInfoLog(fragment, len, &len, log);
		fprintf(stderr, "Fragment shader compile error:\n%s\n", log);
	}

	//Creates a program, links and removes shaders
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		/* Get log length */
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
		auto log = new char[len];

		/* Get the log text and print it */
		glGetProgramInfoLog(program, len, &len, log);
		fprintf(stderr, "Program Link Error:\n%s\n", log);

		/* Cleanup and return an invalid GL Name */
		glDeleteProgram(program);
		delete[] log;
		return 0;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return program;
}
