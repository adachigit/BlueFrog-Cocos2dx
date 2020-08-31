/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "../GUI/System/CocosGUI.h"
#include "../Json/DictionaryHelper.h"
#include "../Action/ActionManager.h"
#include "bin/gui/GUIBinConvertor.h"		// add by hanyi
#include <fstream>
#include <iostream>


NS_CC_EXT_BEGIN
    
static GUIReader* sharedReader = NULL;

GUIReader::GUIReader():
m_strFilePath("")
{
    _fileDesignSizes = CCDictionary::create();
    CC_SAFE_RETAIN(_fileDesignSizes);
}

GUIReader::~GUIReader()
{
    _fileDesignSizes->removeAllObjects();
    CC_SAFE_RELEASE(_fileDesignSizes);
}

GUIReader* GUIReader::shareReader()
{
    if (!sharedReader)
    {
        sharedReader = new GUIReader();
    }
    return sharedReader;
}

void GUIReader::purgeGUIReader()
{
	CC_SAFE_DELETE(sharedReader);
}

int GUIReader::getVersionInteger(const char *str)
{
    /*********temp***********/
    std::string strVersion = str;
    int length = strVersion.length();
    if (length < 7)
    {
        return 0;
    }
    int pos = strVersion.find_first_of(".");
    std::string t = strVersion.substr(0,pos);
    strVersion = strVersion.substr(pos+1,strVersion.length()-1);
    
    pos = strVersion.find_first_of(".");
    std::string h = strVersion.substr(0,pos);
    strVersion = strVersion.substr(pos+1,strVersion.length()-1);
    
    pos = strVersion.find_first_of(".");
    std::string te = strVersion.substr(0,pos);
    strVersion = strVersion.substr(pos+1,strVersion.length()-1);
    
    pos = strVersion.find_first_of(".");
    std::string s = strVersion.substr(0,pos);
    
    int it = atoi(t.c_str());
    int ih = atoi(h.c_str());
    int ite = atoi(te.c_str());
    int is = atoi(s.c_str());
    
    int iVersion = it*1000+ih*100+ite*10+is;
    CCLOG("iversion %d",iVersion);
    return iVersion;
    /************************/
}

void GUIReader::storeFileDesignSize(const char *fileName, const cocos2d::CCSize &size)
{
    if (!_fileDesignSizes)
    {
        return;
    }
    cocos2d::CCString* strSize = cocos2d::CCString::createWithFormat("{%f,%f}", size.width, size.height);
    _fileDesignSizes->setObject(strSize, fileName);
}

const cocos2d::CCSize GUIReader::getFileDesignSize(const char* fileName) const
{
    if (!_fileDesignSizes)
    {
        return cocos2d::CCSizeZero;
    }
    cocos2d::CCSize designSize = cocos2d::CCSizeFromString(((cocos2d::CCString*)_fileDesignSizes->objectForKey(fileName))->m_sString.c_str());
    return designSize;
}

// add by weixin end
UIWidget* GUIReader::widgetFromJsonFile(const char *fileName)
{
// add by weixin begin
    cocos2d::TimeElapse te("create widget from json file");
// add by hanyi begin
#if (USE_GUI_BIN == 0)
#if (USE_JSON_LUA == 0)
// add by weixin end
    unsigned char *pBytes = NULL;
	std::string jsonpath;
	rapidjson::Document jsonDict;
    jsonpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
    int pos = jsonpath.find_last_of('/');
	m_strFilePath = jsonpath.substr(0,pos+1);
    unsigned long size = 0;
    pBytes = CCFileUtils::sharedFileUtils()->getFileData(jsonpath.c_str(),"r" , &size);
	if(NULL == pBytes || strcmp((const char*)pBytes, "") == 0)
	{
		printf("read json file[%s] error!\n", fileName);
		return NULL;
	}
	CCData *data = new CCData(pBytes, size);
	std::string load_str = std::string((const char *)data->getBytes(), data->getSize() );
#ifdef CC_TARGET_OS_MAC
    while(true)   {
        string::size_type   pos(0);
        if((pos = load_str.find(".ttf")) != string::npos)
            load_str.replace(pos,4,"");
        else
            break;
    }
#endif
	CC_SAFE_DELETE(data);
	jsonDict.Parse<0>(load_str.c_str());
    if (jsonDict.HasParseError())
    {
        CCLOG("GetParseError %s\n",jsonDict.GetParseError());
    }
    UIWidget* widget = NULL;
    const char* fileVersion = DICTOOL->getStringValue_json(jsonDict, "version");
    WidgetPropertiesReader * pReader = NULL;
    if (fileVersion)
    {
        int versionInteger = getVersionInteger(fileVersion);
        if (versionInteger < 250)
        {
            pReader = new WidgetPropertiesReader0250();
            widget = pReader->createWidget(jsonDict, m_strFilePath.c_str(), fileName);
        }
        else
        {
            pReader = new WidgetPropertiesReader0300();
            widget = pReader->createWidget(jsonDict, m_strFilePath.c_str(), fileName);
        }
    }
    else
    {
        pReader = new WidgetPropertiesReader0250();
        widget = pReader->createWidget(jsonDict, m_strFilePath.c_str(), fileName);
    }

    CC_SAFE_DELETE(pReader);
    CC_SAFE_DELETE_ARRAY(pBytes);
// add by weixin begin
#else
    cocos2d::TimeElapse *pte2 = new TimeElapse("load json file");
    cs::CSJsonDictionary jsonDict;
    jsonDict.initWithDescription(fileName);
    delete pte2;
    pte2 = NULL;
//#ifdef CC_TARGET_OS_MAC
//    while(true)   {
//        string::size_type   pos(0);
//        if((pos = load_str.find(".ttf")) != string::npos)
//            load_str.replace(pos,4,"");
//        else
//            break;
//    }
//#endif
    UIWidget* widget = NULL;
    const char* fileVersion = jsonDict.getItemStringValue("version");
    if (fileVersion)
    {
        int versionInteger = getVersionInteger(fileVersion);
        if (versionInteger < 250)
        {
            assert(0 && "version lower then 250 is not support");
        }
        else
        {
            WidgetPropertiesReader0300Lua * pReader = new WidgetPropertiesReader0300Lua();
            widget = pReader->createWidget(jsonDict, m_strFilePath.c_str(), fileName);
            CC_SAFE_DELETE(pReader);
        }
    }
    else
    {
        assert(0 && "version lower then 250 is not support");
    }
#endif
// add by weixin end
#else
	UIWidget* widget = NULL;
	widget = cocos2d::extension::ccsbin::gui::GUIBinConvertor::sharedWriter()->widgetFromBinFile(fileName);
#endif
    return widget;
}



UIWidget* WidgetPropertiesReader0250::createWidget(const rapidjson::Value& data, const char* fullPath, const char* fileName)
{
    m_strFilePath = fullPath;
    int texturesCount = DICTOOL->getArrayCount_json(data, "textures");

    for (int i=0; i<texturesCount; i++)
    {
        const char* file = DICTOOL->getStringValueFromArray_json(data, "textures", i);
        std::string tp = fullPath;
        tp.append(file);
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(tp.c_str());
    }
    float fileDesignWidth = DICTOOL->getFloatValue_json(data, "designWidth");
    float fileDesignHeight = DICTOOL->getFloatValue_json(data, "designHeight");
    if (fileDesignWidth <= 0 || fileDesignHeight <= 0) {
        printf("Read design size error!\n");
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        GUIReader::shareReader()->storeFileDesignSize(fileName, winSize);
    }
    else
    {
        GUIReader::shareReader()->storeFileDesignSize(fileName, CCSizeMake(fileDesignWidth, fileDesignHeight));
    }
    const rapidjson::Value& widgetTree = DICTOOL->getSubDictionary_json(data, "widgetTree");
    UIWidget* widget = widgetFromJsonDictionary(widgetTree);
    
    /* *********temp********* */
    if (widget->getContentSize().equals(CCSizeZero))
    {
        UIContainerWidget* rootWidget = dynamic_cast<UIContainerWidget*>(widget);
        rootWidget->setSize(CCSizeMake(fileDesignWidth, fileDesignHeight));
    }
    /* ********************** */
    
    //    widget->setFileDesignSize(CCSizeMake(fileDesignWidth, fileDesignHeight));
    const rapidjson::Value& actions = DICTOOL->getSubDictionary_json(data, "animation");
    /* *********temp********* */
    //    ActionManager::shareManager()->releaseActions();
    /* ********************** */
   // CCLOG("file name == [%s]",fileName);
	CCObject* rootWidget = (CCObject*) widget;
    ActionManager::shareManager()->initWithDictionary(fileName,actions,rootWidget);
    return widget;
}

UIWidget* WidgetPropertiesReader0250::widgetFromJsonDictionary(const rapidjson::Value& data)
{
    UIWidget* widget = NULL;
    const char* classname = DICTOOL->getStringValue_json(data, "classname");
    const rapidjson::Value& uiOptions = DICTOOL->getSubDictionary_json(data, "options");
    if (classname && strcmp(classname, "Button") == 0)
    {
        widget = UIButton::create();
        setPropsForButtonFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "CheckBox") == 0)
    {
        widget = UICheckBox::create();
        setPropsForCheckBoxFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "Label") == 0)
    {
        widget = UILabel::create();
        setPropsForLabelFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "LabelAtlas") == 0)
    {
        widget = UILabelAtlas::create();
        setPropsForLabelAtlasFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "LoadingBar") == 0)
    {
        widget = UILoadingBar::create();
        setPropsForLoadingBarFromJsonDictionary(widget, uiOptions);
    }else if (classname && strcmp(classname, "ScrollView") == 0){
        widget = UIScrollView::create();
        setPropsForScrollViewFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "TextArea") == 0)
    {
        widget = UILabel::create();
        setPropsForLabelFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "TextButton") == 0)
    {
        widget = UIButton::create();
        setPropsForButtonFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "TextField") == 0)
    {
        widget = UITextField::create();
        setPropsForTextFieldFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "ImageView") == 0)
    {
        widget = UIImageView::create();
        setPropsForImageViewFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "Panel") == 0)
    {
        widget = UILayout::create();
        setPropsForLayoutFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "Slider") == 0)
    {
        widget = UISlider::create();
        setPropsForSliderFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "LabelBMFont") == 0)
    {
        widget = UILabelBMFont::create();
        setPropsForLabelBMFontFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "DragPanel") == 0)
    {
        widget = UIScrollView::create();
        setPropsForScrollViewFromJsonDictionary(widget, uiOptions);
    }
    
    int childrenCount = DICTOOL->getArrayCount_json(data, "children");
    for (int i=0;i<childrenCount;i++)
    {
        const rapidjson::Value& subData = DICTOOL->getDictionaryFromArray_json(data, "children", i);
        UIWidget* child = widgetFromJsonDictionary(subData);
        if (child)
        {
            widget->addChild(child);
        }
    }
    return widget;
}

void WidgetPropertiesReader0250::setPropsForWidgetFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    bool ignoreSizeExsit = DICTOOL->checkObjectExist_json(options, "ignoreSize");
    if (ignoreSizeExsit)
    {
        widget->ignoreContentAdaptWithSize(DICTOOL->getBooleanValue_json(options, "ignoreSize"));
    }
    
    float w = DICTOOL->getFloatValue_json(options, "width");
    float h = DICTOOL->getFloatValue_json(options, "height");
    widget->setSize(CCSizeMake(w, h));
    
    widget->setTag(DICTOOL->getIntValue_json(options, "tag"));
	widget->setActionTag(DICTOOL->getIntValue_json(options, "actiontag"));
    widget->setTouchEnabled(DICTOOL->getBooleanValue_json(options, "touchAble"));
    const char* name = DICTOOL->getStringValue_json(options, "name");
    const char* widgetName = name?name:"default";
    widget->setName(widgetName);
    float x = DICTOOL->getFloatValue_json(options, "x");
    float y = DICTOOL->getFloatValue_json(options, "y");
    widget->setPosition(ccp(x,y));
    bool sx = DICTOOL->checkObjectExist_json(options, "scaleX");
    if (sx)
    {
        widget->setScaleX(DICTOOL->getFloatValue_json(options, "scaleX"));
    }
    bool sy = DICTOOL->checkObjectExist_json(options, "scaleY");
    if (sy)
    {
        widget->setScaleY(DICTOOL->getFloatValue_json(options, "scaleY"));
    }
    bool rt = DICTOOL->checkObjectExist_json(options, "rotation");
    if (rt)
    {
        widget->setRotation(DICTOOL->getFloatValue_json(options, "rotation"));
    }
    bool vb = DICTOOL->checkObjectExist_json(options, "visible");
    if (vb)
    {
        widget->setVisible(DICTOOL->getBooleanValue_json(options, "visible"));
    }
    int z = DICTOOL->getIntValue_json(options, "ZOrder");
    widget->setZOrder(z);
}

void WidgetPropertiesReader0250::setColorPropsForWidgetFromJsonDictionary(UIWidget *widget, const rapidjson::Value& options)
{
    bool op = DICTOOL->checkObjectExist_json(options, "opacity");
    if (op)
    {
        widget->setOpacity(DICTOOL->getIntValue_json(options, "opacity"));
    }
    bool cr = DICTOOL->checkObjectExist_json(options, "colorR");
    bool cg = DICTOOL->checkObjectExist_json(options, "colorG");
    bool cb = DICTOOL->checkObjectExist_json(options, "colorB");
    int colorR = cr ? DICTOOL->getIntValue_json(options, "colorR") : 255;
    int colorG = cg ? DICTOOL->getIntValue_json(options, "colorG") : 255;
    int colorB = cb ? DICTOOL->getIntValue_json(options, "colorB") : 255;
    widget->setColor(ccc3(colorR, colorG, colorB));
    bool apx = DICTOOL->checkObjectExist_json(options, "anchorPointX");
    float apxf = apx ? DICTOOL->getFloatValue_json(options, "anchorPointX") : ((widget->getWidgetType() == WidgetTypeWidget) ? 0.5f : 0.0f);
    bool apy = DICTOOL->checkObjectExist_json(options, "anchorPointY");
    float apyf = apy ? DICTOOL->getFloatValue_json(options, "anchorPointY") : ((widget->getWidgetType() == WidgetTypeWidget) ? 0.5f : 0.0f);
    widget->setAnchorPoint(ccp(apxf, apyf));
    bool flipX = DICTOOL->getBooleanValue_json(options, "flipX");
    bool flipY = DICTOOL->getBooleanValue_json(options, "flipY");
    widget->setFlipX(flipX);
    widget->setFlipY(flipY);
}

void WidgetPropertiesReader0250::setPropsForButtonFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UIButton* button = (UIButton*)widget;
    bool scale9Enable = DICTOOL->getBooleanValue_json(options, "scale9Enable");
    button->setScale9Enabled(scale9Enable);
    
    std::string tp_n = m_strFilePath;
    std::string tp_p = m_strFilePath;
    std::string tp_d = m_strFilePath;
    
    const char* normalFileName = DICTOOL->getStringValue_json(options, "normal");
    const char* pressedFileName = DICTOOL->getStringValue_json(options, "pressed");
    const char* disabledFileName = DICTOOL->getStringValue_json(options, "disabled");
    
    const char* normalFileName_tp = (normalFileName && (strcmp(normalFileName, "") != 0))?tp_n.append(normalFileName).c_str():NULL;
    const char* pressedFileName_tp = (pressedFileName && (strcmp(pressedFileName, "") != 0))?tp_p.append(pressedFileName).c_str():NULL;
    const char* disabledFileName_tp =  (disabledFileName && (strcmp(disabledFileName, "") != 0))?tp_d.append(disabledFileName).c_str():NULL;
    bool useMergedTexture = DICTOOL->getBooleanValue_json(options, "useMergedTexture");
    if (scale9Enable)
    {
        float cx = DICTOOL->getFloatValue_json(options, "capInsetsX");
        float cy = DICTOOL->getFloatValue_json(options, "capInsetsY");
        float cw = DICTOOL->getFloatValue_json(options, "capInsetsWidth");
        float ch = DICTOOL->getFloatValue_json(options, "capInsetsHeight");
        
        if (useMergedTexture)
        {
            button->loadTextures(normalFileName, pressedFileName, disabledFileName,UI_TEX_TYPE_PLIST);
        }
        else
        {
            button->loadTextures(normalFileName_tp, pressedFileName_tp, disabledFileName_tp);
        }
        button->setCapInsets(CCRectMake(cx, cy, cw, ch));
        bool sw = DICTOOL->checkObjectExist_json(options, "scale9Width");
        bool sh = DICTOOL->checkObjectExist_json(options, "scale9Height");
        if (sw && sh)
        {
            float swf = DICTOOL->getFloatValue_json(options, "scale9Width");
            float shf = DICTOOL->getFloatValue_json(options, "scale9Height");
            button->setSize(CCSizeMake(swf, shf));
        }
    }
    else
    {
        if (useMergedTexture)
        {
            button->loadTextures(normalFileName, pressedFileName, disabledFileName,UI_TEX_TYPE_PLIST);
        }
        else
        {
            button->loadTextures(normalFileName_tp, pressedFileName_tp, disabledFileName_tp);
        }
    }
    bool tt = DICTOOL->checkObjectExist_json(options, "text");
    if (tt)
    {
        const char* text = DICTOOL->getStringValue_json(options, "text");
        if (text)
        {
            button->setTitleText(text);
        }
    }
    bool cr = DICTOOL->checkObjectExist_json(options, "textColorR");
    bool cg = DICTOOL->checkObjectExist_json(options, "textColorG");
    bool cb = DICTOOL->checkObjectExist_json(options, "textColorB");
    int cri = cr?DICTOOL->getIntValue_json(options, "textColorR"):255;
    int cgi = cg?DICTOOL->getIntValue_json(options, "textColorG"):255;
    int cbi = cb?DICTOOL->getIntValue_json(options, "textColorB"):255;
    button->setTitleColor(ccc3(cri,cgi,cbi));
    bool fs = DICTOOL->checkObjectExist_json(options, "fontSize");
    if (fs)
    {
        button->setTitleFontSize(DICTOOL->getIntValue_json(options, "fontSize"));
    }
    bool fn = DICTOOL->checkObjectExist_json(options, "fontName");
    if (fn)
    {
        button->setTitleFontName(DICTOOL->getStringValue_json(options, "fontName"));
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0250::setPropsForCheckBoxFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UICheckBox* checkBox = (UICheckBox*)widget;
    const char* backGroundFileName = DICTOOL->getStringValue_json(options, "backGroundBox");
    const char* backGroundSelectedFileName = DICTOOL->getStringValue_json(options, "backGroundBoxSelected");
    const char* frontCrossFileName = DICTOOL->getStringValue_json(options, "frontCross");
    const char* backGroundDisabledFileName = DICTOOL->getStringValue_json(options, "backGroundBoxDisabled");
    const char* frontCrossDisabledFileName = DICTOOL->getStringValue_json(options, "frontCrossDisabled");
    
    
    std::string tp_b = m_strFilePath;
    std::string tp_bs = m_strFilePath;
    std::string tp_c = m_strFilePath;
    std::string tp_bd = m_strFilePath;
    std::string tp_cd = m_strFilePath;
    
    const char* backGroundFileName_tp = (backGroundFileName && (strcmp(backGroundFileName, "") != 0))?tp_b.append(backGroundFileName).c_str():NULL;
    const char* backGroundSelectedFileName_tp = (backGroundSelectedFileName && (strcmp(backGroundSelectedFileName, "") != 0))?tp_bs.append(backGroundSelectedFileName).c_str():NULL;
    const char* frontCrossFileName_tp = (frontCrossFileName && (strcmp(frontCrossFileName, "") != 0))?tp_c.append(frontCrossFileName).c_str():NULL;
    const char* backGroundDisabledFileName_tp = (backGroundDisabledFileName && (strcmp(backGroundDisabledFileName, "") != 0))?tp_bd.append(backGroundDisabledFileName).c_str():NULL;
    const char* frontCrossDisabledFileName_tp = (frontCrossDisabledFileName && (strcmp(frontCrossDisabledFileName, "") != 0))?tp_cd.append(frontCrossDisabledFileName).c_str():NULL;
    bool useMergedTexture = DICTOOL->getBooleanValue_json(options, "useMergedTexture");
    
    if (useMergedTexture)
    {
        checkBox->loadTextures(backGroundFileName, backGroundSelectedFileName, frontCrossFileName,backGroundDisabledFileName,frontCrossDisabledFileName,UI_TEX_TYPE_PLIST);
    }
    else
    {
        checkBox->loadTextures(backGroundFileName_tp, backGroundSelectedFileName_tp, frontCrossFileName_tp,backGroundDisabledFileName_tp,frontCrossDisabledFileName_tp);
    }
    
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0250::setPropsForImageViewFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    
    UIImageView* imageView = (UIImageView*)widget;
    const char* imageFileName = DICTOOL->getStringValue_json(options, "fileName");
    bool scale9EnableExist = DICTOOL->checkObjectExist_json(options, "scale9Enable");
    bool scale9Enable = false;
    if (scale9EnableExist)
    {
        scale9Enable = DICTOOL->getBooleanValue_json(options, "scale9Enable");
    }
    imageView->setScale9Enabled(scale9Enable);
    
    std::string tp_i = m_strFilePath;
    const char* imageFileName_tp = NULL;
    if (imageFileName && (strcmp(imageFileName, "") != 0))
    {
        imageFileName_tp = tp_i.append(imageFileName).c_str();
    }
    
    bool useMergedTexture = DICTOOL->getBooleanValue_json(options, "useMergedTexture");
    if (scale9Enable)
    {
        if (useMergedTexture)
        {
            imageView->loadTexture(imageFileName,UI_TEX_TYPE_PLIST);
        }
        else
        {
            imageView->loadTexture(imageFileName_tp);
        }
        
        bool sw = DICTOOL->checkObjectExist_json(options, "scale9Width");
        bool sh = DICTOOL->checkObjectExist_json(options, "scale9Height");
        if (sw && sh)
        {
            float swf = DICTOOL->getFloatValue_json(options, "scale9Width");
            float shf = DICTOOL->getFloatValue_json(options, "scale9Height");
            imageView->setSize(CCSizeMake(swf, shf));
        }
        
        float cx = DICTOOL->getFloatValue_json(options, "capInsetsX");
        float cy = DICTOOL->getFloatValue_json(options, "capInsetsY");
        float cw = DICTOOL->getFloatValue_json(options, "capInsetsWidth");
        float ch = DICTOOL->getFloatValue_json(options, "capInsetsHeight");
        imageView->setCapInsets(CCRectMake(cx, cy, cw, ch));
        
    }
    else
    {
        if (useMergedTexture)
        {
            imageView->loadTexture(imageFileName,UI_TEX_TYPE_PLIST);
        }
        else
        {
            imageView->loadTexture(imageFileName_tp);
        }
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0250::setPropsForLabelFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UILabel* label = (UILabel*)widget;
    bool touchScaleChangeAble = DICTOOL->getBooleanValue_json(options, "touchScaleEnable");
    label->setTouchScaleChangeAble(touchScaleChangeAble);
    const char* text = DICTOOL->getStringValue_json(options, "text");
    label->setText(text);
    bool fs = DICTOOL->checkObjectExist_json(options, "fontSize");
    if (fs)
    {
        label->setFontSize(DICTOOL->getIntValue_json(options, "fontSize"));
    }
    bool fn = DICTOOL->checkObjectExist_json(options, "fontName");
    if (fn)
    {
        label->setFontName(DICTOOL->getStringValue_json(options, "fontName"));
    }
    bool aw = DICTOOL->checkObjectExist_json(options, "areaWidth");
    bool ah = DICTOOL->checkObjectExist_json(options, "areaHeight");
    if (aw && ah)
    {
        CCSize size = CCSize(DICTOOL->getFloatValue_json(options, "areaWidth"),DICTOOL->getFloatValue_json(options,"areaHeight"));
        label->setTextAreaSize(size);
    }
    bool ha = DICTOOL->checkObjectExist_json(options, "hAlignment");
    if (ha)
    {
        label->setTextHorizontalAlignment((CCTextAlignment)DICTOOL->getIntValue_json(options, "hAlignment"));
    }
    bool va = DICTOOL->checkObjectExist_json(options, "vAlignment");
    if (va)
    {
        label->setTextVerticalAlignment((CCVerticalTextAlignment)DICTOOL->getIntValue_json(options, "vAlignment"));
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0250::setPropsForLabelAtlasFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UILabelAtlas* labelAtlas = (UILabelAtlas*)widget;
    bool sv = DICTOOL->checkObjectExist_json(options, "stringValue");
    bool cmf = DICTOOL->checkObjectExist_json(options, "charMapFile");
    bool iw = DICTOOL->checkObjectExist_json(options, "itemWidth");
    bool ih = DICTOOL->checkObjectExist_json(options, "itemHeight");
    bool scm = DICTOOL->checkObjectExist_json(options, "startCharMap");
    if (sv && cmf && iw && ih && scm && (strcmp(DICTOOL->getStringValue_json(options, "charMapFile"), "") != 0))
    {
        std::string tp_c = m_strFilePath;
        const char* cmf_tp = NULL;
        const char* cmft = DICTOOL->getStringValue_json(options, "charMapFile");
        cmf_tp = tp_c.append(cmft).c_str();
        
        labelAtlas->setProperty(DICTOOL->getStringValue_json(options, "stringValue"),cmf_tp,DICTOOL->getIntValue_json(options, "itemWidth"),DICTOOL->getIntValue_json(options,"itemHeight"),DICTOOL->getStringValue_json(options, "startCharMap"));
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0250::setPropsForLayoutFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UIContainerWidget* containerWidget = (UIContainerWidget*)widget;
    if (!dynamic_cast<UIScrollView*>(containerWidget)
        && !dynamic_cast<UIListView*>(containerWidget))
    {
        containerWidget->setClippingEnable(DICTOOL->getBooleanValue_json(options, "clipAble"));
    }
    Layout* panel = (Layout*)widget;
    bool backGroundScale9Enable = DICTOOL->getBooleanValue_json(options, "backGroundScale9Enable");
    panel->setBackGroundImageScale9Enabled(backGroundScale9Enable);
    int cr = DICTOOL->getIntValue_json(options, "bgColorR");
    int cg = DICTOOL->getIntValue_json(options, "bgColorG");
    int cb = DICTOOL->getIntValue_json(options, "bgColorB");
    
    int scr = DICTOOL->getIntValue_json(options, "bgStartColorR");
    int scg = DICTOOL->getIntValue_json(options, "bgStartColorG");
    int scb = DICTOOL->getIntValue_json(options, "bgStartColorB");
    
    int ecr = DICTOOL->getIntValue_json(options, "bgEndColorR");
    int ecg = DICTOOL->getIntValue_json(options, "bgEndColorG");
    int ecb = DICTOOL->getIntValue_json(options, "bgEndColorB");
    
    float bgcv1 = DICTOOL->getFloatValue_json(options, "vectorX");
    float bgcv2 = DICTOOL->getFloatValue_json(options, "vectorY");
    panel->setBackGroundColorVector(ccp(bgcv1, bgcv2));
    
    int co = DICTOOL->getIntValue_json(options, "bgColorOpacity");
    
    int colorType = DICTOOL->getIntValue_json(options, "colorType");
    panel->setBackGroundColorType(PanelColorType(colorType));
    panel->setBackGroundColor(ccc3(scr, scg, scb),ccc3(ecr, ecg, ecb));
    panel->setBackGroundColor(ccc3(cr, cg, cb));
    panel->setBackGroundColorOpacity(co);
    
    std::string tp_b = m_strFilePath;
    const char* imageFileName = DICTOOL->getStringValue_json(options, "backGroundImage");
    const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0))?tp_b.append(imageFileName).c_str():NULL;
    bool useMergedTexture = DICTOOL->getBooleanValue_json(options, "useMergedTexture");
    if (backGroundScale9Enable)
    {
        float cx = DICTOOL->getFloatValue_json(options, "capInsetsX");
        float cy = DICTOOL->getFloatValue_json(options, "capInsetsY");
        float cw = DICTOOL->getFloatValue_json(options, "capInsetsWidth");
        float ch = DICTOOL->getFloatValue_json(options, "capInsetsHeight");
        if (useMergedTexture)
        {
            panel->setBackGroundImage(imageFileName,UI_TEX_TYPE_PLIST);
        }
        else
        {
            panel->setBackGroundImage(imageFileName_tp);
        }
        panel->setBackGroundImageCapInsets(CCRectMake(cx, cy, cw, ch));
    }
    else
    {
        
        if (useMergedTexture)
        {
            panel->setBackGroundImage(imageFileName,UI_TEX_TYPE_PLIST);
        }
        else
        {
            panel->setBackGroundImage(imageFileName_tp);
        }
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0250::setPropsForScrollViewFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForLayoutFromJsonDictionary(widget, options);
    UIScrollView* scrollView = (UIScrollView*)widget;
    float innerWidth = DICTOOL->getFloatValue_json(options, "innerWidth");
    float innerHeight = DICTOOL->getFloatValue_json(options, "innerHeight");
    scrollView->setInnerContainerSize(CCSizeMake(innerWidth, innerHeight));
	int direction = DICTOOL->getFloatValue_json(options, "direction");
	scrollView->setDirection((SCROLLVIEW_DIR)direction);
    scrollView->setBounceEnabled(DICTOOL->getBooleanValue_json(options, "bounceEnable"));
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0250::setPropsForSliderFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UISlider* slider = (UISlider*)widget;
    
    bool barTextureScale9Enable = DICTOOL->getBooleanValue_json(options, "barTextureScale9Enable");
    slider->setScale9Enabled(barTextureScale9Enable);
    bool bt = DICTOOL->checkObjectExist_json(options, "barFileName");
    float barLength = DICTOOL->getFloatValue_json(options, "length");
    bool useMergedTexture = DICTOOL->getBooleanValue_json(options, "useMergedTexture");
    if (bt)
    {
        if (barTextureScale9Enable)
        {
            std::string tp_b = m_strFilePath;
            const char* imageFileName = DICTOOL->getStringValue_json(options, "barFileName");
            const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0))?tp_b.append(imageFileName).c_str():NULL;
            if (useMergedTexture)
            {
                slider->loadBarTexture(imageFileName,UI_TEX_TYPE_PLIST);
            }
            else
            {
                slider->loadBarTexture(imageFileName_tp);
            }
            slider->setSize(CCSizeMake(barLength, slider->getContentSize().height));
        }
        else
        {
            std::string tp_b = m_strFilePath;
            const char* imageFileName = DICTOOL->getStringValue_json(options, "barFileName");
            const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0))?tp_b.append(imageFileName).c_str():NULL;
            if (useMergedTexture)
            {
                slider->loadBarTexture(imageFileName,UI_TEX_TYPE_PLIST);
            }
            else
            {
                slider->loadBarTexture(imageFileName_tp);
            }
        }
    }
    std::string tp_n = m_strFilePath;
    std::string tp_p = m_strFilePath;
    std::string tp_d = m_strFilePath;
    
    const char* normalFileName = DICTOOL->getStringValue_json(options, "ballNormal");
    const char* pressedFileName = DICTOOL->getStringValue_json(options, "ballPressed");
    const char* disabledFileName = DICTOOL->getStringValue_json(options, "ballDisabled");
    
    const char* normalFileName_tp = (normalFileName && (strcmp(normalFileName, "") != 0))?tp_n.append(normalFileName).c_str():NULL;
    const char* pressedFileName_tp = (pressedFileName && (strcmp(pressedFileName, "") != 0))?tp_p.append(pressedFileName).c_str():NULL;
    const char* disabledFileName_tp =  (disabledFileName && (strcmp(disabledFileName, "") != 0))?tp_d.append(disabledFileName).c_str():NULL;
    if (useMergedTexture)
    {
        slider->loadSlidBallTextures(normalFileName,pressedFileName,disabledFileName,UI_TEX_TYPE_PLIST);
    }
    else
    {
        slider->loadSlidBallTextures(normalFileName_tp,pressedFileName_tp,disabledFileName_tp);
    }
    slider->setPercent(DICTOOL->getIntValue_json(options, "percent"));

    std::string tp_b = m_strFilePath;
    const char* imageFileName = DICTOOL->getStringValue_json(options, "progressBarFileName");
    const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0))?tp_b.append(imageFileName).c_str():NULL;
    if (useMergedTexture)
    {
        slider->loadProgressBarTexture(imageFileName, UI_TEX_TYPE_PLIST);
    }
    else
    {
        slider->loadProgressBarTexture(imageFileName_tp);
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0250::setPropsForTextFieldFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UITextField* textField = (UITextField*)widget;
    bool ph = DICTOOL->checkObjectExist_json(options, "placeHolder");
    if (ph)
    {
        textField->setPlaceHolder(DICTOOL->getStringValue_json(options, "placeHolder"));
    }
    textField->setText(DICTOOL->getStringValue_json(options, "text"));
    bool fs = DICTOOL->checkObjectExist_json(options, "fontSize");
    if (fs)
    {
        textField->setFontSize(DICTOOL->getIntValue_json(options, "fontSize"));
    }
    bool fn = DICTOOL->checkObjectExist_json(options, "fontName");
    if (fn)
    {
        textField->setFontName(DICTOOL->getStringValue_json(options, "fontName"));
    }
    bool tsw = DICTOOL->checkObjectExist_json(options, "touchSizeWidth");
    bool tsh = DICTOOL->checkObjectExist_json(options, "touchSizeHeight");
    if (tsw && tsh)
    {
        textField->setTouchSize(CCSizeMake(DICTOOL->getFloatValue_json(options, "touchSizeWidth"), DICTOOL->getFloatValue_json(options,"touchSizeHeight")));
    }
    
    float dw = DICTOOL->getFloatValue_json(options, "width");
    float dh = DICTOOL->getFloatValue_json(options, "height");
    if (dw > 0.0f || dh > 0.0f)
    {
        //textField->setSize(CCSizeMake(dw, dh));
    }
	bool maxLengthEnable = DICTOOL->getBooleanValue_json(options, "maxLengthEnable");
	textField->setMaxLengthEnabled(maxLengthEnable);

	if (maxLengthEnable)
	{
		int maxLength = DICTOOL->getIntValue_json(options, "maxLength");
		textField->setMaxLength(maxLength);
	}
    bool passwordEnable = DICTOOL->getBooleanValue_json(options, "passwordEnable");
    textField->setPasswordEnable(passwordEnable);
    if (passwordEnable)
    {
        textField->setPasswordStyleText(DICTOOL->getStringValue_json(options, "passwordStyleText"));
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0250::setPropsForLoadingBarFromJsonDictionary(UIWidget *widget, const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UILoadingBar* loadingBar = (UILoadingBar*)widget;
    bool useMergedTexture = DICTOOL->getBooleanValue_json(options, "useMergedTexture");
    std::string tp_b = m_strFilePath;
    const char*imageFileName =  DICTOOL->getStringValue_json(options, "texture");
    const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0))?tp_b.append(imageFileName).c_str():NULL;
    if (useMergedTexture)
    {
        loadingBar->loadTexture(imageFileName,UI_TEX_TYPE_PLIST);
    }
    else
    {
        loadingBar->loadTexture(imageFileName_tp);
    }
    loadingBar->setDirection(LoadingBarType(DICTOOL->getIntValue_json(options, "direction")));
    loadingBar->setPercent(DICTOOL->getIntValue_json(options, "percent"));
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0250::setPropsForLabelBMFontFromJsonDictionary(UIWidget *widget, const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    
    UILabelBMFont* labelBMFont = (UILabelBMFont*)widget;
    
    std::string tp_c = m_strFilePath;
    const char* cmf_tp = NULL;
    const char* cmft = DICTOOL->getStringValue_json(options, "fileName");
    cmf_tp = tp_c.append(cmft).c_str();
    
    labelBMFont->setFntFile(cmf_tp);
    
    const char* text = DICTOOL->getStringValue_json(options, "text");
    labelBMFont->setText(text);
    
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}


/*0.3.0.0~1.0.0.0*/
UIWidget* WidgetPropertiesReader0300::createWidget(const rapidjson::Value& data, const char* fullPath, const char* fileName)
{
    m_strFilePath = fullPath;

    int texturesCount = DICTOOL->getArrayCount_json(data, "textures");
    
    for (int i=0; i<texturesCount; i++)
    {
        const char* file = DICTOOL->getStringValueFromArray_json(data, "textures", i);
        std::string tp = fullPath;
        tp.append(file);
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(tp.c_str());
    }
    float fileDesignWidth = DICTOOL->getFloatValue_json(data, "designWidth");
    float fileDesignHeight = DICTOOL->getFloatValue_json(data, "designHeight");
    if (fileDesignWidth <= 0 || fileDesignHeight <= 0) {
        printf("Read design size error!\n");
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        GUIReader::shareReader()->storeFileDesignSize(fileName, winSize);
    }
    else
    {
        GUIReader::shareReader()->storeFileDesignSize(fileName, CCSizeMake(fileDesignWidth, fileDesignHeight));
    }
    const rapidjson::Value& widgetTree = DICTOOL->getSubDictionary_json(data, "widgetTree");
    UIWidget* widget = widgetFromJsonDictionary(widgetTree);
    
    /* *********temp********* */
    if (widget->getContentSize().equals(CCSizeZero))
    {
        UIContainerWidget* rootWidget = dynamic_cast<UIContainerWidget*>(widget);
        rootWidget->setSize(CCSizeMake(fileDesignWidth, fileDesignHeight));
    }
    /* ********************** */
    
    //    widget->setFileDesignSize(CCSizeMake(fileDesignWidth, fileDesignHeight));
    const rapidjson::Value& actions = DICTOOL->getSubDictionary_json(data, "animation");
    /* *********temp********* */
    //    ActionManager::shareManager()->releaseActions();
    /* ********************** */
    CCLOG("file name == [%s]",fileName);
	CCObject* rootWidget = (CCObject*) widget;
    ActionManager::shareManager()->initWithDictionary(fileName,actions,rootWidget);
    return widget;
}

UIWidget* WidgetPropertiesReader0300::widgetFromJsonDictionary(const rapidjson::Value& data)
{
    UIWidget* widget = NULL;
    const char* classname = DICTOOL->getStringValue_json(data, "classname");
    const rapidjson::Value& uiOptions = DICTOOL->getSubDictionary_json(data, "options");
    if (classname && strcmp(classname, "Button") == 0)
    {
        widget = UIButton::create();
        setPropsForButtonFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "CheckBox") == 0)
    {
        widget = UICheckBox::create();
        setPropsForCheckBoxFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "Label") == 0)
    {
        widget = UILabel::create();
        setPropsForLabelFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "LabelAtlas") == 0)
    {
        widget = UILabelAtlas::create();
        setPropsForLabelAtlasFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "LoadingBar") == 0)
    {
        widget = UILoadingBar::create();
        setPropsForLoadingBarFromJsonDictionary(widget, uiOptions);
    }else if (classname && strcmp(classname, "ScrollView") == 0){
        widget = UIScrollView::create();
        setPropsForScrollViewFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "TextArea") == 0)
    {
        widget = UILabel::create();
        setPropsForLabelFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "TextButton") == 0)
    {
        widget = UIButton::create();
        setPropsForButtonFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "TextField") == 0)
    {
        widget = UITextField::create();
        setPropsForTextFieldFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "ImageView") == 0)
    {
        widget = UIImageView::create();
        setPropsForImageViewFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "Panel") == 0)
    {
        widget = UILayout::create();
        setPropsForLayoutFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "Slider") == 0)
    {
        widget = UISlider::create();
        setPropsForSliderFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "LabelBMFont") == 0)
    {
        widget = UILabelBMFont::create();
        setPropsForLabelBMFontFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "DragPanel") == 0)
    {
        widget = UIScrollView::create();
        setPropsForScrollViewFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "ListView") == 0)
    {
        widget = UIListViewEx::create();
        setPropsForListViewFromJsonDictionary(widget, uiOptions);
    }
    else if (classname && strcmp(classname, "PageView") == 0)
    {
		/*modified by hanyi start here*/
        widget = UIPageViewEx::create();
        setPropsForPageViewFromJsonDictionary(widget, uiOptions);
//		widget = UIPageViewEx::create();
//		setPropsForPageViewFromJsonDictionary(widget, uiOptions);
		/*modified by hanyi end here*/
    }
    
    int childrenCount = DICTOOL->getArrayCount_json(data, "children");
    for (int i=0;i<childrenCount;i++)
    {
        const rapidjson::Value& subData = DICTOOL->getDictionaryFromArray_json(data, "children", i);
        UIWidget* child = widgetFromJsonDictionary(subData);
        if (child)
        {
            widget->addChild(child);
        }
    }
    
    UILayout* layout = dynamic_cast<UILayout*>(widget);
    if (layout)
    {
        layout->doLayout();
    }
    return widget;
}

void WidgetPropertiesReader0300::setPropsForWidgetFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    bool ignoreSizeExsit = DICTOOL->checkObjectExist_json(options, "ignoreSize");
    if (ignoreSizeExsit)
    {
        widget->ignoreContentAdaptWithSize(DICTOOL->getBooleanValue_json(options, "ignoreSize"));
    }
    
    widget->setSizeType((SizeType)DICTOOL->getIntValue_json(options, "sizeType"));
    widget->setPositionType((PositionType)DICTOOL->getIntValue_json(options, "positionType"));
    
    widget->setSizePercent(ccp(DICTOOL->getFloatValue_json(options, "sizePercentX"), DICTOOL->getFloatValue_json(options, "sizePercentY")));
    widget->setPositionPercent(ccp(DICTOOL->getFloatValue_json(options, "positionPercentX"), DICTOOL->getFloatValue_json(options, "positionPercentY")));
    
    float w = DICTOOL->getFloatValue_json(options, "width");
    float h = DICTOOL->getFloatValue_json(options, "height");
    widget->setSize(CCSizeMake(w, h));
    
    widget->setTag(DICTOOL->getIntValue_json(options, "tag"));
	widget->setActionTag(DICTOOL->getIntValue_json(options, "actiontag"));
    widget->setTouchEnabled(DICTOOL->getBooleanValue_json(options, "touchAble"));
    const char* name = DICTOOL->getStringValue_json(options, "name");
    const char* widgetName = name?name:"default";
    widget->setName(widgetName);
    float x = DICTOOL->getFloatValue_json(options, "x");
    float y = DICTOOL->getFloatValue_json(options, "y");
    widget->setPosition(ccp(x,y));
    bool sx = DICTOOL->checkObjectExist_json(options, "scaleX");
    if (sx)
    {
        widget->setScaleX(DICTOOL->getFloatValue_json(options, "scaleX"));
    }
    bool sy = DICTOOL->checkObjectExist_json(options, "scaleY");
    if (sy)
    {
        widget->setScaleY(DICTOOL->getFloatValue_json(options, "scaleY"));
    }
    bool rt = DICTOOL->checkObjectExist_json(options, "rotation");
    if (rt)
    {
        widget->setRotation(DICTOOL->getFloatValue_json(options, "rotation"));
    }
    bool vb = DICTOOL->checkObjectExist_json(options, "visible");
    if (vb)
    {
        widget->setVisible(DICTOOL->getBooleanValue_json(options, "visible"));
    }
    int z = DICTOOL->getIntValue_json(options, "ZOrder");
    widget->setZOrder(z);
    
	bool layout = DICTOOL->checkObjectExist_json(options, "layoutParameter");
	if (layout)
	{
		const rapidjson::Value& layoutParameterDic = DICTOOL->getSubDictionary_json(options, "layoutParameter");
		int paramType = DICTOOL->getIntValue_json(layoutParameterDic, "type");
		UILayoutParameter* parameter = NULL;
		switch (paramType)
		{
		case 0:
			break;
		case 1:
			{
				parameter = UILinearLayoutParameter::create();
				int gravity = DICTOOL->getIntValue_json(layoutParameterDic, "gravity");
				((UILinearLayoutParameter*)parameter)->setGravity((UILinearGravity)gravity);
				break;
			}
		case 2:
			{
				parameter = UIRelativeLayoutParameter::create();
				UIRelativeLayoutParameter* rParameter = (UIRelativeLayoutParameter*)parameter;
				const char* relativeName = DICTOOL->getStringValue_json(layoutParameterDic, "relativeName");
				rParameter->setRelativeName(relativeName);
				const char* relativeToName = DICTOOL->getStringValue_json(layoutParameterDic, "relativeToName");
				rParameter->setRelativeToWidgetName(relativeToName);
				int align = DICTOOL->getIntValue_json(layoutParameterDic, "align");
				rParameter->setAlign((UIRelativeAlign)align);
				break;
			}
		default:
			break;
		}
		if (parameter)
		{
			float mgl = DICTOOL->getFloatValue_json(layoutParameterDic, "marginLeft");
			float mgt = DICTOOL->getFloatValue_json(layoutParameterDic, "marginTop");
			float mgr = DICTOOL->getFloatValue_json(layoutParameterDic, "marginRight");
			float mgb = DICTOOL->getFloatValue_json(layoutParameterDic, "marginDown");
			parameter->setMargin(UIMargin(mgl, mgt, mgr, mgb));
			widget->setLayoutParameter(parameter);
		}
	}
}

void WidgetPropertiesReader0300::setColorPropsForWidgetFromJsonDictionary(UIWidget *widget, const rapidjson::Value& options)
{
    bool op = DICTOOL->checkObjectExist_json(options, "opacity");
    if (op)
    {
        widget->setOpacity(DICTOOL->getIntValue_json(options, "opacity"));
    }
    bool cr = DICTOOL->checkObjectExist_json(options, "colorR");
    bool cg = DICTOOL->checkObjectExist_json(options, "colorG");
    bool cb = DICTOOL->checkObjectExist_json(options, "colorB");
    int colorR = cr ? DICTOOL->getIntValue_json(options, "colorR") : 255;
    int colorG = cg ? DICTOOL->getIntValue_json(options, "colorG") : 255;
    int colorB = cb ? DICTOOL->getIntValue_json(options, "colorB") : 255;
    widget->setColor(ccc3(colorR, colorG, colorB));
    bool apx = DICTOOL->checkObjectExist_json(options, "anchorPointX");
    float apxf = apx ? DICTOOL->getFloatValue_json(options, "anchorPointX") : ((widget->getWidgetType() == WidgetTypeWidget) ? 0.5f : 0.0f);
    bool apy = DICTOOL->checkObjectExist_json(options, "anchorPointY");
    float apyf = apy ? DICTOOL->getFloatValue_json(options, "anchorPointY") : ((widget->getWidgetType() == WidgetTypeWidget) ? 0.5f : 0.0f);
    widget->setAnchorPoint(ccp(apxf, apyf));
    bool flipX = DICTOOL->getBooleanValue_json(options, "flipX");
    bool flipY = DICTOOL->getBooleanValue_json(options, "flipY");
    widget->setFlipX(flipX);
    widget->setFlipY(flipY);
}

void WidgetPropertiesReader0300::setPropsForButtonFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UIButton* button = (UIButton*)widget;
    bool scale9Enable = DICTOOL->getBooleanValue_json(options, "scale9Enable");
    button->setScale9Enabled(scale9Enable);
    
    const rapidjson::Value& normalDic = DICTOOL->getSubDictionary_json(options, "normalData");
    int normalType = DICTOOL->getIntValue_json(normalDic, "resourceType");
    switch (normalType)
    {
        case 0:
        {
            std::string tp_n = m_strFilePath;
            const char* normalFileName = DICTOOL->getStringValue_json(normalDic, "path");
			// modify by hanyi start
//            const char* normalFileName_tp = (normalFileName && (strcmp(normalFileName, "") != 0))?tp_n.append(normalFileName).c_str():NULL;
//            button->loadTextureNormal(normalFileName_tp);
			button->loadTextureNormal(normalFileName);
			// modify by hanyi end here
            break;
        }
        case 1:
        {
            const char* normalFileName = DICTOOL->getStringValue_json(normalDic, "path");
            button->loadTextureNormal(normalFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    const rapidjson::Value& pressedDic = DICTOOL->getSubDictionary_json(options, "pressedData");
    int pressedType = DICTOOL->getIntValue_json(pressedDic, "resourceType");
    switch (pressedType)
    {
        case 0:
        {
            std::string tp_p = m_strFilePath;
            const char* pressedFileName = DICTOOL->getStringValue_json(pressedDic, "path");
			// modify by hanyi start
//			const char* pressedFileName_tp = (pressedFileName && (strcmp(pressedFileName, "") != 0)) ? tp_p.append(pressedFileName).c_str() : NULL;
//            button->loadTexturePressed(pressedFileName_tp);
			button->loadTexturePressed(pressedFileName);
			// modify by hanyi end
            break;
        }
        case 1:
        {
            const char* pressedFileName = DICTOOL->getStringValue_json(pressedDic, "path");
            button->loadTexturePressed(pressedFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    const rapidjson::Value& disabledDic = DICTOOL->getSubDictionary_json(options, "disabledData");
    int disabledType = DICTOOL->getIntValue_json(disabledDic, "resourceType");
    switch (disabledType)
    {
        case 0:
        {
            std::string tp_d = m_strFilePath;
            const char* disabledFileName = DICTOOL->getStringValue_json(disabledDic, "path");
			// modify by hanyi start
//            const char* disabledFileName_tp = (disabledFileName && (strcmp(disabledFileName, "") != 0))?tp_d.append(disabledFileName).c_str():NULL;
//            button->loadTextureDisabled(disabledFileName_tp);
			button->loadTextureDisabled(disabledFileName);
			// modify by hanyi end
            break;
        }
        case 1:
        {
            const char* disabledFileName = DICTOOL->getStringValue_json(disabledDic, "path");
            button->loadTextureDisabled(disabledFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    if (scale9Enable)
    {
        float cx = DICTOOL->getFloatValue_json(options, "capInsetsX");
        float cy = DICTOOL->getFloatValue_json(options, "capInsetsY");
        float cw = DICTOOL->getFloatValue_json(options, "capInsetsWidth");
        float ch = DICTOOL->getFloatValue_json(options, "capInsetsHeight");
        
        button->setCapInsets(CCRectMake(cx, cy, cw, ch));
        bool sw = DICTOOL->checkObjectExist_json(options, "scale9Width");
        bool sh = DICTOOL->checkObjectExist_json(options, "scale9Height");
        if (sw && sh)
        {
            float swf = DICTOOL->getFloatValue_json(options, "scale9Width");
            float shf = DICTOOL->getFloatValue_json(options, "scale9Height");
            button->setSize(CCSizeMake(swf, shf));
        }
    }
    bool tt = DICTOOL->checkObjectExist_json(options, "text");
    if (tt)
    {
        const char* text = DICTOOL->getStringValue_json(options, "text");
        if (text)
        {
            button->setTitleText(text);
        }
    }
    
    bool cr = DICTOOL->checkObjectExist_json(options, "textColorR");
    bool cg = DICTOOL->checkObjectExist_json(options, "textColorG");
    bool cb = DICTOOL->checkObjectExist_json(options, "textColorB");
    int cri = cr?DICTOOL->getIntValue_json(options, "textColorR"):255;
    int cgi = cg?DICTOOL->getIntValue_json(options, "textColorG"):255;
    int cbi = cb?DICTOOL->getIntValue_json(options, "textColorB"):255;
    button->setTitleColor(ccc3(cri,cgi,cbi));
    bool fs = DICTOOL->checkObjectExist_json(options, "fontSize");
    if (fs)
    {
        button->setTitleFontSize(DICTOOL->getIntValue_json(options, "fontSize"));
    }
    bool fn = DICTOOL->checkObjectExist_json(options, "fontName");
    if (fn)
    {
        button->setTitleFontName(DICTOOL->getStringValue_json(options, "fontName"));
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300::setPropsForCheckBoxFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UICheckBox* checkBox = (UICheckBox*)widget;
    
    const rapidjson::Value& backGroundDic = DICTOOL->getSubDictionary_json(options, "backGroundBoxData");
    int backGroundType = DICTOOL->getIntValue_json(backGroundDic, "resourceType");
    switch (backGroundType)
    {
        case 0:
        {
            std::string tp_b = m_strFilePath;
            const char* backGroundFileName = DICTOOL->getStringValue_json(backGroundDic, "path");
			// modify by hanyi start
//            const char* backGroundFileName_tp = (backGroundFileName && (strcmp(backGroundFileName, "") != 0))?tp_b.append(backGroundFileName).c_str():NULL;
//            checkBox->loadTextureBackGround(backGroundFileName_tp);
			checkBox->loadTextureBackGround(backGroundFileName);
			// modify by hanyi end
            break;
        }
        case 1:
        {
            const char* backGroundFileName = DICTOOL->getStringValue_json(backGroundDic, "path");
            checkBox->loadTextureBackGround(backGroundFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    
    const rapidjson::Value& backGroundSelectedDic = DICTOOL->getSubDictionary_json(options, "backGroundBoxSelectedData");
    int backGroundSelectedType = DICTOOL->getIntValue_json(backGroundSelectedDic, "resourceType");
    switch (backGroundSelectedType)
    {
        case 0:
        {
            std::string tp_bs = m_strFilePath;
            const char* backGroundSelectedFileName = DICTOOL->getStringValue_json(backGroundSelectedDic, "path");
			// modify by hanyi start
//            const char* backGroundSelectedFileName_tp = (backGroundSelectedFileName && (strcmp(backGroundSelectedFileName, "") != 0))?tp_bs.append(backGroundSelectedFileName).c_str():NULL;
//            checkBox->loadTextureBackGroundSelected(backGroundSelectedFileName_tp);
			checkBox->loadTextureBackGroundSelected(backGroundSelectedFileName);
			// modify by hanyi end
            break;
        }
        case 1:
        {
            const char* backGroundSelectedFileName = DICTOOL->getStringValue_json(backGroundSelectedDic, "path");
            checkBox->loadTextureBackGroundSelected(backGroundSelectedFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    
    const rapidjson::Value& frontCrossDic = DICTOOL->getSubDictionary_json(options, "frontCrossData");
    int frontCrossType = DICTOOL->getIntValue_json(frontCrossDic, "resourceType");
    switch (frontCrossType)
    {
        case 0:
        {
            std::string tp_c = m_strFilePath;
            const char* frontCrossFileName = DICTOOL->getStringValue_json(frontCrossDic, "path");
			// modify by hanyi start
//            const char* frontCrossFileName_tp = (frontCrossFileName && (strcmp(frontCrossFileName, "") != 0))?tp_c.append(frontCrossFileName).c_str():NULL;
//            checkBox->loadTextureFrontCross(frontCrossFileName_tp);
			checkBox->loadTextureFrontCross(frontCrossFileName);
			// modify by hanyi end
            break;
        }
        case 1:
        {
            const char* frontCrossFileName = DICTOOL->getStringValue_json(frontCrossDic, "path");
            checkBox->loadTextureFrontCross(frontCrossFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    
    const rapidjson::Value& backGroundDisabledDic = DICTOOL->getSubDictionary_json(options, "backGroundBoxDisabledData");
    int backGroundDisabledType = DICTOOL->getIntValue_json(backGroundDisabledDic, "resourceType");
    switch (backGroundDisabledType)
    {
        case 0:
        {
            std::string tp_bd = m_strFilePath;
            const char* backGroundDisabledFileName = DICTOOL->getStringValue_json(backGroundDisabledDic, "path");
			// modify by hanyi start
//            const char* backGroundDisabledFileName_tp = (backGroundDisabledFileName && (strcmp(backGroundDisabledFileName, "") != 0))?tp_bd.append(backGroundDisabledFileName).c_str():NULL;
//            checkBox->loadTextureBackGroundDisabled(backGroundDisabledFileName_tp);
			checkBox->loadTextureBackGroundDisabled(backGroundDisabledFileName);
			// modify by hanyi end
            break;
        }
        case 1:
        {
            const char* backGroundDisabledFileName = DICTOOL->getStringValue_json(backGroundDisabledDic, "path");
            checkBox->loadTextureBackGroundDisabled(backGroundDisabledFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    
    const rapidjson::Value& frontCrossDisabledDic = DICTOOL->getSubDictionary_json(options, "frontCrossDisabledData");
    int frontCrossDisabledType = DICTOOL->getIntValue_json(frontCrossDisabledDic, "resourceType");
    switch (frontCrossDisabledType)
    {
        case 0:
        {
            std::string tp_cd = m_strFilePath;
            const char* frontCrossDisabledFileName = DICTOOL->getStringValue_json(options, "path");
			// modify by hanyi start
//            const char* frontCrossDisabledFileName_tp = (frontCrossDisabledFileName && (strcmp(frontCrossDisabledFileName, "") != 0))?tp_cd.append(frontCrossDisabledFileName).c_str():NULL;
//            checkBox->loadTextureFrontCrossDisabled(frontCrossDisabledFileName_tp);
			checkBox->loadTextureFrontCrossDisabled(frontCrossDisabledFileName);
			// modify by hanyi end
            break;
        }
        case 1:
        {
            const char* frontCrossDisabledFileName = DICTOOL->getStringValue_json(options, "path");
            checkBox->loadTextureFrontCrossDisabled(frontCrossDisabledFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300::setPropsForImageViewFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    DictionaryHelper* dicHelper = DICTOOL;
    setPropsForWidgetFromJsonDictionary(widget, options);
    
    UIImageView* imageView = (UIImageView*)widget;
    
    const rapidjson::Value& imageFileNameDic = DICTOOL->getSubDictionary_json(options, "fileNameData");
    int imageFileNameType = DICTOOL->getIntValue_json(imageFileNameDic, "resourceType");
    switch (imageFileNameType)
    {
        case 0:
        {
            std::string tp_i = m_strFilePath;
            const char* imageFileName = DICTOOL->getStringValue_json(imageFileNameDic, "path");
            const char* imageFileName_tp = NULL;
            if (imageFileName && (strcmp(imageFileName, "") != 0))
            {
				// modify by hanyi start
//                imageFileName_tp = tp_i.append(imageFileName).c_str();
//                imageView->loadTexture(imageFileName_tp);
				imageView->loadTexture(imageFileName);
				// modify by hanyi end
            }
            break;
        }
        case 1:
        {
            const char* imageFileName = DICTOOL->getStringValue_json(imageFileNameDic, "path");
            imageView->loadTexture(imageFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    
    bool scale9EnableExist = DICTOOL->checkObjectExist_json(options, "scale9Enable");
    bool scale9Enable = false;
    if (scale9EnableExist)
    {
        scale9Enable = DICTOOL->getBooleanValue_json(options, "scale9Enable");
    }
    imageView->setScale9Enabled(scale9Enable);
    
    
    if (scale9Enable)
    {
        bool sw = DICTOOL->checkObjectExist_json(options, "scale9Width");
        bool sh = DICTOOL->checkObjectExist_json(options, "scale9Height");
        if (sw && sh)
        {
            float swf = DICTOOL->getFloatValue_json(options, "scale9Width");
            float shf = DICTOOL->getFloatValue_json(options, "scale9Height");
            imageView->setSize(CCSizeMake(swf, shf));
        }
        
        float cx = DICTOOL->getFloatValue_json(options, "capInsetsX");
        float cy = DICTOOL->getFloatValue_json(options, "capInsetsY");
        float cw = DICTOOL->getFloatValue_json(options, "capInsetsWidth");
        float ch = DICTOOL->getFloatValue_json(options, "capInsetsHeight");
        
        imageView->setCapInsets(CCRectMake(cx, cy, cw, ch));
        
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300::setPropsForLabelFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UILabel* label = (UILabel*)widget;
    bool touchScaleChangeAble = DICTOOL->getBooleanValue_json(options, "touchScaleEnable");
    label->setTouchScaleChangeAble(touchScaleChangeAble);
    const char* text = DICTOOL->getStringValue_json(options, "text");
    label->setText(text);
    bool fs = DICTOOL->checkObjectExist_json(options, "fontSize");
    if (fs)
    {
        label->setFontSize(DICTOOL->getIntValue_json(options, "fontSize"));
    }
    bool fn = DICTOOL->checkObjectExist_json(options, "fontName");
    if (fn)
    {
        label->setFontName(DICTOOL->getStringValue_json(options, "fontName"));
    }
    bool aw = DICTOOL->checkObjectExist_json(options, "areaWidth");
    bool ah = DICTOOL->checkObjectExist_json(options, "areaHeight");
    if (aw && ah)
    {
        CCSize size = CCSize(DICTOOL->getFloatValue_json(options, "areaWidth"),DICTOOL->getFloatValue_json(options,"areaHeight"));
        label->setTextAreaSize(size);
    }
    bool ha = DICTOOL->checkObjectExist_json(options, "hAlignment");
    if (ha)
    {
        label->setTextHorizontalAlignment((CCTextAlignment)DICTOOL->getIntValue_json(options, "hAlignment"));
    }
    bool va = DICTOOL->checkObjectExist_json(options, "vAlignment");
    if (va)
    {
        label->setTextVerticalAlignment((CCVerticalTextAlignment)DICTOOL->getIntValue_json(options, "vAlignment"));
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300::setPropsForLabelAtlasFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UILabelAtlas* labelAtlas = (UILabelAtlas*)widget;
    bool sv = DICTOOL->checkObjectExist_json(options, "stringValue");
    bool cmf = DICTOOL->checkObjectExist_json(options, "charMapFile");
    bool iw = DICTOOL->checkObjectExist_json(options, "itemWidth");
    bool ih = DICTOOL->checkObjectExist_json(options, "itemHeight");
    bool scm = DICTOOL->checkObjectExist_json(options, "startCharMap");
    if (sv && cmf && iw && ih && scm)
    {
        const rapidjson::Value& cmftDic = DICTOOL->getSubDictionary_json(options, "charMapFileData");
        int cmfType = DICTOOL->getIntValue_json(cmftDic, "resourceType");
        switch (cmfType)
        {
            case 0:
            {
                std::string tp_c = m_strFilePath;
                const char* cmfPath = DICTOOL->getStringValue_json(cmftDic, "path");
				// modify by hanyi start
//                const char* cmf_tp = tp_c.append(cmfPath).c_str();
//                labelAtlas->setProperty(DICTOOL->getStringValue_json(options, "stringValue"),cmf_tp,DICTOOL->getIntValue_json(options, "itemWidth"),DICTOOL->getIntValue_json(options,"itemHeight"),DICTOOL->getStringValue_json(options, "startCharMap"));
                labelAtlas->setProperty(DICTOOL->getStringValue_json(options, "stringValue"),cmfPath,DICTOOL->getIntValue_json(options, "itemWidth"),DICTOOL->getIntValue_json(options,"itemHeight"),DICTOOL->getStringValue_json(options, "startCharMap"));
				// modify by hanyi end
                break;
            }
            case 1:
                CCLOG("Wrong res type of LabelAtlas!");
                break;
            default:
                break;
        }
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300::setPropsForLayoutFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    Layout* panel = (Layout*)widget;
    if (!dynamic_cast<UIScrollView*>(widget)
        && !dynamic_cast<UIListView*>(widget))
    {
        panel->setClippingEnable(DICTOOL->getBooleanValue_json(options, "clipAble"));
    }
    bool backGroundScale9Enable = DICTOOL->getBooleanValue_json(options, "backGroundScale9Enable");
    panel->setBackGroundImageScale9Enabled(backGroundScale9Enable);
    int cr = DICTOOL->getIntValue_json(options, "bgColorR");
    int cg = DICTOOL->getIntValue_json(options, "bgColorG");
    int cb = DICTOOL->getIntValue_json(options, "bgColorB");
    
    int scr = DICTOOL->getIntValue_json(options, "bgStartColorR");
    int scg = DICTOOL->getIntValue_json(options, "bgStartColorG");
    int scb = DICTOOL->getIntValue_json(options, "bgStartColorB");
    
    int ecr = DICTOOL->getIntValue_json(options, "bgEndColorR");
    int ecg = DICTOOL->getIntValue_json(options, "bgEndColorG");
    int ecb = DICTOOL->getIntValue_json(options, "bgEndColorB");
    
    float bgcv1 = DICTOOL->getFloatValue_json(options, "vectorX");
    float bgcv2 = DICTOOL->getFloatValue_json(options, "vectorY");
    panel->setBackGroundColorVector(ccp(bgcv1, bgcv2));
    
    int co = DICTOOL->getIntValue_json(options, "bgColorOpacity");
    
    int colorType = DICTOOL->getIntValue_json(options, "colorType");
    panel->setBackGroundColorType(PanelColorType(colorType));
    panel->setBackGroundColor(ccc3(scr, scg, scb),ccc3(ecr, ecg, ecb));
    panel->setBackGroundColor(ccc3(cr, cg, cb));
    panel->setBackGroundColorOpacity(co);
    
    
    const rapidjson::Value& imageFileNameDic = DICTOOL->getSubDictionary_json(options, "backGroundImageData");
    int imageFileNameType = DICTOOL->getIntValue_json(imageFileNameDic, "resourceType");
    switch (imageFileNameType)
    {
        case 0:
        {
            std::string tp_b = m_strFilePath;
            const char* imageFileName = DICTOOL->getStringValue_json(imageFileNameDic, "path");
			// modify by hanyi start
//            const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0))?tp_b.append(imageFileName).c_str():NULL;
//            panel->setBackGroundImage(imageFileName_tp);
			panel->setBackGroundImage(imageFileName);
			// modify by hanyi end
            break;
        }
        case 1:
        {
            const char* imageFileName = DICTOOL->getStringValue_json(imageFileNameDic, "path");
            panel->setBackGroundImage(imageFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    
    if (backGroundScale9Enable)
    {
        float cx = DICTOOL->getFloatValue_json(options, "capInsetsX");
        float cy = DICTOOL->getFloatValue_json(options, "capInsetsY");
        float cw = DICTOOL->getFloatValue_json(options, "capInsetsWidth");
        float ch = DICTOOL->getFloatValue_json(options, "capInsetsHeight");
        panel->setBackGroundImageCapInsets(CCRectMake(cx, cy, cw, ch));
    }
    panel->setLayoutType((LayoutType)DICTOOL->getIntValue_json(options, "layoutType"));
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300::setPropsForScrollViewFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForLayoutFromJsonDictionary(widget, options);
    UIScrollView* scrollView = (UIScrollView*)widget;
    float innerWidth = DICTOOL->getFloatValue_json(options, "innerWidth");
    float innerHeight = DICTOOL->getFloatValue_json(options, "innerHeight");
    scrollView->setInnerContainerSize(CCSizeMake(innerWidth, innerHeight));
	int direction = DICTOOL->getFloatValue_json(options, "direction");
	scrollView->setDirection((SCROLLVIEW_DIR)direction);
    scrollView->setBounceEnabled(DICTOOL->getBooleanValue_json(options, "bounceEnable"));
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300::setPropsForSliderFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UISlider* slider = (UISlider*)widget;
    
    bool barTextureScale9Enable = DICTOOL->getBooleanValue_json(options, "barTextureScale9Enable");
    slider->setScale9Enabled(barTextureScale9Enable);
    bool bt = DICTOOL->checkObjectExist_json(options, "barFileName");
    float barLength = DICTOOL->getFloatValue_json(options, "length");
    if (bt)
    {
        if (barTextureScale9Enable)
        {
            
            const rapidjson::Value& imageFileNameDic = DICTOOL->getSubDictionary_json(options, "barFileNameData");
            int imageFileType = DICTOOL->getIntValue_json(imageFileNameDic, "resourceType");
            switch (imageFileType)
            {
                case 0:
                {
                    std::string tp_b = m_strFilePath;
                    const char* imageFileName = DICTOOL->getStringValue_json(imageFileNameDic, "path");
					// modify by hanyi start
//					const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0)) ? tp_b.append(imageFileName).c_str() : NULL;
//                    slider->loadBarTexture(imageFileName_tp);
					slider->loadBarTexture(imageFileName);
					// modify by hanyi end
                    break;
                }
                case 1:
                {
                    const char* imageFileName =  DICTOOL->getStringValue_json(imageFileNameDic, "path");
                    slider->loadBarTexture(imageFileName,UI_TEX_TYPE_PLIST);
                    break;
                }
                default:
                    break;
            }
            
            slider->setSize(CCSizeMake(barLength, slider->getContentSize().height));
        }
        else
        {
            const rapidjson::Value& imageFileNameDic = DICTOOL->getSubDictionary_json(options, "barFileNameData");
            int imageFileType = DICTOOL->getIntValue_json(imageFileNameDic, "resourceType");
            switch (imageFileType)
            {
                case 0:
                {
                    std::string tp_b = m_strFilePath;
                    const char*imageFileName =  DICTOOL->getStringValue_json(imageFileNameDic, "path");
					// modify by hanyi start
//                    const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0))?tp_b.append(imageFileName).c_str():NULL;
//                    slider->loadBarTexture(imageFileName_tp);
					slider->loadBarTexture(imageFileName);
					// modify by hanyi end
                    break;
                }
                case 1:
                {
                    const char*imageFileName =  DICTOOL->getStringValue_json(imageFileNameDic, "path");
                    slider->loadBarTexture(imageFileName,UI_TEX_TYPE_PLIST);
                    break;
                }
                default:
                    break;
            }
        }
    }
    
    const rapidjson::Value& normalDic = DICTOOL->getSubDictionary_json(options, "ballNormalData");
    int normalType = DICTOOL->getIntValue_json(normalDic, "resourceType");
    switch (normalType)
    {
        case 0:
        {
            std::string tp_n = m_strFilePath;
            const char* normalFileName = DICTOOL->getStringValue_json(normalDic, "path");
			// modify by hanyi start
//            const char* normalFileName_tp = (normalFileName && (strcmp(normalFileName, "") != 0))?tp_n.append(normalFileName).c_str():NULL;
//            slider->loadSlidBallTextureNormal(normalFileName_tp);
			slider->loadSlidBallTextureNormal(normalFileName);
			// modify by hanyi end
            break;
        }
        case 1:
        {
            const char* normalFileName = DICTOOL->getStringValue_json(normalDic, "path");
            slider->loadSlidBallTextureNormal(normalFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    
    const rapidjson::Value& pressedDic = DICTOOL->getSubDictionary_json(options, "ballPressedData");
    int pressedType = DICTOOL->getIntValue_json(pressedDic, "resourceType");
    switch (pressedType)
    {
        case 0:
        {
            std::string tp_p = m_strFilePath;
            const char* pressedFileName = DICTOOL->getStringValue_json(pressedDic, "path");
			// modify by hanyi start
//            const char* pressedFileName_tp = (pressedFileName && (strcmp(pressedFileName, "") != 0))?tp_p.append(pressedFileName).c_str():NULL;
//            slider->loadSlidBallTexturePressed(pressedFileName_tp);
			slider->loadSlidBallTexturePressed(pressedFileName);
			// modify by hanyi end
            break;
        }
        case 1:
        {
            const char* pressedFileName = DICTOOL->getStringValue_json(pressedDic, "path");
            slider->loadSlidBallTexturePressed(pressedFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    
    const rapidjson::Value& disabledDic = DICTOOL->getSubDictionary_json(options, "ballDisabledData");
    int disabledType = DICTOOL->getIntValue_json(disabledDic, "resourceType");
    switch (disabledType)
    {
        case 0:
        {
            std::string tp_d = m_strFilePath;
            const char* disabledFileName = DICTOOL->getStringValue_json(disabledDic, "path");
			// modify by hanyi start
//            const char* disabledFileName_tp = (disabledFileName && (strcmp(disabledFileName, "") != 0))?tp_d.append(disabledFileName).c_str():NULL;
//            slider->loadSlidBallTextureDisabled(disabledFileName_tp);
			slider->loadSlidBallTextureDisabled(disabledFileName);
			// modify by hanyi end
            break;
        }
        case 1:
        {
            const char* disabledFileName = DICTOOL->getStringValue_json(disabledDic, "path");
            slider->loadSlidBallTextureDisabled(disabledFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    
    slider->setPercent(DICTOOL->getIntValue_json(options, "percent"));
    
    const rapidjson::Value& progressBarDic = DICTOOL->getSubDictionary_json(options, "progressBarData");
    int progressBarType = DICTOOL->getIntValue_json(progressBarDic, "resourceType");
    switch (progressBarType)
    {
        case 0:
        {
            std::string tp_b = m_strFilePath;
            const char* imageFileName = DICTOOL->getStringValue_json(progressBarDic, "path");
			// modify by hanyi start
//            const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0))?tp_b.append(imageFileName).c_str():NULL;
//            slider->loadProgressBarTexture(imageFileName_tp);
			slider->loadProgressBarTexture(imageFileName);
			// modify by hanyi end
            break;
        }
        case 1:
        {
            const char* imageFileName = DICTOOL->getStringValue_json(progressBarDic, "path");
            slider->loadProgressBarTexture(imageFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300::setPropsForTextFieldFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UITextField* textField = (UITextField*)widget;
    bool ph = DICTOOL->checkObjectExist_json(options, "placeHolder");
    if (ph)
    {
        textField->setPlaceHolder(DICTOOL->getStringValue_json(options, "placeHolder"));
    }
    textField->setText(DICTOOL->getStringValue_json(options, "text"));
    bool fs = DICTOOL->checkObjectExist_json(options, "fontSize");
    if (fs)
    {
        textField->setFontSize(DICTOOL->getIntValue_json(options, "fontSize"));
    }
    bool fn = DICTOOL->checkObjectExist_json(options, "fontName");
    if (fn)
    {
        textField->setFontName(DICTOOL->getStringValue_json(options, "fontName"));
    }
    bool tsw = DICTOOL->checkObjectExist_json(options, "touchSizeWidth");
    bool tsh = DICTOOL->checkObjectExist_json(options, "touchSizeHeight");
    if (tsw && tsh)
    {
        textField->setTouchSize(CCSizeMake(DICTOOL->getFloatValue_json(options, "touchSizeWidth"), DICTOOL->getFloatValue_json(options,"touchSizeHeight")));
    }
    
    float dw = DICTOOL->getFloatValue_json(options, "width");
    float dh = DICTOOL->getFloatValue_json(options, "height");
    if (dw > 0.0f || dh > 0.0f)
    {
        //textField->setSize(CCSizeMake(dw, dh));
    }
	bool maxLengthEnable = DICTOOL->getBooleanValue_json(options, "maxLengthEnable");
	textField->setMaxLengthEnabled(maxLengthEnable);
    
	if (maxLengthEnable)
	{
		int maxLength = DICTOOL->getIntValue_json(options, "maxLength");
		textField->setMaxLength(maxLength);
	}
    bool passwordEnable = DICTOOL->getBooleanValue_json(options, "passwordEnable");
    textField->setPasswordEnable(passwordEnable);
    if (passwordEnable)
    {
        textField->setPasswordStyleText(DICTOOL->getStringValue_json(options, "passwordStyleText"));
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300::setPropsForLoadingBarFromJsonDictionary(UIWidget *widget, const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UILoadingBar* loadingBar = (UILoadingBar*)widget;
    
    const rapidjson::Value& imageFileNameDic = DICTOOL->getSubDictionary_json(options, "textureData");
    int imageFileNameType = DICTOOL->getIntValue_json(imageFileNameDic, "resourceType");
    switch (imageFileNameType)
    {
        case 0:
        {
            std::string tp_i = m_strFilePath;
            const char* imageFileName = DICTOOL->getStringValue_json(imageFileNameDic, "path");
            const char* imageFileName_tp = NULL;
            if (imageFileName && (strcmp(imageFileName, "") != 0))
            {
				// modify by hanyi start
//                imageFileName_tp = tp_i.append(imageFileName).c_str();
//                loadingBar->loadTexture(imageFileName_tp);
				loadingBar->loadTexture(imageFileName);
				// modify by hanyi end
            }
            break;
        }
        case 1:
        {
            const char* imageFileName = DICTOOL->getStringValue_json(imageFileNameDic, "path");
            loadingBar->loadTexture(imageFileName,UI_TEX_TYPE_PLIST);
            break;
        }
        default:
            break;
    }
    
    /* gui mark add load bar scale9 parse */
    bool scale9Enable = DICTOOL->getBooleanValue_json(options, "scale9Enable");
    loadingBar->setScale9Enabled(scale9Enable);
    
    if (scale9Enable)
    {
        float cx = DICTOOL->getFloatValue_json(options, "capInsetsX");
        float cy = DICTOOL->getFloatValue_json(options, "capInsetsY");
        float cw = DICTOOL->getFloatValue_json(options, "capInsetsWidth");
        float ch = DICTOOL->getFloatValue_json(options, "capInsetsHeight");
        
        loadingBar->setCapInsets(CCRectMake(cx, cy, cw, ch));
        
        float width = DICTOOL->getFloatValue_json(options, "width");
        float height = DICTOOL->getFloatValue_json(options, "height");
        loadingBar->setSize(CCSizeMake(width, height));
    }
    /**/
    
    loadingBar->setDirection(LoadingBarType(DICTOOL->getIntValue_json(options, "direction")));
    loadingBar->setPercent(DICTOOL->getIntValue_json(options, "percent"));
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300::setPropsForLabelBMFontFromJsonDictionary(UIWidget *widget, const rapidjson::Value& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    
    UILabelBMFont* labelBMFont = (UILabelBMFont*)widget;
    
    const rapidjson::Value& cmftDic = DICTOOL->getSubDictionary_json(options, "fileNameData");
    int cmfType = DICTOOL->getIntValue_json(cmftDic, "resourceType");
    switch (cmfType)
    {
        case 0:
        {
            std::string tp_c = m_strFilePath;
            const char* cmfPath = DICTOOL->getStringValue_json(cmftDic, "path");
			// modify by hanyi start
//            const char* cmf_tp = tp_c.append(cmfPath).c_str();
//            labelBMFont->setFntFile(cmf_tp);
			labelBMFont->setFntFile(cmfPath);
			// modify by hanyi end
            break;
        }
        case 1:
            CCLOG("Wrong res type of LabelAtlas!");
            break;
        default:
            break;
    }
    
    const char* text = DICTOOL->getStringValue_json(options, "text");
    labelBMFont->setText(text);
    
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300::setPropsForPageViewFromJsonDictionary(UIWidget*widget,const rapidjson::Value& options)
{
	/*modified by hanyi start here*/
//	setPropsForLayoutFromJsonDictionary(widget, options);
	setPropsForLayoutFromJsonDictionary(widget, options);

	UIPageViewEx* listView = (UIPageViewEx*)widget;

	float innerWidth = DICTOOL->getFloatValue_json(options, "innerWidth");
	float innerHeight = DICTOOL->getFloatValue_json(options, "innerHeight");
	listView->setInnerContainerSize(CCSizeMake(innerWidth, innerHeight));
	int direction = DICTOOL->getFloatValue_json(options, "direction");
	listView->setDirection((SCROLLVIEW_DIR)direction);

	ListViewGravity gravity = (ListViewGravity)DICTOOL->getIntValue_json(options, "gravity");
	listView->setGravity(gravity);

	float itemMargin = DICTOOL->getFloatValue_json(options, "itemMargin");
	listView->setItemsMargin(itemMargin);
	/*modified by hanyi end here*/
}

void WidgetPropertiesReader0300::setPropsForListViewFromJsonDictionary(UIWidget* widget, const rapidjson::Value& options)
{
	setPropsForLayoutFromJsonDictionary(widget, options);

	UIListViewEx* listView = (UIListViewEx*)widget;

	float innerWidth = DICTOOL->getFloatValue_json(options, "innerWidth");
	float innerHeight = DICTOOL->getFloatValue_json(options, "innerHeight");
	listView->setInnerContainerSize(CCSizeMake(innerWidth, innerHeight));
	int direction = DICTOOL->getFloatValue_json(options, "direction");
	listView->setDirection((SCROLLVIEW_DIR)direction);

	ListViewGravity gravity = (ListViewGravity)DICTOOL->getIntValue_json(options, "gravity");
	listView->setGravity(gravity);

	float itemMargin = DICTOOL->getFloatValue_json(options, "itemMargin");
	listView->setItemsMargin(itemMargin);
}


// add by weixin begin
/*0.3.0.0~1.0.0.0*/
UIWidget* WidgetPropertiesReader0300Lua::createWidget(cs::CSJsonDictionary& data, const char* fullPath, const char* fileName)
{
    m_strFilePath = fullPath;
    
    int texturesCount = data.getArrayItemCount("textures");
    {cocos2d::TimeElapse te("load ui textures total");
    for (int i=0; i<texturesCount; i++)
    {
        const char* file = data.getStringValueFromArray("textures", i);
        std::string tp = fullPath;
        tp.append(file);
        cocos2d::CCLog("load ui texture: %s", tp.c_str());
        cocos2d::TimeElapse te("load ui texture");
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(tp.c_str());
    }}
    float fileDesignWidth = data.getItemFloatValue("designWidth", 0.0);
    float fileDesignHeight = data.getItemFloatValue("designHeight", 0.0);
    if (fileDesignWidth <= 0 || fileDesignHeight <= 0) {
        printf("Read design size error!\n");
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        GUIReader::shareReader()->storeFileDesignSize(fileName, winSize);
    }
    else
    {
        GUIReader::shareReader()->storeFileDesignSize(fileName, CCSizeMake(fileDesignWidth, fileDesignHeight));
    }
    if (cs::CSJsonDictionary* pWidgetTree = data.getSubDictionary("widgetTree"))
    {
        cocos2d::TimeElapse te("create widget tree");
        UIWidget* widget = widgetFromJsonDictionary(*pWidgetTree);
        delete pWidgetTree;
        pWidgetTree = NULL;
        
        
        /* *********temp********* */
        if (widget->getContentSize().equals(CCSizeZero))
        {
            UIContainerWidget* rootWidget = dynamic_cast<UIContainerWidget*>(widget);
            rootWidget->setSize(CCSizeMake(fileDesignWidth, fileDesignHeight));
        }
        /* ********************** */
        
        //    widget->setFileDesignSize(CCSizeMake(fileDesignWidth, fileDesignHeight));
        cs::CSJsonDictionary* pActions = data.getSubDictionary("animation");
        /* *********temp********* */
        //    ActionManager::shareManager()->releaseActions();
        /* ********************** */
        CCLOG("file name == [%s]",fileName);
        CCObject* rootWidget = (CCObject*) widget;
        //    ActionManager::shareManager()->initWithDictionary(fileName,actions,rootWidget); // TODO: actions->*pActions
        delete pActions;
        return widget;
    }
    return NULL;
}

UIWidget* WidgetPropertiesReader0300Lua::widgetFromJsonDictionary(cs::CSJsonDictionary& data)
{
    UIWidget* widget = NULL;
    const char* classname = data.getItemStringValue("classname");
    if (cs::CSJsonDictionary* pUiOptions = data.getSubDictionary("options"))
    {
        if (classname && strcmp(classname, "Button") == 0)
        {
            widget = UIButton::create();
            setPropsForButtonFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "CheckBox") == 0)
        {
            widget = UICheckBox::create();
            setPropsForCheckBoxFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "Label") == 0)
        {
            widget = UILabel::create();
            setPropsForLabelFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "LabelAtlas") == 0)
        {
            widget = UILabelAtlas::create();
            setPropsForLabelAtlasFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "LoadingBar") == 0)
        {
            widget = UILoadingBar::create();
            setPropsForLoadingBarFromJsonDictionary(widget, *pUiOptions);
        }else if (classname && strcmp(classname, "ScrollView") == 0){
            widget = UIScrollView::create();
            setPropsForScrollViewFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "TextArea") == 0)
        {
            widget = UILabel::create();
            setPropsForLabelFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "TextButton") == 0)
        {
            widget = UIButton::create();
            setPropsForButtonFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "TextField") == 0)
        {
            widget = UITextField::create();
            setPropsForTextFieldFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "ImageView") == 0)
        {
            widget = UIImageView::create();
            setPropsForImageViewFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "Panel") == 0)
        {
            widget = UILayout::create();
            setPropsForLayoutFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "Slider") == 0)
        {
            widget = UISlider::create();
            setPropsForSliderFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "LabelBMFont") == 0)
        {
            widget = UILabelBMFont::create();
            setPropsForLabelBMFontFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "DragPanel") == 0)
        {
            widget = UIScrollView::create();
            setPropsForScrollViewFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "ListView") == 0)
        {
            widget = UIListViewEx::create();
            setPropsForListViewFromJsonDictionary(widget, *pUiOptions);
        }
        else if (classname && strcmp(classname, "PageView") == 0)
        {
            /*modified by hanyi start here*/
            widget = UIPageViewEx::create();
            setPropsForPageViewFromJsonDictionary(widget, *pUiOptions);
            //		widget = UIPageViewEx::create();
            //		setPropsForPageViewFromJsonDictionary(widget, uiOptions);
            /*modified by hanyi end here*/
        }
        delete pUiOptions;
        pUiOptions = NULL;
    }
    
    int childrenCount = data.getArrayItemCount("children");
    for (int i=0;i<childrenCount;i++)
    {
        if (cs::CSJsonDictionary* pSubData = data.getSubItemFromArray("children", i))
        {
            UIWidget* child = widgetFromJsonDictionary(*pSubData);
            delete pSubData;
            if (child)
            {
                widget->addChild(child);
            }
        }
    }
    
    UILayout* layout = dynamic_cast<UILayout*>(widget);
    if (layout)
    {
        layout->doLayout();
    }
    return widget;
}

void WidgetPropertiesReader0300Lua::setPropsForWidgetFromJsonDictionary(UIWidget*widget, cs::CSJsonDictionary& options)
{
    bool ignoreSizeExsit = options.isKeyValidate("ignoreSize");
    if (ignoreSizeExsit)
    {
        widget->ignoreContentAdaptWithSize(options.getItemBoolvalue("ignoreSize", false));
    }
    
    widget->setSizeType((SizeType)options.getItemIntValue("sizeType", SIZE_ABSOLUTE));
    widget->setPositionType((PositionType)options.getItemIntValue("positionType", POSITION_ABSOLUTE));
    
    widget->setSizePercent(ccp(options.getItemFloatValue("sizePercentX", 1.0), options.getItemFloatValue("positionPercentY", 1.0)));
    widget->setPositionPercent(ccp(options.getItemFloatValue("positionPercentX", 1.0), options.getItemFloatValue("positionPercentY", 1.0)));
    
    float w = options.getItemFloatValue("width", 0.0);
    float h = options.getItemFloatValue("height", 0.0);
    widget->setSize(CCSizeMake(w, h));
    
    widget->setTag(options.getItemIntValue("tag", 0));
	widget->setActionTag(options.getItemIntValue("actiontag", 0));
    widget->setTouchEnabled(options.getItemBoolvalue("touchAble", false));
    const char* name = options.getItemStringValue("name");
    const char* widgetName = name?name:"default";
    widget->setName(widgetName);
    float x = options.getItemFloatValue("x", 0.0);
    float y = options.getItemFloatValue("y", 0.0);
    widget->setPosition(ccp(x,y));
    bool sx = options.isKeyValidate("scaleX");
    if (sx)
    {
        widget->setScaleX(options.getItemFloatValue("scaleX", 0.0));
    }
    bool sy = options.isKeyValidate("scaleY");
    if (sy)
    {
        widget->setScaleY(options.getItemFloatValue("scaleY", 0.0));
    }
    bool rt = options.isKeyValidate("rotation");
    if (rt)
    {
        widget->setRotation(options.getItemFloatValue("rotation", 0.0));
    }
    bool vb = options.isKeyValidate("visible");
    if (vb)
    {
        widget->setVisible(options.getItemBoolvalue("visible", true));
    }
    int z = options.getItemIntValue("ZOrder", 0);
    widget->setZOrder(z);
    
	bool layout = options.isKeyValidate("layoutParameter");
	if (layout)
	{
        if (cs::CSJsonDictionary* pLayoutParameterDic = options.getSubDictionary("layoutParameter"))
        {
            int paramType = pLayoutParameterDic->getItemIntValue("type", 0);
            UILayoutParameter* parameter = NULL;
            switch (paramType)
            {
                case 0:
                    break;
                case 1:
                {
                    parameter = UILinearLayoutParameter::create();
                    int gravity = pLayoutParameterDic->getItemIntValue("gravity", 0);
                    ((UILinearLayoutParameter*)parameter)->setGravity((UILinearGravity)gravity);
                    break;
                }
                case 2:
                {
                    parameter = UIRelativeLayoutParameter::create();
                    UIRelativeLayoutParameter* rParameter = (UIRelativeLayoutParameter*)parameter;
                    const char* relativeName = pLayoutParameterDic->getItemStringValue("relativeName");
                    rParameter->setRelativeName(relativeName);
                    const char* relativeToName = pLayoutParameterDic->getItemStringValue("relativeToName");
                    rParameter->setRelativeToWidgetName(relativeToName);
                    int align = pLayoutParameterDic->getItemIntValue("align", 0);
                    rParameter->setAlign((UIRelativeAlign)align);
                    break;
                }
                default:
                    break;
            }
            if (parameter)
            {
                float mgl = pLayoutParameterDic->getItemFloatValue("marginLeft", 0.0);
                float mgt = pLayoutParameterDic->getItemFloatValue("marginTop", 0.0);
                float mgr = pLayoutParameterDic->getItemFloatValue("marginRight", 0.0);
                float mgb = pLayoutParameterDic->getItemFloatValue("marginDown", 0.0);
                parameter->setMargin(UIMargin(mgl, mgt, mgr, mgb));
                widget->setLayoutParameter(parameter);
            }
            delete pLayoutParameterDic;
        }
	}
}

void WidgetPropertiesReader0300Lua::setColorPropsForWidgetFromJsonDictionary(UIWidget *widget, cs::CSJsonDictionary& options)
{
    bool op = options.isKeyValidate("opacity");
    if (op)
    {
        widget->setOpacity(options.getItemIntValue("opacity", 0));
    }
    bool cr = options.isKeyValidate("colorR");
    bool cg = options.isKeyValidate("colorG");
    bool cb = options.isKeyValidate("colorB");
    int colorR = cr ? options.getItemIntValue("colorR", 255) : 255;
    int colorG = cg ? options.getItemIntValue("colorG", 255) : 255;
    int colorB = cb ? options.getItemIntValue("colorB", 255) : 255;
    widget->setColor(ccc3(colorR, colorG, colorB));
    bool apx = options.isKeyValidate("anchorPointX");
    float apxf = apx ? options.getItemFloatValue("anchorPointX", 0.0) : ((widget->getWidgetType() == WidgetTypeWidget) ? 0.5f : 0.0f);
    bool apy = options.isKeyValidate("anchorPointY");
    float apyf = apy ? options.getItemFloatValue("anchorPointY", 0.0) : ((widget->getWidgetType() == WidgetTypeWidget) ? 0.5f : 0.0f);
    widget->setAnchorPoint(ccp(apxf, apyf));
    bool flipX = options.getItemFloatValue("flipX", 0.0);
    bool flipY = options.getItemFloatValue("flipY", 0.0);
    widget->setFlipX(flipX);
    widget->setFlipY(flipY);
}

void WidgetPropertiesReader0300Lua::setPropsForButtonFromJsonDictionary(UIWidget*widget, cs::CSJsonDictionary& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UIButton* button = (UIButton*)widget;
    bool scale9Enable = options.getItemBoolvalue("scale9Enable", false);
    button->setScale9Enabled(scale9Enable);
    
    if (cs::CSJsonDictionary* pNormalDic = options.getSubDictionary("normalData"))
    {
        int normalType = pNormalDic->getItemIntValue("resourceType", 0);
        switch (normalType)
        {
            case 0:
            {
                std::string tp_n = m_strFilePath;
                const char* normalFileName = pNormalDic->getItemStringValue("path");
                const char* normalFileName_tp = (normalFileName && (strcmp(normalFileName, "") != 0))?tp_n.append(normalFileName).c_str():NULL;
                button->loadTextureNormal(normalFileName_tp);
                break;
            }
            case 1:
            {
                const char* normalFileName = pNormalDic->getItemStringValue("path");
                button->loadTextureNormal(normalFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pNormalDic;
        pNormalDic = NULL;
    }
    
    if (cs::CSJsonDictionary* pPressedDic = options.getSubDictionary("pressedData"))
    {
        int pressedType = pPressedDic->getItemIntValue("resourceType", 0);
        switch (pressedType)
        {
            case 0:
            {
                std::string tp_p = m_strFilePath;
                const char* pressedFileName = pPressedDic->getItemStringValue("path");
                const char* pressedFileName_tp = (pressedFileName && (strcmp(pressedFileName, "") != 0))?tp_p.append(pressedFileName).c_str():NULL;
                button->loadTexturePressed(pressedFileName_tp);
                break;
            }
            case 1:
            {
                const char* pressedFileName = pPressedDic->getItemStringValue("path");
                button->loadTexturePressed(pressedFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pPressedDic;
        pPressedDic = NULL;
    }
    
    if (cs::CSJsonDictionary* pDisabledDic = options.getSubDictionary("disabledData"))
    {
        int disabledType = pDisabledDic->getItemIntValue("resourceType", 0);
        switch (disabledType)
        {
            case 0:
            {
                std::string tp_d = m_strFilePath;
                const char* disabledFileName = pDisabledDic->getItemStringValue("path");
                const char* disabledFileName_tp = (disabledFileName && (strcmp(disabledFileName, "") != 0))?tp_d.append(disabledFileName).c_str():NULL;
                button->loadTextureDisabled(disabledFileName_tp);
                break;
            }
            case 1:
            {
                const char* disabledFileName = pDisabledDic->getItemStringValue("path");
                button->loadTextureDisabled(disabledFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pDisabledDic;
        pDisabledDic = NULL;
    }
    
    if (scale9Enable)
    {
        float cx = options.getItemFloatValue("capInsetsX", 0.0);
        float cy = options.getItemFloatValue("capInsetsY", 0.0);
        float cw = options.getItemFloatValue("capInsetsWidth", 0.0);
        float ch = options.getItemFloatValue("capInsetsHeight", 0.0);
        
        button->setCapInsets(CCRectMake(cx, cy, cw, ch));
        bool sw = options.isKeyValidate("scale9Width");
        bool sh = options.isKeyValidate("scale9Height");
        if (sw && sh)
        {
            float swf = options.getItemFloatValue("scale9Width", 0.0);
            float shf = options.getItemFloatValue("scale9Height", 0.0);
            button->setSize(CCSizeMake(swf, shf));
        }
    }
    bool tt = options.isKeyValidate("text");
    if (tt)
    {
        const char* text = options.getItemStringValue("text");
        if (text)
        {
            button->setTitleText(text);
        }
    }
    
    bool cr = options.isKeyValidate("textColorR");
    bool cg = options.isKeyValidate("textColorG");
    bool cb = options.isKeyValidate("textColorB");
    int cri = cr?options.getItemIntValue("textColorR", 255):255;
    int cgi = cr?options.getItemIntValue("textColorG", 255):255;
    int cbi = cr?options.getItemIntValue("textColorB", 255):255;
    button->setTitleColor(ccc3(cri,cgi,cbi));
    bool fs = options.isKeyValidate("fontSize");
    if (fs)
    {
        button->setTitleFontSize(options.getItemIntValue("fontSize", 16));
    }
    bool fn = options.isKeyValidate("fontName");
    if (fn)
    {
        button->setTitleFontName(options.getItemStringValue("fontName"));
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300Lua::setPropsForCheckBoxFromJsonDictionary(UIWidget*widget, cs::CSJsonDictionary& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UICheckBox* checkBox = (UICheckBox*)widget;
    
    if (cs::CSJsonDictionary* pBackGroundDic = options.getSubDictionary("backGroundBoxData"))
    {
        int backGroundType = pBackGroundDic->getItemIntValue("resourceType", 0);
        switch (backGroundType)
        {
            case 0:
            {
                std::string tp_b = m_strFilePath;
                const char* backGroundFileName = pBackGroundDic->getItemStringValue("path");
                const char* backGroundFileName_tp = (backGroundFileName && (strcmp(backGroundFileName, "") != 0))?tp_b.append(backGroundFileName).c_str():NULL;
                checkBox->loadTextureBackGround(backGroundFileName_tp);
                break;
            }
            case 1:
            {
                const char* backGroundFileName = pBackGroundDic->getItemStringValue("path");
                checkBox->loadTextureBackGround(backGroundFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pBackGroundDic;
        pBackGroundDic = NULL;
    }
    
    if (cs::CSJsonDictionary* pBackGroundSelectedDic = options.getSubDictionary("backGroundBoxSelectedData"))
    {
        int backGroundSelectedType = pBackGroundSelectedDic->getItemIntValue("resourceType", 0);
        switch (backGroundSelectedType)
        {
            case 0:
            {
                std::string tp_bs = m_strFilePath;
                const char* backGroundSelectedFileName = pBackGroundSelectedDic->getItemStringValue("path");
                const char* backGroundSelectedFileName_tp = (backGroundSelectedFileName && (strcmp(backGroundSelectedFileName, "") != 0))?tp_bs.append(backGroundSelectedFileName).c_str():NULL;
                checkBox->loadTextureBackGroundSelected(backGroundSelectedFileName_tp);
                break;
            }
            case 1:
            {
                const char* backGroundSelectedFileName = pBackGroundSelectedDic->getItemStringValue("path");
                checkBox->loadTextureBackGroundSelected(backGroundSelectedFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pBackGroundSelectedDic;
        pBackGroundSelectedDic = NULL;
    }
    
    if (cs::CSJsonDictionary* pFrontCrossDic = options.getSubDictionary("frontCrossData"))
    {
        int frontCrossType = pFrontCrossDic->getItemIntValue("resourceType", 0);
        switch (frontCrossType)
        {
            case 0:
            {
                std::string tp_c = m_strFilePath;
                const char* frontCrossFileName = pFrontCrossDic->getItemStringValue("path");
                const char* frontCrossFileName_tp = (frontCrossFileName && (strcmp(frontCrossFileName, "") != 0))?tp_c.append(frontCrossFileName).c_str():NULL;
                checkBox->loadTextureFrontCross(frontCrossFileName_tp);
                break;
            }
            case 1:
            {
                const char* frontCrossFileName = pFrontCrossDic->getItemStringValue("path");
                checkBox->loadTextureFrontCross(frontCrossFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pFrontCrossDic;
        pFrontCrossDic = NULL;
    }
    
    if (cs::CSJsonDictionary* pBackGroundDisabledDic = options.getSubDictionary("backGroundBoxDisabledData"))
    {
        int backGroundDisabledType = pBackGroundDisabledDic->getItemIntValue("resourceType", 0);
        switch (backGroundDisabledType)
        {
            case 0:
            {
                std::string tp_bd = m_strFilePath;
                const char* backGroundDisabledFileName = pBackGroundDisabledDic->getItemStringValue("path");
                const char* backGroundDisabledFileName_tp = (backGroundDisabledFileName && (strcmp(backGroundDisabledFileName, "") != 0))?tp_bd.append(backGroundDisabledFileName).c_str():NULL;
                checkBox->loadTextureBackGroundDisabled(backGroundDisabledFileName_tp);
                break;
            }
            case 1:
            {
                const char* backGroundDisabledFileName = pBackGroundDisabledDic->getItemStringValue("path");
                checkBox->loadTextureBackGroundDisabled(backGroundDisabledFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pBackGroundDisabledDic;
        pBackGroundDisabledDic = NULL;
    }
    
    if (cs::CSJsonDictionary* pFrontCrossDisabledDic = options.getSubDictionary("frontCrossDisabledData"))
    {
        int frontCrossDisabledType = pFrontCrossDisabledDic->getItemIntValue("resourceType", 0);
        switch (frontCrossDisabledType)
        {
            case 0:
            {
                std::string tp_cd = m_strFilePath;
                const char* frontCrossDisabledFileName = pFrontCrossDisabledDic->getItemStringValue("path");
                const char* frontCrossDisabledFileName_tp = (frontCrossDisabledFileName && (strcmp(frontCrossDisabledFileName, "") != 0))?tp_cd.append(frontCrossDisabledFileName).c_str():NULL;
                checkBox->loadTextureFrontCrossDisabled(frontCrossDisabledFileName_tp);
                break;
            }
            case 1:
            {
                const char* frontCrossDisabledFileName = pFrontCrossDisabledDic->getItemStringValue("path");
                checkBox->loadTextureFrontCrossDisabled(frontCrossDisabledFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pFrontCrossDisabledDic;
        pFrontCrossDisabledDic = NULL;
    }
    
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300Lua::setPropsForImageViewFromJsonDictionary(UIWidget*widget, cs::CSJsonDictionary& options)
{
    DictionaryHelper* dicHelper = DICTOOL;
    setPropsForWidgetFromJsonDictionary(widget, options);
    
    UIImageView* imageView = (UIImageView*)widget;
    
    if (cs::CSJsonDictionary* pImageFileNameDic = options.getSubDictionary("fileNameData"))
    {
        int imageFileNameType = pImageFileNameDic->getItemIntValue("resourceType", 0);
        switch (imageFileNameType)
        {
            case 0:
            {
                std::string tp_i = m_strFilePath;
                const char* imageFileName = pImageFileNameDic->getItemStringValue("path");
                const char* imageFileName_tp = NULL;
                if (imageFileName && (strcmp(imageFileName, "") != 0))
                {
                    imageFileName_tp = tp_i.append(imageFileName).c_str();
                    imageView->loadTexture(imageFileName_tp);
                }
                break;
            }
            case 1:
            {
                const char* imageFileName = pImageFileNameDic->getItemStringValue("path");
                imageView->loadTexture(imageFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pImageFileNameDic;
        pImageFileNameDic = NULL;
    }
    
    bool scale9EnableExist = options.isKeyValidate("scale9Enable");
    bool scale9Enable = false;
    if (scale9EnableExist)
    {
        scale9Enable = options.getItemBoolvalue("scale9Enable", false);
    }
    imageView->setScale9Enabled(scale9Enable);
    
    
    if (scale9Enable)
    {
        bool sw = options.isKeyValidate("scale9Width");
        bool sh = options.isKeyValidate("scale9Height");
        if (sw && sh)
        {
            float swf = options.getItemFloatValue("scale9Width", 0.0);
            float shf = options.getItemFloatValue("scale9Height", 0.0);
            imageView->setSize(CCSizeMake(swf, shf));
        }
        
        float cx = options.getItemFloatValue("capInsetsX", 0.0);
        float cy = options.getItemFloatValue("capInsetsY", 0.0);
        float cw = options.getItemFloatValue("capInsetsWidth", 0.0);
        float ch = options.getItemFloatValue("capInsetsHeight", 0.0);
        
        imageView->setCapInsets(CCRectMake(cx, cy, cw, ch));
        
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300Lua::setPropsForLabelFromJsonDictionary(UIWidget*widget, cs::CSJsonDictionary& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UILabel* label = (UILabel*)widget;
    bool touchScaleChangeAble = options.getItemBoolvalue("touchScaleEnable", false);
    label->setTouchScaleChangeAble(touchScaleChangeAble);
    const char* text = options.getItemStringValue("text");
    label->setText(text);
    bool fs = options.isKeyValidate("fontSize");
    if (fs)
    {
        label->setFontSize(options.getItemIntValue("fontSize", 16));
    }
    bool fn = options.isKeyValidate("fontName");
    if (fn)
    {
        label->setFontName(options.getItemStringValue("fontName"));
    }
    bool aw = options.isKeyValidate("areaWidth");
    bool ah = options.isKeyValidate("areaHeight");
    if (aw && ah)
    {
        CCSize size = CCSize(options.getItemFloatValue("areaWidth", 0.0),options.getItemFloatValue("areaHeight", 0.0));
        label->setTextAreaSize(size);
    }
    bool ha = options.isKeyValidate("hAlignment");
    if (ha)
    {
        label->setTextHorizontalAlignment((CCTextAlignment)options.getItemIntValue("hAlignment", kCCTextAlignmentLeft));
    }
    bool va = options.isKeyValidate("vAlignment");
    if (va)
    {
        label->setTextVerticalAlignment((CCVerticalTextAlignment)options.getItemIntValue("vAlignment", kCCVerticalTextAlignmentTop));
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300Lua::setPropsForLabelAtlasFromJsonDictionary(UIWidget*widget, cs::CSJsonDictionary& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UILabelAtlas* labelAtlas = (UILabelAtlas*)widget;
    bool sv = options.isKeyValidate("stringValue");
    bool cmf = true;//options.isKeyValidate("charMapFile");
    bool iw = options.isKeyValidate("itemWidth");
    bool ih = options.isKeyValidate("itemHeight");
    bool scm = options.isKeyValidate("startCharMap");
    if (sv && cmf && iw && ih && scm)
    {
        if (cs::CSJsonDictionary* pCmftDic = options.getSubDictionary("charMapFileData"))
        {
            int cmfType = pCmftDic->getItemIntValue("resourceType", -1);
            switch (cmfType)
            {
                case 0:
                {
                    std::string tp_c = m_strFilePath;
                    const char* cmfPath = pCmftDic->getItemStringValue("path");
                    const char* cmf_tp = tp_c.append(cmfPath).c_str();
                    labelAtlas->setProperty(options.getItemStringValue("stringValue"), cmf_tp, options.getItemIntValue("itemWidth", 0), options.getItemIntValue("itemHeight", 0), options.getItemStringValue("startCharMap"));
                    break;
                }
                case 1:
                    CCLOG("Wrong res type of LabelAtlas!");
                    break;
                default:
                    break;
            }
            delete pCmftDic;
            pCmftDic = NULL;
        }
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300Lua::setPropsForLayoutFromJsonDictionary(UIWidget*widget, cs::CSJsonDictionary& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    Layout* panel = (Layout*)widget;
    if (!dynamic_cast<UIScrollView*>(widget)
        && !dynamic_cast<UIListView*>(widget))
    {
        panel->setClippingEnable(options.getItemBoolvalue("clipAble", false));
    }
    bool backGroundScale9Enable = options.getItemBoolvalue("backGroundScale9Enable", false);
    panel->setBackGroundImageScale9Enabled(backGroundScale9Enable);
    int cr = options.getItemIntValue("bgColorR", 255);
    int cg = options.getItemIntValue("bgColorG", 255);
    int cb = options.getItemIntValue("bgColorB", 255);
    
    int scr = options.getItemIntValue("bgStartColorR", 255);
    int scg = options.getItemIntValue("bgStartColorG", 255);
    int scb = options.getItemIntValue("bgStartColorG", 255);
    
    int ecr = options.getItemIntValue("bgEndColorR", 255);
    int ecg = options.getItemIntValue("bgEndColorG", 255);
    int ecb = options.getItemIntValue("bgEndColorB", 255);
    
    float bgcv1 = options.getItemFloatValue("vectorX", 0.0);
    float bgcv2 = options.getItemFloatValue("vectorY", 0.0);
    panel->setBackGroundColorVector(ccp(bgcv1, bgcv2));
    
    int co = options.getItemIntValue("bgColorOpacity", 255);
    
    int colorType = options.getItemIntValue("colorType", LAYOUT_COLOR_NONE);
    panel->setBackGroundColorType(PanelColorType(colorType));
    panel->setBackGroundColor(ccc3(scr, scg, scb),ccc3(ecr, ecg, ecb));
    panel->setBackGroundColor(ccc3(cr, cg, cb));
    panel->setBackGroundColorOpacity(co);
    
    
    if (cs::CSJsonDictionary* pImageFileNameDic = options.getSubDictionary("backGroundImageData"))
    {
        int imageFileNameType = pImageFileNameDic->getItemIntValue("resourceType", -1);
        switch (imageFileNameType)
        {
            case 0:
            {
                std::string tp_b = m_strFilePath;
                const char* imageFileName = pImageFileNameDic->getItemStringValue("path");
                const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0))?tp_b.append(imageFileName).c_str():NULL;
                panel->setBackGroundImage(imageFileName_tp);
                break;
            }
            case 1:
            {
                const char* imageFileName = pImageFileNameDic->getItemStringValue("path");
                panel->setBackGroundImage(imageFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pImageFileNameDic;
        pImageFileNameDic = NULL;
    }
    
    if (backGroundScale9Enable)
    {
        float cx = options.getItemFloatValue("capInsetsX", 0.0);
        float cy = options.getItemFloatValue("capInsetsY", 0.0);
        float cw = options.getItemFloatValue("capInsetsWidth", 0.0);
        float ch = options.getItemFloatValue("capInsetsHeight", 0.0);
        panel->setBackGroundImageCapInsets(CCRectMake(cx, cy, cw, ch));
    }
    panel->setLayoutType((LayoutType)options.getItemIntValue("layoutType", LAYOUT_ABSOLUTE));
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300Lua::setPropsForScrollViewFromJsonDictionary(UIWidget*widget, cs::CSJsonDictionary& options)
{
    setPropsForLayoutFromJsonDictionary(widget, options);
    UIScrollView* scrollView = (UIScrollView*)widget;
    float innerWidth = options.getItemFloatValue("innerWidth", 0.0);
    float innerHeight = options.getItemFloatValue("innerHeight", 0.0);
    scrollView->setInnerContainerSize(CCSizeMake(innerWidth, innerHeight));
	int direction = options.getItemIntValue("direction", 0);
	scrollView->setDirection((SCROLLVIEW_DIR)direction);
    scrollView->setBounceEnabled(options.getItemBoolvalue("bounceEnable", false));
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300Lua::setPropsForSliderFromJsonDictionary(UIWidget*widget, cs::CSJsonDictionary& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UISlider* slider = (UISlider*)widget;
    
    bool barTextureScale9Enable = options.getItemBoolvalue("barTextureScale9Enable", false);
    slider->setScale9Enabled(barTextureScale9Enable);
    bool bt = options.isKeyValidate("barFileName");
    float barLength = options.getItemFloatValue("length", 0.0);
    if (bt)
    {
        if (barTextureScale9Enable)
        {
            if (cs::CSJsonDictionary* pImageFileNameDic = options.getSubDictionary("barFileNameData"))
            {
                int imageFileType = pImageFileNameDic->getItemIntValue("resourceType", 0);
                switch (imageFileType)
                {
                    case 0:
                    {
                        std::string tp_b = m_strFilePath;
                        const char* imageFileName = pImageFileNameDic->getItemStringValue("path");
                        const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0))?tp_b.append(imageFileName).c_str():NULL;
                        slider->loadBarTexture(imageFileName_tp);
                        break;
                    }
                    case 1:
                    {
                        const char* imageFileName =  pImageFileNameDic->getItemStringValue("path");
                        slider->loadBarTexture(imageFileName,UI_TEX_TYPE_PLIST);
                        break;
                    }
                    default:
                        break;
                }
                delete pImageFileNameDic;
                pImageFileNameDic = NULL;
            }
            slider->setSize(CCSizeMake(barLength, slider->getContentSize().height));
        }
        else
        {
            if (cs::CSJsonDictionary* pImageFileNameDic = options.getSubDictionary("barFileNameData"))
            {
                int imageFileType = pImageFileNameDic->getItemIntValue("resourceType", 0);
                switch (imageFileType)
                {
                    case 0:
                    {
                        std::string tp_b = m_strFilePath;
                        const char*imageFileName =  pImageFileNameDic->getItemStringValue("path");
                        const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0))?tp_b.append(imageFileName).c_str():NULL;
                        slider->loadBarTexture(imageFileName_tp);
                        break;
                    }
                    case 1:
                    {
                        const char*imageFileName =  pImageFileNameDic->getItemStringValue("path");
                        slider->loadBarTexture(imageFileName,UI_TEX_TYPE_PLIST);
                        break;
                    }
                    default:
                        break;
                }
                delete pImageFileNameDic;
                pImageFileNameDic = NULL;
            }
        }
    }
    
    if (cs::CSJsonDictionary* pNormalDic = options.getSubDictionary("ballNormalData"))
    {
        int normalType = pNormalDic->getItemIntValue("resourceType", 0);
        switch (normalType)
        {
            case 0:
            {
                std::string tp_n = m_strFilePath;
                const char* normalFileName = pNormalDic->getItemStringValue("path");
                const char* normalFileName_tp = (normalFileName && (strcmp(normalFileName, "") != 0))?tp_n.append(normalFileName).c_str():NULL;
                slider->loadSlidBallTextureNormal(normalFileName_tp);
                break;
            }
            case 1:
            {
                const char* normalFileName = pNormalDic->getItemStringValue("path");
                slider->loadSlidBallTextureNormal(normalFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pNormalDic;
        pNormalDic = NULL;
    }
    
    if (cs::CSJsonDictionary* pPressedDic = options.getSubDictionary("ballPressedData"))
    {
        int pressedType = pPressedDic->getItemIntValue("resourceType", 0);
        switch (pressedType)
        {
            case 0:
            {
                std::string tp_p = m_strFilePath;
                const char* pressedFileName = pPressedDic->getItemStringValue("path");
                const char* pressedFileName_tp = (pressedFileName && (strcmp(pressedFileName, "") != 0))?tp_p.append(pressedFileName).c_str():NULL;
                slider->loadSlidBallTexturePressed(pressedFileName_tp);
                break;
            }
            case 1:
            {
                const char* pressedFileName = pPressedDic->getItemStringValue("path");
                slider->loadSlidBallTexturePressed(pressedFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pPressedDic;
        pPressedDic = NULL;
    }
    
    if (cs::CSJsonDictionary* pDisabledDic = options.getSubDictionary("ballDisabledData"))
    {
        int disabledType = pDisabledDic->getItemIntValue("resourceType", 0);
        switch (disabledType)
        {
            case 0:
            {
                std::string tp_d = m_strFilePath;
                const char* disabledFileName = pDisabledDic->getItemStringValue("path");
                const char* disabledFileName_tp = (disabledFileName && (strcmp(disabledFileName, "") != 0))?tp_d.append(disabledFileName).c_str():NULL;
                slider->loadSlidBallTextureDisabled(disabledFileName_tp);
                break;
            }
            case 1:
            {
                const char* disabledFileName = pDisabledDic->getItemStringValue("path");
                slider->loadSlidBallTextureDisabled(disabledFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pDisabledDic;
        pDisabledDic = NULL;
    }
    
    slider->setPercent(options.getItemIntValue("percent", 0));
    
    if (cs::CSJsonDictionary* pProgressBarDic = options.getSubDictionary("progressBarData"))
    {
        int progressBarType = pProgressBarDic->getItemIntValue("resourceType", 0);
        switch (progressBarType)
        {
            case 0:
            {
                std::string tp_b = m_strFilePath;
                const char* imageFileName = pProgressBarDic->getItemStringValue("path");
                const char* imageFileName_tp = (imageFileName && (strcmp(imageFileName, "") != 0))?tp_b.append(imageFileName).c_str():NULL;
                slider->loadProgressBarTexture(imageFileName_tp);
                break;
            }
            case 1:
            {
                const char* imageFileName = pProgressBarDic->getItemStringValue("path");
                slider->loadProgressBarTexture(imageFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pProgressBarDic;
        pProgressBarDic = NULL;
    }
    
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300Lua::setPropsForTextFieldFromJsonDictionary(UIWidget*widget, cs::CSJsonDictionary& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UITextField* textField = (UITextField*)widget;
    bool ph = options.isKeyValidate("placeHolder");
    if (ph)
    {
        textField->setPlaceHolder(options.getItemStringValue("placeHolder"));
    }
    textField->setText(options.getItemStringValue("text"));
    bool fs = options.isKeyValidate("fontSize");
    if (fs)
    {
        textField->setFontSize(options.getItemIntValue("fontSize", 16));
    }
    bool fn = options.isKeyValidate("fontName");
    if (fn)
    {
        textField->setFontName(options.getItemStringValue("fontName"));
    }
    bool tsw = options.isKeyValidate("touchSizeWidth");
    bool tsh = options.isKeyValidate("touchSizeHeight");
    if (tsw && tsh)
    {
        textField->setTouchSize(CCSizeMake(options.getItemFloatValue("touchSizeWidth", 0.0), options.getItemFloatValue("touchSizeHeight", 0.0)));
    }
    
    float dw = options.getItemFloatValue("width", 0.0);
    float dh = options.getItemFloatValue("height", 0.0);
    if (dw > 0.0f || dh > 0.0f)
    {
        //textField->setSize(CCSizeMake(dw, dh));
    }
	bool maxLengthEnable = options.getItemBoolvalue("maxLengthEnable", false);
	textField->setMaxLengthEnabled(maxLengthEnable);
    
	if (maxLengthEnable)
	{
		int maxLength = options.getItemIntValue("maxLength", 0);
		textField->setMaxLength(maxLength);
	}
    bool passwordEnable = options.getItemBoolvalue("passwordEnable", false);
    textField->setPasswordEnable(passwordEnable);
    if (passwordEnable)
    {
        textField->setPasswordStyleText(options.getItemStringValue("passwordStyleText"));
    }
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300Lua::setPropsForLoadingBarFromJsonDictionary(UIWidget *widget, cs::CSJsonDictionary& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    UILoadingBar* loadingBar = (UILoadingBar*)widget;
    
    if (cs::CSJsonDictionary* pImageFileNameDic = options.getSubDictionary("textureData"))
    {
        int imageFileNameType = pImageFileNameDic->getItemIntValue("resourceType", 0);
        switch (imageFileNameType)
        {
            case 0:
            {
                std::string tp_i = m_strFilePath;
                const char* imageFileName = pImageFileNameDic->getItemStringValue("path");
                const char* imageFileName_tp = NULL;
                if (imageFileName && (strcmp(imageFileName, "") != 0))
                {
                    imageFileName_tp = tp_i.append(imageFileName).c_str();
                    loadingBar->loadTexture(imageFileName_tp);
                }
                break;
            }
            case 1:
            {
                const char* imageFileName = pImageFileNameDic->getItemStringValue("path");
                loadingBar->loadTexture(imageFileName,UI_TEX_TYPE_PLIST);
                break;
            }
            default:
                break;
        }
        delete pImageFileNameDic;
        pImageFileNameDic = NULL;
    }
    
    /* gui mark add load bar scale9 parse */
    bool scale9Enable = options.getItemBoolvalue("scale9Enable", false);
    loadingBar->setScale9Enabled(scale9Enable);
    
    if (scale9Enable)
    {
        float cx = options.getItemFloatValue("capInsetsX", 0.0);
        float cy = options.getItemFloatValue("capInsetsY", 0.0);
        float cw = options.getItemFloatValue("capInsetsWidth", 0.0);
        float ch = options.getItemFloatValue("capInsetsHeight", 0.0);
        
        loadingBar->setCapInsets(CCRectMake(cx, cy, cw, ch));
        
        float width = options.getItemFloatValue("width", 0.0);
        float height = options.getItemFloatValue("height", 0.0);
        loadingBar->setSize(CCSizeMake(width, height));
    }
    /**/
    
    loadingBar->setDirection(LoadingBarType(options.getItemIntValue("direction", 0)));
    loadingBar->setPercent(options.getItemIntValue("percent", 0));
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300Lua::setPropsForLabelBMFontFromJsonDictionary(UIWidget *widget, cs::CSJsonDictionary& options)
{
    setPropsForWidgetFromJsonDictionary(widget, options);
    
    UILabelBMFont* labelBMFont = (UILabelBMFont*)widget;
    
    if (cs::CSJsonDictionary* pCmftDic = options.getSubDictionary("fileNameData"))
    {
        int cmfType = pCmftDic->getItemIntValue("resourceType", 0);
        switch (cmfType)
        {
            case 0:
            {
                std::string tp_c = m_strFilePath;
                const char* cmfPath = pCmftDic->getItemStringValue("path");
                const char* cmf_tp = tp_c.append(cmfPath).c_str();
                labelBMFont->setFntFile(cmf_tp);
                break;
            }
            case 1:
                CCLOG("Wrong res type of LabelAtlas!");
                break;
            default:
                break;
        }
        delete pCmftDic;
        pCmftDic = NULL;
    }
    
    const char* text = options.getItemStringValue("text");
    labelBMFont->setText(text);
    
    setColorPropsForWidgetFromJsonDictionary(widget,options);
}

void WidgetPropertiesReader0300Lua::setPropsForPageViewFromJsonDictionary(UIWidget*widget, cs::CSJsonDictionary& options)
{
	/*modified by hanyi start here*/
    //	setPropsForLayoutFromJsonDictionary(widget, options);
	setPropsForLayoutFromJsonDictionary(widget, options);
    
	UIPageViewEx* listView = (UIPageViewEx*)widget;
    
	float innerWidth = options.getItemFloatValue("innerWidth", 0.0);
	float innerHeight = options.getItemFloatValue("innerHeight", 0.0);
	listView->setInnerContainerSize(CCSizeMake(innerWidth, innerHeight));
	int direction = options.getItemIntValue("direction", 0);
	listView->setDirection((SCROLLVIEW_DIR)direction);
    
	ListViewGravity gravity = (ListViewGravity)options.getItemIntValue("gravity", 0);
	listView->setGravity(gravity);
    
	float itemMargin = options.getItemFloatValue("itemMargin", 0.0);
	listView->setItemsMargin(itemMargin);
	/*modified by hanyi end here*/
}

void WidgetPropertiesReader0300Lua::setPropsForListViewFromJsonDictionary(UIWidget* widget, cs::CSJsonDictionary& options)
{
	setPropsForLayoutFromJsonDictionary(widget, options);
    
	UIListViewEx* listView = (UIListViewEx*)widget;
    
	float innerWidth = options.getItemFloatValue("innerWidth", 0.0);
	float innerHeight = options.getItemFloatValue("innerHeight", 0.0);
	listView->setInnerContainerSize(CCSizeMake(innerWidth, innerHeight));
	int direction = options.getItemIntValue("direction", 0);
	listView->setDirection((SCROLLVIEW_DIR)direction);
    
	ListViewGravity gravity = (ListViewGravity)options.getItemIntValue("gravity", 0);
	listView->setGravity(gravity);
    
	float itemMargin = options.getItemFloatValue("itemMargin", 0.0);
	listView->setItemsMargin(itemMargin);
}
// add by weixin end

NS_CC_EXT_END