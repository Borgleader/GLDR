#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <vector>
#include <Windows.h>
#include <gl/glew.h>
#include <webp/decode.h>

#include "..\src\glid.hpp"

namespace gldr {
	enum class WrappingOption : GLuint {
		GLDR_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE, 
		GLDR_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		GLDR_REPEAT = GL_REPEAT,
	};

	enum class FilteringOption : GLuint {
		GLDR_NEAREST = GL_NEAREST,
		GLDR_LINEAR = GL_LINEAR,
	};

	class Texture {
	public:
		Texture(): id(), width(0), height(0), pixels(nullptr) { }

		Texture(std::string filename, WrappingOption wrapping = WrappingOption::GLDR_CLAMP_TO_EDGE, FilteringOption filtering = FilteringOption::GLDR_NEAREST): 
			id(), width(0), height(0), pixels(nullptr) 
		{
			// Load and decode image data
			std::vector<char> rawData(loadFileContent<std::vector<char>>(filename));
			pixels.reset(WebPDecodeRGBA(reinterpret_cast<unsigned char*>(rawData.data()), rawData.size(), &width, &height));

			// Set texture parameters and data
			glBindTexture(GL_TEXTURE_2D, id.get());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLuint>(wrapping));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLuint>(wrapping));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLuint>(filtering));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLuint>(filtering));
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)pixels.get());
			glBindTexture(GL_TEXTURE_2D, 0);

			assert(pixels.get() != nullptr);
		}

		Texture& operator=(Texture&& other) {
			id = std::move(other.id);
			width = other.width;
			height = other.height;
			pixels = std::move(other.pixels);

			other.width = 0;
			other.height = 0;
			other.pixels.release();

			return *this;
		}

		void bind() {
			glBindTexture(GL_TEXTURE_2D, id.get());
		}

		void unbind() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		static GLuint create() {
			GLuint id;
			glGenTextures(1, &id);

			return id;
		}

		static void destroy(GLuint& id) {
			glDeleteTextures(1, &id);
			id = 0;
		}

	private:
		Glid<Texture> id;
		int width;
		int height;
		std::unique_ptr<uint8_t> pixels;
	};
}
