#include "UIPageViewEx.h"

#include "../../Layouts/UILayout.h"

NS_CC_EXT_BEGIN

UIPageViewEx::UIPageViewEx() :
m_bounceSpeed(1000.0f),
m_autoScrollDir(0.0f),
m_bAutoScroll(false),
m_bAutoBounceScroll(false),
m_scrollAccelaration(0.0f),
m_scrollDestination(CCPointZero),
m_bCheckScrollDestination(false),
m_accelaration(800.0f),
m_curPageIndex(-1),
m_curThroughPageIndex(-1),
m_overLengthRatio(0.3f),
m_bPressed(false),
m_touchBeganPoint(CCPointZero),
m_touchMovingPoint(CCPointZero),
m_frontScrollLimitPos(CCPointZero),
m_backScrollLimitPos(CCPointZero),
m_bOneByOne(false)
{
	m_className = "UIPageViewEx";
}

UIPageViewEx::~UIPageViewEx()
{
}

UIPageViewEx* UIPageViewEx::create()
{
	UIPageViewEx* widget = new UIPageViewEx();
	if (widget && widget->init())
	{
		widget->autorelease();
		widget->setDirection(SCROLLVIEW_DIR_HORIZONTAL);
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return NULL;
}

void UIPageViewEx::setPageModel(UILayout* model)
{
	setItemModel(model);
	m_modelSize = model->getSize();
}

void UIPageViewEx::setOneByOne(bool isOneByOne)
{
	m_bOneByOne = isOneByOne;

	updateLimitCoordinate();
}

UILayout* UIPageViewEx::pushBackModelPage()
{
	this->pushBackDefaultItem();
/*
	CCSize innerSize = getInnerContainerSize();
	switch (getDirection())
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
		innerSize.width = m_modelSize.width * getItems()->count();
		break;
	case SCROLLVIEW_DIR_VERTICAL:
		innerSize.height = m_modelSize.height * getItems()->count();
		break;
	default:
		break;
	}
	setInnerContainerSize(innerSize);
*/
	updateLimitCoordinate();

	return dynamic_cast<UILayout*>(this->getItem(this->getItems()->count() - 1));
}

void UIPageViewEx::updateLimitCoordinate()
{
	CCSize innerSize = getInnerContainerSize();
	CCSize size = getContentSize();

	if (m_bOneByOne)
	{
		m_backScrollLimitPos = getPageBackOverPosition(m_curPageIndex - 1);
		m_frontScrollLimitPos = getPageFrontOverPosition(m_curPageIndex + 1);
	}
	else
	{
		m_backScrollLimitPos = getPageBackOverPosition(0);
		m_frontScrollLimitPos = getPageFrontOverPosition(getItems()->count() - 1);
	}
	/*
	m_lowerLimitY = -innerSize.height + size.height / 2.0f + m_modelSize.height / 2.0f - m_overLengthRatio * m_modelSize.height;
	m_upperLimitY = size.height / 2.0f - m_modelSize.height /2.0f + m_modelSize.height* m_overLengthRatio;
	m_rightLimitX = size.width / 2.0f - m_modelSize.width / 2.0f + m_modelSize.width * m_overLengthRatio;
	m_leftLimitX = -innerSize.width + size.width / 2.0f + m_modelSize.width / 2.0f - m_overLengthRatio * m_modelSize.width;
	*/
}

void UIPageViewEx::refreshView()
{
	CCSize innerSize = getInnerContainerSize();
	switch (getDirection())
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
		innerSize.width = m_modelSize.width * getItems()->count();
		break;
	case SCROLLVIEW_DIR_VERTICAL:
		innerSize.height = m_modelSize.height * getItems()->count();
		break;
	default:
		break;
	}
	setInnerContainerSize(innerSize);

	updateLimitCoordinate();
}

void UIPageViewEx::checkChildInfo(int handleState, UIWidget* sender, const CCPoint &touchPoint)
{
	interceptTouchEvent(handleState, sender, touchPoint);
}

void UIPageViewEx::interceptTouchEvent(int handleState, UIWidget* sender, const cocos2d::CCPoint &touchPoint)
{
	switch (handleState)
	{
	case 0:		//down
		handlePressLogic(touchPoint);
		break;
	case 1:		//move
	{
		float delta = ccpSub(sender->getTouchStartPos(), touchPoint).getLength();
		if (delta > 5.0f)
		{
			sender->setFocused(false);
			handleMoveLogic(touchPoint);
		}
		break;
	}
	case 2:		//up
		handleMoveEndLogic(touchPoint);
		break;
	case 3:		//cancel?
		handleReleaseLogic(touchPoint);
		break;
	}
}

void UIPageViewEx::handlePressLogic(const CCPoint& touchPoint)
{
	m_touchBeganPoint = m_pRenderer->convertToNodeSpace(touchPoint);
	m_touchMovingPoint = m_touchBeganPoint;
	m_touchEndedPoint = m_touchBeganPoint;
	m_bPressed = true;
	stopAutoScroll(false);
	m_slideTime = 0.0f;
}

void UIPageViewEx::handleMoveLogic(const CCPoint& touchPoint)
{
	CCPoint movedPoint = m_pRenderer->convertToNodeSpace(touchPoint);
	CCPoint delta = ccpSub(movedPoint, m_touchMovingPoint);

	if (delta.getLength() <= 2.0f)
	{
		m_touchBeganPoint = movedPoint;
		m_slideTime = 0.0f;
	}
	m_touchMovingPoint = movedPoint;

	switch (getDirection())
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
		moveChildren(delta.x, 0.0f);
		break;
	case SCROLLVIEW_DIR_VERTICAL:
		moveChildren(0.0f, delta.y);
		break;
	default:
		break;
	}
}

void UIPageViewEx::handleMoveEndLogic(const CCPoint& touchPoint)
{
	m_touchEndPoint = m_pRenderer->convertToNodeSpace(touchPoint);
	m_slideTime = 0.0f;
	endSlideAction();
	m_bPressed = false;
}

void UIPageViewEx::handleReleaseLogic(const CCPoint& touchPoint)
{
	m_touchEndPoint = m_pRenderer->convertToNodeSpace(touchPoint);
	endSlideAction();
	m_bPressed = false;
}

void UIPageViewEx::endSlideAction()
{
	float scrollSpeed = 0.0f;

	if (m_slideTime <= 0.0f)
	{
		startAutoBounceScroll();
	}
	else
	{
		int pageIndex = m_curPageIndex;

		switch (getDirection())
		{
		case SCROLLVIEW_DIR_HORIZONTAL:
			scrollSpeed = (m_touchEndPoint.x - m_touchBeganPoint.x) / m_slideTime;
			break;
		case SCROLLVIEW_DIR_VERTICAL:
			scrollSpeed = (m_touchEndPoint.y - m_touchBeganPoint.y) / m_slideTime;
			break;
		default:
			break;
		}

		if (scrollSpeed > 0.0f)
		{
			pageIndex = (pageIndex - 1 < 0) ? 0 : pageIndex - 1;
		}
		else if (scrollSpeed < 0.0f)
		{
			pageIndex = (pageIndex + 1 >= getItems()->count()) ? getItems()->count() - 1 : pageIndex + 1;
		}

		startAutoScroll(scrollSpeed, m_accelaration);
	}

	m_slideTime = 0.0f;
}

void UIPageViewEx::stopAutoScroll(bool needBounce)
{
	m_bAutoScroll = false;
	m_bCheckScrollDestination = false;
	m_scrollDestination = CCPointZero;
	m_autoScrollDir = 0.0f;
	m_curScrollSpeed = 0.0f;
	m_scrollAccelaration = 0.0f;

	if (needBounce)
	{
		startAutoBounceScroll();
	}

	if (!m_bAutoScroll)
	{	//consider that the current position is focus position of a page
		onPageFocused(getPageIndexByInnerPosition(getInnerContainer()->getPosition()));
	}
}

void UIPageViewEx::startAutoScroll(float speed, float accelaration)
{
	m_bAutoScroll = true;
	m_curScrollSpeed = fabs(speed);
	m_scrollAccelaration = accelaration;
	m_autoScrollDir = (speed > 0.0f) ? 1.0f : -1.0f;
}

void UIPageViewEx::scrollToPage(int index, float speed)
{
	if (index < 0 || index >= getItems()->count()) return;

	CCPoint pos = getPageFocusPosition(index);
	CCPoint innerPos = getInnerContainer()->getPosition();
	
	switch (getDirection())
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
		if (pos.x >= innerPos.x)
		{
			pos.x += m_modelSize.width * m_overLengthRatio;
		}
		else
		{
			pos.x -= m_modelSize.width * m_overLengthRatio;
		}
		break;
	case SCROLLVIEW_DIR_VERTICAL:
		if (pos.y >= innerPos.y)
		{
			pos.y += m_modelSize.height * m_overLengthRatio;
		}
		else
		{
			pos.y -= m_modelSize.height * m_overLengthRatio;
		}
		break;
	default:
		return;
	}
	
	startAutoScrollWithDestination(pos, speed, 0.0f);
}

void UIPageViewEx::jumpToPage(int index)
{
	if (index < 0 || index >= getItems()->count()) return;

	CCPoint pos = getPageFocusPosition(index);
	CCPoint innerPos = getInnerContainer()->getPosition();

	if (m_bOneByOne)
	{
		m_curPageIndex = index;
		updateLimitCoordinate();
	}
	moveChildren(pos.x - innerPos.x, pos.y - innerPos.y);

	onPageFocused(index);
}

void UIPageViewEx::startAutoScrollWithDestination(const CCPoint& dest, float speed, float accelaration)
{
	CCPoint innerPos = getInnerContainer()->getPosition();

	switch (getDirection())
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
		if (dest.x - innerPos.x >= 0.0f)
		{
			speed *= 1.0f;
		}
		else
		{
			speed *= -1.0f;
		}
		break;
	case SCROLLVIEW_DIR_VERTICAL:
		if (dest.y - innerPos.y >= 0.0f)
		{
			speed *= 1.0f;
		}
		else
		{
			speed *= -1.0f;
		}
		break;
	default:
		return;
	}

	m_bCheckScrollDestination = true;
	m_scrollDestination = dest;

	startAutoScroll(speed, accelaration);
}

void UIPageViewEx::autoScrollChildren(float dt)
{
	m_curScrollSpeed -= m_scrollAccelaration * dt;
	CCPoint innerPos = getInnerContainer()->getPosition();

	switch (getDirection())
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
		if (m_curScrollSpeed > 0.0f)
		{
			if (m_bCheckScrollDestination)
			{
				float destX = innerPos.x + m_curScrollSpeed * dt * m_autoScrollDir;
				if ((m_autoScrollDir > 0.0f && destX >= m_scrollDestination.x) ||
					(m_autoScrollDir < 0.0f && destX <= m_scrollDestination.x))
				{
					moveChildren(fabs(m_scrollDestination.x - innerPos.x) * m_autoScrollDir, 0.0f);
					stopAutoScroll(true);
				}
				else
				{
					if (!moveChildren(m_curScrollSpeed * dt * m_autoScrollDir, 0.0f))
					{
						stopAutoScroll(false);
					}
				}
			}
			else
			{
				if (!moveChildren(m_curScrollSpeed * dt * m_autoScrollDir, 0.0f))
				{
					stopAutoScroll(true);
				}
			}
		}
		else
		{
			stopAutoScroll(!m_bCheckScrollDestination);
		}
		break;
	case SCROLLVIEW_DIR_VERTICAL:
		if (m_curScrollSpeed > 0.0f)
		{
			if (m_bCheckScrollDestination)
			{
				float destY = innerPos.y + m_curScrollSpeed * dt * m_autoScrollDir;
				if ((m_autoScrollDir > 0.0f && destY >= m_scrollDestination.y) ||
					(m_autoScrollDir < 0.0f && destY <= m_scrollDestination.y))
				{
					moveChildren(0.0f, fabs(m_scrollDestination.y - innerPos.y) * m_autoScrollDir);
					stopAutoScroll(true);
				}
				else
				{
					if (!moveChildren(0.0f, m_curScrollSpeed * dt * m_autoScrollDir))
					{
						stopAutoScroll(false);
					}
				}
			}
			else
			{
				if (!moveChildren(0.0f, m_curScrollSpeed * dt * m_autoScrollDir))
				{
					stopAutoScroll(true);
				}
			}
		}
		else
		{
			stopAutoScroll(!m_bCheckScrollDestination);
		}
		break;
	default:
		break;
	}

}

void UIPageViewEx::startAutoBounceScroll()
{
	CCPoint innerPos = getInnerContainer()->getPosition();

	int pageIndex = getPageIndexByInnerPosition(getInnerContainer()->getPosition());
	CCPoint pageFocusPos = getPageFocusPosition(pageIndex);

	if (pageFocusPos.equals(innerPos))
	{
		return;
	}

	if (m_curScrollSpeed >= m_bounceSpeed)
	{
		startAutoScrollWithDestination(pageFocusPos, m_curScrollSpeed, 0.0f);
	}
	else
	{
		startAutoScrollWithDestination(pageFocusPos, m_bounceSpeed, 0.0f);
	}
}

CCPoint UIPageViewEx::getPageFocusPosition(int index)
{
	CCPoint pos = CCPointZero;
	if (index < 0 || index >= getItems()->count()) return pos;

	switch (getDirection())
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
		pos.x = getSize().width / 2.0f - m_modelSize.width / 2.0f - m_modelSize.width * index;
		break;
	case SCROLLVIEW_DIR_VERTICAL:
	{
		CCSize innerSize = getInnerContainerSize();
		pos.y = -innerSize.height + getSize().height / 2.0f + m_modelSize.height / 2.0f + index * m_modelSize.height;
	}
		break;
	default:
		break;
	}

	return pos;
}

CCPoint UIPageViewEx::getPageBackOverPosition(int index)
{
	if (getItems()->count() <= 0) return CCPointZero;
	if (index < 0) index = 0;
	if (index >= getItems()->count()) index = getItems()->count() - 1;

	CCPoint pos = getPageFocusPosition(index);

	switch (getDirection())
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
		pos.x += m_modelSize.width * m_overLengthRatio;
		break;
	case SCROLLVIEW_DIR_VERTICAL:
		pos.y -= m_modelSize.height * m_overLengthRatio;
		break;
	default:
		break;
	}

	return pos;
}

CCPoint UIPageViewEx::getPageFrontOverPosition(int index)
{
	if (index < 0) index = 0;
	if (index >= getItems()->count()) index = getItems()->count() - 1;

	CCPoint pos = getPageFocusPosition(index);

	switch (getDirection())
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
		pos.x -= m_modelSize.width * m_overLengthRatio;
		break;
	case SCROLLVIEW_DIR_VERTICAL:
		pos.y += m_modelSize.height * m_overLengthRatio;
		break;
	default:
		break;
	}

	return pos;
}

int UIPageViewEx::getPageIndexByInnerPosition(const CCPoint& pos)
{
	int index = 0;

	switch (getDirection())
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
		index = (int)(fabs(pos.x - getSize().width / 2.0f) / m_modelSize.width);
		if (index >= getItems()->count()) index = getItems()->count() - 1;
		break;
	case SCROLLVIEW_DIR_VERTICAL:
	{
		index = (int)((getInnerContainerSize().height + pos.y - getSize().height / 2.0f) / m_modelSize.height);
		if (index >= getItems()->count()) index = getItems()->count() - 1;
		break;
	}
	default:
		break;
	}

	return abs(index);
}

void UIPageViewEx::onPageFocused(int index)
{
	m_curPageIndex = index;

	if (m_bOneByOne)
	{
		m_backScrollLimitPos = getPageBackOverPosition(index - 1);
		m_frontScrollLimitPos = getPageFrontOverPosition(index + 1);
	}

	if (m_nScriptHandler && m_pScript)
	{
		CCArray* args = CCArray::create();
		args->addObject(CCString::create("onPageFocus"));
		args->addObject(CCInteger::create(m_curPageIndex));
		m_pScript->executeEventWithArgs(m_nScriptHandler, args);
	}
}

void UIPageViewEx::update(float dt)
{
	if (m_bAutoScroll)
	{
		autoScrollChildren(dt);
	}
	if (m_bPressed)
	{
		m_slideTime += dt;
	}
}

bool UIPageViewEx::moveChildren(float offsetX, float offsetY)
{
	SCROLLVIEW_DIR dir = getDirection();
	CCSize innerSize = getInnerContainerSize();
	CCSize size = getSize();
	CCPoint innerPos = getInnerContainer()->getPosition();
	
	float halfWidth = size.width / 2.0f;
	float halfHeight = size.height / 2.0f;

	bool ret = true;
	switch (dir)
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
	{
		innerPos.x += offsetX;
		if (innerPos.x > m_backScrollLimitPos.x)
		{
			innerPos.x = m_backScrollLimitPos.x;
			ret = false;
		}
		else if (innerPos.x < m_frontScrollLimitPos.x)
		{
			innerPos.x = m_frontScrollLimitPos.x;
			ret = false;
		}
	}
		break;
	case SCROLLVIEW_DIR_VERTICAL:
	{
		innerPos.y += offsetY;
		if (innerPos.y < m_backScrollLimitPos.y)
		{
			innerPos.y = m_backScrollLimitPos.y;
			ret = false;
		}
		if (innerPos.y > m_frontScrollLimitPos.y)
		{
			innerPos.y = m_frontScrollLimitPos.y;
			ret = false;
		}
	}
		break;
	default:
		return false;
	}

	getInnerContainer()->setPosition(innerPos);

	m_curThroughPageIndex = getPageIndexByInnerPosition(innerPos);
	executeScrollPercentEvent();

	return ret;
}

void UIPageViewEx::executeScrollPercentEvent()
{
	CCPoint innerPos = getInnerContainer()->getPosition();

	CCPoint pageFocusPos = getPageFocusPosition(m_curThroughPageIndex);

	float baseLength = 0.0f;
	switch (getDirection())
	{
	case SCROLLVIEW_DIR_HORIZONTAL:
		baseLength = m_modelSize.width;
		break;
	case SCROLLVIEW_DIR_VERTICAL:
		baseLength = m_modelSize.height;
		break;
	default:
		return;
	}

	if (m_nScriptHandler && m_pScript)
	{
		for (int pageIndex = 0; pageIndex < getItems()->count(); ++pageIndex)
		{
//			int pageIndex = m_curThroughPageIndex + i;
//			if (pageIndex < 0 || pageIndex >= getItems()->count()) continue;

			CCPoint pageFocusPos = getPageFocusPosition(pageIndex);
			float dist = innerPos.getDistance(pageFocusPos);
//			if (dist > baseLength) continue;

			CCArray* args = CCArray::create();
			args->addObject(CCString::create("onPageScroll"));
			args->addObject(CCInteger::create(pageIndex));
			args->addObject(CCFloat::create(dist / baseLength));
			m_pScript->executeEventWithArgs(m_nScriptHandler, args);
		}
	}
	else
	{
		return;
	}
}

const char* UIPageViewEx::getDescription() const
{
	return "PageView";
}

UIWidget* UIPageViewEx::createCloneInstance()
{
	return UIPageViewEx::create();
}

void UIPageViewEx::copySpecialProperties(UIWidget* model)
{
	UIListViewEx::copySpecialProperties(model);

	UIPageViewEx* pageView = dynamic_cast<UIPageViewEx*>(model);
	if (pageView)
	{
		pageView->setAccelaration(m_accelaration);
		pageView->setBounceSpeed(m_bounceSpeed);
		pageView->m_modelSize = m_modelSize;
	}
}

NS_CC_EXT_END