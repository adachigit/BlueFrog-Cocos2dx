#include "WidgetConvertor.h"

#include "DictionaryHelper.h"

#include "ButtonConvertor.h"
#include "CheckBoxConvertor.h"
#include "ImageViewConvertor.h"
#include "LabelAtlasConvertor.h"
#include "LabelBMFontConvertor.h"
#include "LabelConvertor.h"
#include "LayoutConvertor.h"
#include "ListViewConvertor.h"
#include "LoadingBarConvertor.h"
#include "PageViewConvertor.h"
#include "ScrollViewConvertor.h"
#include "SliderConvertor.h"
#include "TextFieldConvertor.h"

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	void WidgetConvertor::writeProperties(const rapidjson::Value& data, uint32_t node, BinWriter& writer)
	{
		const char* classname = DICTOOL->getStringValue_json(data, "classname");
		const rapidjson::Value& uiOptions = DICTOOL->getSubDictionary_json(data, "options");
		uint32_t optionsNode = writer.newNode();
		if (classname && strcmp(classname, "Button") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::Button));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			ButtonConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "CheckBox") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::CheckBox));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			CheckBoxConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "Label") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::Label));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			LabelConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "LabelAtlas") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::LabelAtlas));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			LabelAtlasConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "LoadingBar") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::LoadingBar));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			LoadingBarConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "ScrollView") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::ScrollView));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			ScrollViewConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "TextArea") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::Label));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			LabelConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "TextButton") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::Button));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			ButtonConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "TextField") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::TextField));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			TextFieldConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "ImageView") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::ImageView));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			ImageViewConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "Panel") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::Layout));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			LayoutConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "Slider") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::Slider));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			SliderConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "LabelBMFont") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::LabelBMFont));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			LabelBMFontConvetor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "DragPanel") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::ScrollView));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			ScrollViewConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "ListView") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::ListView));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			ListViewConvertor::writeProperties(uiOptions, optionsNode, writer);
		}
		else if (classname && strcmp(classname, "PageView") == 0)
		{
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::classname, WIDGET_TYPE::PageView));
			writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::options, optionsNode));
			PageViewConvertor::writeProperties(uiOptions, optionsNode, writer);
		}

		int childrenCount = DICTOOL->getArrayCount_json(data, "children");
		uint32_t* children = new uint32_t[childrenCount];
		for (int i = 0; i < childrenCount; i++)
		{
			const rapidjson::Value& subData = DICTOOL->getDictionaryFromArray_json(data, "children", i);
			uint32_t childNode = writer.newNode();
			children[i] = childNode;
			WidgetConvertor::writeProperties(subData, childNode, writer);
		}
		writer.addNodeAttribute(node, writer.newAttribute(ATTRIBUTE_TYPE::children, writer.newArray(children, childrenCount)));
		delete[] children;
	}

	UIWidget* WidgetConvertor::readProperties(NodeElement* pNode, BinReader& reader)
	{
		uint32_t attrIndex = 0;
		AttrElement* attributes = (AttrElement*)reader.getArray(pNode->attrArray, NULL);
		UIWidget* pWidget = NULL;

		if (attributes[attrIndex].attrType == ATTRIBUTE_TYPE::classname)
		{
			uint32_t widgetType = attributes[attrIndex++].attrIValue;
			NodeElement* optionsNode = reader.getNode(attributes[attrIndex++].attrUIValue);

			if (WIDGET_TYPE::Button == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from ButtonConvertor file");
				pWidget = UIButton::create();
				ButtonConvertor::readProperties((UIButton*)pWidget, optionsNode, reader);
			}
			else if (WIDGET_TYPE::CheckBox == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from CheckBoxConvertor file");
				pWidget = UICheckBox::create();
				CheckBoxConvertor::readProperties((UICheckBox*)pWidget, optionsNode, reader);
			}
			else if (WIDGET_TYPE::Label == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from LabelConvertor file");
				pWidget = UILabel::create();
				LabelConvertor::readProperties((UILabel*)pWidget, optionsNode, reader);
			}
			else if (WIDGET_TYPE::LabelAtlas == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from LabelAtlasConvertor file");
				pWidget = UILabelAtlas::create();
				LabelAtlasConvertor::readProperties((UILabelAtlas*)pWidget, optionsNode, reader);
			}
			else if (WIDGET_TYPE::LoadingBar == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from LoadingBarConvertor file");
				pWidget = UILoadingBar::create();
				LoadingBarConvertor::readProperties((UILoadingBar*)pWidget, optionsNode, reader);
			}
			else if (WIDGET_TYPE::ScrollView == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from ScrollViewConvertor file");
				pWidget = UIScrollView::create();
				ScrollViewConvertor::readProperties((UIScrollView*)pWidget, optionsNode, reader);
			}
			else if (WIDGET_TYPE::TextField == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from TextFieldConvertor file");
				pWidget = UITextField::create();
				TextFieldConvertor::readProperties((UITextField*)pWidget, optionsNode, reader);
			}
			else if (WIDGET_TYPE::ImageView == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from ImageViewConvertor file");
				pWidget = UIImageView::create();
				ImageViewConvertor::readProperties((UIImageView*)pWidget, optionsNode, reader);
			}
			else if (WIDGET_TYPE::Layout == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from LayoutConvertor file");
				pWidget = UILayout::create();
				LayoutConvertor::readProperties((UILayout*)pWidget, optionsNode, reader);
			}
			else if (WIDGET_TYPE::Slider == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from SliderConvertor file");
				pWidget = UISlider::create();
				SliderConvertor::readProperties((UISlider*)pWidget, optionsNode, reader);
			}
			else if (WIDGET_TYPE::LabelBMFont == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from LabelBMFontConvertor file");
				pWidget = UILabelBMFont::create();
				LabelBMFontConvetor::readProperties((UILabelBMFont*)pWidget, optionsNode, reader);
			}
			else if (WIDGET_TYPE::ScrollView == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from ScrollViewConvertor file");
				pWidget = UIScrollView::create();
				ScrollViewConvertor::readProperties((UIScrollView*)pWidget, optionsNode, reader);
			}
			else if (WIDGET_TYPE::ListView == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from ListViewConvertor file");
				pWidget = UIListViewEx::create();
				ListViewConvertor::readProperties((UIListViewEx*)pWidget, optionsNode, reader);
			}
			else if (WIDGET_TYPE::PageView == widgetType)
			{
//				cocos2d::TimeElapse te("create widget from PageViewConvertor file");
				pWidget = UIPageViewEx::create();
				PageViewConvertor::readProperties((UIPageViewEx*)pWidget, optionsNode, reader);
			}
		}

		int32_t childrenCount = 0;
		uint32_t* children = (uint32_t*)reader.getArray(attributes[attrIndex++].attrUIValue, &childrenCount);
		for (int i = 0; i < childrenCount; i++)
		{
			NodeElement* childNode = reader.getNode(children[i]);
			UIWidget* pChildWidget = readProperties(childNode, reader);
			if (pChildWidget)
			{
				pWidget->addChild(pChildWidget);
			}
		}

		UILayout* layout = dynamic_cast<UILayout*>(pWidget);
		if (layout)
		{
			layout->doLayout();
		}

		return pWidget;
	}
}
}

NS_CC_EXT_END