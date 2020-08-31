//
//  WebViewImpl_IOS.mm
//  quickcocos2dx
//
//  Created by weesin on 14-10-10.
//  Copyright (c) 2014年 qeeplay.com. All rights reserved.
//

#include "WebViewImpl_IOS.h"
#include "CCWebView.h"
#include "UIWebViewWrapper.h"
#import "EAGLView.h"

NS_CC_EXTRA_BEGIN

WebViewImpl::WebViewImpl(CCWebView *webView)
: m_pUIWebViewWrapper([UIWebViewWrapper webViewWrapper]), m_pWebView(webView) {
    [m_pUIWebViewWrapper retain];
    m_pUIWebViewWrapper.shouldStartLoading = [this](std::string url) {
        if (this->m_pWebView->shouldStartLoading) {
            return this->m_pWebView->shouldStartLoading(this->m_pWebView, url);
        }
        return true;
    };
    m_pUIWebViewWrapper.didFinishLoading = [this](std::string url) {
        if (this->m_pWebView->didFinishLoading) {
            this->m_pWebView->didFinishLoading(this->m_pWebView, url);
        }
    };
    m_pUIWebViewWrapper.didFailLoading = [this](std::string url) {
        if (this->m_pWebView->didFailLoading) {
            this->m_pWebView->didFailLoading(this->m_pWebView, url);
        }
    };
    m_pUIWebViewWrapper.onJsCallback = [this](std::string url) {
        if (this->m_pWebView->onJsCallback) {
            this->m_pWebView->onJsCallback(this->m_pWebView, url);
        }
    };
}

WebViewImpl::~WebViewImpl() {
    [m_pUIWebViewWrapper release];
    m_pUIWebViewWrapper = nullptr;
}

void WebViewImpl::setJavascriptInterfaceScheme(const std::string &scheme) {
    [m_pUIWebViewWrapper setJavascriptInterfaceScheme:scheme];
}

void WebViewImpl::loadData(const std::string &data, const std::string &MIMEType, const std::string &encoding, const std::string &baseURL) {
    [m_pUIWebViewWrapper loadData:data MIMEType:MIMEType textEncodingName:encoding baseURL:baseURL];
}

void WebViewImpl::loadHTMLString(const std::string &string, const std::string &baseURL) {
    [m_pUIWebViewWrapper loadHTMLString:string baseURL:baseURL];
}

void WebViewImpl::loadUrl(const std::string &url) {
    [m_pUIWebViewWrapper loadUrl:url];
}

void WebViewImpl::loadFile(const std::string &fileName) {
    auto fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName.c_str());
    [m_pUIWebViewWrapper loadFile:fullPath];
}

void WebViewImpl::stopLoading() {
    [m_pUIWebViewWrapper stopLoading];
}

void WebViewImpl::reload() {
    [m_pUIWebViewWrapper reload];
}

bool WebViewImpl::canGoBack() {
    return m_pUIWebViewWrapper.canGoBack;
}

bool WebViewImpl::canGoForward() {
    return m_pUIWebViewWrapper.canGoForward;
}

void WebViewImpl::goBack() {
    [m_pUIWebViewWrapper goBack];
}

void WebViewImpl::goForward() {
    [m_pUIWebViewWrapper goForward];
}

void WebViewImpl::evaluateJS(const std::string &js) {
    [m_pUIWebViewWrapper evaluateJS:js];
}

void WebViewImpl::setScalesPageToFit(const bool scalesPageToFit) {
    [m_pUIWebViewWrapper setScalesPageToFit:scalesPageToFit];
}

void WebViewImpl::draw(void) {
    auto direcrot = cocos2d::CCDirector::sharedDirector();
    auto glView = direcrot->getOpenGLView();
    auto frameSize = glView->getFrameSize();
    auto scaleFactor = [[EAGLView sharedEGLView] contentScaleFactor];
    auto winSize = direcrot->getWinSize();
    auto leftBottom = this->m_pWebView->convertToWorldSpace(cocos2d::CCSize(0, 0));
    auto rightTop = this->m_pWebView->convertToWorldSpace(this->m_pWebView->getContentSize());
    auto x = (frameSize.width / 2 + (leftBottom.x - winSize.width / 2) * glView->getScaleX()) / scaleFactor;
    auto y = (frameSize.height / 2 - (rightTop.y - winSize.height / 2) * glView->getScaleY()) / scaleFactor;
    auto width = (rightTop.x - leftBottom.x) * glView->getScaleX() / scaleFactor;
    auto height = (rightTop.y - leftBottom.y) * glView->getScaleY() / scaleFactor;
    [m_pUIWebViewWrapper setFrameWithX:x
                                     y:y
                                 width:width
                                height:height];
}

void WebViewImpl::setVisible(bool visible) {
    [m_pUIWebViewWrapper setVisible:visible];
}


NS_CC_EXTRA_END