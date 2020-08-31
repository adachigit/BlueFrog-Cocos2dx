#include "RootNodeConvertor.h"

#include "DictionaryHelper.h"
#include "WidgetConvertor.h"
#include "GUIBinConvertor.h"

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	void RootNodeConvertor::writeProperties(const rapidjson::Value& data, uint32_t node, BinWriter& writer)
	{
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::version, DICTOOL->getStringValue_json(data, "version")));

		int texturesCount = DICTOOL->getArrayCount_json(data, "textures");
		char** textures = new char*[texturesCount];
		for (int i = 0; i < texturesCount; i++)
		{
			textures[i] = const_cast<char*>(DICTOOL->getStringValueFromArray_json(data, "textures", i));
		}
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::textures, writer.newArray(textures, texturesCount)));
		CC_SAFE_DELETE_ARRAY(textures);

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::designWidth, DICTOOL->getFloatValue_json(data, "designWidth")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::disignHeight, DICTOOL->getFloatValue_json(data, "designHeight")));

		const rapidjson::Value& widgetTree = DICTOOL->getSubDictionary_json(data, "widgetTree");
		uint32_t widgetTreeNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::widgetTree, widgetTreeNode));
		WidgetConvertor::writeProperties(widgetTree, widgetTreeNode, writer);
	}

	UIWidget* RootNodeConvertor::readProperties(NodeElement* pNode, BinReader& reader)
	{
		AttrElement* attributes = (AttrElement*)reader.getArray(pNode->attrArray, NULL);

		uint32_t attrIndex = 0;
		++attrIndex;		// jump version attr
		
		int32_t textureCount = 0;
		uint32_t* textures = (uint32_t*)reader.getArray(attributes[attrIndex++].attrUIValue, &textureCount);
		for (int32_t i = 0; i < textureCount; ++i)
		{
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(reader.getString(textures[i]));
		}

		float fileDesignWidth = attributes[attrIndex++].attrFValue;
		float fileDesignHeight = attributes[attrIndex++].attrFValue;
		if (fileDesignWidth <= 0 || fileDesignHeight <= 0) {
			printf("Read design size error!\n");
			CCSize winSize = CCDirector::sharedDirector()->getWinSize();
			GUIReader::shareReader()->storeFileDesignSize(reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileName), winSize);
		}
		else
		{
			GUIReader::shareReader()->storeFileDesignSize(reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileName), CCSizeMake(fileDesignWidth, fileDesignHeight));
		}

		NodeElement* widgetTreeNode = reader.getNode(attributes[attrIndex++].attrUIValue);

//		cocos2d::TimeElapse te("create widget from WidgetConvertor file");
		UIWidget* pWidget = WidgetConvertor::readProperties(widgetTreeNode, reader);

		if (pWidget->getContentSize().equals(CCSizeZero))
		{
			UIContainerWidget* rootWidget = dynamic_cast<UIContainerWidget*>(pWidget);
			rootWidget->setSize(CCSizeMake(fileDesignWidth, fileDesignHeight));
		}

		return pWidget;
	}

	const char* RootNodeConvertor::getVersion(NodeElement* pNode, BinReader& reader)
	{
		AttrElement* elements = (AttrElement*)reader.getArray(pNode->attrArray, NULL);
		return reader.getString(elements[0].attrUIValue);
	}
}
}

NS_CC_EXT_END