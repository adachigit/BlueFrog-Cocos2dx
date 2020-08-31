//
//  WebView.cpp
//  quickcocos2dx
//
//  Created by weesin on 14-10-10.
//  Copyright (c) 2014年 qeeplay.com. All rights reserved.
//

#include "cocos2dx_extra.h"

NS_CC_EXTRA_BEGIN

class CCWebView;

class WebViewImpl
{
public:
    WebViewImpl(CCWebView* WebView) {};
    
    virtual ~WebViewImpl() {};
    
    void setJavascriptInterfaceScheme(const std::string &scheme) {};
    void loadData(const std::string &data, const std::string &MIMEType, const std::string &encoding, const std::string &baseURL) {};
    void loadHTMLString(const std::string &string, const std::string &baseURL) {};
    void loadUrl(const std::string &url) {};
    void loadFile(const std::string &fileName) {};
    void stopLoading() {};
    void reload() {};
    bool canGoBack() { return false; };
    bool canGoForward() { return false; };
    void goBack() {};
    void goForward() {};
    void evaluateJS(const std::string &js) {};
    void setScalesPageToFit(const bool scalesPageToFit) {};
    virtual void draw(void) {};
    virtual void setVisible(bool visible) {};
    
};

NS_CC_EXTRA_END


#include "../CCWebView_inl.h"
