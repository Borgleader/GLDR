#pragma once

#include "Utils.h"

#include <cassert>
#include <functional>
#include <Windows.h>
#include <gl/glew.h>

#include "..\src\glid.hpp"

namespace gldr {
	enum class ShaderType : GLuint {
		GLDR_VERT_SHADER = GL_VERTEX_SHADER,
		GLDR_GEOM_SHADER = GL_GEOMETRY_SHADER,
		GLDR_FRAG_SHADER = GL_FRAGMENT_SHADER,
	};

	template<ShaderType type>
	class Shader {
	public:
		Shader(std::string filepath): id() {
			std::string shaderContent(loadFileContent<std::string>(filepath));
			assert(!shaderContent.empty());

			int length = shaderContent.length();
			const char* shaderString = shaderContent.c_str();
			glShaderSource(id.get(), 1, &shaderString, &length);
			glCompileShader(id.get());

			GLint compiled;
			glGetShaderiv(id.get(), GL_COMPILE_STATUS, &compiled);
			assert(compiled == GL_TRUE);
		}

		static GLuint create() {
			GLuint id;
			id = glCreateShader(static_cast<GLuint>(type));

			return id;
		}

		static void destroy(GLuint& id) {
			glDeleteShader(id);
			id = 0;
		}

	private:
		Glid<Shader> id;
		friend class ShaderProgram;
	};
}
