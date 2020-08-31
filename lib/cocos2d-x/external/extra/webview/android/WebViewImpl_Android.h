//
//  WebViewImpl_Android.h
//  quickcocos2dx
//
//  Created by weesin on 14-10-10.
//  Copyright (c) 2014年 qeeplay.com. All rights reserved.
//

#ifndef __quickcocos2dx__WebViewImpl_Android__
#define __quickcocos2dx__WebViewImpl_Android__

#include "cocos2dx_extra.h"

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
    
    
	static bool shouldStartLoading(const int viewTag, const std::string &url);
	static void didFinishLoading(const int viewTag, const std::string &url);
	static void didFailLoading(const int viewTag, const std::string &url);
	static void onJsCallback(const int viewTag, const std::string &message);
    
private:
    int m_viewTag;
    CCWebView* m_pWebView;
};

NS_CC_EXTRA_END


#endif /* defined(__quickcocos2dx__WebViewImpl_Android__) */
