#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"
#include "Renderer.h"

//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "IndexBuffer.h"
//#include "Shader.h"
//#include "Texture.h"
//#include "VertexArray.h"
//#include "VertexBuffer.h"

static void glfw_error_callback(int error, const char* description)
{
	std::cout << "[GLFW ERROR]: " << error << " " << description << std::endl;
	std::cin.get();
}

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwSetErrorCallback(glfw_error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 720, "OpenGL Sandbox", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
		return -1;

	glfwSwapInterval(1);

	test::TestClearColor test;

	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Renderer renderer;

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();

		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			test.OnUpdate(0.0f);
			test.OnRender();

			// imgui
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			test.OnImGuiRender();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

//int main(void)
//{
//	GLFWwindow* window;
//
//	if (!glfwInit())
//		return -1;
//
//	glfwSetErrorCallback(glfw_error_callback);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	window = glfwCreateWindow(1280, 720, "OpenGL Sandbox", NULL, NULL);
//	if (!window)
//	{
//		glfwTerminate();
//		return -1;
//	}
//
//	glfwMakeContextCurrent(window);
//	if (glewInit() != GLEW_OK)
//		return -1;
//
//	glfwSwapInterval(1);
//
//	{
//		float positions[] = {
//			-100.0f,  -100.0f,  0.0f,  0.0f,
//			 100.0f,  -100.0f,  1.0f,  0.0f,
//			 100.0f,   100.0f,  1.0f,  1.0f,
//			-100.0f,   100.0f,  0.0f,  1.0f,
//		};
//
//		unsigned int indicies[] = {
//			0, 1, 2,
//			2, 3, 0
//		};
//
//		GLCall(glEnable(GL_BLEND));
//		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
//
//		VertexArray va;
//
//		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
//		VertexBufferLayout vl;
//		vl.Push<float>(2);
//		vl.Push<float>(2);
//		va.AddBuffer(vb, vl);
//
//		IndexBuffer ib(indicies, 6);
//		va.AddIndexBuffer(ib);
//
//		va.Unbind();
//		vb.Unbind();
//		ib.Unbind();
//
//		Texture texture("res/textures/sav_design.png");
//		texture.Bind();
//
//		glm::mat4 proj = glm::ortho<float>(0, 1280, 0, 720, -1.0, 1.0);
//		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
//
//		Shader shader("res/shaders/Basic.shader");
//		shader.Bind();
//		shader.SetUniform4f("u_Color", 0.6, 0.8f, 0.4f, 1.0f);
//		shader.SetUniform1i("u_Texture", 0);
//		shader.Unbind();
//
//		Renderer renderer;
//
//		ImGui::CreateContext();
//		ImGuiIO& io = ImGui::GetIO(); (void)io;
//		ImGui::StyleColorsDark();
//
//		ImGui_ImplGlfw_InitForOpenGL(window, true);
//		ImGui_ImplOpenGL3_Init();
//
//		float r = 0.6f;
//		float increment_amt = 0.01f;
//
//		glm::vec3 modelTranslate(0, 0, 0);
//		glm::vec3 modelTranslate2(0, 0, 0);
//
//		while (!glfwWindowShouldClose(window))
//		{
//			renderer.Clear();
//
//			ImGui_ImplOpenGL3_NewFrame();
//			ImGui_ImplGlfw_NewFrame();
//			ImGui::NewFrame();
//
//			shader.Bind();
//			{
//				glm::mat4 model1 = glm::translate(glm::mat4(1.0f), modelTranslate);
//				glm::mat4 mvp1 = proj * view * model1;
//				shader.SetUniformMat4("u_MVP", mvp1);
//				shader.SetUniform4f("u_Color", r, 0.8f, 0.4f, 1.0f);
//				renderer.Draw(va, shader);
//			}
//			{
//				glm::mat4 model2 = glm::translate(glm::mat4(1.0f), modelTranslate2);
//				glm::mat4 mvp2 = proj * view * model2;
//				shader.SetUniformMat4("u_MVP", mvp2);
//				shader.SetUniform4f("u_Color", 0.65f, 1 - (r / 2), 0.5f, 1.0f);
//				renderer.Draw(va, shader);
//			}
//
//			if (r > 1.0f || r < 0.6f)
//				increment_amt *= -1;
//			r += increment_amt;
//
//			{
//				// ImGui control window
//				ImGui::Begin("Model data");
//				ImGui::SliderFloat2("object x", &modelTranslate.x, 0.0f, 1280.0f);
//				ImGui::SliderFloat2("object2 x", &modelTranslate2.x, 0.0f, 1280.0f);
//				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
//				ImGui::End();
//			}
//
//			ImGui::Render();
//			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//			/* Swap front and back buffers */
//			/* Poll for and process events */
//			glfwSwapBuffers(window);
//			glfwPollEvents();
//		}
//	}
//
//	ImGui_ImplOpenGL3_Shutdown();
//	ImGui_ImplGlfw_Shutdown();
//	ImGui::DestroyContext();
//
//	glfwTerminate();
//	return 0;
//}