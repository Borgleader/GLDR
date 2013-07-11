#pragma once

#include <cassert>
#include <functional>
#include <Windows.h>
#include <gl/glew.h>

#include "..\src\glid.hpp"

namespace gldr {
	enum class BufferType : GLuint {
		ARRAY_BUFFER = GL_ARRAY_BUFFER,
		ATOMIC_COUNTER_BUFFER = GL_ATOMIC_COUNTER_BUFFER,
		COPY_READ_BUFFER = GL_COPY_READ_BUFFER,
		COPY_WRITE_BUFFER = GL_COPY_WRITE_BUFFER,
		DRAW_INDIRECT_BUFFER = GL_DRAW_INDIRECT_BUFFER,
		DISPATCH_INDIRECT_BUFFER = GL_DISPATCH_INDIRECT_BUFFER,
		ELEMENT_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
		PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
		PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
		SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER,
		TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
		TRANSFORM_BUFFER = GL_TRANSFORM_FEEDBACK_BUFFER,
		UNIFORM_BUFFER = GL_UNIFORM_BUFFER,
	};

	enum class BufferUsageType : GLuint {
		DYNAMIC_COPY = GL_DYNAMIC_COPY,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
		DYNAMIC_READ = GL_DYNAMIC_READ,
		STATIC_COPY = GL_STATIC_COPY,
		STATIC_DRAW = GL_STATIC_DRAW,
		STATIC_READ = GL_STATIC_READ,
		STREAM_COPY = GL_STREAM_COPY,
		STREAM_DRAW = GL_STREAM_DRAW,
		STREAM_READ = GL_STREAM_READ,
	};

	enum class BufferAttribType : GLuint {
		BYTE_ATTRIB = GL_BYTE,
		DOUBLE_ATTRIB = GL_DOUBLE,
		FLOAT_ATTRIB = GL_FLOAT,
		INT_ATTRIB = GL_INT,
		SHORT_ATTRIB = GL_SHORT,
		UBYTE_ATTRIB = GL_UNSIGNED_BYTE,
		UINT_ATTRIB = GL_UNSIGNED_INT,
		USHORT_ATTRIB = GL_UNSIGNED_SHORT,
	};

	class Buffer {
	public:
		Buffer(BufferType bufType, BufferUsageType bufUsage, GLuint bufSize, GLuint bufStride, const void* bufData): 
			id(), type(bufType), usage(bufUsage), numElements(bufSize/bufStride), stride(bufStride), size(bufSize) 
		{
			// Make sure the size is a multiple of stride
			assert((size/stride)*stride == size);

			glBindBuffer(static_cast<GLuint>(type), id.get());
			glBufferData(static_cast<GLuint>(type), size, bufData, static_cast<GLuint>(usage));
		}

		template<typename T, typename U>
		void enableAttribute(size_t attribIndex, U T::* member, BufferAttribType attribType, GLboolean isNormalized) {
			enableAttribute(attribIndex, sizeof(U)/sizeofmember(member), attribType, isNormalized, sizeof(T), offsetOfMember<T, U>(member));
		}

		void enableAttribute(size_t attribIndex, size_t size, BufferAttribType attribType, GLboolean isNormalized, size_t stride, size_t offset) {
			glBindBuffer(static_cast<GLuint>(type),  id.get());
			glVertexAttribPointer(attribIndex, size, static_cast<GLuint>(attribType), isNormalized, stride, (void*)offset);
			glEnableVertexAttribArray(attribIndex);
		}

		size_t getElementCount() {
			return numElements;
		}

		BufferType getType() {
			return type;
		}

		static GLuint create() {
			GLuint id;
			glGenBuffers(1, &id);

			return id;
		}

		static void destroy(GLuint& id) {
			glDeleteBuffers(1, &id);
			id = 0;
		}

	private:
		template<typename T, typename U>
		static size_t offsetOfMember(U T::* member) {
			return static_cast<size_t>(reinterpret_cast<const volatile char&>(member));
		}

		template<typename U>
		static size_t sizeofmember(U* member) {
			return sizeof(U);
		}

		template<typename U>
		static size_t sizeofmember(U member) {
			return sizeof(U);
		}

		Glid<Buffer> id;
		BufferType type;
		BufferUsageType usage;
		GLuint numElements;
		GLuint stride;
		GLuint size;
	};
}
