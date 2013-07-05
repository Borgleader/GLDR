#pragma once

#include "ResourceID.h"

#include <functional>
#include <iostream>
#include <Windows.h>
#include <gl/glew.h>

enum VADrawMode
{
	DRAW_POINTS = GL_POINTS,
	DRAW_LINE_STRIP = GL_LINE_STRIP,
	DRAW_LINE_LOOP = GL_LINE_LOOP,
	DRAW_LINES = GL_LINES,
	DRAW_LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
	DRAW_LINES_ADJACENCY = GL_LINES_ADJACENCY,
	DRAW_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
	DRAW_TRIANGLE_FAN = GL_TRIANGLE_FAN,
	DRAW_TRIANGLES = GL_TRIANGLES,
	DRAW_TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
	DRAW_TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
	DRAW_PATCHES = GL_PATCHES,
};

class VertexArrayID: public ResourceID<GLuint, std::function<void()>>
{
public:
	VertexArrayID(): ResourceID(0, [&](){ glDeleteVertexArrays(1, &id_); })
	{
		glGenVertexArrays(1, &id_);
	}
};

class VertexArray
{
public:
	VertexArray(VADrawMode drawmode): id_(), mode_(drawmode)
	{
	}

	void drawElements(size_t count, bool indexed = false)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindVertexArray(id_.get());

		if(indexed)
		{
			glDrawElements(mode_, count, GL_UNSIGNED_BYTE, (void*)0);
		}
		else
		{
			glDrawArrays(mode_, 0, count);
		}

		glBindVertexArray(0);
	}

	void bind()
	{
		glBindVertexArray(id_.get());
	}

	void unbind()
	{
		glBindVertexArray(0);
	}

private:
	VertexArrayID id_;
	VADrawMode mode_;
};
