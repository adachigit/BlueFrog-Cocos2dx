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
	class ListViewConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 1401 - 1500
		{
			innerWidth			= 1501,
			innerHeight			= 1502,
			direction			= 1503,
			gravity				= 1504,
			itemMargin			= 1505,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static void readProperties(UIListViewEx* pWidget, NodeElement* optionsNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END