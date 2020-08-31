#include "TextFieldConvertor.h"

#include "DictionaryHelper.h"
#include "BasePropsConvertor.h"
#include "ColorPropsConvertor.h"

USING_NS_CC_EXT;

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	void TextFieldConvertor::writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer)
	{
		BasePropsConvertor::writeProperties(options, node, writer);

		bool ph = DICTOOL->checkObjectExist_json(options, "placeHolder");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::placeHolder, ph ? DICTOOL->getStringValue_json(options, "placeHolder") : ""));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::text, DICTOOL->getStringValue_json(options, "text")));
		bool fs = DICTOOL->checkObjectExist_json(options, "fontSize");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::fontSize, fs ? DICTOOL->getIntValue_json(options, "fontSize") : 0.0f));
		bool fn = DICTOOL->checkObjectExist_json(options, "fontName");
		if (fn)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::fontName, DICTOOL->getStringValue_json(options, "fontName")));
		}

		bool tsw = DICTOOL->checkObjectExist_json(options, "touchSizeWidth");
		bool tsh = DICTOOL->checkObjectExist_json(options, "touchSizeHeight");
		if (tsw && tsh)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::touchSizeWidth, DICTOOL->getFloatValue_json(options, "touchSizeWidth")));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::touchSizeHeight, DICTOOL->getFloatValue_json(options, "touchSizeHeight")));
		}

		float dw = DICTOOL->getFloatValue_json(options, "width");
		float dh = DICTOOL->getFloatValue_json(options, "height");
		if (dw > 0.0f || dh > 0.0f)
		{
			//textField->setSize(CCSizeMake(dw, dh));
		}

		bool maxLengthEnable = DICTOOL->getBooleanValue_json(options, "maxLengthEnable");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::maxLengthEnable, (uint32_t)maxLengthEnable));
		if (maxLengthEnable)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::maxLength, DICTOOL->getIntValue_json(options, "maxLength")));
		}

		bool passwordEnable = DICTOOL->getBooleanValue_json(options, "passwordEnable");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::passwordEnable, (uint32_t)passwordEnable));
		if (passwordEnable)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::passwordStyleText, DICTOOL->getStringValue_json(options, "passwordStyleText")));
		}

		ColorPropsConvertor::writeProperties(options, node, writer, WidgetType::WidgetTypeWidget);
	}

	void TextFieldConvertor::readProperties(UITextField* pWidget, NodeElement* optionsNode, BinReader& reader)
	{
		uint32_t attrIndex = BasePropsConvertor::readProperties(pWidget, optionsNode, 0, reader);

		AttrElement* attributes = (AttrElement*)reader.getArray(optionsNode->attrArray, NULL);

		pWidget->setPlaceHolder(reader.getString(attributes[attrIndex++].attrUIValue));
		pWidget->setText(reader.getString(attributes[attrIndex++].attrUIValue));
		pWidget->setFontSize(attributes[attrIndex++].attrIValue);

		if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::fontName)
		{
			pWidget->setFontName(reader.getString(attributes[attrIndex++].attrUIValue));
		}
		if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::touchSizeWidth)
		{
			float fw = attributes[attrIndex++].attrFValue;
			float fh = attributes[attrIndex++].attrFValue;
			pWidget->setTouchSize(CCSizeMake(fw, fh));
		}

		bool maxLengthEnable = (bool)attributes[attrIndex++].attrIValue;
		pWidget->setMaxLengthEnabled(maxLengthEnable);
		if (maxLengthEnable)
		{
			pWidget->setMaxLength(attributes[attrIndex++].attrIValue);
		}

		bool passwordEnable = (bool)attributes[attrIndex++].attrIValue;
		pWidget->setPasswordEnable(passwordEnable);
		if (passwordEnable)
		{
			pWidget->setPasswordStyleText(reader.getString(attributes[attrIndex++].attrUIValue));
		}

		ColorPropsConvertor::readProperties(pWidget, optionsNode, attrIndex, reader);
	}
}
}

NS_CC_EXT_END