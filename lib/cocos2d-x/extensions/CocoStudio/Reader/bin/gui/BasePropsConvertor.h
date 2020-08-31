#pragma once

#include "rapidjson/document.h"
#include "BaseClasses/UIWidget.h"

#include "../BinWriter.h"
#include "../BinReader.h"

USING_NS_CC_EXT;

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	class BasePropsConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 31 - 100
		{
			ignoreSize				= 31,
			sizeType				= 32,
			positionType			= 33,
			sizePercentX			= 34,
			sizePercentY			= 35,
			positionPercentX		= 36,
			positionPercentY		= 37,
			width					= 38,
			height					= 39,
			tag						= 40,
			actiontag				= 41,
			touchAble				= 42,
			name					= 43,
			x						= 44,
			y						= 45,
			scaleX					= 46,
			scaleY					= 47,
			rotation				= 48,
			visible					= 49,
			ZOrder					= 50,
			layoutParameter			= 51,
			//layout parameter
			lpType					= 61,
			lpGravity				= 62,
			lpRelativeName			= 63,
			lpRelativeToName		= 64,
			lpAlign					= 65,
			lpMarginLeft			= 66,
			lpMarginTop				= 67,
			lpMarginRight			= 68,
			lpMarginDown			= 69,
		};
	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static uint32_t readProperties(UIWidget* pWidget, NodeElement* optionsNode, uint32_t attrIndex, BinReader& reader);
	};
}
}

NS_CC_EXT_END
