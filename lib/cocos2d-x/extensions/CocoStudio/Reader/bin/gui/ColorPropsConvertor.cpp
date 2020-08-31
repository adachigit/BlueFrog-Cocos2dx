#include "ColorPropsConvertor.h"

#include "DictionaryHelper.h"

USING_NS_CC_EXT;

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	void ColorPropsConvertor::writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer, const WidgetType widgetType)
	{
		bool op = DICTOOL->checkObjectExist_json(options, "opacity");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::opacity, op ? DICTOOL->getIntValue_json(options, "opacity") : 255));

		bool cr = DICTOOL->checkObjectExist_json(options, "colorR");
		bool cg = DICTOOL->checkObjectExist_json(options, "colorG");
		bool cb = DICTOOL->checkObjectExist_json(options, "colorB");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::colorR, cr ? DICTOOL->getIntValue_json(options, "colorR") : 255));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::colorG, cg ? DICTOOL->getIntValue_json(options, "colorG") : 255));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::colorB, cb ? DICTOOL->getIntValue_json(options, "colorB") : 255));

		bool apx = DICTOOL->checkObjectExist_json(options, "anchorPointX");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::anchorPointX, apx ? DICTOOL->getFloatValue_json(options, "anchorPointX") : ((widgetType == WidgetTypeWidget) ? 0.5f : 0.0f)));
		bool apy = DICTOOL->checkObjectExist_json(options, "anchorPointY");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::anchorPointY, apx ? DICTOOL->getFloatValue_json(options, "anchorPointY") : ((widgetType == WidgetTypeWidget) ? 0.5f : 0.0f)));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::flipX, (uint32_t)DICTOOL->getBooleanValue_json(options, "flipX")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::flipY, (uint32_t)DICTOOL->getBooleanValue_json(options, "flipY")));
	}

	uint32_t ColorPropsConvertor::readProperties(UIWidget* pWidget, NodeElement* optionsNode, uint32_t attrIndex, BinReader& reader)
	{
//		cocos2d::TimeElapse te("ColorPropsConvertor readProperties");

		uint32_t index = 0;
		AttrElement* attributes = (AttrElement*)reader.getArray(optionsNode->attrArray, NULL);

		pWidget->setOpacity(attributes[attrIndex + index++].attrIValue);

		int colorR = attributes[attrIndex + index++].attrIValue;
		int colorG = attributes[attrIndex + index++].attrIValue;
		int colorB = attributes[attrIndex + index++].attrIValue;
		pWidget->setColor(ccc3(colorR, colorG, colorB));

		float fx = attributes[attrIndex + index++].attrFValue;
		float fy = attributes[attrIndex + index++].attrFValue;
		pWidget->setAnchorPoint(ccp(fx, fy));
		pWidget->setFlipX((bool)attributes[attrIndex + index++].attrUIValue);
		pWidget->setFlipY((bool)attributes[attrIndex + index++].attrUIValue);

		return index;
	}
}
}

NS_CC_EXT_END