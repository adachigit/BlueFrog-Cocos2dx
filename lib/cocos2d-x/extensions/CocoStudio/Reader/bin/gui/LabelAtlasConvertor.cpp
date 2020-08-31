#include "LabelAtlasConvertor.h"

#include "DictionaryHelper.h"
#include "BasePropsConvertor.h"
#include "ColorPropsConvertor.h"
#include "GUIBinConvertor.h"

USING_NS_CC_EXT;

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	void LabelAtlasConvertor::writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer)
	{
		BasePropsConvertor::writeProperties(options, node, writer);

		bool sv = DICTOOL->checkObjectExist_json(options, "stringValue");
		bool cmf = DICTOOL->checkObjectExist_json(options, "charMapFile");
		bool iw = DICTOOL->checkObjectExist_json(options, "itemWidth");
		bool ih = DICTOOL->checkObjectExist_json(options, "itemHeight");
		bool scm = DICTOOL->checkObjectExist_json(options, "startCharMap");
		if (sv && cmf && iw && ih && scm)
		{
			const rapidjson::Value& cmftDic = DICTOOL->getSubDictionary_json(options, "charMapFileData");
			int cmfType = DICTOOL->getIntValue_json(cmftDic, "resourceType");
			uint32_t charMapFileDataNode = writer.newNode();
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::charMapFileData, charMapFileDataNode));
			writer.addNodeAttribute(charMapFileDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, cmfType));
			switch (cmfType)
			{
			case 0:
			{
				const char* cmfPath = DICTOOL->getStringValue_json(cmftDic, "path");
				writer.addNodeAttribute(charMapFileDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, cmfPath));
				const char* stringValue = DICTOOL->getStringValue_json(options, "stringValue");
				writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::stringValue, writer.addString(stringValue ? stringValue : "")));
				const char* startCharMap = DICTOOL->getStringValue_json(options, "startCharMap");
				writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::startCharMap, writer.addString(startCharMap ? startCharMap : "")));
				writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::itemWidth, DICTOOL->getIntValue_json(options, "itemWidth")));
				writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::itemHeight, DICTOOL->getIntValue_json(options, "itemHeight")));
				break;
			}
			case 1:
				printf("Wrong res type of LabelAtlas!");
				break;
			default:
				break;
			}
		}

		ColorPropsConvertor::writeProperties(options, node, writer, WidgetType::WidgetTypeWidget);
	}

	void LabelAtlasConvertor::readProperties(UILabelAtlas* pWidget, NodeElement* optionsNode, BinReader& reader)
	{
		uint32_t attrIndex = BasePropsConvertor::readProperties(pWidget, optionsNode, 0, reader);

//		cocos2d::TimeElapse* te = new cocos2d::TimeElapse("LabelAtlas Set Properties in readProperties");

		AttrElement* attributes = (AttrElement*)reader.getArray(optionsNode->attrArray, NULL);

		if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::charMapFileData)
		{
			NodeElement* charMapFileDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
			AttrElement* charMapFileAttrs = (AttrElement*)reader.getArray(charMapFileDataNode->attrArray, NULL);
			int cmfType = charMapFileAttrs[0].attrIValue;
			switch (cmfType)
			{
				case 0:
				{
					std::string tp_c = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
					const char* cmfPath = reader.getString(charMapFileAttrs[1].attrUIValue);
					const char* cmf_tp = tp_c.append(cmfPath).c_str();
					const char* stringValue = reader.getString(attributes[attrIndex++].attrUIValue);
					const char* startCharMap = reader.getString(attributes[attrIndex++].attrUIValue);
					int itemWidth = attributes[attrIndex++].attrIValue;
					int itemHeight = attributes[attrIndex++].attrIValue;
//					pWidget->setProperty(stringValue, cmf_tp, itemWidth, itemHeight, startCharMap);
					pWidget->setProperty(stringValue, cmfPath, itemWidth, itemHeight, startCharMap);
					break;
				}
				default:
					break;
			}
		}

//		delete te;

		ColorPropsConvertor::readProperties(pWidget, optionsNode, attrIndex, reader);
	}
}
}

NS_CC_EXT_END