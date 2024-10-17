#include "Renderer.h"
#include <iostream>

void GLClearErrors() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError(const char* function, const char* file, const int& line) {
	bool noError = true;
	while (GLenum error = glGetError()) {
		std::cout << "[OPENGL ERROR]: (" << error << ") " << function << file << ":" << line << std::endl;
		noError = false;
	}
	return noError;
}

void Renderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const Shader& shader) const {
	shader.Bind();
	va.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
