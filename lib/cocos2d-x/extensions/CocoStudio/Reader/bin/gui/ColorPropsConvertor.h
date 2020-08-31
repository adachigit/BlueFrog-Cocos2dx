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

	class ColorPropsConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 101 - 200
		{
			opacity			= 101,
			colorR			= 102,
			colorG			= 103,
			colorB			= 104,
			anchorPointX	= 105,
			anchorPointY	= 106,
			flipX			= 107,
			flipY			= 108,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer, const WidgetType widgetType);
		static uint32_t readProperties(UIWidget* pWidget, NodeElement* optionsNode, uint32_t attrIndex, BinReader& reader);
	};
}
}

NS_CC_EXT_END