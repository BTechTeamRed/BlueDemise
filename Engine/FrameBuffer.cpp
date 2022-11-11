#include "FrameBuffer.h"

#include "glad/glad.h"

namespace Engine
{
	FrameBuffer::FrameBuffer(float width, float height)
		: fbo(0)
		, textureID(0)
		, renderBufferID(0)
		, windowWidth(width)
		, windowHeight(height)
		, clearColorRed(1)
		, clearColorGreen(1)
		, clearColorBlue(1)
		, clearColorAlpha(1)
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// create color texture
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowHeight, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

		//create depth/sencil renderbuffer
		glGenRenderbuffers(1, &renderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);

		// check for completeness
		unsigned int completeStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (completeStatus != GL_FRAMEBUFFER_COMPLETE) 
		{
			//error 
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &fbo);
		fbo = 0;
		textureID = 0;
		renderBufferID = 0;
	}
}