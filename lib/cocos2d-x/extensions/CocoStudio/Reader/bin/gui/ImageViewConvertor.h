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
	class ImageViewConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 401 - 500
		{
			fileNameData			= 401,
			dataResourceType		= 402,
			dataResourcePath		= 403,
			scale9Enable			= 404,
			scale9Width				= 405,
			scale9Height			= 406,
			capInsetsX				= 407,
			capInsetsY				= 408,
			capInsetsWidth			= 409,
			capInsetsHeight			= 410,
		};

	public:
		static void writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer);
		static void readProperties(UIImageView* pWidget, NodeElement* optionsNode, BinReader& reader);
	};
}
}

NS_CC_EXT_END