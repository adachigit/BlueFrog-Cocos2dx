//
//  WebViewImpl_IOS.h
//  quickcocos2dx
//
//  Created by weesin on 14-10-10.
//  Copyright (c) 2014年 qeeplay.com. All rights reserved.
//

#ifndef quickcocos2dx_WebViewImpl_IOS_h
#define quickcocos2dx_WebViewImpl_IOS_h

#include "cocos2dx_extra.h"
#include <iosfwd>

@class UIWebViewWrapper;

NS_CC_EXTRA_BEGIN

class CCWebView;

class WebViewImpl
{
public:
    WebViewImpl(CCWebView* WebView);
    
    virtual ~WebViewImpl();
    
    void setJavascriptInterfaceScheme(const std::string &scheme);
    void loadData(const std::string &data, const std::string &MIMEType, const std::string &encoding, const std::string &baseURL);
    void loadHTMLString(const std::string &string, const std::string &baseURL);
    void loadUrl(const std::string &url);
    void loadFile(const std::string &fileName);
    void stopLoading();
    void reload();
    bool canGoBack();
    bool canGoForward();
    void goBack();
    void goForward();
    void evaluateJS(const std::string &js);
    void setScalesPageToFit(const bool scalesPageToFit);
    virtual void draw(void);
    virtual void setVisible(bool visible);

private:
    CCWebView* m_pWebView;
    UIWebViewWrapper * m_pUIWebViewWrapper;
};

NS_CC_EXTRA_END


#endif
