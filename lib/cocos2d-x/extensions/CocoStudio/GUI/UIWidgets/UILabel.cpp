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

#include "UILabel.h"

NS_CC_EXT_BEGIN

UILabel::UILabel():
m_bTouchScaleChangeEnabled(false),
m_fNormalScaleValueX(1.0f),
m_fNormalScaleValueY(1.0f),
m_sFontName("Thonburi"),
m_nFontSize(10),
m_fOnSelectedScaleOffset(0.5),
m_pLabelRenderer(NULL),
//add by hanyi start here
m_pStrokeRenderer(NULL),
m_strokeEnabled(false),
m_strokeWidth(0),
m_strokeColor(ccc3(0, 0, 0)),
m_strokeAntiAlias(false),
m_strokeOpacity(255)
//add by hanyi end here
{
}

UILabel::~UILabel()
{
    
}

UILabel* UILabel::create()
{
    UILabel* widget = new UILabel();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool UILabel::init()
{
    if (UIWidget::init())
    {
        return true;
    }
    return false;
}

void UILabel::initRenderer()
{
    UIWidget::initRenderer();
    m_pLabelRenderer = CCLabelTTF::create();
    m_pRenderer->addChild(m_pLabelRenderer);
}

void UILabel::setText(const char* text)
{
	if (!text)
	{
		return;
	}
    std::string strText(text);
    m_pLabelRenderer->setString(strText.c_str());

	//add by hanyi start here
	if (m_strokeEnabled)
	{
		renderStrokeTexture();
	}
	//add by hanyi end here

    labelScaleChangedWithSize();
}

void UILabel::enableStroke(int width, ccColor3B& color, int opacity, bool antiAlias)
{
	m_strokeEnabled = true;
	m_strokeWidth = width;
	m_strokeColor = color;
	m_strokeOpacity = opacity;
	m_strokeAntiAlias = antiAlias;

	m_pLabelRenderer->setColor(getColor());
	setColor(ccWHITE);

	renderStrokeTexture();
}

void UILabel::disableStroke()
{
	m_strokeEnabled = false;

	if (m_pStrokeRenderer)
	{
		m_pRenderer->removeChild(m_pStrokeRenderer, true);
		m_pStrokeRenderer = NULL;
	}

	setColor(m_pLabelRenderer->getColor());
	m_pLabelRenderer->setColor(ccWHITE);
}

void UILabel::renderStrokeTexture()
{
	CCPoint originPos = m_pLabelRenderer->getPosition();
	ccColor3B originColor = m_pLabelRenderer->getColor();
	int originOpacity = m_pLabelRenderer->getOpacity();
	int originVisible = m_pLabelRenderer->isVisible();
	ccBlendFunc originBlendFunc = m_pLabelRenderer->getBlendFunc();
	const CCPoint& originAnchor = m_pLabelRenderer->getAnchorPoint();

	m_pLabelRenderer->setColor(m_strokeColor);
	m_pLabelRenderer->setOpacity(m_strokeOpacity);
	m_pLabelRenderer->setVisible(true);
	ccBlendFunc blendFunc = ccBlendFunc();
	blendFunc.src = GL_SRC_ALPHA;
	blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
	m_pLabelRenderer->setBlendFunc(blendFunc);

	float bottomLeftX = m_pLabelRenderer->getTexture()->getContentSize().width * originAnchor.x + m_strokeWidth;
	float bottomLeftY = m_pLabelRenderer->getTexture()->getContentSize().height * originAnchor.y + m_strokeWidth;

	float positionOffsetX = m_pLabelRenderer->getTexture()->getContentSize().width * originAnchor.x - m_pLabelRenderer->getTexture()->getContentSize().width / 2;
	float positionOffsetY = m_pLabelRenderer->getTexture()->getContentSize().height * originAnchor.y - m_pLabelRenderer->getTexture()->getContentSize().height / 2;

	CCPoint strokePos = ccp(originPos.x - positionOffsetX, originPos.y - positionOffsetY);

	CCSize size = m_pLabelRenderer->getContentSize();
	CCRenderTexture* rt = CCRenderTexture::create(size.width + m_strokeWidth * 2, size.height + m_strokeWidth * 2);

	rt->setAnchorPoint(originAnchor);
	rt->begin();
	for (int i = m_strokeWidth; i >= -m_strokeWidth; --i)
	{
		for (int j = -m_strokeWidth; j <= m_strokeWidth; ++j)
		{
			m_pLabelRenderer->setPosition(ccp(bottomLeftX + j, bottomLeftY + i));
			m_pLabelRenderer->visit();
		}
	}
	rt->end();
	if (m_strokeAntiAlias)
	{
		rt->getSprite()->getTexture()->setAntiAliasTexParameters();
	}

	rt->setPosition(strokePos);

	if (m_pStrokeRenderer)
	{
		m_pRenderer->removeChild(m_pStrokeRenderer);
	}
	m_pStrokeRenderer = rt;
	blendFunc = ccBlendFunc();
	blendFunc.src = GL_SRC_ALPHA;
	blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
	m_pStrokeRenderer->getSprite()->setBlendFunc(blendFunc);
	m_pRenderer->addChild(m_pStrokeRenderer, m_pLabelRenderer->getZOrder() - 1);

	m_pLabelRenderer->setPosition(originPos);
	m_pLabelRenderer->setColor(originColor);
	m_pLabelRenderer->setOpacity(originOpacity);
	m_pLabelRenderer->setVisible(originVisible);
	m_pLabelRenderer->setBlendFunc(originBlendFunc);
}

const char* UILabel::getStringValue()
{
    return m_pLabelRenderer->getString();
}

int UILabel::getStringLength()
{
    const char* str = m_pLabelRenderer->getString();
    return strlen(str);
}

void UILabel::setFontSize(int size)
{
    m_nFontSize = size;
    m_pLabelRenderer->setFontSize(size);
    labelScaleChangedWithSize();
}

void UILabel::setFontName(const char* name)
{
    m_sFontName = name;
    m_pLabelRenderer->setFontName(name);
    labelScaleChangedWithSize();
}
// add by hanyi start here
void UILabel::setTextInfo(const char* text, int fontSize, const char* fontName, CCSize* dimensions, int hAlignment, int vAlignment)
{
	if (fontSize > 0 && m_nFontSize != fontSize)
	{
		m_nFontSize = fontSize;
	}
	if (fontName)
	{
		m_sFontName = fontName;
	}
	m_pLabelRenderer->setTextInfo(text, fontSize, fontName, dimensions, hAlignment, vAlignment);
	//add by hanyi start here
	if (m_strokeEnabled)
	{
		renderStrokeTexture();
	}
	//add by hanyi end here
	labelScaleChangedWithSize();
}
// add by hanyi end here
void UILabel::setTextAreaSize(const CCSize &size)
{
    m_pLabelRenderer->setDimensions(size);
    labelScaleChangedWithSize();
}

void UILabel::setTextHorizontalAlignment(CCTextAlignment alignment)
{
    m_pLabelRenderer->setHorizontalAlignment(alignment);
    labelScaleChangedWithSize();
}

void UILabel::setTextVerticalAlignment(CCVerticalTextAlignment alignment)
{
    m_pLabelRenderer->setVerticalAlignment(alignment);
    labelScaleChangedWithSize();
}

void UILabel::setTouchScaleChangeEnabled(bool enable)
{
    m_bTouchScaleChangeEnabled = enable;
    m_fNormalScaleValueX = getScaleX();
    m_fNormalScaleValueY = getScaleY();
}

bool UILabel::isTouchScaleChangeEnabled()
{
    return m_bTouchScaleChangeEnabled;
}

void UILabel::onPressStateChangedToNormal()
{
    if (!m_bTouchScaleChangeEnabled)
    {
        return;
    }
    clickScale(m_fNormalScaleValueX, m_fNormalScaleValueY);
	// add by rrobin
	UIWidget::onPressStateChangedToNormal();
	// end by rrobin
}

void UILabel::onPressStateChangedToPressed()
{
    if (!m_bTouchScaleChangeEnabled)
    {
        return;
    }
    clickScale(m_fNormalScaleValueX + m_fOnSelectedScaleOffset, m_fNormalScaleValueY + m_fOnSelectedScaleOffset);

	// add by rrobin
	UIWidget::onPressStateChangedToPressed();
	// end by rrobin
}

void UILabel::onPressStateChangedToDisabled()
{
    // add by rrobin
	UIWidget::onPressStateChangedToDisabled();
	// end by rrobin
}

void UILabel::setScale(float fScale)
{
    UIWidget::setScale(fScale);
    m_fNormalScaleValueX = m_fNormalScaleValueY = fScale;
}

void UILabel::setScaleX(float fScaleX)
{
    UIWidget::setScaleX(fScaleX);
    m_fNormalScaleValueX = fScaleX;
}

void UILabel::setScaleY(float fScaleY)
{
    UIWidget::setScaleY(fScaleY);
    m_fNormalScaleValueY = fScaleY;
}

void UILabel::clickScale(float scaleX, float scaleY)
{
    m_pRenderer->setScaleX(scaleX);
    m_pRenderer->setScaleY(scaleY);
}

void UILabel::setFlipX(bool flipX)
{
    m_pLabelRenderer->setFlipX(flipX);
}

void UILabel::setFlipY(bool flipY)
{
    m_pLabelRenderer->setFlipY(flipY);
}

bool UILabel::isFlipX()
{
    return m_pLabelRenderer->isFlipX();
}

bool UILabel::isFlipY()
{
    return m_pLabelRenderer->isFlipY();
}

void UILabel::setAnchorPoint(const CCPoint &pt)
{
    UIWidget::setAnchorPoint(pt);
    m_pLabelRenderer->setAnchorPoint(pt);
}

void UILabel::onSizeChanged()
{
    labelScaleChangedWithSize();
}

const CCSize& UILabel::getContentSize() const
{
    return m_pLabelRenderer->getContentSize();
}

CCNode* UILabel::getVirtualRenderer()
{
    return m_pLabelRenderer;
}

void UILabel::labelScaleChangedWithSize()
{
    if (m_bIgnoreSize)
    {
        m_pLabelRenderer->setScale(1.0f);
        m_size = m_pLabelRenderer->getContentSize();
    }
    else
    {
        CCSize textureSize = m_pLabelRenderer->getContentSize();
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            m_pLabelRenderer->setScale(1.0f);
            return;
        }
        float scaleX = m_size.width / textureSize.width;
        float scaleY = m_size.height / textureSize.height;
        m_pLabelRenderer->setScaleX(scaleX);
        m_pLabelRenderer->setScaleY(scaleY);
    }
    
}

const char* UILabel::getDescription() const
{
    return "Label";
}

UIWidget* UILabel::createCloneInstance()
{
    return UILabel::create();
}

void UILabel::copySpecialProperties(UIWidget *widget)
{
    UILabel* label = dynamic_cast<UILabel*>(widget);
    if (label)
    {
		/* commit by hanyi start here
        setFontName(label->m_sFontName.c_str());
        setFontSize(label->m_pLabelRenderer->getFontSize());
		setTextAreaSize(label->m_pLabelRenderer->getDimensions());
		setTextHorizontalAlignment(label->m_pLabelRenderer->getHorizontalAlignment());
		setTextVerticalAlignment(label->m_pLabelRenderer->getVerticalAlignment());
        setText(label->getStringValue());
		commit by hanyi end here */
		// add by hanyi start here
		CCTextAlignment hAlignment = label->m_pLabelRenderer->getHorizontalAlignment();
		CCVerticalTextAlignment vAlignment = label->m_pLabelRenderer->getVerticalAlignment();
		CCSize dimesions = label->m_pLabelRenderer->getDimensions();
		setTextInfo(label->getStringValue(), label->m_pLabelRenderer->getFontSize(), label->m_sFontName.c_str(), &dimesions, hAlignment, vAlignment);
		// add by hanyi end here
        setTouchScaleChangeEnabled(label->m_bTouchScaleChangeEnabled);
		if (label->m_strokeEnabled)
		{
			enableStroke(label->m_strokeWidth, label->m_strokeColor, label->m_strokeOpacity, label->m_strokeAntiAlias);
		}
    }
}

NS_CC_EXT_END