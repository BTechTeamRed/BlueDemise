#pragma once

namespace Engine
{
	class FrameBuffer
	{
	public:	
		FrameBuffer(float width, float height);
		~FrameBuffer();

		inline unsigned int GetFbo() const { return fbo; }
		inline unsigned int GetTextureID() const { return textureID; }
		inline unsigned int GetRenderBufferID() const { return renderBufferID; }


	private:
		unsigned int fbo;
		unsigned int textureID;
		unsigned int renderBufferID;

		float windowWidth, windowHeight;
		float clearColorRed, clearColorGreen, clearColorBlue, clearColorAlpha;

	};
}