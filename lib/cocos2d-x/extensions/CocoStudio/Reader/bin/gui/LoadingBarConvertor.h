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
	class LoadingBarConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 1101 - 1200
		{
			textureData				= 1101,
			dataResourceType		= 1102,
			dataResourcePath		= 1103,
			scale9Enable			= 1104,
			capInsetsX				= 1105,
			capInsetsY				= 1106,
			capInsetsWidth			= 1107,
			capInsetsHeight			= 1108,
			width					= 1109,
			height					= 1110,
			direction				= 1111,
			percent					= 1112,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static void readProperties(UILoadingBar* pWidget, NodeElement* optionsNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END