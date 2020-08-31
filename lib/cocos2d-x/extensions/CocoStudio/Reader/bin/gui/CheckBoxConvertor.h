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
	class CheckBoxConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 301 - 400
		{
			backGroundBoxData			= 301,
			backGroundBoxSelectedData	= 302,
			frontCrossData				= 303,
			backGroundBoxDisabledData	= 304,
			frontCrossDisabledData		= 305,
			dataResourceType			= 306,
			dataResourcePath			= 307,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static void readProperties(UICheckBox* pWidget, NodeElement* optionsNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END