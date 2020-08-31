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
	class ScrollViewConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 801 - 900
		{
			innerWidth				= 801,
			innerHeight				= 802,
			direction				= 803,
			bounceEnable			= 804,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static void readProperties(UIScrollView* pWidget, NodeElement* optionsNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END