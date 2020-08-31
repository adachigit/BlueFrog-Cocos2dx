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
	class PageViewConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 1301 - 1400
		{
			innerWidth				= 1301,
			innerHeight				= 1302,
			direction				= 1303,
			gravity					= 1304,
			itemMargin				= 1305,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static void readProperties(UIPageViewEx* pWidget, NodeElement* optionsNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END