#include "SliderConvertor.h"

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
	void SliderConvertor::writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer)
	{
		BasePropsConvertor::writeProperties(options, node, writer);

		bool barTextureScale9Enable = DICTOOL->getBooleanValue_json(options, "barTextureScale9Enable");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::barTextureScale9Enable, (uint32_t)barTextureScale9Enable));
		bool bt = DICTOOL->checkObjectExist_json(options, "barFileName");
		float barLength = DICTOOL->getFloatValue_json(options, "length");
		if (bt)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::barFileName, DICTOOL->getStringValue_json(options, "barFileName")));
			if (barTextureScale9Enable)
			{
				const rapidjson::Value& imageFileNameDic = DICTOOL->getSubDictionary_json(options, "barFileNameData");
				int imageFileType = DICTOOL->getIntValue_json(imageFileNameDic, "resourceType");
				uint32_t barFileNameDataNode = writer.newNode();
				writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::barFileNameData, barFileNameDataNode));
				writer.addNodeAttribute(barFileNameDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, imageFileType));
				switch (imageFileType)
				{
				case 0:
				case 1:
				{
					const char* imageFileName = DICTOOL->getStringValue_json(imageFileNameDic, "path");
					writer.addNodeAttribute(barFileNameDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, imageFileName ? imageFileName : ""));
					break;
				}
				default:
					break;
				}

				writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::length, barLength));
			}
			else
			{
				const rapidjson::Value& imageFileNameDic = DICTOOL->getSubDictionary_json(options, "barFileNameData");
				int imageFileType = DICTOOL->getIntValue_json(imageFileNameDic, "resourceType");
				uint32_t barFileNameDataNode = writer.newNode();
				writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::barFileNameData, barFileNameDataNode));
				writer.addNodeAttribute(barFileNameDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, imageFileType));
				switch (imageFileType)
				{
				case 0:
				case 1:
				{
					const char* imageFileName = DICTOOL->getStringValue_json(imageFileNameDic, "path");
					writer.addNodeAttribute(barFileNameDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, imageFileName ? imageFileName : ""));
					break;
				}
				default:
					break;
				}
			}
		}

		const rapidjson::Value& normalDic = DICTOOL->getSubDictionary_json(options, "ballNormalData");
		int normalType = DICTOOL->getIntValue_json(normalDic, "resourceType");
		uint32_t ballNormalDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::ballNormalData, ballNormalDataNode));
		writer.addNodeAttribute(ballNormalDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, normalType));
		switch (normalType)
		{
		case 0:
		case 1:
		{
			const char* normalFileName = DICTOOL->getStringValue_json(normalDic, "path");
			writer.addNodeAttribute(ballNormalDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, normalFileName ? normalFileName : ""));
			break;
		}
		default:
			break;
		}

		const rapidjson::Value& pressedDic = DICTOOL->getSubDictionary_json(options, "ballPressedData");
		int pressedType = DICTOOL->getIntValue_json(pressedDic, "resourceType");
		uint32_t ballPressedDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::ballPressedData, ballPressedDataNode));
		writer.addNodeAttribute(ballPressedDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, pressedType));
		switch (pressedType)
		{
		case 0:
		case 1:
		{
			const char* pressedFileName = DICTOOL->getStringValue_json(pressedDic, "path");
			writer.addNodeAttribute(ballPressedDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, pressedFileName ? pressedFileName : ""));
			break;
		}
		default:
			break;
		}

		const rapidjson::Value& disabledDic = DICTOOL->getSubDictionary_json(options, "ballDisabledData");
		int disabledType = DICTOOL->getIntValue_json(disabledDic, "resourceType");
		uint32_t ballDisabledDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::ballDisabledData, ballDisabledDataNode));
		writer.addNodeAttribute(ballDisabledDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, disabledType));
		switch (disabledType)
		{
		case 0:
		case 1:
		{
			const char* disabledFileName = DICTOOL->getStringValue_json(disabledDic, "path");
			writer.addNodeAttribute(ballDisabledDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, disabledFileName ? disabledFileName : ""));
			break;
		}
		default:
			break;
		}

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::percent, DICTOOL->getIntValue_json(options, "percent")));

		const rapidjson::Value& progressBarDic = DICTOOL->getSubDictionary_json(options, "progressBarData");
		int progressBarType = DICTOOL->getIntValue_json(progressBarDic, "resourceType");
		uint32_t progressBarDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::progressBarData, progressBarDataNode));
		writer.addNodeAttribute(progressBarDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, progressBarType));
		switch (progressBarType)
		{
		case 0:
		case 1:
		{
			const char* imageFileName = DICTOOL->getStringValue_json(progressBarDic, "path");
			writer.addNodeAttribute(progressBarDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, imageFileName ? imageFileName : ""));
			break;
		}
		default:
			break;
		}

		ColorPropsConvertor::writeProperties(options, node, writer, WidgetType::WidgetTypeWidget);
	}

	void SliderConvertor::readProperties(UISlider* pWidget, NodeElement* optionsNode, BinReader& reader)
	{
		uint32_t attrIndex = BasePropsConvertor::readProperties(pWidget, optionsNode, 0, reader);

		AttrElement* attributes = (AttrElement*)reader.getArray(optionsNode->attrArray, NULL);

		bool barTextureScale9Enable = (bool)attributes[attrIndex++].attrIValue;
		pWidget->setScale9Enabled(barTextureScale9Enable);

		if (attributes[attrIndex++].attrType == ATTRIBUTE_TYPE::barFileName)
		{
			NodeElement* barFileNameDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
			AttrElement* barFileNameAttrs = (AttrElement*)reader.getArray(barFileNameDataNode->attrArray, NULL);
			int imageFileType = barFileNameAttrs[0].attrIValue;
			switch (imageFileType)
			{
				case 0:
				{
					std::string tp_b = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
					const char* imageFileName = reader.getString(barFileNameAttrs[1].attrUIValue);
//					const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0)) ? tp_b.append(imageFileName).c_str() : NULL;
//					pWidget->loadBarTexture(imageFileName_tp);
					pWidget->loadBarTexture(imageFileName);
					break;
				}
				case 1:
				{
					const char* imageFileName = reader.getString(barFileNameAttrs[1].attrUIValue);
					pWidget->loadBarTexture(imageFileName, UI_TEX_TYPE_PLIST);
					break;
				}
				default:
					break;
			}

			if (barTextureScale9Enable)
			{
				pWidget->setSize(CCSizeMake(attributes[attrIndex++].attrFValue, pWidget->getContentSize().height));
			}
		}

		NodeElement* barNormalDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* barNormalDataAttrs = (AttrElement*)reader.getArray(barNormalDataNode->attrArray, NULL);
		int normalType = barNormalDataAttrs[0].attrIValue;
		switch (normalType)
		{
			case 0:
			{
				std::string tp_n = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* normalFileName = reader.getString(barNormalDataAttrs[1].attrUIValue);
//				const char* normalFileName_tp = (normalFileName && (strcmp(normalFileName, "") != 0)) ? tp_n.append(normalFileName).c_str() : NULL;
//				pWidget->loadSlidBallTextureNormal(normalFileName_tp);
				pWidget->loadSlidBallTextureNormal(normalFileName);
				break;
			}
			case 1:
			{
				const char* normalFileName = reader.getString(barNormalDataAttrs[1].attrUIValue);
				pWidget->loadSlidBallTextureNormal(normalFileName, UI_TEX_TYPE_PLIST);
				break;
			}
			default:
				break;
		}

		NodeElement* ballPressedDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* ballPressedDataAttrs = (AttrElement*)reader.getArray(ballPressedDataNode->attrArray, NULL);
		int pressedType = ballPressedDataAttrs[0].attrIValue;
		switch (pressedType)
		{
			case 0:
			{
				std::string tp_p = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* pressedFileName = reader.getString(ballPressedDataAttrs[1].attrUIValue);
//				const char* pressedFileName_tp = (pressedFileName && (strcmp(pressedFileName, "") != 0)) ? tp_p.append(pressedFileName).c_str() : NULL;
//				pWidget->loadSlidBallTexturePressed(pressedFileName_tp);
				pWidget->loadSlidBallTexturePressed(pressedFileName);
				break;
			}
			case 1:
			{
				const char* pressedFileName = reader.getString(ballPressedDataAttrs[1].attrUIValue);
				pWidget->loadSlidBallTexturePressed(pressedFileName, UI_TEX_TYPE_PLIST);
				break;
			}
			default:
				break;
		}

		NodeElement* ballDisabledDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* ballDisabledDataAttrs = (AttrElement*)reader.getArray(ballDisabledDataNode->attrArray, NULL);
		int disabledType = ballDisabledDataAttrs[0].attrIValue;
		switch (disabledType)
		{
			case 0:
			{
				std::string tp_d = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* disabledFileName = reader.getString(ballDisabledDataAttrs[1].attrUIValue);
//				const char* disabledFileName_tp = (disabledFileName && (strcmp(disabledFileName, "") != 0)) ? tp_d.append(disabledFileName).c_str() : NULL;
//				pWidget->loadSlidBallTextureDisabled(disabledFileName_tp);
				pWidget->loadSlidBallTextureDisabled(disabledFileName);
				break;
			}
			case 1:
			{
				const char* disabledFileName = reader.getString(ballDisabledDataAttrs[1].attrUIValue);
				pWidget->loadSlidBallTextureDisabled(disabledFileName, UI_TEX_TYPE_PLIST);
				break;
			}
			default:
				break;
		}

		pWidget->setPercent(attributes[attrIndex++].attrIValue);

		NodeElement* progressBarDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* progressBarDataAttrs = (AttrElement*)reader.getArray(progressBarDataNode->attrArray, NULL);
		int progressBarType = progressBarDataAttrs[0].attrIValue;
		switch (progressBarType)
		{
			case 0:
			{
				std::string tp_b = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* imageFileName = reader.getString(progressBarDataAttrs[1].attrUIValue);
//				const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0)) ? tp_b.append(imageFileName).c_str() : NULL;
//				pWidget->loadProgressBarTexture(imageFileName_tp);
				pWidget->loadProgressBarTexture(imageFileName);
				break;
			}
			case 1:
			{
				const char* imageFileName = reader.getString(progressBarDataAttrs[1].attrUIValue);
				pWidget->loadProgressBarTexture(imageFileName, UI_TEX_TYPE_PLIST);
				break;
			}
			default:
				break;
		}

		ColorPropsConvertor::readProperties(pWidget, optionsNode, attrIndex, reader);
	}
}
}

NS_CC_EXT_END