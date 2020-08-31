#include "ButtonConvertor.h"

#include <string>

#include "DictionaryHelper.h"
#include "GUIBinConvertor.h"
#include "BasePropsConvertor.h"
#include "ColorPropsConvertor.h"

USING_NS_CC_EXT;

NS_CC_EXT_BEGIN;

namespace ccsbin
{
namespace gui
{
	void ButtonConvertor::writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer)
	{
		BasePropsConvertor::writeProperties(options, node, writer);

		bool scale9Enabled = DICTOOL->getBooleanValue_json(options, "scale9Enable");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::scale9Enable, (uint32_t)scale9Enabled));

		uint32_t normalDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::normalData, normalDataNode));
		const rapidjson::Value& normalDic = DICTOOL->getSubDictionary_json(options, "normalData");
		int normalType = DICTOOL->getIntValue_json(normalDic, "resourceType");
		writer.addNodeAttribute(normalDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, normalType));
		switch (normalType)
		{
		case 0:
		case 1:
		{
			const char* normalFileName = DICTOOL->getStringValue_json(normalDic, "path");
			writer.addNodeAttribute(normalDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, writer.addString(normalFileName ? normalFileName : "")));
			break;
		}
		default:
			break;
		}

		uint32_t pressedDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::pressedData, pressedDataNode));
		const rapidjson::Value& pressedDic = DICTOOL->getSubDictionary_json(options, "pressedData");
		int pressedType = DICTOOL->getIntValue_json(pressedDic, "resourceType");
		writer.addNodeAttribute(pressedDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, pressedType));
		switch (pressedType)
		{
		case 0:
		case 1:
		{
			const char* pressedFileName = DICTOOL->getStringValue_json(pressedDic, "path");
			writer.addNodeAttribute(pressedDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, writer.addString(pressedFileName ? pressedFileName : "")));
			break;
		}
		default:
			break;
		}

		uint32_t disabledDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::disabledData, disabledDataNode));
		const rapidjson::Value& disabledDic = DICTOOL->getSubDictionary_json(options, "disabledData");
		int disabledType = DICTOOL->getIntValue_json(disabledDic, "resourceType");
		writer.addNodeAttribute(disabledDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, disabledType));
		switch (disabledType)
		{
		case 0:
		case 1:
		{
			const char* disabledFileName = DICTOOL->getStringValue_json(disabledDic, "path");
			writer.addNodeAttribute(disabledDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, writer.addString(disabledFileName ? disabledFileName : "")));
			break;
		}
		default:
			break;
		}

		if (scale9Enabled)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsX, DICTOOL->getFloatValue_json(options, "capInsetsX")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsY, DICTOOL->getFloatValue_json(options, "capInsetsY")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsWidth, DICTOOL->getFloatValue_json(options, "capInsetsWidth")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsHeight, DICTOOL->getFloatValue_json(options, "capInsetsHeight")));

			bool sw = DICTOOL->checkObjectExist_json(options, "scale9Width");
			bool sh = DICTOOL->checkObjectExist_json(options, "scale9Height");
			if (sw && sh)
			{
				writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::scale9Width, DICTOOL->getFloatValue_json(options, "scale9Width")));
				writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::scale9Height, DICTOOL->getFloatValue_json(options, "scale9Height")));
			}
		}

		bool tt = DICTOOL->checkObjectExist_json(options, "text");
		if (tt)
		{
			const char* text = DICTOOL->getStringValue_json(options, "text");
			if (text)
			{
				writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::text, text));

			}
		}

		bool cr = DICTOOL->checkObjectExist_json(options, "textColorR");
		bool cg = DICTOOL->checkObjectExist_json(options, "textColorG");
		bool cb = DICTOOL->checkObjectExist_json(options, "textColorB");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::textColorR, cr ? DICTOOL->getIntValue_json(options, "textColorR") : 255));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::textColorG, cg ? DICTOOL->getIntValue_json(options, "textColorG") : 255));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::textColorB, cb ? DICTOOL->getIntValue_json(options, "textColorB") : 255));

		bool fs = DICTOOL->checkObjectExist_json(options, "fontSize");
		if (fs)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::fontSize, DICTOOL->getIntValue_json(options, "fontSize")));
		}
		bool fn = DICTOOL->checkObjectExist_json(options, "fontName");
		if (fn)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::fontName, DICTOOL->getStringValue_json(options, "fontName")));
		}

		ColorPropsConvertor::writeProperties(options, node, writer, WidgetType::WidgetTypeWidget);
	}

	void ButtonConvertor::readProperties(UIButton* pWidget, NodeElement* optionsNode, BinReader& reader)
	{
		uint32_t attrIndex = BasePropsConvertor::readProperties(pWidget, optionsNode, 0, reader);
		
//		cocos2d::TimeElapse* te = new cocos2d::TimeElapse("Button Set Properties in readProperties");

		AttrElement* attributes = (AttrElement*)reader.getArray(optionsNode->attrArray, NULL);
		
		bool scale9Enabled = (bool)attributes[attrIndex++].attrUIValue;
		pWidget->setScale9Enabled(scale9Enabled);

		NodeElement* normalDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* normalDataAttrs = (AttrElement*)reader.getArray(normalDataNode->attrArray, NULL);
		int normalType = normalDataAttrs[0].attrIValue;
		switch (normalType)
		{
			case 0:
			{
				std::string tp_n = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* normalFileName = reader.getString(normalDataAttrs[1].attrUIValue);
//				const char* normalFileName_tp = (normalFileName && (strcmp(normalFileName, "") != 0)) ? tp_n.append(normalFileName).c_str() : NULL;
//				pWidget->loadTextureNormal(normalFileName_tp);
				pWidget->loadTextureNormal(normalFileName);
				break;
			}
			case 1:
			{
				const char* normalFileName = reader.getString(normalDataAttrs[1].attrUIValue);
				pWidget->loadTextureNormal(normalFileName, UI_TEX_TYPE_PLIST);
				break;
			}
			default:
				break;
		}

		NodeElement* pressedDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* pressedDataAttrs = (AttrElement*)reader.getArray(pressedDataNode->attrArray, NULL);
		int pressedType = pressedDataAttrs[0].attrIValue;
		switch (pressedType)
		{
			case 0:
			{
				std::string tp_n = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* pressedFileName = reader.getString(pressedDataAttrs[1].attrUIValue);
//				const char* pressedFileName_tp = (pressedFileName && (strcmp(pressedFileName, "") != 0)) ? tp_n.append(pressedFileName).c_str() : NULL;
//				pWidget->loadTexturePressed(pressedFileName_tp);
				pWidget->loadTexturePressed(pressedFileName);
				break;
			}
			case 1:
			{
				const char* pressedFileName = reader.getString(pressedDataAttrs[1].attrUIValue);
				pWidget->loadTexturePressed(pressedFileName, UI_TEX_TYPE_PLIST);
				break;
			}
			default:
				break;
		}

		NodeElement* disabledDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* disabledDataAttrs = (AttrElement*)reader.getArray(disabledDataNode->attrArray, NULL);
		int disabledType = disabledDataAttrs[0].attrIValue;
		switch (disabledType)
		{
			case 0:
			{
				std::string tp_n = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* disabledFileName = reader.getString(disabledDataAttrs[1].attrUIValue);
//				const char* disabledFileName_tp = (disabledFileName && (strcmp(disabledFileName, "") != 0)) ? tp_n.append(disabledFileName).c_str() : NULL;
//				pWidget->loadTextureDisabled(disabledFileName_tp);
				pWidget->loadTextureDisabled(disabledFileName);
				break;
			}
			case 1:
			{
				const char* disabledFileName = reader.getString(disabledDataAttrs[1].attrUIValue);
				pWidget->loadTextureDisabled(disabledFileName, UI_TEX_TYPE_PLIST);
				break;
			}
			default:
				break;
		}

		if (scale9Enabled)
		{
			float cx = attributes[attrIndex++].attrFValue;;
			float cy = attributes[attrIndex++].attrFValue;;
			float cw = attributes[attrIndex++].attrFValue;
			float ch = attributes[attrIndex++].attrFValue;;
			pWidget->setCapInsets(CCRectMake(cx, cy, cw, ch));

			if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::scale9Width)
			{
				float swf = attributes[attrIndex++].attrFValue;
				float shf = attributes[attrIndex++].attrFValue;
				pWidget->setSize(CCSizeMake(swf, shf));
			}
		}

		if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::text)
		{
			pWidget->setTitleText(reader.getString(attributes[attrIndex++].attrUIValue));
		}

		int cri = attributes[attrIndex++].attrIValue;
		int cgi = attributes[attrIndex++].attrIValue;
		int cbi = attributes[attrIndex++].attrIValue;
		pWidget->setTitleColor(ccc3(cri, cgi, cbi));

		if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::fontSize)
		{
			pWidget->setTitleFontSize(attributes[attrIndex++].attrIValue);
		}
		if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::fontName)
		{
			pWidget->setTitleFontName(reader.getString(attributes[attrIndex++].attrUIValue));
		}

//		delete te;

		ColorPropsConvertor::readProperties(pWidget, optionsNode, attrIndex, reader);
	}
}
}

NS_CC_EXT_END;