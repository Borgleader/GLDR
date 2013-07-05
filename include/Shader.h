#pragma once

#include "ResourceID.h"
#include "Utils.h"

#include <cassert>
#include <functional>
#include <Windows.h>
#include <gl/glew.h>

enum ShaderType
{
	GLDR_VERT_SHADER = GL_VERTEX_SHADER,
	GLDR_GEOM_SHADER = GL_GEOMETRY_SHADER,
	GLDR_FRAG_SHADER = GL_FRAGMENT_SHADER,
};

class ShaderID: public ResourceID<GLuint, std::function<void()>>
{
public:
	ShaderID(ShaderType type): ResourceID(0, [&](){ glDeleteBuffers(1, &id_); })
	{
		id_ = glCreateShader(type);
	}
};

class Shader
{
	friend class ShaderProgram;
public:
	Shader(ShaderType type, std::string filepath): id_(type), type_(type)
	{
		std::string shaderContent(loadFileContent<std::string>(filepath));
		assert(!shaderContent.empty());

		int length = shaderContent.length();
		const char* shaderString = shaderContent.c_str();
		glShaderSource(id_.get(), 1, &shaderString, &length);
		glCompileShader(id_.get());

		GLint compiled;
		glGetShaderiv(id_.get(), GL_COMPILE_STATUS, &compiled);
		assert(compiled == GL_TRUE);
	}

private:
	ShaderID id_;
	ShaderType type_;
};
