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
	class LabelAtlasConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 601 - 700
		{
			stringValue				= 601,
			charMapFile				= 602,
			itemWidth				= 603,
			itemHeight				= 604,
			startCharMap			= 605,
			charMapFileData			= 606,
			dataResourceType		= 607,
			dataResourcePath		= 608,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static void readProperties(UILabelAtlas* pWidget, NodeElement* optionsNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END