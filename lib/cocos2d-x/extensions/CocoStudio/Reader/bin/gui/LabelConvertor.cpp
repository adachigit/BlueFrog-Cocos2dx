#include "LabelConvertor.h"

#include "DictionaryHelper.h"
#include "BasePropsConvertor.h"
#include "ColorPropsConvertor.h"

USING_NS_CC_EXT;

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	void LabelConvertor::writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer)
	{
		BasePropsConvertor::writeProperties(options, node, writer);

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::touchScaleEnable, (uint32_t)DICTOOL->getBooleanValue_json(options, "touchScaleEnable")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::text, DICTOOL->getStringValue_json(options, "text")));

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

		bool aw = DICTOOL->checkObjectExist_json(options, "areaWidth");
		bool ah = DICTOOL->checkObjectExist_json(options, "areaHeight");
		if (aw && ah)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::areaWidth, DICTOOL->getFloatValue_json(options, "areaWidth")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::areaHeight, DICTOOL->getFloatValue_json(options, "areaHeight")));
		}
		bool ha = DICTOOL->checkObjectExist_json(options, "hAlignment");
		if (ha)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::hAlignment, DICTOOL->getIntValue_json(options, "hAlignment")));
		}
		bool va = DICTOOL->checkObjectExist_json(options, "vAlignment");
		if (va)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::vAlignment, DICTOOL->getIntValue_json(options, "vAlignment")));
		}

		ColorPropsConvertor::writeProperties(options, node, writer, WidgetType::WidgetTypeWidget);
	}

	void LabelConvertor::readProperties(UILabel* pWidget, NodeElement* optionsNode, BinReader& reader)
	{
		uint32_t attrIndex = BasePropsConvertor::readProperties(pWidget, optionsNode, 0, reader);

//		cocos2d::TimeElapse* te = new cocos2d::TimeElapse("Label Set Properties in readProperties");

		AttrElement* attributes = (AttrElement*)reader.getArray(optionsNode->attrArray, NULL);

		pWidget->setTouchScaleChangeAble((bool)attributes[attrIndex++].attrUIValue);
		
		const char* text = reader.getString(attributes[attrIndex++].attrUIValue);
		int fontSize = 0;
		const char* fontName = NULL;
		CCSize* dimensions = NULL;
		int hAlignment;
		int vAlignment;
		if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::fontSize)
		{
			fontSize = attributes[attrIndex++].attrIValue;
		}
		if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::fontName)
		{
			fontName = reader.getString(attributes[attrIndex++].attrUIValue);
		}
		if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::areaWidth)
		{
			float fw = attributes[attrIndex++].attrFValue;
			float fh = attributes[attrIndex++].attrFValue;
			dimensions = new CCSize(fw, fh);
		}
		if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::hAlignment)
		{
			hAlignment = attributes[attrIndex++].attrIValue;
		}
		if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::vAlignment)
		{
			vAlignment = attributes[attrIndex++].attrIValue;
		}

		pWidget->setTextInfo(text, fontSize, fontName, dimensions, hAlignment, vAlignment);

		CC_SAFE_DELETE(dimensions);

//		delete te;

		ColorPropsConvertor::readProperties(pWidget, optionsNode, attrIndex, reader);
	}
}
}

NS_CC_EXT_END