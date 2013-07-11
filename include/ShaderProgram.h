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
			glAttachShader(id.get(), shader.id.get());
		}

		void bindAttribute(size_t attribIndex, std::string attribute) {
			glBindAttribLocation(id.get(), attribIndex, attribute.c_str());
		}

		void link() {
			glLinkProgram(id.get());

			GLint isLinked;
			glGetProgramiv(id.get(), GL_LINK_STATUS, &isLinked);
			assert(isLinked == GL_TRUE);
		}

		GLuint getUniformLocation(std::string uniformName) {
			return glGetUniformLocation(id.get(), uniformName.c_str());
		}

		void use() {
			glUseProgram(id.get());
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
