//
//  CCWebView.h
//  quickcocos2dx
//
//  Created by weesin on 14-10-10.
//  Copyright (c) 2014年 qeeplay.com. All rights reserved.
//

#ifndef quickcocos2dx_CCWebView_h
#define quickcocos2dx_CCWebView_h

#include "cocos2dx_extra.h"

NS_CC_EXTRA_BEGIN

class WebViewImpl;

class CCWebView : public CCSprite
{
public:
    /**
     * Allocates and initializes a WebView.
     */
    static CCWebView *create();
    
    /**
     * Default constructor
     */
    CCWebView();
    
    /**
     * Default destructor
     */
    virtual ~CCWebView();
    
    /**
     * Call before a web view begins loading.
     * @param sender The web view that is about to load new content.
     * @param url content URL.
     * @return YES if the web view should begin loading content; otherwise, NO .
     */
    std::function<bool(CCWebView *sender, std::string url)> shouldStartLoading;
    
    /**
     * Call after a web view finishes loading.
     * @param sender The web view that has finished loading.
     * @param url content URL.
     */
    std::function<void(CCWebView *sender, std::string url)> didFinishLoading;
    
    /**
     * Call if a web view failed to load content.
     * @param sender The web view that has failed loading.
     * @param url content URL.
     */
    std::function<void(CCWebView *sender, std::string url)> didFailLoading;
    
    /**
     * Set javascript interface scheme.
     * @see #onJsCallback
     */
    void setJavascriptInterfaceScheme(const std::string &scheme);
    
    /**
     * This callback called when load URL that start with javascript interface scheme.
     */
    std::function<void(CCWebView *sender, std::string message)> onJsCallback;
    /**
     * Sets the main page contents, MIME type, content encoding, and base URL.
     * @param data The content for the main page.
     * @param MIMEType The MIME type of the data.
     * @param encoding the encoding of the data.
     * @param baseURL The base URL for the content.
     */
    void loadData(const std::string &data, const std::string &MIMEType, const std::string &encoding, const std::string &baseURL);
    
    /**
     * Sets the main page content and base URL.
     * @param string The content for the main page.
     * @param baseURL The base URL for the content.
     */
    void loadHTMLString(const std::string &string, const std::string &baseURL);
    
    /**
     * Loads the given URL.
     * @param url content URL
     */
    void loadUrl(const std::string &url);
    
    /**
     * Loads the given fileName.
     * @param fileName content fileName
     */
    void loadFile(const std::string &fileName);
    
    /**
     * Stops the current load.
     */
    void stopLoading();
    
    /**
     * Reloads the current URL.
     */
    void reload();
    
    /**
     * Gets whether this WebView has a back history item.
     * @return web view has a back history item.
     */
    bool canGoBack();
    
    /**
     * Gets whether this WebView has a forward history item.
     * @return web view has a forward history item.
     */
    bool canGoForward();
    
    /**
     * Goes back in the history.
     */
    void goBack();
    
    /**
     * Goes forward in the history.
     */
    void goForward();
    
    /**
     * evaluates JavaScript in the context of the currently displayed page
     */
    void evaluateJS(const std::string &js);
    
    /**
     * Set WebView should support zooming. The default value is false.
     */
    void setScalesPageToFit(const bool scalesPageToFit);
    
    virtual void draw(void);
    
    virtual void setVisible(bool visible);
private:
    WebViewImpl* m_pWebViewImpl;
};

NS_CC_EXTRA_END


#endif
