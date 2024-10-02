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

#include <os/log.h>
#define DLog(N, ...) os_log_with_type(os_log_create("com.shishkabibal.StopStoplightLight", "DEBUG"),OS_LOG_TYPE_DEFAULT,N ,##__VA_ARGS__)


#pragma mark - Global Variables

NSBundle *bundle;
NSStatusItem *statusItem;

static NSString *const preferencesSuiteName = @"com.shishkabibal.StopStoplightLight";


#pragma mark - Main Interface

@interface StopStoplightLight : NSObject
+ (instancetype)sharedInstance;
@end

StopStoplightLight* plugin;


#pragma mark - Main Implementation

@implementation StopStoplightLight

+ (StopStoplightLight*)sharedInstance {
    static StopStoplightLight* plugin = nil;
    
    if (!plugin)
        plugin = [[StopStoplightLight alloc] init];
    
    return plugin;
}

// Called on MacForge plugin initialization
+ (void)load {
    // Create plugin singleton + bundle & statusItem
    plugin = [StopStoplightLight sharedInstance];
        
//    // Log loading
//    NSUInteger major = [[NSProcessInfo processInfo] operatingSystemVersion].majorVersion;
//    NSUInteger minor = [[NSProcessInfo processInfo] operatingSystemVersion].minorVersion;
//    DLog("%{public}@: Loaded (%{public}@ - macOS %ld.%ld)", [self className], [[NSBundle mainBundle] bundleIdentifier], (long)major, (long)minor);
}

@end


#pragma mark - *** Handling

ZKSwizzleInterface(BS_NSWindow, NSWindow, NSResponder)

@implementation BS_NSWindow

// "Returns the window button of a given window button kind in the window's view hierarchy."
- (nullable NSButton *)standardWindowButton:(NSWindowButton)b {
    // Call original method
    return ZKOrig(NSButton*, b);
}

// "Moves the window to the front of the screen list, within its level, and makes it the key window; that is, it shows the window."
- (void)makeKeyAndOrderFront:(id)sender {
    // Call original method
    ZKOrig(void, sender);
    
    // Hide traffic lights
    [self hideTrafficLights];
    
    // Modify titlebar appearance
    [self modifyTitlebarAppearance];

    // Make all windows resizable to any size
    [self makeResizableToAnySize];
}

// Hide traffic lights
- (void)hideTrafficLights {
    NSButton* close = [self standardWindowButton:NSWindowCloseButton];
    NSButton* miniaturize = [self standardWindowButton:NSWindowMiniaturizeButton];
    NSButton* zoom = [self standardWindowButton:NSWindowZoomButton];

    [self hideButton:close];
    [self hideButton:miniaturize];
    [self hideButton:zoom];
}

// Hide traffic light
- (void)hideButton:(NSButton*)button {
    if (button) {
        button.hidden = YES;
    }
}

// Modify titlebar appearance
- (void)modifyTitlebarAppearance {
    NSWindow *window = (NSWindow *)self;
    
    // Make titlebar transparent
    window.titlebarAppearsTransparent = YES;
    
    // Hide title
    window.titleVisibility = NSWindowTitleHidden;
    
    // Allow clicking and dragging on the entire window
    [window setMovableByWindowBackground:YES];
    
    // Extend content into the titlebar area
    window.styleMask |= NSWindowStyleMaskFullSizeContentView;
}

// Make all windows resizable to any size
- (void)makeResizableToAnySize {
    NSWindow *window = (NSWindow *)self;

    // Allow resizing to any size
    window.styleMask |= NSWindowStyleMaskResizable;

    // Adjust size constraints set on the window's content view
    // [self adjustContentViewConstraints];

    // Adjust size constraints set on any subviews
    // [self adjustSubviewConstraints:window.contentView];

    // Remove minimum and maximum size limitations
    window.minSize = NSMakeSize(0.0, 0.0); // Set minimum window size
    window.maxSize = NSMakeSize(CGFLOAT_MAX, CGFLOAT_MAX); // Set maximum window size
}

// Adjust constraints from the content view
- (void)adjustContentViewConstraints {
    NSView *contentView = [(NSWindow *)self contentView];
    [contentView setTranslatesAutoresizingMaskIntoConstraints:NO];
    for (NSLayoutConstraint *constraint in [contentView constraints]) {
        if (constraint.firstAttribute == NSLayoutAttributeWidth || constraint.firstAttribute == NSLayoutAttributeHeight) {
            [contentView removeConstraint:constraint];
        }
    }
    // Add constraints to ensure content is rendered properly
    [contentView setNeedsDisplay:YES];
    
    // Reapply constraints when the window becomes active
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(windowDidBecomeKey:)
                                                 name:NSWindowDidBecomeKeyNotification
                                               object:self];
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
    [self adjustContentViewConstraints];
}

// Recursively adjust constraints from all subviews
- (void)adjustSubviewConstraints:(NSView *)view {
    for (NSView *subview in [view subviews]) {
        [subview setTranslatesAutoresizingMaskIntoConstraints:NO];
        for (NSLayoutConstraint *constraint in [subview constraints]) {
            if (constraint.firstAttribute == NSLayoutAttributeWidth || constraint.firstAttribute == NSLayoutAttributeHeight) {
                [subview removeConstraint:constraint];
            }
        }
        // Add constraints to ensure subview content is rendered properly
        [subview setNeedsDisplay:YES];
        [self adjustSubviewConstraints:subview]; // Recursive call
    }
}

@end