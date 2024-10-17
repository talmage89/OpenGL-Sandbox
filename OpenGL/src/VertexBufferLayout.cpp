#include "VertexBufferLayout.h"
#include "Renderer.h"

unsigned int VertexBufferLayoutElement::GetSizeOfType(const unsigned int& type) {
	switch (type) {
	case GL_FLOAT:			return 4;
	case GL_UNSIGNED_INT:	return 4;
	case GL_BYTE:			return 1;
	}
	ASSERT(false);
	return 0;
}

VertexBufferLayout::VertexBufferLayout() : m_Stride(0) {}

const std::vector<VertexBufferLayoutElement>& VertexBufferLayout::GetElements() const {
	return m_Elements;
}

const unsigned int& VertexBufferLayout::GetStride() const {
	return m_Stride;
}

template<typename T>
void VertexBufferLayout::Push(unsigned int count) {
	static_assert(false);
}

template<>
void VertexBufferLayout::Push<float>(unsigned int count) {
	m_Elements.push_back({ GL_FLOAT, count,  false });
	m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
	m_Elements.push_back({ GL_UNSIGNED_INT, count,  false });
	m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::Push<char>(unsigned int count) {
	m_Elements.push_back({ GL_BYTE, count,  false });
	m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_BYTE);
}

