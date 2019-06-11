#pragma once
#include "Engine/Source/Utility/Singleton.h"
#include <dwrite.h>
#include <d2d1_3.h>
#include <d3d11on12.h>



namespace Hashira {

	class TextUI : Singleton< TextUI>
	{
	public:
		
		struct TextBlock{
			String text;
			D2D1_RECT_F layout;
			bool useCustomColor;
			Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> customTextBrush;
			bool useCustomFormat;
			Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> customTextFormat;
			
		};
	private:
		//デバイス
	public:
	};

}