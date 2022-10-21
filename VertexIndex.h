#pragma once

class VertexIndex
{
public:

	VertexIndex(unsigned int vertex0, unsigned int vertex1, unsigned int vertex2)
		:vertex0(vertex0), vertex1(vertex1), vertex2(vertex2) {}

	unsigned int vertex0;
	unsigned int vertex1;
	unsigned int vertex2;
};