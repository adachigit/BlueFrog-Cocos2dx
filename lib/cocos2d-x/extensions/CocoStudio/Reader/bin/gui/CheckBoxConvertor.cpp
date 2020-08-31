#include "CheckBoxConvertor.h"

#include "DictionaryHelper.h"
#include "BasePropsConvertor.h"
#include "ColorPropsConvertor.h"
#include "GUIBinConvertor.h"

USING_NS_CC_EXT;

NS_CC_EXT_BEGIN;

namespace ccsbin
{
namespace gui
{
	void CheckBoxConvertor::writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer)
	{
		BasePropsConvertor::writeProperties(options, node, writer);

		const rapidjson::Value& backGroundDic = DICTOOL->getSubDictionary_json(options, "backGroundBoxData");
		int backGroundType = DICTOOL->getIntValue_json(backGroundDic, "resourceType");
		uint32_t backGroundBoxDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::backGroundBoxData, backGroundBoxDataNode));
		writer.addNodeAttribute(backGroundBoxDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, backGroundType));
		switch (backGroundType)
		{
		case 0:
		case 1:
		{
			const char* backGroundFileName = DICTOOL->getStringValue_json(backGroundDic, "path");
			writer.addNodeAttribute(backGroundBoxDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, backGroundFileName ? backGroundFileName : ""));
			break;
		}
		default:
			break;
		}

		const rapidjson::Value& backGroundSelectedDic = DICTOOL->getSubDictionary_json(options, "backGroundBoxSelectedData");
		int backGroundSelectedType = DICTOOL->getIntValue_json(backGroundSelectedDic, "resourceType");
		uint32_t backGroundBoxSelectedDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::backGroundBoxSelectedData, backGroundBoxSelectedDataNode));
		writer.addNodeAttribute(backGroundBoxSelectedDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, backGroundSelectedType));
		switch (backGroundSelectedType)
		{
		case 0:
		case 1:
		{
			const char* backGroundSelectedFileName = DICTOOL->getStringValue_json(backGroundSelectedDic, "path");
			writer.addNodeAttribute(backGroundBoxSelectedDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, backGroundSelectedFileName ? backGroundSelectedFileName : ""));
			break;
		}
		default:
			break;
		}

		const rapidjson::Value& frontCrossDic = DICTOOL->getSubDictionary_json(options, "frontCrossData");
		int frontCrossType = DICTOOL->getIntValue_json(frontCrossDic, "resourceType");
		uint32_t frontCrossDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::frontCrossData, frontCrossDataNode));
		writer.addNodeAttribute(frontCrossDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, frontCrossType));
		switch (frontCrossType)
		{
		case 0:
		case 1:
		{
			const char* frontCrossFileName = DICTOOL->getStringValue_json(frontCrossDic, "path");
			writer.addNodeAttribute(frontCrossDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, frontCrossFileName ? frontCrossFileName : ""));
			break;
		}
		default:
			break;
		}

		const rapidjson::Value& backGroundDisabledDic = DICTOOL->getSubDictionary_json(options, "backGroundBoxDisabledData");
		int backGroundDisabledType = DICTOOL->getIntValue_json(backGroundDisabledDic, "resourceType");
		uint32_t backGroundBoxDisabledDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::backGroundBoxDisabledData, backGroundBoxDisabledDataNode));
		writer.addNodeAttribute(backGroundBoxDisabledDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, backGroundDisabledType));
		switch (backGroundDisabledType)
		{
		case 0:
		case 1:
		{
			const char* backGroundDisabledFileName = DICTOOL->getStringValue_json(backGroundDisabledDic, "path");
			writer.addNodeAttribute(backGroundBoxDisabledDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, backGroundDisabledFileName ? backGroundDisabledFileName : ""));
			break;
		}
		default:
			break;
		}

		const rapidjson::Value& frontCrossDisabledDic = DICTOOL->getSubDictionary_json(options, "frontCrossDisabledData");
		int frontCrossDisabledType = DICTOOL->getIntValue_json(frontCrossDisabledDic, "resourceType");
		uint32_t frontCrossDisabledDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::frontCrossDisabledData, frontCrossDisabledDataNode));
		writer.addNodeAttribute(frontCrossDisabledDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, frontCrossDisabledType));
		switch (frontCrossDisabledType)
		{
		case 0:
		case 1:
		{
			const char* frontCrossDisabledFileName = DICTOOL->getStringValue_json(options, "path");
			writer.addNodeAttribute(frontCrossDisabledDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, frontCrossDisabledFileName ? frontCrossDisabledFileName : ""));
			break;
		}
		default:
			break;
		}

		ColorPropsConvertor::writeProperties(options, node, writer, WidgetType::WidgetTypeWidget);
	}

	void CheckBoxConvertor::readProperties(UICheckBox* pWidget, NodeElement* optionsNode, BinReader& reader)
	{
		uint32_t attrIndex = BasePropsConvertor::readProperties(pWidget, optionsNode, 0, reader);

//		cocos2d::TimeElapse* te = new cocos2d::TimeElapse("CheckBox Set Properties in readProperties");

		AttrElement* attributes = (AttrElement*)reader.getArray(optionsNode->attrArray, NULL);

		NodeElement* backGroundDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* backGroundDataAttrs = (AttrElement*)reader.getArray(backGroundDataNode->attrArray, NULL);
		int backGroundType = backGroundDataAttrs[0].attrIValue;
		switch (backGroundType)
		{
			case 0:
			{
				std::string tp_b = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* backGroundFileName = reader.getString(backGroundDataAttrs[1].attrUIValue);
//				const char* backGroundFileName_tp = (backGroundFileName && (strcmp(backGroundFileName, "") != 0)) ? tp_b.append(backGroundFileName).c_str() : NULL;
//				pWidget->loadTextureBackGround(backGroundFileName_tp);
				pWidget->loadTextureBackGround(backGroundFileName);
				break;
			}
			case 1:
			{
				const char* backGroundFileName = reader.getString(backGroundDataAttrs[1].attrUIValue);
				pWidget->loadTextureBackGround(backGroundFileName, UI_TEX_TYPE_PLIST);
				break;
			}
			default:
				break;
		}

		NodeElement* backGroundSelectedDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* backGroundSelectedAttrs = (AttrElement*)reader.getArray(backGroundSelectedDataNode->attrArray, NULL);
		int backGroundSelectedType = backGroundSelectedAttrs[0].attrIValue;
		switch (backGroundSelectedType)
		{
			case 0:
			{
				std::string tp_b = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* backGroundSelectedFileName = reader.getString(backGroundSelectedAttrs[1].attrUIValue);
//				const char* backGroundSelectedFileName_tp = (backGroundSelectedFileName && (strcmp(backGroundSelectedFileName, "") != 0)) ? tp_b.append(backGroundSelectedFileName).c_str() : NULL;
//				pWidget->loadTextureBackGroundSelected(backGroundSelectedFileName_tp);
				pWidget->loadTextureBackGroundSelected(backGroundSelectedFileName);
				break;
			}
			case 1:
			{
				const char* backGroundSelectedFileName = reader.getString(backGroundSelectedAttrs[1].attrUIValue);
				pWidget->loadTextureBackGroundSelected(backGroundSelectedFileName, UI_TEX_TYPE_PLIST);
				break;
			}
			default:
				break;
		}

		NodeElement* frontCrossDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* frontCrossDataAttrs = (AttrElement*)reader.getArray(frontCrossDataNode->attrArray, NULL);
		int frontCrossType = frontCrossDataAttrs[0].attrIValue;
		switch (frontCrossType)
		{
			case 0:
			{
				std::string tp_c = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* frontCrossFileName = reader.getString(frontCrossDataAttrs[1].attrUIValue);
//				const char* frontCrossFileName_tp = (frontCrossFileName && (strcmp(frontCrossFileName, "") != 0)) ? tp_c.append(frontCrossFileName).c_str() : NULL;
//				pWidget->loadTextureFrontCross(frontCrossFileName_tp);
				pWidget->loadTextureFrontCross(frontCrossFileName);
				break;
			}
			case 1:
			{
				const char* frontCrossFileName = reader.getString(frontCrossDataAttrs[1].attrUIValue);
				pWidget->loadTextureFrontCross(frontCrossFileName, UI_TEX_TYPE_PLIST);
				break;
			}
			default:
				break;
		}

		NodeElement* backGroundDisabledDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* backGroundDisabledAttrs = (AttrElement*)reader.getArray(backGroundDisabledDataNode->attrArray, NULL);
		int backGroundDisabledType = backGroundDisabledAttrs[0].attrIValue;
		switch (backGroundDisabledType)
		{
			case 0:
			{
				std::string tp_bd = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* backGroundDisabledFileName = reader.getString(backGroundDisabledAttrs[1].attrUIValue);
//				const char* backGroundDisabledFileName_tp = (backGroundDisabledFileName && (strcmp(backGroundDisabledFileName, "") != 0)) ? tp_bd.append(backGroundDisabledFileName).c_str() : NULL;
//				pWidget->loadTextureBackGroundDisabled(backGroundDisabledFileName_tp);
				pWidget->loadTextureBackGroundDisabled(backGroundDisabledFileName);
				break;
			}
			case 1:
			{
				const char* backGroundDisabledFileName = reader.getString(backGroundDisabledAttrs[1].attrUIValue);
				pWidget->loadTextureBackGroundDisabled(backGroundDisabledFileName, UI_TEX_TYPE_PLIST);
				break;
			}
			default:
				break;
		}

		NodeElement* frontCrossDisabledDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* frontCrossDisabledAttrs = (AttrElement*)reader.getArray(frontCrossDisabledDataNode->attrArray, NULL);
		int frontCrossDisabledType = frontCrossDisabledAttrs[0].attrIValue;
		switch (frontCrossDisabledType)
		{
			case 0:
			{
				std::string tp_cd = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* frontCrossDisabledFileName = reader.getString(frontCrossDisabledAttrs[1].attrUIValue);
//				const char* frontCrossDisabledFileName_tp = (frontCrossDisabledFileName && (strcmp(frontCrossDisabledFileName, "") != 0)) ? tp_cd.append(frontCrossDisabledFileName).c_str() : NULL;
//				pWidget->loadTextureFrontCrossDisabled(frontCrossDisabledFileName_tp);
				pWidget->loadTextureFrontCrossDisabled(frontCrossDisabledFileName);
				break;
			}
			case 1:
			{
				const char* frontCrossDisabledFileName = reader.getString(frontCrossDisabledAttrs[1].attrUIValue);
				pWidget->loadTextureFrontCrossDisabled(frontCrossDisabledFileName, UI_TEX_TYPE_PLIST);
				break;
			}
			default:
				break;
		}

//		delete te;

		ColorPropsConvertor::readProperties(pWidget, optionsNode, attrIndex, reader);
	}
}
}

NS_CC_EXT_END;