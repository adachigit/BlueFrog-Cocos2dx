#include "LoadingBarConvertor.h"

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
	void LoadingBarConvertor::writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer)
	{
		BasePropsConvertor::writeProperties(options, node, writer);

		const rapidjson::Value& imageFileNameDic = DICTOOL->getSubDictionary_json(options, "textureData");
		int imageFileNameType = DICTOOL->getIntValue_json(imageFileNameDic, "resourceType");
		uint32_t textureDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::textureData, textureDataNode));
		writer.addNodeAttribute(textureDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, imageFileNameType));
		switch (imageFileNameType)
		{
		case 0:
		case 1:
		{
			const char* imageFileName = DICTOOL->getStringValue_json(imageFileNameDic, "path");
			writer.addNodeAttribute(textureDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, imageFileName));
			break;
		}
		default:
			break;
		}

		/* gui mark add load bar scale9 parse */
		bool scale9Enable = DICTOOL->getBooleanValue_json(options, "scale9Enable");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::scale9Enable, (uint32_t)scale9Enable));

		if (scale9Enable)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsX, DICTOOL->getFloatValue_json(options, "capInsetsX")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsY, DICTOOL->getFloatValue_json(options, "capInsetsY")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsWidth, DICTOOL->getFloatValue_json(options, "capInsetsWidth")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsHeight, DICTOOL->getFloatValue_json(options, "capInsetsHeight")));

			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::width, DICTOOL->getFloatValue_json(options, "width")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::height, DICTOOL->getFloatValue_json(options, "height")));
		}
		/**/

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::direction, DICTOOL->getIntValue_json(options, "direction")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::percent, DICTOOL->getIntValue_json(options, "percent")));

		ColorPropsConvertor::writeProperties(options, node, writer, WidgetType::WidgetTypeWidget);
	}

	void LoadingBarConvertor::readProperties(UILoadingBar* pWidget, NodeElement* optionsNode, BinReader& reader)
	{
		uint32_t attrIndex = BasePropsConvertor::readProperties(pWidget, optionsNode, 0, reader);

		AttrElement* attributes = (AttrElement*)reader.getArray(optionsNode->attrArray, NULL);

		NodeElement* textureDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* textureDataAttrs = (AttrElement*)reader.getArray(textureDataNode->attrArray, NULL);
		int imageFileNameType = textureDataAttrs[0].attrIValue;
		switch (imageFileNameType)
		{
		case 0:
		{
			std::string tp_i = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
			const char* imageFileName = reader.getString(textureDataAttrs[1].attrUIValue);
			const char* imageFileName_tp = NULL;
			if (imageFileName && (strcmp(imageFileName, "") != 0))
			{
//				imageFileName_tp = tp_i.append(imageFileName).c_str();
//				pWidget->loadTexture(imageFileName_tp);
				pWidget->loadTexture(imageFileName);
			}
			break;
		}
		case 1:
		{
			const char* imageFileName = reader.getString(textureDataAttrs[1].attrUIValue);
			pWidget->loadTexture(imageFileName, UI_TEX_TYPE_PLIST);
			break;
		}
		default:
			break;
		}

		/* gui mark add load bar scale9 parse */
		bool scale9Enable = (bool)attributes[attrIndex++].attrIValue;
		pWidget->setScale9Enabled(scale9Enable);

		if (scale9Enable)
		{
			float cx = attributes[attrIndex++].attrFValue;
			float cy = attributes[attrIndex++].attrFValue;
			float cw = attributes[attrIndex++].attrFValue;
			float ch = attributes[attrIndex++].attrFValue;

			pWidget->setCapInsets(CCRectMake(cx, cy, cw, ch));

			float width = attributes[attrIndex++].attrFValue;
			float height = attributes[attrIndex++].attrFValue;
			pWidget->setSize(CCSizeMake(width, height));
		}

		pWidget->setDirection((LoadingBarType)attributes[attrIndex++].attrIValue);
		pWidget->setPercent(attributes[attrIndex++].attrIValue);

		ColorPropsConvertor::readProperties(pWidget, optionsNode, attrIndex, reader);
	}
}
}

NS_CC_EXT_END