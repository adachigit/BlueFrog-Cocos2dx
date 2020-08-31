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
	class LabelConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 501 - 600
		{
			touchScaleEnable		= 501,
			text					= 502,
			fontSize				= 503,
			fontName				= 504,
			areaWidth				= 505,
			areaHeight				= 506,
			hAlignment				= 507,
			vAlignment				= 508,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static void readProperties(UILabel* pWidget, NodeElement* optionsNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END