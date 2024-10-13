//
//  StopStoplightLight.m
//  StopStoplightLight
//
//  Created by Brian "Shishkabibal" on 6/25/24.
//  Copyright (c) 2024 Brian "Shishkabibal". All rights reserved.
//

#pragma mark - Library/Header Imports

@import AppKit;
#import <objc/runtime.h>
#import "ZKSwizzle.h"
#import "NSWindow+StopStoplightLight.h"

#include <os/log.h>
#define DLog(N, ...) os_log_with_type(os_log_create("com.shishkabibal.StopStoplightLight", "DEBUG"), OS_LOG_TYPE_DEFAULT, N, ##__VA_ARGS__)

#pragma mark - Global Variables

static NSString *const preferencesSuiteName = @"com.shishkabibal.StopStoplightLight";

// Feature flags
static BOOL enableTrafficLightsDisabler = YES;
static BOOL enableTitlebarDisabler = YES;
static BOOL enableResizability = YES;
static BOOL enableWindowBorders = YES;

#pragma mark - Main Implementation

@interface StopStoplightLight ()

@end

@implementation StopStoplightLight

+ (instancetype)sharedInstance {
    static StopStoplightLight *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

+ (void)load {
    [self sharedInstance];
}

@end

#pragma mark - NSWindow Swizzling

ZKSwizzleInterface(BS_NSWindow, NSWindow, NSWindow)

@implementation BS_NSWindow

- (nullable NSButton *)standardWindowButton:(NSWindowButton)b {
    return ZKOrig(NSButton*, b);
}

- (void)makeKeyAndOrderFront:(id)sender {
    ZKOrig(void, sender);
    
    if (enableTitlebarDisabler) {
        [self modifyTitlebarAppearance];
    }
    
    if (enableTrafficLightsDisabler) {
        [self hideTrafficLights];
    }
    
    if (enableResizability) {
        [self makeResizableToAnySize];
    }
    
    if (enableWindowBorders) {
        [self addWindowBorders];
    }
}

- (void)orderOut:(id)sender {
    ZKOrig(void, sender);
}

- (void)becomeKeyWindow {
    ZKOrig(void);
}

- (void)resignKeyWindow {
    ZKOrig(void);
}

- (void)hideTrafficLights {
    [self hideButton:[self standardWindowButton:NSWindowCloseButton]];
    [self hideButton:[self standardWindowButton:NSWindowMiniaturizeButton]];
    [self hideButton:[self standardWindowButton:NSWindowZoomButton]];
}

- (void)hideButton:(NSButton *)button {
    button.hidden = YES;
}

- (void)modifyTitlebarAppearance {
    NSWindow *window = (NSWindow *)self;
    window.titlebarAppearsTransparent = YES;
    window.titleVisibility = NSWindowTitleHidden;
    window.styleMask |= NSWindowStyleMaskFullSizeContentView;
    window.contentView.wantsLayer = YES; // Ensure contentView is layer-backed
}

- (void)makeResizableToAnySize {
    NSWindow *window = (NSWindow *)self;
    window.styleMask |= NSWindowStyleMaskResizable;
    [window setMinSize:NSMakeSize(0.0, 0.0)];
    [window setMaxSize:NSMakeSize(CGFLOAT_MAX, CGFLOAT_MAX)];
}

- (void)addWindowBorders {
    NSWindow *window = (NSWindow *)self;
    if (!window.contentView.layer) {
        window.contentView.wantsLayer = YES;
    }
    window.contentView.layer.borderWidth = 2.0;
    window.contentView.layer.borderColor = [NSColor whiteColor].CGColor;
    window.contentView.layer.cornerRadius = 5.0;
    window.contentView.layer.masksToBounds = YES;
    
    // Set inactive borders to dark gray
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowDidResignKey:) name:NSWindowDidResignKeyNotification object:window];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowDidBecomeKey:) name:NSWindowDidBecomeKeyNotification object:window];
}

- (void)windowDidResignKey:(NSNotification *)notification {
    NSWindow *window = (NSWindow *)self;
    window.contentView.layer.borderColor = [NSColor darkGrayColor].CGColor;
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
    NSWindow *window = (NSWindow *)self;
    window.contentView.layer.borderColor = [NSColor whiteColor].CGColor;
}

@end
