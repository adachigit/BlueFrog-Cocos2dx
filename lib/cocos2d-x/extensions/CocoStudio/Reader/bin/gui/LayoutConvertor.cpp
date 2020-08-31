#include "LayoutConvertor.h"

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
	void LayoutConvertor::writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer)
	{
		BasePropsConvertor::writeProperties(options, node, writer);

		bool clipAble = DICTOOL->checkObjectExist_json(options, "clipAble") ? DICTOOL->getBooleanValue_json(options, "clipAble") : false;
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::clipAble, (uint32_t)clipAble));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::backGroundScale9Enable, (uint32_t)DICTOOL->getBooleanValue_json(options, "backGroundScale9Enable")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::bgColorR, DICTOOL->getIntValue_json(options, "bgColorR")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::bgColorG, DICTOOL->getIntValue_json(options, "bgColorG")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::bgColorB, DICTOOL->getIntValue_json(options, "bgColorB")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::bgStartColorR, DICTOOL->getIntValue_json(options, "bgStartColorR")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::bgStartColorG, DICTOOL->getIntValue_json(options, "bgStartColorG")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::bgStartColorB, DICTOOL->getIntValue_json(options, "bgStartColorB")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::bgEndColorR, DICTOOL->getIntValue_json(options, "bgEndColorR")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::bgEndColorG, DICTOOL->getIntValue_json(options, "bgEndColorG")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::bgEndColorB, DICTOOL->getIntValue_json(options, "bgEndColorB")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::vectorX, DICTOOL->getFloatValue_json(options, "vectorX")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::vectorY, DICTOOL->getFloatValue_json(options, "vectorY")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::bgColorOpacity, DICTOOL->getIntValue_json(options, "bgColorOpacity")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::colorType, DICTOOL->getIntValue_json(options, "colorType")));

		const rapidjson::Value& imageFileNameDic = DICTOOL->getSubDictionary_json(options, "backGroundImageData");
		int imageFileNameType = DICTOOL->getIntValue_json(imageFileNameDic, "resourceType");
		uint32_t backGroundImageDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::backGroundImageData, backGroundImageDataNode));
		writer.addNodeAttribute(backGroundImageDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, imageFileNameType));
		switch (imageFileNameType)
		{
		case 0:
		case 1:
		{
			const char* imageFileName = DICTOOL->getStringValue_json(imageFileNameDic, "path");
			writer.addNodeAttribute(backGroundImageDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, imageFileName ? imageFileName : ""));
			break;
		}
		default:
			break;
		}

		if (backGroundScale9Enable)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsX, DICTOOL->getFloatValue_json(options, "capInsetsX")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsY, DICTOOL->getFloatValue_json(options, "capInsetsY")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsWidth, DICTOOL->getFloatValue_json(options, "capInsetsWidth")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsHeight, DICTOOL->getFloatValue_json(options, "capInsetsHeight")));
		}

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::layoutType, DICTOOL->getIntValue_json(options, "layoutType")));

		ColorPropsConvertor::writeProperties(options, node, writer, WidgetType::WidgetTypeContainer);
	}

	void LayoutConvertor::readProperties(Layout* pWidget, NodeElement* optionsNode, BinReader& reader)
	{
		uint32_t attrIndex = BasePropsConvertor::readProperties(pWidget, optionsNode, 0, reader);

//		cocos2d::TimeElapse* te = new cocos2d::TimeElapse("Layout Set Properties in readProperties");

		AttrElement* attributes = (AttrElement*)reader.getArray(optionsNode->attrArray, NULL);

		pWidget->setClippingEnable((bool)attributes[attrIndex++].attrIValue);
		pWidget->setBackGroundImageScale9Enabled((bool)attributes[attrIndex++].attrIValue);

		int cr = attributes[attrIndex++].attrIValue;
		int cg = attributes[attrIndex++].attrIValue;
		int cb = attributes[attrIndex++].attrIValue;;

		int scr = attributes[attrIndex++].attrIValue;
		int scg = attributes[attrIndex++].attrIValue;
		int scb = attributes[attrIndex++].attrIValue;

		int ecr = attributes[attrIndex++].attrIValue;
		int ecg = attributes[attrIndex++].attrIValue;
		int ecb = attributes[attrIndex++].attrIValue;

		float bgcv1 = attributes[attrIndex++].attrFValue;
		float bgcv2 = attributes[attrIndex++].attrFValue;
		pWidget->setBackGroundColorVector(ccp(bgcv1, bgcv2));

		int co = attributes[attrIndex++].attrIValue;

		int colorType = attributes[attrIndex++].attrIValue;
		pWidget->setBackGroundColorType(PanelColorType(colorType));
		pWidget->setBackGroundColor(ccc3(scr, scg, scb), ccc3(ecr, ecg, ecb));
		pWidget->setBackGroundColor(ccc3(cr, cg, cb));
		pWidget->setBackGroundColorOpacity(co);

		NodeElement* backGroundImageDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* backGroundImageAttrs = (AttrElement*)reader.getArray(backGroundImageDataNode->attrArray, NULL);
		int imageFileNameType = backGroundImageAttrs[0].attrIValue;
		switch (imageFileNameType)
		{
		case 0:
		{
			std::string tp_b = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
			const char* imageFileName = reader.getString(backGroundImageAttrs[1].attrUIValue);
//			const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0)) ? tp_b.append(imageFileName).c_str() : NULL;
//			pWidget->setBackGroundImage(imageFileName_tp);
			pWidget->setBackGroundImage(imageFileName);
			break;
		}
		case 1:
		{
			const char* imageFileName = reader.getString(backGroundImageAttrs[1].attrUIValue);
			pWidget->setBackGroundImage(imageFileName, UI_TEX_TYPE_PLIST);
			break;
		}
		default:
			break;
		}

		if (backGroundScale9Enable)
		{
			float cx = attributes[attrIndex++].attrFValue;
			float cy = attributes[attrIndex++].attrFValue;
			float cw = attributes[attrIndex++].attrFValue;
			float ch = attributes[attrIndex++].attrFValue;
			pWidget->setBackGroundImageCapInsets(CCRectMake(cx, cy, cw, ch));
		}
		pWidget->setLayoutType((LayoutType)attributes[attrIndex++].attrIValue);

//		delete te;

		ColorPropsConvertor::readProperties(pWidget, optionsNode, attrIndex, reader);
	}
}
}

NS_CC_EXT_END