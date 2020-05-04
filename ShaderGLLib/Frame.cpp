#include "Frame.h"
#include <stdexcept>
#include <GL/glew.h>

namespace sgl {

	Frame::Frame()
	{
#pragma message ("You have to complete this code!")
	}

	Frame::~Frame()
	{
#pragma message ("You have to complete this code!")
	}

	void Frame::Bind() const
	{
#pragma message ("You have to complete this code!")
	}

	void Frame::UnBind() const
	{
#pragma message ("You have to complete this code!")
	}

	void Frame::BindAttach(const Render& render) const
	{
#pragma message ("You have to complete this code!")
	}

	void Frame::BindTexture(
		const Texture& texture,
		const int mipmap /*= 0*/,
		const FrameTextureType frame_texture_type 
			/*= FrameTextureType::TEXTURE_2D*/) const
	{
#pragma message ("You have to complete this code!")
	}

	const int Frame::GetFrameTextureType(
		const FrameTextureType frame_texture_type) const
	{
		int value = static_cast<int>(frame_texture_type);
		if (value >= 0)
		{
			return GL_TEXTURE_CUBE_MAP_POSITIVE_X + value;
		}
		return GL_TEXTURE_2D;
	}

} // End namespace sgl.
