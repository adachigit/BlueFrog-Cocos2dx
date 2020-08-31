#include "ImageViewConvertor.h"

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
	void ImageViewConvertor::writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer)
	{
		BasePropsConvertor::writeProperties(options, node, writer);

		const rapidjson::Value& imageFileNameDic = DICTOOL->getSubDictionary_json(options, "fileNameData");
		int imageFileNameType = DICTOOL->getIntValue_json(imageFileNameDic, "resourceType");
		uint32_t fileNameDataNode = writer.newNode();
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::fileNameData, fileNameDataNode));
		writer.addNodeAttribute(fileNameDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourceType, imageFileNameType));
		switch (imageFileNameType)
		{
		case 0:
		case 1:
		{
			const char* imageFileName = DICTOOL->getStringValue_json(imageFileNameDic, "path");
			writer.addNodeAttribute(fileNameDataNode, writer.newAttribute(ATTRIBUTE_TYPE::dataResourcePath, imageFileName ? imageFileName : ""));
			break;
		}
		default:
			break;
		}

		bool scale9EnableExist = DICTOOL->checkObjectExist_json(options, "scale9Enable");
		bool scale9Enabled = scale9EnableExist ? (uint32_t)DICTOOL->getBooleanValue_json(options, "scale9Enable") : false;
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::scale9Enable, (int32_t)scale9Enabled));

		if (scale9Enabled)
		{
			bool sw = DICTOOL->checkObjectExist_json(options, "scale9Width");
			bool sh = DICTOOL->checkObjectExist_json(options, "scale9Height");
			if (sw && sh)
			{
				writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::scale9Width, DICTOOL->getFloatValue_json(options, "scale9Width")));
				writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::scale9Height, DICTOOL->getFloatValue_json(options, "scale9Height")));
			}

			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsX, DICTOOL->getFloatValue_json(options, "capInsetsX")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsY, DICTOOL->getFloatValue_json(options, "capInsetsY")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsWidth, DICTOOL->getFloatValue_json(options, "capInsetsWidth")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::capInsetsHeight, DICTOOL->getFloatValue_json(options, "capInsetsHeight")));

		}

		ColorPropsConvertor::writeProperties(options, node, writer, WidgetType::WidgetTypeWidget);
	}

	void ImageViewConvertor::readProperties(UIImageView* pWidget, NodeElement* optionsNode, BinReader& reader)
	{
		uint32_t attrIndex = BasePropsConvertor::readProperties(pWidget, optionsNode, 0, reader);

//		cocos2d::TimeElapse* te = new cocos2d::TimeElapse("ImageView Set Properties in readProperties");

		AttrElement* attributes = (AttrElement*)reader.getArray(optionsNode->attrArray, NULL);

		NodeElement* fileNameDataNode = reader.getNode(attributes[attrIndex++].attrUIValue);
		AttrElement* fileNameDataAttrs = (AttrElement*)reader.getArray(fileNameDataNode->attrArray, NULL);
		int fileNameType = fileNameDataAttrs[0].attrIValue;
		switch (fileNameType)
		{
			case 0:
			{
				std::string tp_i = reader.getExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath);
				const char* imageFileName = reader.getString(fileNameDataAttrs[1].attrUIValue);
				const char* imageFileName_tp = NULL;
				if (imageFileName && (strcmp(imageFileName, "") != 0))
				{
//					imageFileName_tp = tp_i.append(imageFileName).c_str();
//					pWidget->loadTexture(imageFileName_tp);
					pWidget->loadTexture(imageFileName);
				}
				break;
			}
			case 1:
			{
				const char* imageFileName = reader.getString(fileNameDataAttrs[1].attrUIValue);
				pWidget->loadTexture(imageFileName, UI_TEX_TYPE_PLIST);
				break;
			}
			default:
				break;
		}

		bool scale9Enabled = (bool)attributes[attrIndex++].attrIValue;
		pWidget->setScale9Enabled(scale9Enabled);

		if (scale9Enabled)
		{
			if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::scale9Width)
			{
				float swf = attributes[attrIndex++].attrFValue;
				float shf = attributes[attrIndex++].attrFValue;
				pWidget->setSize(CCSizeMake(swf, shf));
			}

			float cx = attributes[attrIndex++].attrFValue;
			float cy = attributes[attrIndex++].attrFValue;
			float cw = attributes[attrIndex++].attrFValue;
			float ch = attributes[attrIndex++].attrFValue;

			pWidget->setCapInsets(CCRectMake(cx, cy, cw, ch));
		}

//		delete te;

		ColorPropsConvertor::readProperties(pWidget, optionsNode, attrIndex, reader);
	}
}
}

NS_CC_EXT_END