#pragma once

#include <functional>
#include <iostream>
#include <Windows.h>
#include <gl/glew.h>

#include "..\src\glid.hpp"

namespace gldr {
	enum class VADrawMode : GLuint {
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

	class VertexArray {
	public:
		VertexArray(VADrawMode drawmode): id(), mode(drawmode) { }

		void drawElements(size_t count, bool indexed = false) {
			glEnableClientState(GL_VERTEX_ARRAY);
			glBindVertexArray(static_cast<GLuint>(id));

			if(indexed) {
				glDrawElements(static_cast<GLuint>(mode), count, GL_UNSIGNED_BYTE, (void*)0);
			}
			else {
				glDrawArrays(static_cast<GLuint>(mode), 0, count);
			}

			glBindVertexArray(0);
		}

		void bind() {
			glBindVertexArray(static_cast<GLuint>(id));
		}

		void unbind() {
			glBindVertexArray(0);
		}

		static GLuint create() {
			GLuint id;
			glGenVertexArrays(1, &id);

			return id;
		}

		static void destroy(GLuint& id) {
			glDeleteVertexArrays(1, &id);
			id = 0;
		}

	private:
		Glid<VertexArray> id;
		VADrawMode mode;
	};
}
