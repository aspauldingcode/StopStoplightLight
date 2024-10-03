//
//  NSWindow+StopStoplightLight.m
//  StopStoplightLight
//
//  Created by Alex Spaulding on 10/2/24.
//  Copyright © 2024 Shishkabibal. All rights reserved.
//

#import "NSWindow+StopStoplightLight.h"
#import <objc/runtime.h>
#import <objc/message.h>

@implementation NSWindow (StopStoplightLight)

+ (NSWindow *)topWindow {
    NSWindow *window = [NSApp mainWindow];
    if (!window) {
        // Use objc_msgSend to call the private method 'frontWindow'
        window = ((NSWindow* (*)(id, SEL))objc_msgSend)(NSApp, sel_getUid("frontWindow"));
    }
    return window;
}

- (void)setCGWindowLevel:(CGWindowLevel)level {
    if ([self attachedSheet]) {
        [self.attachedSheet setCGWindowLevel:level];
    }
    
    for (NSWindow *childWindow in [self childWindows]) {
        [childWindow setCGWindowLevel:level];
    }
    
    CGSSetWindowLevel(CGSMainConnectionID(), (unsigned int)[self windowNumber], level);
}

@end

