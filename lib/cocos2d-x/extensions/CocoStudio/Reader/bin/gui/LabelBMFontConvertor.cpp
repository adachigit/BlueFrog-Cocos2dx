#include "LabelBMFontConvertor.h"

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
	void LabelBMFontConvetor::writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer)
	{
		BasePropsConvertor::writeProperties(options, node, writer);

		const rapidjson::Value& cmftDic = DICTOOL->getSubDictionary_json(options, "fileNameData");
		int cmfType = DICTOOL->getIntValue_json(cmftDic, "resourceType");
		uint32_t fileNameDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::fileNameData, fileNameDataNode));
		writer.addNodeAttribute(fileNameDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, cmfType));
		switch (cmfType)
		{
		case 0:
		{
			const char* cmfPath = DICTOOL->getStringValue_json(cmftDic, "path");
			writer.addNodeAttribute(fileNameDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, cmfPath ? cmfPath : ""));
			break;
		}
		case 1:
			CCLOG("Wrong res type of LabelAtlas!");
			break;
		default:
			break;
		}

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::text, DICTOOL->getStringValue_json(options, "text")));

		ColorPropsConvertor::writeProperties(options, node, writer, WidgetType::WidgetTypeWidget);
	}

	void LabelBMFontConvetor::readProperties(UILabelBMFont* pWidget, NodeElement* optionsNode, BinReader& reader)
	{
		uint32_t attrIndex = BasePropsConvertor::readProperties(pWidget, optionsNode, 0, reader);

//		cocos2d::TimeElapse* te = new cocos2d::TimeElapse("LabelBMFont Set Properties in readProperties");

		AttrElement* attributes = (AttrElement*)reader.getArray(optionsNode->attrArray, NULL);

		NodeElement* fileNameDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* fileNameDataAttrs = (AttrElement*)reader.getArray(fileNameDataNode->attrArray, NULL);
		int cmfType = fileNameDataAttrs[0].attrIValue;
		switch (cmfType)
		{
			case 0:
			{
				std::string tp_c = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* cmfPath = reader.getString(fileNameDataAttrs[1].attrUIValue);
//				const char* cmf_tp = tp_c.append(cmfPath).c_str();
//				pWidget->setFntFile(cmf_tp);
				pWidget->setFntFile(cmfPath);
				break;
			}
			default:
				break;
		}

		pWidget->setText(reader.getString(attributes[attrIndex++].attrUIValue));

//		delete te;

		ColorPropsConvertor::readProperties(pWidget, optionsNode, attrIndex, reader);
	}
}
}

NS_CC_EXT_END