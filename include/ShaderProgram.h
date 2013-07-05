#pragma once

#include "ResourceID.h"
#include "Shader.h"
#include "Utils.h"

#include <cassert>
#include <functional>
#include <Windows.h>
#include <gl/glew.h>

class ShaderProgramID: public ResourceID<GLuint, std::function<void()>>
{
public:
	ShaderProgramID(): ResourceID(0, [&](){ glDeleteShader(id_); })
	{
		id_ = glCreateProgram();
	}
};

class ShaderProgram
{
public:
	ShaderProgram(): id_()
	{
	}

	void attachShader(Shader& shader)
	{
		glAttachShader(id_.get(), shader.id_.get());
	}

	void bindAttribute(size_t attribIndex, std::string attribute)
	{
		glBindAttribLocation(id_.get(), attribIndex, attribute.c_str());
	}

	void link()
	{
		glLinkProgram(id_.get());

		GLint isLinked;
		glGetProgramiv(id_.get(), GL_LINK_STATUS, &isLinked);
		assert(isLinked == GL_TRUE);
	}

	GLuint getUniformLocation(std::string uniformName)
	{
		return glGetUniformLocation(id_.get(), uniformName.c_str());
	}

	void Use()
	{
		glUseProgram(id_.get());
	}

private:
	ShaderProgramID id_;
};
