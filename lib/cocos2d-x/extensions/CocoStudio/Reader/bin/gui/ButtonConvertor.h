#pragma once

#include "rapidjson/document.h"
#include "BaseClasses/UIWidget.h"
#include "System/CocosGUI.h"

#include "../BinWriter.h"
#include "../BinReader.h"

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	class ButtonConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 201 - 300
		{
			scale9Enable		= 201,
			normalData			= 202,
			pressedData			= 203,
			disabledData		= 204,
			dataResourceType	= 205,
			dataResourcePath	= 206,
			capInsetsX			= 207,
			capInsetsY			= 208,
			capInsetsWidth		= 209,
			capInsetsHeight		= 210,
			scale9Width			= 211,
			scale9Height		= 212,
			text				= 213,
			textColorR			= 214,
			textColorG			= 215,
			textColorB			= 216,
			fontSize			= 217,
			fontName			= 218,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static void readProperties(UIButton* pWidget, NodeElement* optionsNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END