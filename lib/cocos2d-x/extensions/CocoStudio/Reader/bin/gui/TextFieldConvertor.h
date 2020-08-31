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
	class TextFieldConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 1001 - 1100
		{
			placeHolder					= 1001,
			text						= 1002,
			fontSize					= 1003,
			fontName					= 1004,
			touchSizeWidth				= 1005,
			touchSizeHeight				= 1006,
			width						= 1007,
			height						= 1008,
			maxLengthEnable				= 1009,
			maxLength					= 1010,
			passwordEnable				= 1011,
			passwordStyleText			= 1012,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static void readProperties(UITextField* pWidget, NodeElement* optionsNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END