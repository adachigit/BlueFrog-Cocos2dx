#pragma once

#include "rapidjson/document.h"
#include "BaseClasses/UIWidget.h"

#include "../common.h"
#include "../BinWriter.h"
#include "../BinReader.h"

USING_NS_CC_EXT;

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	class WidgetConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 11 - 20
		{
			classname			= 11,
			options				= 12,
			children			= 13,
		};

		enum WIDGET_TYPE : uint32_t
		{
			Button				= 1,
			CheckBox			= 2,
			ImageView			= 3,
			LabelAtlas			= 4,
			LabelBMFont			= 5,
			Label				= 6,
			Layout				= 7,
			ListView			= 8,
			LoadingBar			= 9,
			PageView			= 10,
			ScrollView			= 11,
			Slider				= 12,
			TextField			= 13,
		};

	public:
		static void writeProperties(const rapidjson::Value& data, uint32_t node, BinWriter& writer);
		static UIWidget* readProperties(NodeElement* pNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END