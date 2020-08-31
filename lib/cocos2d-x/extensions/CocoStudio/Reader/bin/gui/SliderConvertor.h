#pragma once

#include "rapidjson/document.h"
#include "BaseClasses/UIWidget.h"
#include "System/CocosGUI.h"

#include "../common.h"
#include "../BinWriter.h"
#include "../BinReader.h"

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	class SliderConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 901 - 1000
		{
			barTextureScale9Enable				= 901,
			barFileName							= 902,
			length								= 903,
			barFileNameData						= 904,
			dataResourceType					= 905,
			dataResourcePath					= 906,
			ballNormalData						= 907,
			ballPressedData						= 908,
			ballDisabledData					= 909,
			progressBarData						= 910,
			percent								= 911,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static void readProperties(UISlider* pWidget, NodeElement* optionsNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END