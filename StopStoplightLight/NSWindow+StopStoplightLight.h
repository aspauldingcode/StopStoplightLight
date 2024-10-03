//
//  NSWindow+StopStoplightLight.h
//  StopStoplightLight
//
//  Created by Alex Spaulding on 10/2/24.
//  Copyright © 2024 Shishkabibal. All rights reserved.
//

#import <Cocoa/Cocoa.h>

typedef int CGSConnectionID;
CG_EXTERN CGSConnectionID CGSMainConnectionID(void);
CG_EXTERN CGError CGSGetWindowLevel(CGSConnectionID cid, CGWindowID wid, CGWindowLevel *outLevel);
CG_EXTERN CGError CGSSetWindowLevel(CGSConnectionID cid, CGWindowID wid, CGWindowLevel level);

@interface NSWindow (StopStoplightLight)

+ (NSWindow *)topWindow;
- (void)setCGWindowLevel:(CGWindowLevel)level;

@end

