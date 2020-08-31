#include "BasePropsConvertor.h"

#include "DictionaryHelper.h"

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	void BasePropsConvertor::writeProperties(const rapidjson::Value& options, uint32_t node, BinWriter& writer)
	{
		bool ignoreSizeExist = DICTOOL->checkObjectExist_json(options, "ignoreSize");
		if (ignoreSizeExist)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::ignoreSize, (uint32_t)DICTOOL->getBooleanValue_json(options, "ignoreSize")));
		}

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::sizeType, DICTOOL->getIntValue_json(options, "sizeType")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::positionType, DICTOOL->getIntValue_json(options, "positionType")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::sizePercentX, DICTOOL->getFloatValue_json(options, "sizePercentX")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::sizePercentY, DICTOOL->getFloatValue_json(options, "sizePercentY")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::positionPercentX, DICTOOL->getFloatValue_json(options, "positionPercentX")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::positionPercentY, DICTOOL->getFloatValue_json(options, "positionPercentY")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::width, DICTOOL->getFloatValue_json(options, "width")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::height, DICTOOL->getFloatValue_json(options, "height")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::tag, DICTOOL->getIntValue_json(options, "tag")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::actiontag, DICTOOL->getIntValue_json(options, "actiontag")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::touchAble, (uint32_t)DICTOOL->getBooleanValue_json(options, "touchAble")));
		const char* name = DICTOOL->getStringValue_json(options, "name");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::name, writer.addString(name ? name : "default")));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::x, DICTOOL->getFloatValue_json(options, "x")));
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::y, DICTOOL->getFloatValue_json(options, "y")));

		bool sx = DICTOOL->checkObjectExist_json(options, "scaleX");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::scaleX, sx ? DICTOOL->getFloatValue_json(options, "scaleX") : 1.0f));
		bool sy = DICTOOL->checkObjectExist_json(options, "scaleY");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::scaleY, sy ? DICTOOL->getFloatValue_json(options, "scaleY") : 1.0f));
		bool rt = DICTOOL->checkObjectExist_json(options, "rotation");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::rotation, rt ? DICTOOL->getFloatValue_json(options, "rotation") : 0.0f));
		bool vb = DICTOOL->checkObjectExist_json(options, "visible");
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::visible, vb ? (uint32_t)DICTOOL->getBooleanValue_json(options, "visible") : (uint32_t)true));

		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::ZOrder, DICTOOL->getIntValue_json(options, "ZOrder")));

		bool layout = DICTOOL->checkObjectExist_json(options, "layoutParameter");
		if (layout)
		{
			const rapidjson::Value& layoutParameterDic = DICTOOL->getSubDictionary_json(options, "layoutParameter");
			int paramType = DICTOOL->getIntValue_json(layoutParameterDic, "type");
			if (paramType == 1 || paramType == 2)
			{
				uint32_t layoutParamNode = writer.newNode();
				writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::layoutParameter, layoutParamNode));
				writer.addNodeAttribute(layoutParamNode, writer.newAttribute(ATTRIBUTE_TYPE::lpType, paramType));

				switch (paramType)
				{
					case 1:
						writer.addNodeAttribute(layoutParamNode, writer.newAttribute(ATTRIBUTE_TYPE::lpGravity, DICTOOL->getIntValue_json(layoutParameterDic, "gravity")));
						break;
					case 2:
					{
						const char* relativeName = DICTOOL->getStringValue_json(layoutParameterDic, "relativeName");
						writer.addNodeAttribute(layoutParamNode, writer.newAttribute(ATTRIBUTE_TYPE::lpRelativeName, relativeName ? relativeName : ""));
						const char* relativeToName = DICTOOL->getStringValue_json(layoutParameterDic, "relativeToName");
						writer.addNodeAttribute(layoutParamNode, writer.newAttribute(ATTRIBUTE_TYPE::lpRelativeToName, relativeToName ? relativeToName : ""));
						writer.addNodeAttribute(layoutParamNode, writer.newAttribute(ATTRIBUTE_TYPE::lpAlign, DICTOOL->getIntValue_json(layoutParameterDic, "align")));
						break;
					}
					default:
						break;
				}

				writer.addNodeAttribute(layoutParamNode, writer.newAttribute(ATTRIBUTE_TYPE::lpMarginLeft, DICTOOL->getFloatValue_json(layoutParameterDic, "marginLeft")));
				writer.addNodeAttribute(layoutParamNode, writer.newAttribute(ATTRIBUTE_TYPE::lpMarginTop, DICTOOL->getFloatValue_json(layoutParameterDic, "marginTop")));
				writer.addNodeAttribute(layoutParamNode, writer.newAttribute(ATTRIBUTE_TYPE::lpMarginRight, DICTOOL->getFloatValue_json(layoutParameterDic, "marginRight")));
				writer.addNodeAttribute(layoutParamNode, writer.newAttribute(ATTRIBUTE_TYPE::lpMarginDown, DICTOOL->getFloatValue_json(layoutParameterDic, "marginDown")));
			}
		}
	}

	uint32_t BasePropsConvertor::readProperties(UIWidget* pWidget, NodeElement* optionsNode, uint32_t attrIndex, BinReader& reader)
	{
//		cocos2d::TimeElapse te("BasePropsConvertor readProperties");

		uint32_t index = 0;
		AttrElement* attributes = (AttrElement*)reader.getArray(optionsNode->attrArray, NULL);

		if (attributes[attrIndex + index].attrType == ATTRIBUTE_TYPE::ignoreSize)
		{
			pWidget->ignoreContentAdaptWithSize((bool)attributes[attrIndex + index++].attrIValue);
		}

		pWidget->setSizeType((SizeType)attributes[attrIndex + index++].attrIValue);
		pWidget->setPositionType((PositionType)attributes[attrIndex + index++].attrIValue);

		float fx = attributes[attrIndex + index++].attrFValue;
		float fy = attributes[attrIndex + index++].attrFValue;
		pWidget->setSizePercent(ccp(fx, fy));

		fx = attributes[attrIndex + index++].attrFValue;
		fy = attributes[attrIndex + index++].attrFValue;
		pWidget->setPositionPercent(ccp(fx, fy));

		fx = attributes[attrIndex + index++].attrFValue;
		fy = attributes[attrIndex + index++].attrFValue;
		pWidget->setSize(CCSizeMake(fx, fy));

		pWidget->setTag(attributes[attrIndex + index++].attrIValue);
		pWidget->setActionTag(attributes[attrIndex + index++].attrIValue);

		pWidget->setTouchEnabled((bool)attributes[attrIndex + index++].attrUIValue);
		pWidget->setName(reader.getString(attributes[attrIndex + index++].attrUIValue));

		fx = attributes[attrIndex + index++].attrFValue;
		fy = attributes[attrIndex + index++].attrFValue;
		pWidget->setPosition(ccp(fx, fy));

		pWidget->setScaleX(attributes[attrIndex + index++].attrFValue);
		pWidget->setScaleY(attributes[attrIndex + index++].attrFValue);
		pWidget->setRotation(attributes[attrIndex + index++].attrFValue);
		pWidget->setVisible((bool)attributes[attrIndex + index++].attrUIValue);

		pWidget->setZOrder(attributes[attrIndex + index++].attrIValue);

		if (attributes[attrIndex + index].attrType == ATTRIBUTE_TYPE::layoutParameter)
		{
			NodeElement* layoutParamNode = reader.getNode(attributes[attrIndex + index++].attrUIValue);
			AttrElement* layoutAttributes = (AttrElement*)reader.getArray(layoutParamNode->attrArray, NULL);
			uint32_t layoutIndex = 0;
			int paramType = layoutAttributes[layoutIndex++].attrIValue;
			if (paramType == 1 || paramType == 2)
			{
				UILayoutParameter* parameter = NULL;
				switch (paramType)
				{
				case 1:
					{
						parameter = UILinearLayoutParameter::create();
						((UILinearLayoutParameter*)parameter)->setGravity((UILinearGravity)layoutAttributes[layoutIndex++].attrIValue);
						break;
					}
				case 2:
					{
						parameter = UIRelativeLayoutParameter::create();
						((UIRelativeLayoutParameter*)parameter)->setRelativeName(reader.getString(layoutAttributes[layoutIndex++].attrUIValue));
						((UIRelativeLayoutParameter*)parameter)->setRelativeToWidgetName(reader.getString(layoutAttributes[layoutIndex++].attrUIValue));
						((UIRelativeLayoutParameter*)parameter)->setAlign((UIRelativeAlign)layoutAttributes[layoutIndex++].attrIValue);
						break;
					}
				default:
					break;
				}

				if (parameter)
				{
					float mgl = attributes[layoutIndex++].attrFValue;
					float mgt = attributes[layoutIndex++].attrFValue;
					float mgr = attributes[layoutIndex++].attrFValue;
					float mgb = attributes[layoutIndex++].attrFValue;
					parameter->setMargin(UIMargin(mgl, mgt, mgr, mgb));
					pWidget->setLayoutParameter(parameter);
				}
			}
		}

		return index;
	}
}
}

NS_CC_EXT_END
