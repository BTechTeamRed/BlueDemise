#pragma once
namespace Engine
{
	//shader tokens to access map of shader types
	class ShaderFillType
	{
	public:
		static enum FillType
		{
			SN_TEXTURE_FILL,
			SN_COLOR_FILL,
			SN_TEXT_FILL,
			SN_GRADIENT_FILL
		};
		inline static const FillType DEFAULT_FILL_TYPE = FillType::SN_TEXTURE_FILL;
		inline static const int types = 3;
	};
}