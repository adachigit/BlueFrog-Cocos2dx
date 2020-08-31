#include "ListViewConvertor.h"

#include "DictionaryHelper.h"
#include "BasePropsConvertor.h"

USING_NS_CC_EXT;

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	void ListViewConvertor::writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer)
	{
		BasePropsConvertor::writeProperties(options, node, writer);

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::innerWidth, DICTOOL->getFloatValue_json(options, "innerWidth")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::innerHeight, DICTOOL->getFloatValue_json(options, "innerHeight")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::direction, DICTOOL->getFloatValue_json(options, "direction")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::gravity, DICTOOL->getIntValue_json(options, "gravity")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::itemMargin, DICTOOL->getFloatValue_json(options, "itemMargin")));
	}

	void ListViewConvertor::readProperties(UIListViewEx* pWidget, NodeElement* optionsNode, BinReader& reader)
	{
		uint32_t attrIndex = BasePropsConvertor::readProperties(pWidget, optionsNode, 0, reader);

		AttrElement* attributes = (AttrElement*)reader.getArray(optionsNode->attrArray, NULL);

		float innerWidth = attributes[attrIndex++].attrFValue;
		float innerHeight = attributes[attrIndex++].attrFValue;
		pWidget->setInnerContainerSize(CCSizeMake(innerWidth, innerHeight));
		pWidget->setDirection((SCROLLVIEW_DIR)(int)(attributes[attrIndex++].attrFValue));

		pWidget->setGravity((ListViewGravity)attributes[attrIndex++].attrIValue);

		pWidget->setItemsMargin(attributes[attrIndex++].attrFValue);
	}
}
}

NS_CC_EXT_END