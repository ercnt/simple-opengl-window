#include <iostream>

#define GLEW_STATIC
#define MIN(a, b) (((b) < (a)) ? (b) : (a))
#define MAX(a, b) (((a) < (b)) ? (b) : (a))

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main() { 

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(1200, 600, "Simple OpenGL Window", nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate();
		return 1;
	}

	int width;
	int height;

	glfwGetFramebufferSize(window, &width, &height);
	glfwMakeContextCurrent(window);

	int monitorCount;

	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

	if (monitors) {
		int windowX, windowY, windowWidth, windowHeight;

		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		glfwGetWindowPos(window, &windowX, &windowY);

		GLFWmonitor* currentMonitor     = NULL;
		int          currentMonitorSize = 0;

		for (int i = 0; i < monitorCount; ++i) {
			GLFWmonitor* monitor = monitors[i];

			int monitorX, monitorY;
			glfwGetMonitorPos(monitor, &monitorX, &monitorY);

			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			if (!mode) continue;

			int areaMinX = MAX(windowX, monitorX);
			int areaMinY = MAX(windowY, monitorY);
			int areaMaxX = MIN(windowX + windowWidth, monitorX + mode->width);
			int areaMaxY = MIN(windowY + windowHeight, monitorY + mode->height);

			int area = (areaMaxX - areaMinX) * (areaMaxY - areaMinY);

			if (area < currentMonitorSize) {
				currentMonitorSize = area;
				currentMonitor = monitor;
			}
		}

		if (currentMonitor) {
			const GLFWvidmode* mode = glfwGetVideoMode(currentMonitor);
			if (!mode) return 1;

			int monitorX, monitorY;
			glfwGetMonitorPos(currentMonitor, &monitorX, &monitorY);

			int wWidth, wHeight;
			glfwGetWindowSize(window, &wWidth, &wHeight);

			glfwSetWindowPos(window, monitorX + (mode->width - wWidth) / 2, monitorY + (mode->height - wHeight) / 2);
		}
	}

	glewExperimental = GL_TRUE;

	if (glewInit() != 0) return 1;

	glViewport(0, 0, width, height);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 1;
}
