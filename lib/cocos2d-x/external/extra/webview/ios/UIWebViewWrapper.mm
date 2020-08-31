//
//  UIWebViewWrapper.m
//  quickcocos2dx
//
//  Created by weesin on 14-10-10.
//  Copyright (c) 2014年 qeeplay.com. All rights reserved.
//

#import "UIWebViewWrapper.h"
#import "CCWebView.h"
#import "CCEGLView.h"
#import "EAGLView.h"
#import "CCDirector.h"

using namespace cocos2d;

@interface UIWebViewWrapper () <UIWebViewDelegate>
@property(nonatomic, retain) UIWebView *uiWebView;
@property(nonatomic, copy) NSString *jsScheme;
@end

@implementation UIWebViewWrapper {
    CGSize fittingSize;
    float scale;
}

+ (instancetype)webViewWrapper {
    return [[[self alloc] init] autorelease];
}

- (instancetype)init {
    self = [super init];
    if (self) {
        self.uiWebView = nil;
        self.shouldStartLoading = nullptr;
        self.didFinishLoading = nullptr;
        self.didFailLoading = nullptr;
    }
    return self;
}

- (void)dealloc {
    if (self.uiWebView)
    {
        if ([self.uiWebView isLoading])
            [self.uiWebView stopLoading];
        [self.uiWebView removeFromSuperview];
        [self.uiWebView setDelegate:nil];
        [self.uiWebView release];
    }
    self.jsScheme = nil;
    [super dealloc];
}

- (void)setupWebView {
    if (!self.uiWebView) {
        self.uiWebView = [[[UIWebView alloc] init] autorelease];
        self.uiWebView.delegate = self;
//        [self.uiWebView setScalesPageToFit:YES]; //用来保证页面在屏幕中自适应，不过虚拟机iphone好像不对
        [self.uiWebView setBackgroundColor:[UIColor clearColor]];
        [self.uiWebView setOpaque:NO]; // 优化，说明该空间不透明
        for (UIView *aView in [self.uiWebView subviews])
        {
            if ([aView isKindOfClass:[UIScrollView class]])
            {
                UIScrollView* scView = (UIScrollView *)aView;
                
                // 是否显示右侧的滚动条 （水平的类似）
                // set Vertical Scroll bar visible
                [(UIScrollView *)aView setShowsVerticalScrollIndicator:NO];
                [scView setShowsHorizontalScrollIndicator:NO];
                scView.bounces = NO; // 禁止下拉拖动
                
                for (UIView *shadowView in aView.subviews)
                {
                    if ([shadowView isKindOfClass:[UIImageView class]])
                    {
                        // 隐藏上下滚动出边界时的黑色的图片 也就是拖拽后的上下阴影
                        // hide black background when webpage is out of border.
                        shadowView.hidden = YES;
                    }
                }
            }
        }
    }
    if (!self.uiWebView.superview) {
        [[EAGLView sharedEGLView] addSubview:self.uiWebView];
    }
}

- (void)setVisible:(bool)visible {
    self.uiWebView.hidden = !visible;
}

- (void)setFrameWithX:(float)x y:(float)y width:(float)width height:(float)height {
    if (!self.uiWebView) {[self setupWebView];}
    CGRect newFrame = CGRectMake(x, y, width, height);
    if (!CGRectEqualToRect(self.uiWebView.frame, newFrame)) {
        self.uiWebView.frame = CGRectMake(x, y, width, height);
    }
}

- (void)setJavascriptInterfaceScheme:(const std::string &)scheme {
    self.jsScheme = @(scheme.c_str());
}

- (void)loadData:(const std::string &)data MIMEType:(const std::string &)MIMEType textEncodingName:(const std::string &)encodingName baseURL:(const std::string &)baseURL {
    [self.uiWebView loadData:[NSData dataWithBytes:data.c_str() length:data.length()]
                    MIMEType:@(MIMEType.c_str())
            textEncodingName:@(encodingName.c_str())
                     baseURL:[NSURL URLWithString:@(baseURL.c_str())]];
}

- (void)loadHTMLString:(const std::string &)string baseURL:(const std::string &)baseURL {
    [self.uiWebView loadHTMLString:@(string.c_str()) baseURL:[NSURL URLWithString:@(baseURL.c_str())]];
}

- (void)loadUrl:(const std::string &)urlString {
    if (!self.uiWebView) {[self setupWebView];}
    NSURL *url = [NSURL URLWithString:@(urlString.c_str())];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    [self.uiWebView loadRequest:request];
}

- (void)loadFile:(const std::string &)filePath {
    if (!self.uiWebView) {[self setupWebView];}
    NSURL *url = [NSURL fileURLWithPath:@(filePath.c_str())];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    [self.uiWebView loadRequest:request];
}

- (void)stopLoading {
    [self.uiWebView stopLoading];
}

- (void)reload {
    [self.uiWebView reload];
}

- (BOOL)canGoForward {
    return self.uiWebView.canGoForward;
}

- (BOOL)canGoBack {
    return self.uiWebView.canGoBack;
}

- (void)goBack {
    [self.uiWebView goBack];
}

- (void)goForward {
    [self.uiWebView goForward];
}

- (void)evaluateJS:(const std::string &)js {
    if (!self.uiWebView) {[self setupWebView];}
    [self.uiWebView stringByEvaluatingJavaScriptFromString:@(js.c_str())];
}

- (void)setScalesPageToFit:(const bool)scalesPageToFit {
    [self.uiWebView setScalesPageToFit:scalesPageToFit];
}

#pragma mark - UIWebViewDelegate
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    NSString *url = [[request URL] absoluteString];
    if ([[[request URL] scheme] isEqualToString:self.jsScheme]) {
        self.onJsCallback([url UTF8String]);
        return NO;
    }
    if (self.shouldStartLoading) {
        return self.shouldStartLoading([url UTF8String]);
    }
    return YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
    if (self.didFinishLoading) {
        NSString *url = [[webView.request URL] absoluteString];
        self.didFinishLoading([url UTF8String]);
    }

    fittingSize = [webView sizeThatFits:CGSizeZero];
    if (fittingSize.width > 0 && self.uiWebView.scrollView.zoomScale == 1) {
        CGRect frame = webView.frame;
        float scale = frame.size.width / fittingSize.width;
        if (scale < 0.9) {
            NSLog(@"Zoom out fix for web view: %f", scale);
            
            self.uiWebView.scrollView.minimumZoomScale = scale;
            self.uiWebView.scrollView.maximumZoomScale = scale;
            self.uiWebView.scrollView.zoomScale = scale;
        }
    }
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error {
    if (self.didFailLoading) {
        NSString *url = error.userInfo[NSURLErrorFailingURLStringErrorKey];
        self.didFailLoading([url UTF8String]);
    }
}


@end
