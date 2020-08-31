//
//  CCCCWebView.cpp
//  quickcocos2dx
//
//  Created by weesin on 14-10-10.
//  Copyright (c) 2014年 qeeplay.com. All rights reserved.
//

#include "CCWebView.h"

NS_CC_EXTRA_BEGIN

CCWebView::CCWebView() : m_pWebViewImpl(new WebViewImpl(this)) {
}

CCWebView::~CCWebView() {
    delete m_pWebViewImpl;
}

CCWebView *CCWebView::create() {
    auto pRet = new CCWebView();
    if (pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    return nullptr;
}

void CCWebView::setJavascriptInterfaceScheme(const std::string &scheme) {
    m_pWebViewImpl->setJavascriptInterfaceScheme(scheme);
}

void CCWebView::loadData(const std::string &data, const std::string &MIMEType, const std::string &encoding, const std::string &baseURL) {
    m_pWebViewImpl->loadData(data, MIMEType, encoding, baseURL);
}

void CCWebView::loadHTMLString(const std::string &string, const std::string &baseURL) {
    m_pWebViewImpl->loadHTMLString(string, baseURL);
}

void CCWebView::loadUrl(const std::string &url) {
    m_pWebViewImpl->loadUrl(url);
}

void CCWebView::loadFile(const std::string &fileName) {
    m_pWebViewImpl->loadFile(fileName);
}

void CCWebView::stopLoading() {
    m_pWebViewImpl->stopLoading();
}

void CCWebView::reload() {
    m_pWebViewImpl->reload();
}

bool CCWebView::canGoBack() {
    return m_pWebViewImpl->canGoBack();
}

bool CCWebView::canGoForward() {
    return m_pWebViewImpl->canGoForward();
}

void CCWebView::goBack() {
    m_pWebViewImpl->goBack();
}

void CCWebView::goForward() {
    m_pWebViewImpl->goForward();
}

void CCWebView::evaluateJS(const std::string &js) {
    m_pWebViewImpl->evaluateJS(js);
}

void CCWebView::setScalesPageToFit(bool const scalesPageToFit) {
    m_pWebViewImpl->setScalesPageToFit(scalesPageToFit);
}

void CCWebView::draw(void) {
    m_pWebViewImpl->draw();
}

void CCWebView::setVisible(bool visible) {
    CCSprite::setVisible(visible);
    m_pWebViewImpl->setVisible(visible);
}

NS_CC_EXTRA_END