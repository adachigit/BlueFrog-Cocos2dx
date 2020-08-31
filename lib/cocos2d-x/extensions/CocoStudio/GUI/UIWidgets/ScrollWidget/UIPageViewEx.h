#ifndef __UIPAGEVIEWEX_H_
#define __UIPAGEVIEWEX_H_

#include "UIListViewEx.h"

NS_CC_EXT_BEGIN

class UIPageViewEx : public UIListViewEx
{
public:
	UIPageViewEx();
	virtual ~UIPageViewEx();

	static UIPageViewEx* create();

	void setPageModel(UILayout* pModel);
	UILayout* pushBackModelPage();
	void setBounceSpeed(const float speed) { m_bounceSpeed = speed; }
	void setOverLengthRatio(const float ratio)
	{
		m_overLengthRatio = (ratio > 0.5f) ? 0.5f : ratio;
		updateLimitCoordinate();
	}
	int getCurPageIndex() { return m_curPageIndex; }
	UILayout* getCurPage() { return (UILayout*)getItem(m_curPageIndex); }
	void jumpToPage(int index);

	void setAccelaration(float accelaration) { m_accelaration = accelaration; }
	void setOneByOne(bool isOneByOne);
	virtual const char* getDescription() const;
	virtual CCArray* getChildren(){ return UIListViewEx::getChildren(); };

	void refreshView();

	void scrollToPage(int index, float speed);
protected:
	void updateLimitCoordinate();

	virtual void interceptTouchEvent(int handleState, UIWidget* sender, const cocos2d::CCPoint &touchPoint);
	virtual void handlePressLogic(const CCPoint &touchPoint);
	virtual void handleMoveLogic(const CCPoint &touchPoint);
	virtual void handleMoveEndLogic(const CCPoint &touchPoint);
	virtual void handleReleaseLogic(const CCPoint &touchPoint);

	virtual UIWidget* createCloneInstance();

	virtual void checkChildInfo(int handleState, UIWidget* sender, const CCPoint &touchPoint);

	CCPoint getPageFocusPosition(int index);
	CCPoint getPageBackOverPosition(int index);
	CCPoint getPageFrontOverPosition(int index);
	int getPageIndexByInnerPosition(const CCPoint& pos);

	void endSlideAction();
	void startAutoScroll(float speed, float accelaration);
	void startAutoScrollWithDestination(const CCPoint& dest, float time, float accelaration);
	void stopAutoScroll(bool needBounce);
	void autoScrollChildren(float dt);

	void startAutoBounceScroll();

	bool moveChildren(float offsetX, float offsetY);
	virtual void update(float dt);

	void onPageFocused(int index);
	void executeScrollPercentEvent();

	virtual void copySpecialProperties(UIWidget* model);
protected:
	float m_scrollAccelaration;
	float m_accelaration;
	float m_autoScrollDir;
	bool m_bCheckScrollDestination;
	CCPoint m_scrollDestination;

	CCPoint m_frontScrollLimitPos;
	CCPoint m_backScrollLimitPos;
	bool m_bOneByOne;

	float m_bounceSpeed;

	float m_overLengthRatio;
	CCSize m_modelSize;

	int m_curPageIndex;
	int m_curThroughPageIndex;

	/*for scroll*/
	bool m_bAutoScroll;
	bool m_bAutoBounceScroll;

	bool m_bPressed;
	float m_curScrollSpeed;
	float m_slideTime;
	CCPoint m_touchBeganPoint;
	CCPoint m_touchMovingPoint;
	CCPoint m_touchEndPoint;
};

NS_CC_EXT_END

#endif