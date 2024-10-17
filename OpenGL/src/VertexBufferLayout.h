#pragma once
#include <GL/glew.h>
#include <vector>

struct VertexBufferLayoutElement {
	unsigned int type;
	unsigned int count;
	bool normalized;
	static unsigned int GetSizeOfType(const unsigned int& type);
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferLayoutElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout();

	const std::vector<VertexBufferLayoutElement>& GetElements() const;

	const unsigned int& GetStride() const;

	template<typename T>
	void Push(unsigned int count);

	template<>
	void Push<float>(unsigned int count);
	
	template<>
	void Push<unsigned int>(unsigned int count);
	
	template<>
	void Push<char>(unsigned int count);
};