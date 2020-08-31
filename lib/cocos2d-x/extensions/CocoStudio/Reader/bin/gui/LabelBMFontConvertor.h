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
	class LabelBMFontConvetor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 1201 - 1300
		{
			fileNameData				= 1201,
			dataResourceType			= 1202,
			dataResourcePath			= 1203,
			text						= 1204,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static void readProperties(UILabelBMFont* pWidget, NodeElement* optionsNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END