//
//  UIWebViewWrapper.h
//  quickcocos2dx
//
//  Created by weesin on 14-10-10.
//  Copyright (c) 2014年 qeeplay.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <iosfwd>
#include <functional>
#include <string>

@interface UIWebViewWrapper : NSObject
@property (nonatomic) std::function<bool(std::string url)> shouldStartLoading;
@property (nonatomic) std::function<void(std::string url)> didFinishLoading;
@property (nonatomic) std::function<void(std::string url)> didFailLoading;
@property (nonatomic) std::function<void(std::string url)> onJsCallback;
@property(nonatomic, readonly, getter=canGoBack) BOOL canGoBack;
@property(nonatomic, readonly, getter=canGoForward) BOOL canGoForward;

+ (instancetype)webViewWrapper;
- (void)setVisible:(bool)visible;
- (void)setFrameWithX:(float)x y:(float)y width:(float)width height:(float)height;
- (void)loadData:(const std::string &)data MIMEType:(const std::string &)MIMEType textEncodingName:(const std::string &)encodingName baseURL:(const std::string &)baseURL;
- (void)setJavascriptInterfaceScheme:(const std::string &)scheme;
- (void)loadHTMLString:(const std::string &)string baseURL:(const std::string &)baseURL;
- (void)loadUrl:(const std::string &)urlString;
- (void)loadFile:(const std::string &)filePath;
- (void)stopLoading;
- (void)reload;
- (void)evaluateJS:(const std::string &)js;
- (void)goBack;
- (void)goForward;
- (void)setScalesPageToFit:(const bool)scalesPageToFit;
@end
