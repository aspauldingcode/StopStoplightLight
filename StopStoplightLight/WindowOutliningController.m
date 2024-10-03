//
//  WindowOutliningController.m
//  StopStoplightLight
//
//  Created by Alex Spaulding on 10/2/24.
//  Copyright © 2024 Shishkabibal. All rights reserved.
//

#import "WindowOutliningController.h"
#import "NSWindow+StopStoplightLight.h"

@interface WindowOutliningController ()
// Properties for different color options
@property (strong) NSColor *NSColorWhite;
@property (strong) NSColor *NSColorPurple;
@end

@implementation WindowOutliningController {
    // Variables to save the original border properties
    CGFloat _savedCornerRadius;
    CGFloat _savedBorderWidth;
    CGColorRef _savedBorderColor;
}

/**
 * Returns the theme frame view to be modified.
 *
 * @return The theme frame view.
 */
- (NSView *)themeFrameToModify {
    return [[[NSWindow topWindow] contentView] superview];
}

/**
 * Returns the layer of the theme frame view.
 *
 * @return The layer of the theme frame view.
 */
- (CALayer *)themeFrameLayer {
    NSView *themeFrame = [self themeFrameToModify];
    themeFrame.wantsLayer = YES;
    return themeFrame.layer;
}

/**
 * Adds a border with the specified color to the theme frame layer.
 *
 * @param color The color of the border.
 */
- (void)addBorderWithColor:(CGColorRef)color  {
    CALayer *layer = [self themeFrameLayer];
    CGFloat cornerRadius = layer.cornerRadius;
    
    if (@available(macOS 11, *)) {
        if (cornerRadius < 10) {
            layer.cornerRadius = 12;
        }
    };
    
    _savedCornerRadius = cornerRadius;
    _savedBorderWidth = layer.borderWidth;
    if (layer.borderColor) {
        _savedBorderColor = (CGColorRef)CFRetain(layer.borderColor);
    }
    
    layer.borderWidth = 1.5;
    layer.borderColor = color;
}

/**
 * Restores the original border properties of the theme frame layer.
 */
- (void)restoreBorder {
    CALayer *layer = [self themeFrameLayer];
    layer.cornerRadius = _savedCornerRadius;
    layer.borderWidth = _savedBorderWidth;
    if (_savedBorderColor) {
        layer.borderColor = _savedBorderColor;
        CFRelease(_savedBorderColor);
    }
}

/**
 * Updates the border color based on the window's active state.
 */
- (void)updateBorderColor {
    NSWindow *window = [NSWindow topWindow];
    NSLog(@"Updating border color for window: %@", window);
    if (window.isKeyWindow) {
        NSLog(@"Window is key, setting white border");
        [self addBorderWithColor:self.NSColorWhite.CGColor];
    } else {
        NSLog(@"Window is not key, setting purple border");
        [self addBorderWithColor:self.NSColorPurple.CGColor];
    }
}

/**
 * Initializes a new instance of the WindowOutliningController class.
 *
 * @return An initialized instance of the WindowOutliningController class.
 */
- (instancetype)init {
    self = [super init];
    if(self) {
        _savedCornerRadius = 0;
        _savedBorderWidth = 0;
        _savedBorderColor = nil;
        
        // Initialize color properties
        self.NSColorWhite = [NSColor whiteColor];
        self.NSColorPurple = [NSColor systemPurpleColor];
        
        // Add observers for window activation and deactivation
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(updateBorderColor)
                                                     name:NSWindowDidBecomeKeyNotification
                                                   object:nil];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(updateBorderColor)
                                                     name:NSWindowDidResignKeyNotification
                                                   object:nil];
        
        // Initial border color update
        [self updateBorderColor];
    }
    
    return self;
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)someMethod {
    NSWindow *topWindow = [NSWindow topWindow];
    [topWindow setCGWindowLevel:kCGMaximumWindowLevelKey];
}

@end
