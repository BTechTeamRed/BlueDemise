#pragma once
namespace Engine
{
	//shader tokens to access map of shader types
	class ShaderFillType
	{
	public:
		static enum FillType
		{
			FT_TEXTURE_FILL,
			FT_COLOR_FILL,
			FT_TEXT_FILL,
			FT_GRADIENT_FILL,
			FT_EMPTY_VALUE
		};
		inline static const FillType DEFAULT_FILL_TYPE = FillType::FT_TEXTURE_FILL;
		inline static const int types = 3;
	};
}