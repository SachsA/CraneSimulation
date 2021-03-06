//#define GLFW_INCLUDE_GLU
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "MyGlWindow.h"

MyGlWindow *win;

GLFWwindow* window;

int width = 800;
int height = 800;

ImVec4 change_color = ImVec4(0.4, 0.4, 0.4, 1.00f);

float rotation = 0.0f;
float horizontal_translation = 0.0f;
float vertical_translation = 0.0f;

int choice;

bool lbutton_down;
bool rbutton_down;
bool mbutton_down;

double m_lastMouseX;
double m_lastMouseY;
double cx, cy;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	win->setSize(width, height);
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	cx = xpos;
	cy = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		m_lastMouseX = xpos;
		m_lastMouseY = ypos;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			lbutton_down = true;
		else if (GLFW_RELEASE == action)
			lbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (GLFW_PRESS == action)
			rbutton_down = true;
		else if (GLFW_RELEASE == action)
			rbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (GLFW_PRESS == action)
			mbutton_down = true;
		else if (GLFW_RELEASE == action)
			mbutton_down = false;
	}
}

void mouseDragging(double width, double height)
{
	ImGuiIO& io = ImGui::GetIO();

	if (!io.WantCaptureMouse) {
		if (lbutton_down) {
			float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
			float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
			win->m_viewer->rotate(fractionChangeX, fractionChangeY);
		}
		else if (mbutton_down) {
			float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
			float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
			win->m_viewer->zoom(fractionChangeY);
		}
		else if (rbutton_down) {
			float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
			float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
			win->m_viewer->translate(-fractionChangeX, -fractionChangeY, 1);
		}
	}

	m_lastMouseX = cx;
	m_lastMouseY = cy;
}

int setupGlfwWindow()
{
	//Initialize the library
	if (!glfwInit())
	{
		fprintf(stderr, "Error: glfwInit window failed");
		return -1;
	}

	window = glfwCreateWindow(width, height, "OpenGL FrameWork", NULL, NULL);

	if (!window)
	{
		//Problem: create window failed
		fprintf(stderr, "Error: problem glfwCreateWindow failed");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//Enable vsync
	glfwSwapInterval(1);

	glfwSetWindowTitle(window, "GL Window SACHS_A");

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	return 0;
}

int setupGlew()
{
	GLenum err = glewInit();
	
	if (err != GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return -1;
	}

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	return 0;
}

void setupImgui()
{
	const char* glsl_version = "#version 430";

	// Setup ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();
}

void startImguiFrames()
{
	// Start ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void destroyImgui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void createImguiElements()
{	
	ImGui::SliderAngle("Boom Rotation", &rotation, -180.0f, 180.0f);
	win->setAngle(rotation);

	ImGui::SliderFloat("Box Horizontal", &horizontal_translation, 0.0f, 1.0f, "ratio = %.3f");
	win->setHorizontalTranslation(horizontal_translation);

	ImGui::SliderFloat("Box Vertical", &vertical_translation, 0.0f, 1.0f, "ratio = %.3f");
	win->setVerticalTranslation(vertical_translation);

	ImGui::RadioButton("Default", &choice, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Box", &choice, 1);
	win->setCamera(choice == 0);
	
	win->setVerticalCubes(7);
	win->setHorizontalCubes(8);
}

void drawImgui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void drawWindowBackground()
{
	//Clear this 2 buffers before drawing : very important
	//Background color
	glClearColor(change_color.x, change_color.y, change_color.z, change_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

int main()
{
	if (setupGlfwWindow() == -1)
		return -1;
	
	if (setupGlew() == -1)
		return -1;

	setupImgui();

	win = new MyGlWindow(width, height);

	while (!glfwWindowShouldClose(window))
	{
		startImguiFrames();

		drawWindowBackground();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h); //Draw in a certain part of the screen

		if (ImGui::Begin("Control Window")) {
			ImGui::Text("Crane Control");
			createImguiElements();
			ImGui::End();
		}

		win->draw();

		drawImgui();

		glfwSwapBuffers(window);

		glfwPollEvents();
		mouseDragging(display_w, display_h);
	}

	destroyImgui();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}