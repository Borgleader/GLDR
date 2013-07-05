#pragma once

#include "ResourceID.h"

#include <cassert>
#include <functional>
#include <memory>
#include <vector>
#include <Windows.h>
#include <gl/glew.h>
#include <webp/decode.h>

enum WrappingOption
{
	GLDR_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE, 
	GLDR_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
	GLDR_REPEAT = GL_REPEAT,
};

enum FilteringOption
{
	GLDR_NEAREST = GL_NEAREST,
	GLDR_LINEAR = GL_LINEAR,
};

class TextureID: public ResourceID<GLuint, std::function<void()>>
{
public:
	TextureID(): ResourceID(0, [&](){ glDeleteTextures(1, &id_); })
	{
		glGenTextures(1, &id_);
	}
};

class Texture
{
public:
	Texture(): id_(), width_(0), height_(0), pixels_(nullptr)
	{
	}

	Texture(std::string filename, WrappingOption wrapping = GLDR_CLAMP_TO_EDGE, FilteringOption filtering = GLDR_NEAREST): 
		id_(), width_(0), height_(0), pixels_(nullptr)
	{
		// Load and decode image data
		std::vector<char> rawData(loadFileContent<std::vector<char>>(filename));
		pixels_.reset(WebPDecodeRGBA(reinterpret_cast<unsigned char*>(rawData.data()), rawData.size(), &width_, &height_));

		// Set texture parameters and data
		glBindTexture(GL_TEXTURE_2D, id_.get());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)pixels_.get());
		glBindTexture(GL_TEXTURE_2D, 0);

		assert(pixels_.get() != nullptr);
	}

	Texture& operator=(Texture&& other)
	{
		id_ = other.id_;
		width_ = other.width_;
		height_ = other.height_;
		pixels_ = std::move(other.pixels_);

		other.width_ = 0;
		other.height_ = 0;
		other.pixels_.release();

		return *this;
	}

	void bind()
	{
		glBindTexture(GL_TEXTURE_2D, id_.get());
	}

	void unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

private:
	TextureID id_;
	int width_;
	int height_;
	std::unique_ptr<uint8_t> pixels_;
};
