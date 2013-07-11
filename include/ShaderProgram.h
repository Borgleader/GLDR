#pragma once

#include <cassert>
#include <functional>
#include <Windows.h>
#include <gl/glew.h>

#include "Shader.h"
#include "Utils.h"

#include "..\src\glid.hpp"

namespace gldr {
	class ShaderProgram {
	public:
		ShaderProgram(): id() { }

		template<ShaderType type>
		void attachShader(Shader<type>& shader) {
			glAttachShader(static_cast<GLuint>(id), static_cast<GLuint>(shader.id));
		}

		void bindAttribute(size_t attribIndex, std::string attribute) {
			glBindAttribLocation(static_cast<GLuint>(id), attribIndex, attribute.c_str());
		}

		void link() {
			glLinkProgram(static_cast<GLuint>(id));

			GLint isLinked;
			glGetProgramiv(static_cast<GLuint>(id), GL_LINK_STATUS, &isLinked);
			assert(isLinked == GL_TRUE);
		}

		GLuint getUniformLocation(std::string uniformName) {
			return glGetUniformLocation(static_cast<GLuint>(id), uniformName.c_str());
		}

		void use() {
			glUseProgram(static_cast<GLuint>(id));
		}

		void unuse() {
			glUseProgram(0);
		}

		static GLuint create() {
			GLuint id;
			id = glCreateProgram();

			return id;
		}

		static void destroy(GLuint& id) {
			glDeleteProgram(id);
			id = 0;
		}

	private:
		Glid<ShaderProgram> id;
	};
}
