/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2009      Valentin Milea
 Copyright (c) 2011      Zynga Inc.

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
#include "cocoa/CCString.h"
#include "CCNode.h"
#include "support/CCPointExtension.h"
#include "support/TransformUtils.h"
#include "CCCamera.h"
#include "effects/CCGrid.h"
#include "CCDirector.h"
#include "CCScheduler.h"
#include "touch_dispatcher/CCTouch.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "actions/CCActionManager.h"
#include "script_support/CCScriptSupport.h"
#include "shaders/CCGLProgram.h"
#include "layers_scenes_transitions_nodes/CCScene.h"
// externals
#include "kazmath/GL/matrix.h"
#include "support/component/CCComponent.h"
#include "support/component/CCComponentContainer.h"

#if CC_NODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__) (ceil(__ARGS__))
#endif

NS_CC_BEGIN

// XXX: Yes, nodes might have a sort problem once every 15 days if the game runs at 60 FPS and each frame sprites are reordered.
static int s_globalOrderOfArrival = 1;

unsigned int CCNode::g_drawOrder = 0;

CCNode::CCNode(void)
: m_fRotationX(0.0f)
, m_fRotationY(0.0f)
, m_fScaleX(1.0f)
, m_fScaleY(1.0f)
, m_fVertexZ(0.0f)
, m_obPosition(CCPointZero)
, m_fSkewX(0.0f)
, m_fSkewY(0.0f)
, m_obAnchorPointInPoints(CCPointZero)
, m_obAnchorPoint(CCPointZero)
, m_obContentSize(CCSizeZero)
, m_obTextureSize(CCSizeZero)
, m_sAdditionalTransform(CCAffineTransformMakeIdentity())
, m_pCamera(NULL)
// children (lazy allocs)
// lazy alloc
, m_pGrid(NULL)
, m_nZOrder(0)
, m_pChildren(NULL)
, m_pParent(NULL)
// "whole screen" objects. like Scenes and Layers, should set m_bIgnoreAnchorPointForPosition to true
, m_nTag(kCCNodeTagInvalid)
// userData is always inited as nil
, m_pUserData(NULL)
, m_pUserObject(NULL)
, m_pShaderProgram(NULL)
, m_eGLServerState(ccGLServerState(0))
, m_uOrderOfArrival(0)
, m_bRunning(false)
, m_bTransformDirty(true)
, m_bInverseDirty(true)
, m_bAdditionalTransformDirty(false)
, m_bVisible(true)
, m_bIgnoreAnchorPointForPosition(false)
, m_bReorderChildDirty(false)
, m_nScriptHandler(0)
, m_nUpdateScriptHandler(0)
, m_pComponentContainer(NULL)
, m_displayedOpacity(255)
, m_realOpacity(255)
, m_isOpacityModifyRGB(false)
, m_displayedColor(ccWHITE)
, m_realColor(ccWHITE)
, m_cascadeColorEnabled(false)
, m_cascadeOpacityEnabled(false)
, m_drawOrder(0)
, m_bTouchEnabled(false)
, m_pScriptTouchHandlerEntry(NULL)
, m_nTouchPriority(0)
, m_eTouchMode(kCCTouchesOneByOne)
// add by weixin begin
, m_fShaderParam1(0.0)
// add by weixin end
// add by hanyi start here
, m_bUpdateEventEnabled(false)
// add by hanyi end here
{
    // set default scheduler and actionManager
    CCDirector *director = CCDirector::sharedDirector();
    m_pActionManager = director->getActionManager();
    m_pActionManager->retain();
    m_pScheduler = director->getScheduler();
    m_pScheduler->retain();

    CCScriptEngineProtocol* pEngine = CCScriptEngineManager::sharedManager()->getScriptEngine();
    m_eScriptType = pEngine != NULL ? pEngine->getScriptType() : kScriptTypeNone;
    m_pComponentContainer = new CCComponentContainer(this);
}

CCNode::~CCNode(void)
{
    CCLOGINFO( "cocos2d: deallocing" );

    unregisterScriptHandler();
    if (m_nUpdateScriptHandler)
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nUpdateScriptHandler);
    }
    unregisterScriptTouchHandler();

    CC_SAFE_RELEASE(m_pActionManager);
    CC_SAFE_RELEASE(m_pScheduler);
    // attributes
    CC_SAFE_RELEASE(m_pCamera);

    CC_SAFE_RELEASE(m_pGrid);
    CC_SAFE_RELEASE(m_pShaderProgram);
    CC_SAFE_RELEASE(m_pUserObject);

    if(m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(m_pChildren, child)
        {
            CCNode* pChild = (CCNode*) child;
            if (pChild)
            {
                pChild->m_pParent = NULL;
            }
        }
    }

    // children
    CC_SAFE_RELEASE(m_pChildren);

    // m_pComsContainer
    m_pComponentContainer->removeAll();
    CC_SAFE_DELETE(m_pComponentContainer);
}

bool CCNode::init()
{
    return true;
}

float CCNode::getSkewX()
{
    return m_fSkewX;
}

void CCNode::setSkewX(float newSkewX)
{
    m_fSkewX = newSkewX;
    m_bTransformDirty = m_bInverseDirty = true;
}

float CCNode::getSkewY()
{
    return m_fSkewY;
}

void CCNode::setSkewY(float newSkewY)
{
    m_fSkewY = newSkewY;

    m_bTransformDirty = m_bInverseDirty = true;
}

/// zOrder getter
int CCNode::getZOrder()
{
    return m_nZOrder;
}

/// zOrder setter : private method
/// used internally to alter the zOrder variable. DON'T call this method manually
void CCNode::_setZOrder(int z)
{
    m_nZOrder = z;
}

void CCNode::setZOrder(int z)
{
    _setZOrder(z);
    if (m_pParent)
    {
        m_pParent->reorderChild(this, z);
    }
}

/// vertexZ getter
float CCNode::getVertexZ()
{
    return m_fVertexZ;
}


/// vertexZ setter
void CCNode::setVertexZ(float var)
{
    m_fVertexZ = var;
}


/// rotation getter
float CCNode::getRotation()
{
    CCAssert(m_fRotationX == m_fRotationY, "CCNode#rotation. RotationX != RotationY. Don't know which one to return");
    return m_fRotationX;
}

/// rotation setter
void CCNode::setRotation(float newRotation)
{
    m_fRotationX = m_fRotationY = newRotation;
    m_bTransformDirty = m_bInverseDirty = true;
}

float CCNode::getRotationX()
{
    return m_fRotationX;
}

void CCNode::setRotationX(float fRotationX)
{
    m_fRotationX = fRotationX;
    m_bTransformDirty = m_bInverseDirty = true;
}

float CCNode::getRotationY()
{
    return m_fRotationY;
}

void CCNode::setRotationY(float fRotationY)
{
    m_fRotationY = fRotationY;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// scale getter
float CCNode::getScale(void)
{
    CCAssert( m_fScaleX == m_fScaleY, "CCNode#scale. ScaleX != ScaleY. Don't know which one to return");
    return m_fScaleX;
}

/// scale setter
void CCNode::setScale(float scale)
{
    m_fScaleX = m_fScaleY = scale;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// scale setter
void CCNode::setScale(float fScaleX,float fScaleY)
{
    m_fScaleX = fScaleX;
    m_fScaleY = fScaleY;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// scaleX getter
float CCNode::getScaleX()
{
    return m_fScaleX;
}

/// scaleX setter
void CCNode::setScaleX(float newScaleX)
{
    m_fScaleX = newScaleX;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// scaleY getter
float CCNode::getScaleY()
{
    return m_fScaleY;
}

/// scaleY setter
void CCNode::setScaleY(float newScaleY)
{
    m_fScaleY = newScaleY;
    m_bTransformDirty = m_bInverseDirty = true;
}

/// position getter
const CCPoint& CCNode::getPosition()
{
    return m_obPosition;
}

/// position setter
void CCNode::setPosition(const CCPoint& newPosition)
{
    m_obPosition = newPosition;
    m_bTransformDirty = m_bInverseDirty = true;
}

void CCNode::getPosition(float* x, float* y)
{
    *x = m_obPosition.x;
    *y = m_obPosition.y;
}

void CCNode::setPosition(float x, float y)
{
    setPosition(ccp(x, y));
}

float CCNode::getPositionX(void)
{
    return m_obPosition.x;
}

float CCNode::getPositionY(void)
{
    return  m_obPosition.y;
}

void CCNode::setPositionX(float x)
{
    setPosition(ccp(x, m_obPosition.y));
}

void CCNode::setPositionY(float y)
{
    setPosition(ccp(m_obPosition.x, y));
}

/// children getter
CCArray* CCNode::getChildren()
{
    return m_pChildren;
}

unsigned int CCNode::getChildrenCount(void) const
{
    return m_pChildren ? m_pChildren->count() : 0;
}

/// camera getter: lazy alloc
CCCamera* CCNode::getCamera()
{
    if (!m_pCamera)
    {
        m_pCamera = new CCCamera();
    }

    return m_pCamera;
}


/// grid getter
CCGridBase* CCNode::getGrid()
{
    return m_pGrid;
}

/// grid setter
void CCNode::setGrid(CCGridBase* pGrid)
{
    CC_SAFE_RETAIN(pGrid);
    CC_SAFE_RELEASE(m_pGrid);
    m_pGrid = pGrid;
}


/// isVisible getter
bool CCNode::isVisible()
{
    return m_bVisible;
}

/// isVisible setter
void CCNode::setVisible(bool var)
{
    m_bVisible = var;
}

const CCPoint& CCNode::getAnchorPointInPoints()
{
    return m_obAnchorPointInPoints;
}

/// anchorPoint getter
const CCPoint& CCNode::getAnchorPoint()
{
    return m_obAnchorPoint;
}

void CCNode::setAnchorPoint(const CCPoint& point)
{
    if( ! point.equals(m_obAnchorPoint))
    {
        m_obAnchorPoint = point;
        m_obAnchorPointInPoints = ccp(m_obContentSize.width * m_obAnchorPoint.x, m_obContentSize.height * m_obAnchorPoint.y );
        m_bTransformDirty = m_bInverseDirty = true;
    }
}

/// contentSize getter
const CCSize& CCNode::getContentSize() const
{
    return m_obContentSize;
}

void CCNode::setContentSize(const CCSize & size)
{
    if ( ! size.equals(m_obContentSize))
    {
        m_obContentSize = size;

        m_obAnchorPointInPoints = ccp(m_obContentSize.width * m_obAnchorPoint.x, m_obContentSize.height * m_obAnchorPoint.y );
        m_bTransformDirty = m_bInverseDirty = true;
    }
}

// isRunning getter
bool CCNode::isRunning()
{
    return m_bRunning;
}

/// parent getter
CCNode * CCNode::getParent()
{
    return m_pParent;
}
/// parent setter
void CCNode::setParent(CCNode * var)
{
    if (var == NULL)
    {
        if (m_bTouchEnabled)
        {
            CCScene *scene = getScene();
            if (scene)
            {
                scene->removeTouchableNode(this);
            }
            m_bTouchEnabled = false;
        }
    }
    m_pParent = var;
}

/// isRelativeAnchorPoint getter
bool CCNode::isIgnoreAnchorPointForPosition()
{
    return m_bIgnoreAnchorPointForPosition;
}
/// isRelativeAnchorPoint setter
void CCNode::ignoreAnchorPointForPosition(bool newValue)
{
    if (newValue != m_bIgnoreAnchorPointForPosition)
    {
		m_bIgnoreAnchorPointForPosition = newValue;
		m_bTransformDirty = m_bInverseDirty = true;
	}
}

/// tag getter
int CCNode::getTag() const
{
    return m_nTag;
}

/// tag setter
void CCNode::setTag(int var)
{
    m_nTag = var;
}

/// userData getter
void * CCNode::getUserData()
{
    return m_pUserData;
}

/// userData setter
void CCNode::setUserData(void *var)
{
    m_pUserData = var;
}

unsigned int CCNode::getOrderOfArrival()
{
    return m_uOrderOfArrival;
}

void CCNode::setOrderOfArrival(unsigned int uOrderOfArrival)
{
    m_uOrderOfArrival = uOrderOfArrival;
}

CCGLProgram* CCNode::getShaderProgram()
{
    return m_pShaderProgram;
}

CCObject* CCNode::getUserObject()
{
    return m_pUserObject;
}

ccGLServerState CCNode::getGLServerState()
{
    return m_eGLServerState;
}

void CCNode::setGLServerState(ccGLServerState glServerState)
{
    m_eGLServerState = glServerState;
}

void CCNode::setUserObject(CCObject *pUserObject)
{
    CC_SAFE_RETAIN(pUserObject);
    CC_SAFE_RELEASE(m_pUserObject);
    m_pUserObject = pUserObject;
}

void CCNode::setShaderProgram(CCGLProgram *pShaderProgram)
{
    if (!pShaderProgram)
    {
        return;
    }
    CC_SAFE_RETAIN(pShaderProgram);
    CC_SAFE_RELEASE(m_pShaderProgram);
    m_pShaderProgram = pShaderProgram;
}

CCRect CCNode::boundingBox()
{
    CCRect rect = CCRectMake(0, 0, m_obContentSize.width, m_obContentSize.height);
    return CCRectApplyAffineTransform(rect, nodeToParentTransform());
}

CCRect CCNode::getCascadeBoundingBox(bool convertToWorld)
{
    float minx, miny, maxx, maxy = 0;

    bool first = true;

    CCRect box = CCRect(0, 0, 0, 0);
    CCObject *object = NULL;
    CCARRAY_FOREACH(m_pChildren, object)
    {
        CCRect r = dynamic_cast<CCNode*>(object)->getCascadeBoundingBox(false);
        if (r.size.width == 0 || r.size.height == 0) continue;
        r = CCRectApplyAffineTransform(r, nodeToParentTransform());

        if (first)
        {
            box = r;
            first = false;
        }
        else
        {
            minx = r.getMinX() < box.getMinX() ? r.getMinX() : box.getMinX();
            miny = r.getMinY() < box.getMinY() ? r.getMinY() : box.getMinY();
            maxx = r.getMaxX() > box.getMaxX() ? r.getMaxX() : box.getMaxX();
            maxy = r.getMaxY() > box.getMaxY() ? r.getMaxY() : box.getMaxY();
            box.setRect(minx, miny, maxx - minx, maxy - miny);
        }
    }

    CCRect r;
    bool mergeRect = false;
    if (m_cascadeBoundingBox.size.width > 0 && m_cascadeBoundingBox.size.height > 0)
    {
        r = CCRectApplyAffineTransform(m_cascadeBoundingBox, nodeToParentTransform());
        mergeRect = true;
    }
    else if (m_obTextureSize.width > 0 && m_obTextureSize.height > 0)
    {
        r = CCRectApplyAffineTransform(CCRectMake(0, 0, m_obTextureSize.width, m_obTextureSize.height), nodeToParentTransform());
        mergeRect = true;
    }

    if (mergeRect)
    {
        if (first)
        {
            box = r;
        }
        else
        {
            minx = r.getMinX() < box.getMinX() ? r.getMinX() : box.getMinX();
            miny = r.getMinY() < box.getMinY() ? r.getMinY() : box.getMinY();
            maxx = r.getMaxX() > box.getMaxX() ? r.getMaxX() : box.getMaxX();
            maxy = r.getMaxY() > box.getMaxY() ? r.getMaxY() : box.getMaxY();
            box.setRect(minx, miny, maxx - minx, maxy - miny);
        }
    }

    if (convertToWorld && m_pParent)
    {
        box = CCRectApplyAffineTransform(box, m_pParent->nodeToWorldTransform());
    }
    return box;
}

void CCNode::setCascadeBoundingBox(const cocos2d::CCRect &boundingBox)
{
    m_cascadeBoundingBox = boundingBox;
}

CCNode * CCNode::create(void)
{
	CCNode * pRet = new CCNode();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

void CCNode::cleanup()
{
    // actions
    this->stopAllActions();
    this->unscheduleAllSelectors();

    if ( m_eScriptType != kScriptTypeNone)
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->executeNodeEvent(this, kCCNodeOnCleanup);
    }

    // timers
    arrayMakeObjectsPerformSelector(m_pChildren, cleanup, CCNode*);
}


const char* CCNode::description()
{
    return CCString::createWithFormat("<CCNode | Tag = %d>", m_nTag)->getCString();
}

// lazy allocs
void CCNode::childrenAlloc(void)
{
    m_pChildren = CCArray::createWithCapacity(4);
    m_pChildren->retain();
}

CCNode* CCNode::getChildByTag(int aTag)
{
    CCAssert( aTag != kCCNodeTagInvalid, "Invalid tag");

    if(m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(m_pChildren, child)
        {
            CCNode* pNode = (CCNode*) child;
            if(pNode && pNode->m_nTag == aTag)
                return pNode;
        }
    }
    return NULL;
}

/* "add" logic MUST only be on this method
 * If a class want's to extend the 'addChild' behavior it only needs
 * to override this method
 */
void CCNode::addChild(CCNode *child, int zOrder, int tag)
{
    CCAssert( child != NULL, "Argument must be non-nil");
    CCAssert( child->m_pParent == NULL, "child already added. It can't be added again");

    if( ! m_pChildren )
    {
        this->childrenAlloc();
    }

    this->insertChild(child, zOrder);

    child->m_nTag = tag;

    child->setParent(this);
    child->setOrderOfArrival(s_globalOrderOfArrival++);

    if( m_bRunning )
    {
        child->onEnter();
        child->onEnterTransitionDidFinish();
    }
}

void CCNode::addChild(CCNode *child, int zOrder)
{
    CCAssert( child != NULL, "Argument must be non-nil");
    this->addChild(child, zOrder, child->m_nTag);
}

void CCNode::addChild(CCNode *child)
{
    CCAssert( child != NULL, "Argument must be non-nil");
    this->addChild(child, child->m_nZOrder, child->m_nTag);
}

void CCNode::removeFromParent()
{
    this->removeFromParentAndCleanup(true);
}

void CCNode::removeFromParentAndCleanup(bool cleanup)
{
    if (m_pParent != NULL)
    {
        m_pParent->removeChild(this,cleanup);
    }
}

void CCNode::removeChild(CCNode* child)
{
    this->removeChild(child, true);
}

/* "remove" logic MUST only be on this method
 * If a class want's to extend the 'removeChild' behavior it only needs
 * to override this method
 */
void CCNode::removeChild(CCNode* child, bool cleanup)
{
    // explicit nil handling
    if (m_pChildren == NULL)
    {
        return;
    }

    if ( m_pChildren->containsObject(child) )
    {
        this->detachChild(child,cleanup);
    }
}

void CCNode::removeChildByTag(int tag)
{
    this->removeChildByTag(tag, true);
}

void CCNode::removeChildByTag(int tag, bool cleanup)
{
    CCAssert( tag != kCCNodeTagInvalid, "Invalid tag");

    CCNode *child = this->getChildByTag(tag);

    if (child == NULL)
    {
        CCLOG("cocos2d: removeChildByTag(tag = %d): child not found!", tag);
    }
    else
    {
        this->removeChild(child, cleanup);
    }
}

void CCNode::removeAllChildren()
{
    this->removeAllChildrenWithCleanup(true);
}

void CCNode::removeAllChildrenWithCleanup(bool cleanup)
{
    // not using detachChild improves speed here
    if ( m_pChildren && m_pChildren->count() > 0 )
    {
        CCObject* child;
        CCARRAY_FOREACH(m_pChildren, child)
        {
            CCNode* pNode = (CCNode*) child;
            if (pNode)
            {
                // IMPORTANT:
                //  -1st do onExit
                //  -2nd cleanup
                if(m_bRunning)
                {
                    pNode->onExitTransitionDidStart();
                    pNode->onExit();
                }

                if (cleanup)
                {
                    pNode->cleanup();
                }
                // set parent nil at the end
                pNode->setParent(NULL);
            }
        }

        m_pChildren->removeAllObjects();
    }

}

void CCNode::detachChild(CCNode *child, bool doCleanup)
{
    // IMPORTANT:
    //  -1st do onExit
    //  -2nd cleanup
    if (m_bRunning)
    {
        child->onExitTransitionDidStart();
        child->onExit();
    }

    // If you don't do cleanup, the child's actions will not get removed and the
    // its scheduledSelectors_ dict will not get released!
    if (doCleanup)
    {
        child->cleanup();
    }

    // set parent nil at the end
    child->setParent(NULL);

    m_pChildren->removeObject(child);
}


// helper used by reorderChild & add
void CCNode::insertChild(CCNode* child, int z)
{
    m_bReorderChildDirty = true;
    ccArrayAppendObjectWithResize(m_pChildren->data, child);
    child->_setZOrder(z);
}

void CCNode::reorderChild(CCNode *child, int zOrder)
{
    CCAssert( child != NULL, "Child must be non-nil");
    m_bReorderChildDirty = true;
    child->setOrderOfArrival(s_globalOrderOfArrival++);
    child->_setZOrder(zOrder);
}

void CCNode::sortAllChildren()
{
    if (m_bReorderChildDirty)
    {
        int i,j,length = m_pChildren->data->num;
        CCNode ** x = (CCNode**)m_pChildren->data->arr;
        CCNode *tempItem;

        // insertion sort
        for(i=1; i<length; i++)
        {
            tempItem = x[i];
            j = i-1;

            //continue moving element downwards while zOrder is smaller or when zOrder is the same but mutatedIndex is smaller
            while(j>=0 && ( tempItem->m_nZOrder < x[j]->m_nZOrder || ( tempItem->m_nZOrder== x[j]->m_nZOrder && tempItem->m_uOrderOfArrival < x[j]->m_uOrderOfArrival ) ) )
            {
                x[j+1] = x[j];
                j = j-1;
            }
            x[j+1] = tempItem;
        }

        //don't need to check children recursively, that's done in visit of each child

        m_bReorderChildDirty = false;
    }
}

void CCNode::draw()
{
    //CCAssert(0);
    // override me
    // Only use- this function to draw your stuff.
    // DON'T draw your stuff outside this method
}

void CCNode::visit()
{
    m_drawOrder = ++g_drawOrder;
    // quick return if not visible. children won't be drawn.
    if (!m_bVisible) return;
    kmGLPushMatrix();

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
    }

    this->transform();

    CCNode* pNode = NULL;
    unsigned int i = 0;

    if(m_pChildren && m_pChildren->count() > 0)
    {
        sortAllChildren();
        // draw children zOrder < 0
        ccArray *arrayData = m_pChildren->data;
        for( ; i < arrayData->num; i++ )
        {
            pNode = (CCNode*) arrayData->arr[i];

            if ( pNode && pNode->m_nZOrder < 0 )
            {
                pNode->visit();
            }
            else
            {
                break;
            }
        }
        // self draw
        this->draw();

        for( ; i < arrayData->num; i++ )
        {
            pNode = (CCNode*) arrayData->arr[i];
            if (pNode)
            {
                pNode->visit();
            }
        }
    }
    else
    {
        this->draw();
    }

    // reset for next frame
    m_uOrderOfArrival = 0;

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }

    kmGLPopMatrix();
}

void CCNode::transformAncestors()
{
    if( m_pParent != NULL  )
    {
        m_pParent->transformAncestors();
        m_pParent->transform();
    }
}

void CCNode::transform()
{
    kmMat4 transfrom4x4;

    // Convert 3x3 into 4x4 matrix
    CCAffineTransform tmpAffine = this->nodeToParentTransform();
    CGAffineToGL(&tmpAffine, transfrom4x4.mat);

    // Update Z vertex manually
    transfrom4x4.mat[14] = m_fVertexZ;

    kmGLMultMatrix( &transfrom4x4 );


    // XXX: Expensive calls. Camera should be integrated into the cached affine matrix
    if ( m_pCamera != NULL && !(m_pGrid != NULL && m_pGrid->isActive()) )
    {
        bool translate = (m_obAnchorPointInPoints.x != 0.0f || m_obAnchorPointInPoints.y != 0.0f);

        if( translate )
            kmGLTranslatef(RENDER_IN_SUBPIXEL(m_obAnchorPointInPoints.x), RENDER_IN_SUBPIXEL(m_obAnchorPointInPoints.y), 0 );

        m_pCamera->locate();

        if( translate )
            kmGLTranslatef(RENDER_IN_SUBPIXEL(-m_obAnchorPointInPoints.x), RENDER_IN_SUBPIXEL(-m_obAnchorPointInPoints.y), 0 );
    }

}


void CCNode::onEnter()
{
    arrayMakeObjectsPerformSelector(m_pChildren, onEnter, CCNode*);

    this->resumeSchedulerAndActions();

    m_bRunning = true;

    if (m_bTouchEnabled)
    {
        this->registerWithTouchDispatcher();
    }

    if (m_nScriptHandler || hasScriptEventListener(ENTER_SCENE_EVENT))
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->executeNodeEvent(this, kCCNodeOnEnter);
    }
}

void CCNode::onEnterTransitionDidFinish()
{
    arrayMakeObjectsPerformSelector(m_pChildren, onEnterTransitionDidFinish, CCNode*);

    if (m_nScriptHandler || hasScriptEventListener(ENTER_TRANSITION_DID_FINISH_EVENT))
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->executeNodeEvent(this, kCCNodeOnEnterTransitionDidFinish);
    }
}

void CCNode::onExitTransitionDidStart()
{
    arrayMakeObjectsPerformSelector(m_pChildren, onExitTransitionDidStart, CCNode*);

    if (m_nScriptHandler || hasScriptEventListener(EXIT_TRANSITION_DID_START_EVENT))
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->executeNodeEvent(this, kCCNodeOnExitTransitionDidStart);
    }
}

void CCNode::onExit()
{
    this->pauseSchedulerAndActions();

    if( m_bTouchEnabled )
    {
        unregisterWithTouchDispatcher();
    }

    m_bRunning = false;

    if (m_nScriptHandler || hasScriptEventListener(EXIT_SCENE_EVENT))
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->executeNodeEvent(this, kCCNodeOnExit);
    }

    arrayMakeObjectsPerformSelector(m_pChildren, onExit, CCNode*);
}

void CCNode::registerScriptHandler(int nHandler)
{
    unregisterScriptHandler();
    m_nScriptHandler = nHandler;
    LUALOG("[LUA] Add CCNode event handler: %d", m_nScriptHandler);
}

void CCNode::unregisterScriptHandler(void)
{
    if (m_nScriptHandler)
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nScriptHandler);
        LUALOG("[LUA] Remove CCNode event handler: %d", m_nScriptHandler);
        m_nScriptHandler = 0;
    }
}

void CCNode::setActionManager(CCActionManager* actionManager)
{
    if( actionManager != m_pActionManager ) {
        this->stopAllActions();
        CC_SAFE_RETAIN(actionManager);
        CC_SAFE_RELEASE(m_pActionManager);
        m_pActionManager = actionManager;
    }
}

CCActionManager* CCNode::getActionManager()
{
    return m_pActionManager;
}

CCAction * CCNode::runAction(CCAction* action)
{
    CCAssert( action != NULL, "Argument must be non-nil");
    m_pActionManager->addAction(action, this, !m_bRunning);
    return action;
}

void CCNode::stopAllActions()
{
    m_pActionManager->removeAllActionsFromTarget(this);
}

void CCNode::stopAction(CCAction* action)
{
    m_pActionManager->removeAction(action);
}

void CCNode::stopActionByTag(int tag)
{
    CCAssert( tag != kCCActionTagInvalid, "Invalid tag");
    m_pActionManager->removeActionByTag(tag, this);
}

CCAction * CCNode::getActionByTag(int tag)
{
    CCAssert( tag != kCCActionTagInvalid, "Invalid tag");
    return m_pActionManager->getActionByTag(tag, this);
}

unsigned int CCNode::numberOfRunningActions()
{
    return m_pActionManager->numberOfRunningActionsInTarget(this);
}

// CCNode - Callbacks

void CCNode::setScheduler(CCScheduler* scheduler)
{
    if( scheduler != m_pScheduler ) {
        this->unscheduleAllSelectors();
        CC_SAFE_RETAIN(scheduler);
        CC_SAFE_RELEASE(m_pScheduler);
        m_pScheduler = scheduler;
    }
}

CCScheduler* CCNode::getScheduler()
{
    return m_pScheduler;
}

void CCNode::scheduleUpdate()
{
    scheduleUpdateWithPriority(0);
}

void CCNode::scheduleUpdateWithPriority(int priority)
{
    m_pScheduler->scheduleUpdateForTarget(this, priority, !m_bRunning);
}

void CCNode::scheduleUpdateWithPriorityLua(int nHandler, int priority)
{
    unscheduleUpdate();
    m_nUpdateScriptHandler = nHandler;
    m_pScheduler->scheduleUpdateForTarget(this, priority, !m_bRunning);
}

void CCNode::scheduleUpdateForNodeEvent()
{
    scheduleUpdateWithPriority(0);
}

void CCNode::unscheduleUpdate()
{
    m_pScheduler->unscheduleUpdateForTarget(this);
    if (m_nUpdateScriptHandler)
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nUpdateScriptHandler);
        m_nUpdateScriptHandler = 0;
    }
}

// add by hanyi start here
void CCNode::setUpdateEventEnabled(bool enable)
{
	m_bUpdateEventEnabled = enable;
	if (enable)
	{
		scheduleUpdate();
	}
	else
	{
		unscheduleUpdate();
	}
}

// add by hanyi end here

void CCNode::schedule(SEL_SCHEDULE selector)
{
    this->schedule(selector, 0.0f, kCCRepeatForever, 0.0f);
}

void CCNode::schedule(SEL_SCHEDULE selector, float interval)
{
    this->schedule(selector, interval, kCCRepeatForever, 0.0f);
}

void CCNode::schedule(SEL_SCHEDULE selector, float interval, unsigned int repeat, float delay)
{
    CCAssert( selector, "Argument must be non-nil");
    CCAssert( interval >=0, "Argument must be positive");

    m_pScheduler->scheduleSelector(selector, this, interval , repeat, delay, !m_bRunning);
}

void CCNode::scheduleOnce(SEL_SCHEDULE selector, float delay)
{
    this->schedule(selector, 0.0f, 0, delay);
}

void CCNode::unschedule(SEL_SCHEDULE selector)
{
    // explicit nil handling
    if (selector == 0)
        return;

    m_pScheduler->unscheduleSelector(selector, this);
}

void CCNode::unscheduleAllSelectors()
{
    m_pScheduler->unscheduleAllForTarget(this);
}

void CCNode::resumeSchedulerAndActions()
{
    m_pScheduler->resumeTarget(this);
    m_pActionManager->resumeTarget(this);
}

void CCNode::pauseSchedulerAndActions()
{
    m_pScheduler->pauseTarget(this);
    m_pActionManager->pauseTarget(this);
}

// override me
void CCNode::update(float fDelta)
{
    if (m_nUpdateScriptHandler)
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->executeSchedule(m_nUpdateScriptHandler, fDelta, this);
    }
    if (m_bUpdateEventEnabled || hasScriptEventListener(ENTER_FRAME_EVENT))
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->executeNodeEvent(this, kCCNodeOnEnterFrame, fDelta);
    }
    if (m_pComponentContainer && !m_pComponentContainer->isEmpty())
    {
        m_pComponentContainer->visit(fDelta);
    }
}

CCAffineTransform CCNode::nodeToParentTransform(void)
{
    if (m_bTransformDirty)
    {

        // Translate values
        float x = m_obPosition.x;
        float y = m_obPosition.y;

        if (m_bIgnoreAnchorPointForPosition)
        {
            x += m_obAnchorPointInPoints.x;
            y += m_obAnchorPointInPoints.y;
        }

        // Rotation values
		// Change rotation code to handle X and Y
		// If we skew with the exact same value for both x and y then we're simply just rotating
        float cx = 1, sx = 0, cy = 1, sy = 0;
        if (m_fRotationX || m_fRotationY)
        {
            float radiansX = -CC_DEGREES_TO_RADIANS(m_fRotationX);
            float radiansY = -CC_DEGREES_TO_RADIANS(m_fRotationY);
            cx = cosf(radiansX);
            sx = sinf(radiansX);
            cy = cosf(radiansY);
            sy = sinf(radiansY);
        }

        bool needsSkewMatrix = ( m_fSkewX || m_fSkewY );


        // optimization:
        // inline anchor point calculation if skew is not needed
        // Adjusted transform calculation for rotational skew
        if (! needsSkewMatrix && !m_obAnchorPointInPoints.equals(CCPointZero))
        {
            x += cy * -m_obAnchorPointInPoints.x * m_fScaleX + -sx * -m_obAnchorPointInPoints.y * m_fScaleY;
            y += sy * -m_obAnchorPointInPoints.x * m_fScaleX +  cx * -m_obAnchorPointInPoints.y * m_fScaleY;
        }


        // Build Transform Matrix
        // Adjusted transform calculation for rotational skew
        m_sTransform = CCAffineTransformMake( cy * m_fScaleX,  sy * m_fScaleX,
                                             -sx * m_fScaleY, cx * m_fScaleY,
                                             x, y );

        // XXX: Try to inline skew
        // If skew is needed, apply skew and then anchor point
        if (needsSkewMatrix)
        {
            CCAffineTransform skewMatrix = CCAffineTransformMake(1.0f, tanf(CC_DEGREES_TO_RADIANS(m_fSkewY)),
                                                                 tanf(CC_DEGREES_TO_RADIANS(m_fSkewX)), 1.0f,
                                                                 0.0f, 0.0f );
            m_sTransform = CCAffineTransformConcat(skewMatrix, m_sTransform);

            // adjust anchor point
            if (!m_obAnchorPointInPoints.equals(CCPointZero))
            {
                m_sTransform = CCAffineTransformTranslate(m_sTransform, -m_obAnchorPointInPoints.x, -m_obAnchorPointInPoints.y);
            }
        }

        if (m_bAdditionalTransformDirty)
        {
            m_sTransform = CCAffineTransformConcat(m_sTransform, m_sAdditionalTransform);
            m_bAdditionalTransformDirty = false;
        }

        m_bTransformDirty = false;
    }

    return m_sTransform;
}

void CCNode::setAdditionalTransform(const CCAffineTransform& additionalTransform)
{
    m_sAdditionalTransform = additionalTransform;
    m_bTransformDirty = true;
    m_bAdditionalTransformDirty = true;
}

CCAffineTransform CCNode::parentToNodeTransform(void)
{
    if ( m_bInverseDirty ) {
        m_sInverse = CCAffineTransformInvert(this->nodeToParentTransform());
        m_bInverseDirty = false;
    }

    return m_sInverse;
}

CCAffineTransform CCNode::nodeToWorldTransform()
{
    CCAffineTransform t = this->nodeToParentTransform();

    for (CCNode *p = m_pParent; p != NULL; p = p->getParent())
        t = CCAffineTransformConcat(t, p->nodeToParentTransform());

    return t;
}

CCAffineTransform CCNode::worldToNodeTransform(void)
{
    return CCAffineTransformInvert(this->nodeToWorldTransform());
}

CCPoint CCNode::convertToNodeSpace(const CCPoint& worldPoint)
{
    CCPoint ret = CCPointApplyAffineTransform(worldPoint, worldToNodeTransform());
    return ret;
}

CCPoint CCNode::convertToWorldSpace(const CCPoint& nodePoint)
{
    CCPoint ret = CCPointApplyAffineTransform(nodePoint, nodeToWorldTransform());
    return ret;
}

CCPoint CCNode::convertToNodeSpaceAR(const CCPoint& worldPoint)
{
    CCPoint nodePoint = convertToNodeSpace(worldPoint);
    return ccpSub(nodePoint, m_obAnchorPointInPoints);
}

CCPoint CCNode::convertToWorldSpaceAR(const CCPoint& nodePoint)
{
    CCPoint pt = ccpAdd(nodePoint, m_obAnchorPointInPoints);
    return convertToWorldSpace(pt);
}

CCPoint CCNode::convertToWindowSpace(const CCPoint& nodePoint)
{
    CCPoint worldPoint = this->convertToWorldSpace(nodePoint);
    return CCDirector::sharedDirector()->convertToUI(worldPoint);
}

// convenience methods which take a CCTouch instead of CCPoint
CCPoint CCNode::convertTouchToNodeSpace(CCTouch *touch)
{
    CCPoint point = touch->getLocation();
    return this->convertToNodeSpace(point);
}
CCPoint CCNode::convertTouchToNodeSpaceAR(CCTouch *touch)
{
    CCPoint point = touch->getLocation();
    return this->convertToNodeSpaceAR(point);
}

void CCNode::updateTransform()
{
    m_drawOrder = ++g_drawOrder;
    // Recursively iterate over children
    arrayMakeObjectsPerformSelector(m_pChildren, updateTransform, CCNode*);
}

CCComponent* CCNode::getComponent(const char *pName) const
{
    return m_pComponentContainer->get(pName);
}

bool CCNode::addComponent(CCComponent *pComponent)
{
    return m_pComponentContainer->add(pComponent);
}

bool CCNode::removeComponent(const char *pName)
{
    return m_pComponentContainer->remove(pName);
}

void CCNode::removeAllComponents()
{
    m_pComponentContainer->removeAll();
}

// CCNode

GLubyte CCNode::getOpacity(void)
{
	return m_realOpacity;
}

GLubyte CCNode::getDisplayedOpacity(void)
{
	return m_displayedOpacity;
}

void CCNode::setOpacity(GLubyte opacity)
{
    m_displayedOpacity = m_realOpacity = opacity;

	if (m_cascadeOpacityEnabled)
    {
		GLubyte parentOpacity = 255;
        if (m_pParent && m_pParent->isCascadeOpacityEnabled())
        {
            parentOpacity = m_pParent->getDisplayedOpacity();
        }
        this->updateDisplayedOpacity(parentOpacity);
    }
}

void CCNode::setOpacityModifyRGB(bool var)
{
    m_isOpacityModifyRGB = var;
    if (m_pChildren && m_pChildren->count() != 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(m_pChildren, child)
        {
            dynamic_cast<CCNode*>(child)->setOpacityModifyRGB(var);
        }
    }
}

bool CCNode::isOpacityModifyRGB(void)
{
    return m_isOpacityModifyRGB;
}

void CCNode::updateDisplayedOpacity(GLubyte parentOpacity)
{
	m_displayedOpacity = (GLubyte)(m_realOpacity * parentOpacity/255.0);

    if (m_cascadeOpacityEnabled)
    {
        CCObject* pObj;
        CCARRAY_FOREACH(m_pChildren, pObj)
        {
            dynamic_cast<CCNode*>(pObj)->updateDisplayedOpacity(m_displayedOpacity);
        }
    }
}

bool CCNode::isCascadeOpacityEnabled(void)
{
    return m_cascadeOpacityEnabled;
}

void CCNode::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
    m_cascadeOpacityEnabled = cascadeOpacityEnabled;
}

const ccColor3B& CCNode::getColor(void)
{
	return m_realColor;
}

const ccColor3B& CCNode::getDisplayedColor()
{
	return m_displayedColor;
}

void CCNode::setColor(const ccColor3B& color)
{
	m_displayedColor = m_realColor = color;

	if (m_cascadeColorEnabled)
    {
		ccColor3B parentColor = ccWHITE;
		if (m_pParent && m_pParent->isCascadeColorEnabled())
        {
            parentColor = m_pParent->getDisplayedColor();
        }

        updateDisplayedColor(parentColor);
	}
}

void CCNode::updateDisplayedColor(const ccColor3B& parentColor)
{
	m_displayedColor.r = (GLubyte)(m_realColor.r * (float)parentColor.r/255.0f);
	m_displayedColor.g = (GLubyte)(m_realColor.g * (float)parentColor.g/255.0f);
	m_displayedColor.b = (GLubyte)(m_realColor.b * (float)parentColor.b/255.0f);

    if (m_cascadeColorEnabled)
    {
        CCObject *obj = NULL;
        CCARRAY_FOREACH(m_pChildren, obj)
        {
            dynamic_cast<CCNode*>(obj)->updateDisplayedColor(m_displayedColor);
        }
    }
}

bool CCNode::isCascadeColorEnabled(void)
{
    return m_cascadeColorEnabled;
}

void CCNode::setCascadeColorEnabled(bool cascadeColorEnabled)
{
    m_cascadeColorEnabled = cascadeColorEnabled;
}

// ----------------------------------------

CCScene *CCNode::getScene()
{
    if (!m_bRunning) return NULL;
    CCNode *parent = getParent();
    if (!parent) return NULL;

    CCNode *scene = parent;
    while (parent)
    {
        parent = parent->getParent();
        if (parent) scene = parent;
    }
    return dynamic_cast<CCScene*>(scene);
}


void CCNode::registerWithTouchDispatcher()
{
    //    CCLOG("CCNODE: REGISTER WITH TOUCH DISPATHCER");
    CCScene *scene = getScene();
    if (scene)
    {
        scene->addTouchableNode(this);
    }
}

void CCNode::unregisterWithTouchDispatcher()
{
    //    CCLOG("CCNODE: REGISTER WITH TOUCH DISPATHCER");
    CCScene *scene = getScene();
    if (scene)
    {
        scene->removeTouchableNode(this);
    }
}

void CCNode::registerScriptTouchHandler(int nHandler, bool bIsMultiTouches, int nPriority, bool bSwallowsTouches)
{
    unregisterScriptTouchHandler();
    m_pScriptTouchHandlerEntry = CCTouchScriptHandlerEntry::create(nHandler, bIsMultiTouches, nPriority, bSwallowsTouches);
    m_pScriptTouchHandlerEntry->retain();
}

void CCNode::unregisterScriptTouchHandler(void)
{
    CC_SAFE_RELEASE_NULL(m_pScriptTouchHandlerEntry);
}

int CCNode::excuteScriptTouchHandler(int nEventType, CCTouch *pTouch)
{
    return CCScriptEngineManager::sharedManager()->getScriptEngine()->executeNodeTouchEvent(this, nEventType, pTouch);
}

int CCNode::excuteScriptTouchHandler(int nEventType, CCSet *pTouches)
{
    return CCScriptEngineManager::sharedManager()->getScriptEngine()->executeNodeTouchesEvent(this, nEventType, pTouches);
}

/// isTouchEnabled getter
bool CCNode::isTouchEnabled()
{
    return m_bTouchEnabled;
}
/// isTouchEnabled setter

void CCNode::setTouchEnabled(bool enabled)
{
    if (m_bTouchEnabled != enabled)
    {
        m_bTouchEnabled = enabled;
        if (m_bRunning)
        {
            if (enabled)
            {
                registerWithTouchDispatcher();
            }
            else
            {
                unregisterWithTouchDispatcher();
            }
        }
    }
}

void CCNode::setTouchMode(ccTouchesMode mode)
{
    if(m_eTouchMode != mode)
    {
        m_eTouchMode = mode;

		if( m_bTouchEnabled)
        {
			setTouchEnabled(false);
			setTouchEnabled(true);
		}
    }
}

void CCNode::setTouchPriority(int priority)
{
    if (m_nTouchPriority != priority)
    {
        m_nTouchPriority = priority;

		if( m_bTouchEnabled)
        {
			setTouchEnabled(false);
			setTouchEnabled(true);
        }
    }
}

int CCNode::getTouchPriority()
{
    return m_nTouchPriority;
}

int CCNode::getTouchMode()
{
    return m_eTouchMode;
}


int CCNode::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (kScriptTypeNone != m_eScriptType)
    {
        return excuteScriptTouchHandler(CCTOUCHBEGAN, pTouch);
    }

    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    CCAssert(false, "Layer#ccTouchBegan override me");
    return kCCTouchBegan;
}

int CCNode::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (kScriptTypeNone != m_eScriptType)
    {
        return excuteScriptTouchHandler(CCTOUCHMOVED, pTouch);
    }

    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    return kCCTouchMoved;
}

void CCNode::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (kScriptTypeNone != m_eScriptType)
    {
        excuteScriptTouchHandler(CCTOUCHENDED, pTouch);
        return;
    }

    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CCNode::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    if (kScriptTypeNone != m_eScriptType)
    {
        excuteScriptTouchHandler(CCTOUCHCANCELLED, pTouch);
        return;
    }

    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void CCNode::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    if (kScriptTypeNone != m_eScriptType)
    {
        excuteScriptTouchHandler(CCTOUCHBEGAN, pTouches);
        return;
    }

    CC_UNUSED_PARAM(pTouches);
    CC_UNUSED_PARAM(pEvent);
}

void CCNode::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if (kScriptTypeNone != m_eScriptType)
    {
        excuteScriptTouchHandler(CCTOUCHMOVED, pTouches);
        return;
    }
    
    CC_UNUSED_PARAM(pTouches);
    CC_UNUSED_PARAM(pEvent);
}

void CCNode::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    if (kScriptTypeNone != m_eScriptType)
    {
        excuteScriptTouchHandler(CCTOUCHENDED, pTouches);
        return;
    }
    
    CC_UNUSED_PARAM(pTouches);
    CC_UNUSED_PARAM(pEvent);
}

void CCNode::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    if (kScriptTypeNone != m_eScriptType)
    {
        excuteScriptTouchHandler(CCTOUCHCANCELLED, pTouches);
        return;
    }
    
    CC_UNUSED_PARAM(pTouches);
    CC_UNUSED_PARAM(pEvent);
}

NS_CC_END