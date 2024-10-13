//
//  StopStoplightLight.m
//  StopStoplightLight
//
//  Created by Brian "Shishkabibal" on 6/25/24.
//  Copyright (c) 2024 Brian "Shishkabibal". All rights reserved.
//

#pragma mark - Library/Header Imports

@import AppKit;
@import QuartzCore;
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
    
    // Ensure the contentView is layer-backed
    window.contentView.wantsLayer = YES;

    // Set flags for border
    CGFloat borderWidth = 2.0;
    CGColorRef borderColor = [NSColor whiteColor].CGColor;
    CGFloat cornerRadius = 40.0;

    CALayer *contentLayer = window.contentView.layer;
    if (contentLayer) {
        // Create a mask layer
        CAShapeLayer *maskLayer = [CAShapeLayer layer];
        
        // Create a path that subtracts the corner radius
        CGMutablePathRef path = CGPathCreateMutable();
        CGRect bounds = window.contentView.bounds;
        
        // Start at top-left corner
        CGPathMoveToPoint(path, NULL, cornerRadius, 0);
        
        // Top edge
        CGPathAddLineToPoint(path, NULL, bounds.size.width - cornerRadius, 0);
        
        // Top-right corner
        CGPathAddArcToPoint(path, NULL, bounds.size.width, 0, bounds.size.width, cornerRadius, cornerRadius);
        
        // Right edge
        CGPathAddLineToPoint(path, NULL, bounds.size.width, bounds.size.height - cornerRadius);
        
        // Bottom-right corner
        CGPathAddArcToPoint(path, NULL, bounds.size.width, bounds.size.height, bounds.size.width - cornerRadius, bounds.size.height, cornerRadius);
        
        // Bottom edge
        CGPathAddLineToPoint(path, NULL, cornerRadius, bounds.size.height);
        
        // Bottom-left corner
        CGPathAddArcToPoint(path, NULL, 0, bounds.size.height, 0, bounds.size.height - cornerRadius, cornerRadius);
        
        // Left edge
        CGPathAddLineToPoint(path, NULL, 0, cornerRadius);
        
        // Top-left corner
        CGPathAddArcToPoint(path, NULL, 0, 0, cornerRadius, 0, cornerRadius);
        
        CGPathCloseSubpath(path);
        
        maskLayer.path = path;
        
        // Apply the mask to the window's content view layer
        contentLayer.mask = maskLayer;
        
        // Create a border layer
        CAShapeLayer *borderLayer = [CAShapeLayer layer];
        borderLayer.path = path;
        borderLayer.fillColor = [NSColor clearColor].CGColor;
        borderLayer.strokeColor = borderColor;
        borderLayer.lineWidth = borderWidth;
        borderLayer.frame = bounds;
        
        // Add the border layer above the content layer
        [contentLayer addSublayer:borderLayer];
        
        // Create an outline layer
        CAShapeLayer *outlineLayer = [CAShapeLayer layer];
        outlineLayer.path = path;
        outlineLayer.fillColor = [NSColor clearColor].CGColor;
        outlineLayer.strokeColor = borderColor;
        outlineLayer.lineWidth = borderWidth;
        outlineLayer.frame = bounds;
        
        // Add the outline layer as the top-most layer
        [window.contentView.layer addSublayer:outlineLayer];
        
        // Store the outline layer for later updates
        objc_setAssociatedObject(self, "outlineLayer", outlineLayer, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
        
        CGPathRelease(path);
        
        // Modify window properties
        window.opaque = NO;
        window.backgroundColor = [NSColor clearColor];
        window.hasShadow = YES;
        
        // Set window to have a full-size content view
        window.styleMask |= NSWindowStyleMaskFullSizeContentView;
        
        // Remove title bar
        window.titlebarAppearsTransparent = YES;
        window.titleVisibility = NSWindowTitleHidden;
        
        // Make sure the window can still be moved
        window.movableByWindowBackground = YES;
        
        // Set up notifications for active/inactive state and resizing
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowDidResignKey:) name:NSWindowDidResignKeyNotification object:window];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowDidBecomeKey:) name:NSWindowDidBecomeKeyNotification object:window];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowDidResize:) name:NSWindowDidResizeNotification object:window];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowWillStartLiveResize:) name:NSWindowWillStartLiveResizeNotification object:window];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowDidEndLiveResize:) name:NSWindowDidEndLiveResizeNotification object:window];
        
        // Initial update of border color
        [self updateBorderColorForWindow:window];
    } else {
        DLog("Failed to add window borders: contentView.layer is nil");
    }
}

- (void)updateBorderColorForWindow:(NSWindow *)window {
    if (window.isKeyWindow) {
        window.contentView.layer.borderColor = [NSColor whiteColor].CGColor;
    } else {
        window.contentView.layer.borderColor = [NSColor darkGrayColor].CGColor;
    }
}

- (void)windowDidResignKey:(NSNotification *)notification {
    NSWindow *window = (NSWindow *)self;
    window.contentView.layer.borderColor = [NSColor darkGrayColor].CGColor;
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
    NSWindow *window = (NSWindow *)self;
    window.contentView.layer.borderColor = [NSColor whiteColor].CGColor;
}

- (void)windowWillStartLiveResize:(NSNotification *)notification {
    NSWindow *window = (NSWindow *)self;
    [CATransaction begin];
    [CATransaction setDisableActions:YES];
    [self updateMaskForWindow:window];
    [CATransaction commit];
}

- (void)windowDidResize:(NSNotification *)notification {
    NSWindow *window = (NSWindow *)self;
    [CATransaction begin];
    [CATransaction setDisableActions:YES];
    [self updateMaskForWindow:window];
    [self updateOutlineLayerForWindow:window];
    [CATransaction commit];
}

- (void)windowDidEndLiveResize:(NSNotification *)notification {
    NSWindow *window = (NSWindow *)self;
    [self updateBorderColorForWindow:window];
    [self modifyTitlebarAppearance];
    [window.contentView setNeedsDisplay:YES];
    [window display];
}

- (void)updateMaskForWindow:(NSWindow *)window {
    CALayer *contentLayer = window.contentView.layer;
    CGFloat cornerRadius = 40.0;
    
    CAShapeLayer *maskLayer = (CAShapeLayer *)contentLayer.mask;
    if (!maskLayer) {
        maskLayer = [CAShapeLayer layer];
        contentLayer.mask = maskLayer;
    }
    
    CGRect bounds = window.contentView.bounds;
    CGMutablePathRef path = CGPathCreateMutable();
    
    CGPathMoveToPoint(path, NULL, cornerRadius, 0);
    CGPathAddArcToPoint(path, NULL, bounds.size.width, 0, bounds.size.width, cornerRadius, cornerRadius);
    CGPathAddArcToPoint(path, NULL, bounds.size.width, bounds.size.height, bounds.size.width - cornerRadius, bounds.size.height, cornerRadius);
    CGPathAddArcToPoint(path, NULL, 0, bounds.size.height, 0, bounds.size.height - cornerRadius, cornerRadius);
    CGPathAddArcToPoint(path, NULL, 0, 0, cornerRadius, 0, cornerRadius);
    CGPathCloseSubpath(path);
    
    maskLayer.path = path;
    CGPathRelease(path);
}

- (void)updateOutlineLayerForWindow:(NSWindow *)window {
    CALayer *contentLayer = window.contentView.layer;
    CGFloat cornerRadius = 40.0;
    
    CAShapeLayer *outlineLayer = objc_getAssociatedObject(self, "outlineLayer");
    if (!outlineLayer) {
        outlineLayer = [CAShapeLayer layer];
        objc_setAssociatedObject(self, "outlineLayer", outlineLayer, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    }
    
    // Remove the existing outline layer if it exists
    [outlineLayer removeFromSuperlayer];
    
    CGRect bounds = window.contentView.bounds;
    CGMutablePathRef path = CGPathCreateMutable();
    
    CGPathMoveToPoint(path, NULL, cornerRadius, 0);
    CGPathAddArcToPoint(path, NULL, bounds.size.width, 0, bounds.size.width, cornerRadius, cornerRadius);
    CGPathAddArcToPoint(path, NULL, bounds.size.width, bounds.size.height, bounds.size.width - cornerRadius, bounds.size.height, cornerRadius);
    CGPathAddArcToPoint(path, NULL, 0, bounds.size.height, 0, bounds.size.height - cornerRadius, cornerRadius);
    CGPathAddArcToPoint(path, NULL, 0, 0, cornerRadius, 0, cornerRadius);
    CGPathCloseSubpath(path);
    
    outlineLayer.path = path;
    outlineLayer.fillColor = [NSColor clearColor].CGColor;
    outlineLayer.strokeColor = [NSColor whiteColor].CGColor;
    outlineLayer.lineWidth = 2.0;
    outlineLayer.frame = bounds;
    
    [contentLayer addSublayer:outlineLayer];
    [contentLayer setNeedsDisplay];
    CGPathRelease(path);
}

- (void)setFrame:(NSRect)frameRect display:(BOOL)flag {
    ZKOrig(void, frameRect, flag);
    [CATransaction begin];
    [CATransaction setDisableActions:YES];
    [self updateMaskForWindow:(NSWindow *)self];
    [self updateOutlineLayerForWindow:(NSWindow *)self];
    [CATransaction commit];
    [self updateBorderColorForWindow:(NSWindow *)self];
    [self modifyTitlebarAppearance];
    [(NSWindow *)self display];
}

@end
