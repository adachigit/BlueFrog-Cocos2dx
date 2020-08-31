#pragma once

#include "rapidjson/document.h"
#include "BaseClasses/UIWidget.h"

#include "../BinWriter.h"
#include "../BinReader.h"

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	class RootNodeConvertor
	{
		enum ATTRIBUTE_TYPE : uint32_t		// 1 - 10
		{
			version				= 1,
			textures			= 2,
			designWidth			= 3,
			disignHeight		= 4,
			widgetTree			= 5,
		};

	public:
		static void writeProperties(const rapidjson::Value& data, uint32_t node, BinWriter& writer);
		static UIWidget* readProperties(NodeElement* pNode, BinReader& reader);
		static const char* getVersion(NodeElement* pNode, BinReader& reader);
	};
}
}
NS_CC_EXT_END