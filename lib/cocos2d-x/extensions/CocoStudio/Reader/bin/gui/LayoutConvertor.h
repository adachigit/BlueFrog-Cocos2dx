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
	class LayoutConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 701 - 800
		{
			clipAble					= 701,
			backGroundScale9Enable		= 702,
			bgColorR					= 703,
			bgColorG					= 704,
			bgColorB					= 705,
			bgStartColorR				= 706,
			bgStartColorG				= 707,
			bgStartColorB				= 708,
			bgEndColorR					= 709,
			bgEndColorG					= 710,
			bgEndColorB					= 711,
			vectorX						= 712,
			vectorY						= 713,
			bgColorOpacity				= 714,
			colorType					= 715,
			backGroundImageData			= 716,
			dataResourceType			= 717,
			dataResourcePath			= 718,
			capInsetsX					= 719,
			capInsetsY					= 720,
			capInsetsWidth				= 721,
			capInsetsHeight				= 722,
			layoutType					= 723,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static void readProperties(Layout* pWidget, NodeElement* optionsNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END