/*
    NSWindow.h
    Application Kit
    Copyright (c) 1994-2023, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDate.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSGeometry.h>

#import <AppKit/AppKitDefines.h>
#import <AppKit/NSAnimation.h>
#import <AppKit/NSAppearance.h>
#import <AppKit/NSApplication.h>
#import <AppKit/NSGraphics.h>
#import <AppKit/NSPasteboard.h>
#import <AppKit/NSPreviewRepresentingActivityItem.h>
#import <AppKit/NSResponder.h>
#import <AppKit/NSUserInterfaceItemIdentification.h>
#import <AppKit/NSUserInterfaceValidation.h>
#import <AppKit/NSMenu.h>

#import <ApplicationServices/ApplicationServices.h>

NS_HEADER_AUDIT_BEGIN(nullability, sendability)
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class CADisplayLink, NSButton, NSButtonCell, NSColor, NSImage, NSScreen, NSNotification, NSText, NSView, NSMutableSet, NSSet, NSDate, NSToolbar, NSGraphicsContext, NSURL, NSColorSpace, NSDockTile, NSViewController, NSTitlebarAccessoryViewController, NSEvent, NSWindowController, NSWindowTab, NSWindowTabGroup;
    
@protocol NSWindowDelegate;

static const NSAppKitVersion NSAppKitVersionNumberWithCustomSheetPosition = 686.0;
static const NSAppKitVersion NSAppKitVersionNumberWithDeferredWindowDisplaySupport = 1019.0;

/*!
 * @typedef NSWindowStyleMask
 *
 * @const NSWindowStyleMaskBorderless
 * @const NSWindowStyleMaskTitled
 * @const NSWindowStyleMaskClosable
 * @const NSWindowStyleMaskMiniaturizable
 * @const NSWindowStyleMaskResizable
 * @const NSWindowStyleMaskTexturedBackground  Textured window style is deprecated and should no longer be used. Specifies a window with textured background. Textured windows generally don't draw a top border line under the titlebar/toolbar. To get that line, use the \c NSUnifiedTitleAndToolbarWindowMask mask.
 * @const NSWindowStyleMaskUnifiedTitleAndToolbar  Specifies a window whose titlebar and toolbar have a unified look - that is, a continuous background. Under the titlebar and toolbar a horizontal separator line will appear.
 * @const NSWindowStyleMaskFullScreen  When present, the window will appear full screen. This mask is automatically toggled when \c -toggleFullScreen: is called.
 * @const NSWindowStyleMaskFullSizeContentView If set, the \c contentView will consume the full size of the window; it can be combined with other window style masks, but is only respected for windows with a titlebar. Utilizing this mask opts-in to layer-backing. Utilize the \c contentLayoutRect or auto-layout \c contentLayoutGuide to layout views underneath the titlebar/toolbar area.
 * @const NSWindowStyleMaskUtilityWindow Only applicable for \c NSPanel (or a subclass thereof).
 * @const NSWindowStyleMaskDocModalWindow Only applicable for \c NSPanel (or a subclass thereof).
 * @const NSWindowStyleMaskNonactivatingPanel  Specifies that a panel that does not activate the owning application. Only applicable for \c NSPanel (or a subclass thereof).
 * @const NSWindowStyleMaskHUDWindow Specifies a heads up display panel.  Only applicable for \c NSPanel (or a subclass thereof).
 */
typedef NS_OPTIONS(NSUInteger, NSWindowStyleMask) {
    NSWindowStyleMaskBorderless = 0,
    NSWindowStyleMaskTitled = 1 << 0,
    NSWindowStyleMaskClosable = 1 << 1,
    NSWindowStyleMaskMiniaturizable = 1 << 2,
    NSWindowStyleMaskResizable	= 1 << 3,
    NSWindowStyleMaskTexturedBackground API_DEPRECATED("Textured window style should no longer be used", macos(10.2, 11.0)) = 1 << 8,
    NSWindowStyleMaskUnifiedTitleAndToolbar = 1 << 12,
    NSWindowStyleMaskFullScreen API_AVAILABLE(macos(10.7)) = 1 << 14,
    NSWindowStyleMaskFullSizeContentView API_AVAILABLE(macos(10.10)) = 1 << 15,
    NSWindowStyleMaskUtilityWindow = 1 << 4,
    NSWindowStyleMaskDocModalWindow = 1 << 6,
    NSWindowStyleMaskNonactivatingPanel = 1 << 7,
    NSWindowStyleMaskHUDWindow API_AVAILABLE(macos(10.6)) = 1 << 13
};

// Additional NSModalResponse values
static const NSModalResponse NSModalResponseOK = 1;
static const NSModalResponse NSModalResponseCancel = 0;

/// Used with \c NSRunLoop's `-performSelector:target:argument:order:modes:`.
enum {
    NSDisplayWindowRunLoopOrdering	= 600000,
    NSResetCursorRectsRunLoopOrdering	= 700000
};

/*!
 * @typedef NSWindowSharingType
 *
 * @const NSWindowSharingNone          Window contents may not be read by another process.
 * @const NSWindowSharingReadOnly   Window contents may be read but not modified by another process.
 * @const NSWindowSharingReadWrite  Window contents may be read or modified by another process.
 */
typedef NS_ENUM(NSUInteger, NSWindowSharingType) {
    NSWindowSharingNone = 0,
    NSWindowSharingReadOnly = 1,
    NSWindowSharingReadWrite = 2
} API_AVAILABLE(macos(10.5));

/*!
 * @typedef NSWindowCollectionBehavior
 *
 * @const NSWindowCollectionBehaviorPrimary Marks a window as primary. This collection behavior should commonly be used for document or viewer windows.
 * @const NSWindowCollectionBehaviorAuxiliary Marks a window as auxiliary. This collection behavior should commonly be used for About or Settings windows, as well as utility panes.
 * @const NSWindowCollectionBehaviorCanJoinAllApplications Marks a window as able to join all applications, allowing it to join other apps' sets and full screen spaces when eligible. This collection behavior should commonly be used for floating windows and system overlays.
 *
 * @discussion You may specify at most one of @c NSWindowCollectionBehaviorPrimary, @c NSWindowCollectionBehaviorAuxiliary, or @c NSWindowCollectionBehaviorCanJoinAllApplications. If unspecified, the window gets the default treatment determined by its other collection behaviors.
 *
 * @const NSWindowCollectionBehaviorDefault
 * @const NSWindowCollectionBehaviorCanJoinAllSpaces
 * @const NSWindowCollectionBehaviorMoveToActiveSpace
 *
 * @discussion You may specify at most one of \c NSWindowCollectionBehaviorManaged, \c NSWindowCollectionBehaviorTransient, or \c NSWindowCollectionBehaviorStationary.  If neither is specified, the window gets the default behavior determined by its window level.
 *
 * @const NSWindowCollectionBehaviorManaged Participates in spaces, exposé.  Default behavior if `windowLevel == NSNormalWindowLevel`.
 * @const NSWindowCollectionBehaviorTransient Floats in spaces, hidden by exposé.  Default behavior if `windowLevel != NSNormalWindowLevel`.
 * @const NSWindowCollectionBehaviorStationary Unaffected by exposé.  Stays visible and stationary, like desktop window.
 *
 * @discussion You may specify at most one of \c NSWindowCollectionBehaviorParticipatesInCycle or \c NSWindowCollectionBehaviorIgnoresCycle.  If unspecified, the window gets the default behavior determined by its window level.
 *
 * @const NSWindowCollectionBehaviorParticipatesInCycle Default behavior if `windowLevel != NSNormalWindowLevel`.
 * @const NSWindowCollectionBehaviorIgnoresCycle Default behavior if `windowLevel != NSNormalWindowLevel`.
 *
 * @discussion You may specify at most one of \c NSWindowCollectionBehaviorFullScreenPrimary, \c NSWindowCollectionBehaviorFullScreenAuxiliary, or \c NSWindowCollectionBehaviorFullScreenNone.
 *
 * @const NSWindowCollectionBehaviorFullScreenPrimary The frontmost window with this collection behavior will be the fullscreen window.
 * @const NSWindowCollectionBehaviorFullScreenAuxiliary Windows with this collection behavior can be shown with the fullscreen window.
 * @const NSWindowCollectionBehaviorFullScreenNone The window can not be made fullscreen when this bit is set.
 *
 * @discussion You may specify at most one of \c NSWindowCollectionBehaviorFullScreenAllowsTiling or \c NSWindowCollectionBehaviorFullScreenDisallowsTiling, or an assertion will be raised.
 *
 * The default behavior is to allow any window to participate in full screen tiling, as long as it meets certain requirements, such as being resizable and not a panel or sheet. Windows which are not full screen capable can still become a secondary tile in full screen. A window can explicitly allow itself to be placed into a full screen tile by including \c NSWindowCollectionBehaviorFullScreenAllowsTiling. Even if a window allows itself to be placed in a tile, it still may not be put in the tile if its \c minFullScreenContentSize is too large to fit. A window can explicitly disallow itself from being placed in a full screen tile by including \c NSWindowCollectionBehaviorFullScreenDisallowsTiling. This is useful for non-full screen capable windows to explicitly prevent themselves from being tiled. It can also be used by a full screen window to prevent any other windows from being placed in its full screen tile.
 *
 * @const NSWindowCollectionBehaviorFullScreenAllowsTiling This window can be a full screen tile window. It does not have to have \c NSWindowCollectionBehaviorFullScreenPrimary set.
 * @const NSWindowCollectionBehaviorFullScreenDisallowsTiling This window can NOT be made a full screen tile window; it still may be allowed to be a regular \c NSWindowCollectionBehaviorFullScreenPrimary window.
 */
typedef NS_OPTIONS(NSUInteger, NSWindowCollectionBehavior) {
    NSWindowCollectionBehaviorDefault = 0,
    NSWindowCollectionBehaviorCanJoinAllSpaces = 1 << 0,
    NSWindowCollectionBehaviorMoveToActiveSpace = 1 << 1,
    
    NSWindowCollectionBehaviorManaged API_AVAILABLE(macos(10.6)) = 1 << 2,
    NSWindowCollectionBehaviorTransient API_AVAILABLE(macos(10.6)) = 1 << 3,
    NSWindowCollectionBehaviorStationary API_AVAILABLE(macos(10.6)) = 1 << 4,

    NSWindowCollectionBehaviorParticipatesInCycle API_AVAILABLE(macos(10.6)) = 1 << 5,
    NSWindowCollectionBehaviorIgnoresCycle API_AVAILABLE(macos(10.6)) = 1 << 6,
    
    NSWindowCollectionBehaviorFullScreenPrimary API_AVAILABLE(macos(10.7)) = 1 << 7,
    NSWindowCollectionBehaviorFullScreenAuxiliary API_AVAILABLE(macos(10.7)) = 1 << 8,
    NSWindowCollectionBehaviorFullScreenNone API_AVAILABLE(macos(10.7)) = 1 << 9,
    
    NSWindowCollectionBehaviorFullScreenAllowsTiling API_AVAILABLE(macos(10.11)) = 1 << 11,
    NSWindowCollectionBehaviorFullScreenDisallowsTiling API_AVAILABLE(macos(10.11)) = 1 << 12,
    
    NSWindowCollectionBehaviorPrimary API_AVAILABLE(macos(13.0)) = 1 << 16,
    NSWindowCollectionBehaviorAuxiliary API_AVAILABLE(macos(13.0)) = 1 << 17,
    NSWindowCollectionBehaviorCanJoinAllApplications API_AVAILABLE(macos(13.0)) = 1 << 18,
} API_AVAILABLE(macos(10.5));

/*!
 * @typedef NSWindowAnimationBehavior
 *
 * @const NSWindowAnimationBehaviorDefault  Let AppKit infer animation behavior for this window.
 * @const NSWindowAnimationBehaviorNone     Suppress inferred animations (don't animate).
 * @const NSWindowAnimationBehaviorDocumentWindow
 * @const NSWindowAnimationBehaviorUtilityWindow
 * @const NSWindowAnimationBehaviorAlertPanel
 */
typedef NS_ENUM(NSInteger, NSWindowAnimationBehavior) {
    NSWindowAnimationBehaviorDefault = 0,
    NSWindowAnimationBehaviorNone = 2,

    NSWindowAnimationBehaviorDocumentWindow = 3,
    NSWindowAnimationBehaviorUtilityWindow = 4,
    NSWindowAnimationBehaviorAlertPanel = 5
} API_AVAILABLE(macos(10.7));

/*!
 * @typedef NSWindowNumberListOptions
 *
 * Options used in `+windowNumbersWithOptions:`.  If no options are specified, the returned list contains window numbers for visible windows on the active space belonging to the calling application.
 *
 * @const NSWindowNumberListAllApplications
 * @const NSWindowNumberListAllSpaces
 */
typedef NS_OPTIONS(NSUInteger, NSWindowNumberListOptions) {
    NSWindowNumberListAllApplications = 1 << 0,
    NSWindowNumberListAllSpaces = 1 << 4
} API_AVAILABLE(macos(10.6));

/*!
 * @typedef NSWindowOcclusionState
 *
 * @const NSWindowOcclusionStateVisible If set, at least part of the window is visible. If not set, the entire window is occluded. Windows with non-rectangular shapes may be completely occluded on screen but still count as visible, if their bounding box falls into a visible region. Windows that are completely transparent may also still count as visible.
 */
typedef NS_OPTIONS(NSUInteger, NSWindowOcclusionState) {
    NSWindowOcclusionStateVisible = 1UL << 1,
} API_AVAILABLE(macos(10.9));

typedef NSInteger NSWindowLevel NS_TYPED_EXTENSIBLE_ENUM;
static const NSWindowLevel NSNormalWindowLevel = kCGNormalWindowLevel;
static const NSWindowLevel NSFloatingWindowLevel = kCGFloatingWindowLevel;
static const NSWindowLevel NSSubmenuWindowLevel = kCGTornOffMenuWindowLevel;
static const NSWindowLevel NSTornOffMenuWindowLevel = kCGTornOffMenuWindowLevel;
static const NSWindowLevel NSMainMenuWindowLevel = kCGMainMenuWindowLevel;
static const NSWindowLevel NSStatusWindowLevel = kCGStatusWindowLevel;
static const NSWindowLevel NSModalPanelWindowLevel = kCGModalPanelWindowLevel;
static const NSWindowLevel NSPopUpMenuWindowLevel = kCGPopUpMenuWindowLevel;
static const NSWindowLevel NSScreenSaverWindowLevel = kCGScreenSaverWindowLevel;

typedef NS_ENUM(NSUInteger, NSSelectionDirection) {
    NSDirectSelection = 0,
    NSSelectingNext,
    NSSelectingPrevious
};

/*!
 * @typedef NSWindowButton
 *
 * Standard window buttons.
 */
typedef NS_ENUM(NSUInteger, NSWindowButton) {
    NSWindowCloseButton,
    NSWindowMiniaturizeButton,
    NSWindowZoomButton,
    NSWindowToolbarButton,
    NSWindowDocumentIconButton,
    NSWindowDocumentVersionsButton API_AVAILABLE(macos(10.7)) = 6,
};

/*!
 * @typedef NSWindowTitleVisibility
 *
 * @const NSWindowTitleVisible  The default mode has a normal window title and titlebar buttons.
 * @const NSWindowTitleHidden The always hidden mode hides the title and moves the toolbar up into the area previously occupied by the title.
 */
typedef NS_ENUM(NSInteger, NSWindowTitleVisibility) {
    NSWindowTitleVisible = 0,
    NSWindowTitleHidden = 1,
} API_AVAILABLE(macos(10.10));

/*!
 * @typedef NSWindowToolbarStyle
 *
 * @const NSWindowToolbarStyleAutomatic The default value. The style will be determined by the window's given configuration.
 * @const NSWindowToolbarStyleExpanded The toolbar will appear below the window title.
 * @const NSWindowToolbarStylePreference The toolbar will appear below the window title and the items in the toolbar will attempt to have equal widths when possible.
 * @const NSWindowToolbarStyleUnified The window title will appear inline with the toolbar when visible.
 * @const NSWindowToolbarStyleUnifiedCompact Same as \c NSWindowToolbarStyleUnified, but with reduced margins in the toolbar allowing more focus to be on the contents of the window.
 */
typedef NS_ENUM(NSInteger, NSWindowToolbarStyle) {
    NSWindowToolbarStyleAutomatic,
    NSWindowToolbarStyleExpanded,
    NSWindowToolbarStylePreference,
    NSWindowToolbarStyleUnified,
    NSWindowToolbarStyleUnifiedCompact
} API_AVAILABLE(macos(11.0));

static const NSTimeInterval NSEventDurationForever = DBL_MAX;

typedef NS_ENUM(NSInteger, NSWindowUserTabbingPreference) {
    NSWindowUserTabbingPreferenceManual,
    NSWindowUserTabbingPreferenceAlways,
    NSWindowUserTabbingPreferenceInFullScreen,
} API_AVAILABLE(macos(10.12));

/*!
 * @typedef NSWindowTabbingMode
 *
 * @const NSWindowTabbingModeAutomatic The system automatically prefers to tab this window when appropriate.
 * @const NSWindowTabbingModePreferred The window explicitly should prefer to tab when shown.
 * @const NSWindowTabbingModeDisallowed The window explicitly should not prefer to tab when shown.
 */
typedef NS_ENUM(NSInteger, NSWindowTabbingMode) {
    NSWindowTabbingModeAutomatic,
    NSWindowTabbingModePreferred,
    NSWindowTabbingModeDisallowed
}  API_AVAILABLE(macos(10.12));

typedef NS_ENUM(NSInteger, NSTitlebarSeparatorStyle) {
    NSTitlebarSeparatorStyleAutomatic,
    NSTitlebarSeparatorStyleNone,
    NSTitlebarSeparatorStyleLine,
    NSTitlebarSeparatorStyleShadow
} API_AVAILABLE(macos(11.0));

typedef NSString * NSWindowFrameAutosaveName NS_SWIFT_BRIDGED_TYPEDEF;
typedef NSString * NSWindowPersistableFrameDescriptor NS_SWIFT_BRIDGED_TYPEDEF;
typedef NSString * NSWindowTabbingIdentifier NS_SWIFT_BRIDGED_TYPEDEF;

@interface NSWindow : NSResponder <NSAnimatablePropertyContainer, NSMenuItemValidation, NSUserInterfaceValidations, NSUserInterfaceItemIdentification, NSAppearanceCustomization, NSAccessibilityElement, NSAccessibility>

+ (NSRect)frameRectForContentRect:(NSRect)cRect styleMask:(NSWindowStyleMask)style;
+ (NSRect)contentRectForFrameRect:(NSRect)fRect styleMask:(NSWindowStyleMask)style;
+ (CGFloat)minFrameWidthWithTitle:(NSString *)title styleMask:(NSWindowStyleMask)style;
@property (class, readonly) NSWindowDepth defaultDepthLimit;

- (NSRect)frameRectForContentRect:(NSRect)contentRect;
- (NSRect)contentRectForFrameRect:(NSRect)frameRect;

- (instancetype)initWithContentRect:(NSRect)contentRect styleMask:(NSWindowStyleMask)style backing:(NSBackingStoreType)backingStoreType defer:(BOOL)flag NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithContentRect:(NSRect)contentRect styleMask:(NSWindowStyleMask)style backing:(NSBackingStoreType)backingStoreType defer:(BOOL)flag screen:(nullable NSScreen *)screen;
- (instancetype)initWithCoder:(NSCoder *)coder NS_UNAVAILABLE; // Use initWithContentRect:. This method will throw an exception for coders that support allowsKeyedCoding, and is only available for compatibility with non keyed coding.

@property (copy) NSString *title;

/*!
 * Secondary text that may be displayed adjacent to or below the primary title depending on the configuration of the window.
 * A value of empty string will remove the subtitle from the window layout.
 */
@property (copy) NSString *subtitle API_AVAILABLE(macos(11.0));

/// See the enum values for how this property works.
@property NSWindowTitleVisibility titleVisibility API_AVAILABLE(macos(10.10)); // Default value is NSWindowTitleVisible

/// When \c YES, the titlebar doesn't draw its background, allowing all buttons to show through, and "click through" to happen. In general, this is only useful when \c NSFullSizeContentViewWindowMask is set.
@property BOOL titlebarAppearsTransparent API_AVAILABLE(macos(10.10));

/// Specifies how the titlebar area of the window should appear when the window displays an NSToolbar
@property NSWindowToolbarStyle toolbarStyle API_AVAILABLE(macos(11.0));

/// The \c contentLayoutRect will return the area inside the window that is for non-obscured content. Typically, this is the same thing as the `contentView`'s frame. However, for windows with the \c NSFullSizeContentViewWindowMask set, there needs to be a way to determine the portion that is not under the toolbar. The \c contentLayoutRect returns the portion of the layout that is not obscured under the toolbar. \c contentLayoutRect is in window coordinates. It is KVO compliant. */
@property (readonly) NSRect contentLayoutRect API_AVAILABLE(macos(10.10));

/// \c contentLayoutGuide is a corollary to \c contentLayoutRect. It can be used by autolayout constraints to automatically bind to the \c contentLayoutRect.
@property (nullable, readonly) id contentLayoutGuide API_AVAILABLE(macos(10.10));

/* The following methods allow you to add accessory views to the titlebar/toolbar area of a window. See NSTitlebarAccessoryViewController for more details.
 */
@property (copy) NSArray<__kindof NSTitlebarAccessoryViewController *> *titlebarAccessoryViewControllers API_AVAILABLE(macos(10.10));
- (void)addTitlebarAccessoryViewController:(NSTitlebarAccessoryViewController *)childViewController API_AVAILABLE(macos(10.10));
- (void)insertTitlebarAccessoryViewController:(NSTitlebarAccessoryViewController *)childViewController atIndex:(NSInteger)index API_AVAILABLE(macos(10.10));
/* NOTE: you can use this method, or removeFromParentViewController, which ever is easier. */
- (void)removeTitlebarAccessoryViewControllerAtIndex:(NSInteger)index API_AVAILABLE(macos(10.10));

/*!
 * If url is not nil and its path is not empty, the window will show a document icon in the titlebar.
 * If the url represents a filename or other resource with a known icon, that icon will be used as the document icon.  Otherwise the default document icon will be used.  The icon can be customized using `-[[NSWindow standardWindowButton:NSWindowDocumentIconButton] setImage:customImage]`.  If url is not nil and its path is not empty, the window will have a pop-up menu which can be shown via command-click on the area containing the document icon and title.  By default, this menu will display the path components of the url.  The presence and contents of this menu can be controlled by the delegate method `-[window:shouldPopUpDocumentPathMenu:]`
 * If the url is nil or has an empty path, the window will not show a document icon and will not have a pop-up menu available via command-click.
 */
@property (nullable, copy) NSURL *representedURL API_AVAILABLE(macos(10.5));
@property (copy) NSString *representedFilename;
- (void)setTitleWithRepresentedFilename:(NSString *)filename;
@property (getter=isExcludedFromWindowsMenu) BOOL excludedFromWindowsMenu;
@property (nullable, strong) __kindof NSView *contentView;
@property (nullable, weak) id<NSWindowDelegate> delegate;
@property (readonly) NSInteger windowNumber;

/// @note The styleMask can only be set on macOS 10.6 and later. Valid \c styleMask settings have the same restrictions as the \c styleMask passed to `-initWithContentRect:styleMask:backing:defer:`.  Some \c styleMask changes will cause the view hierarchy to be rebuilt, since there is a different subclass for the top level view of a borderless window than for the top level view of a titled window.
@property NSWindowStyleMask styleMask;

- (nullable NSText *)fieldEditor:(BOOL)createFlag forObject:(nullable id)object;
- (void)endEditingFor:(nullable id)object;

- (NSRect)constrainFrameRect:(NSRect)frameRect toScreen:(nullable NSScreen *)screen;
- (void)setFrame:(NSRect)frameRect display:(BOOL)flag;
- (void)setContentSize:(NSSize)size;
- (void)setFrameOrigin:(NSPoint)point;
- (void)setFrameTopLeftPoint:(NSPoint)point;
- (NSPoint)cascadeTopLeftFromPoint:(NSPoint)topLeftPoint;
@property (readonly) NSRect frame;

/*! Subclasses can override \c animationResizeTime: to control the total time for the frame change.
 \c newFrame is the rect passed into \c setFrame:display:animate:
 */
- (NSTimeInterval)animationResizeTime:(NSRect)newFrame;

/*! \c setFrame:display:animate: is equivalent to \c setFrame:display: if the \c animateFlag is NO.
    If the \c animationFlag is YES, this method will perform a smooth resize of the window, where the total time for the resize is specified by \c -animationResizeTime:
*/
- (void)setFrame:(NSRect)frameRect display:(BOOL)displayFlag animate:(BOOL)animateFlag;

@property (readonly) BOOL inLiveResize    API_AVAILABLE(macos(10.6));

@property NSSize resizeIncrements;
@property NSSize aspectRatio;

@property NSSize contentResizeIncrements;
@property NSSize contentAspectRatio;

@property BOOL viewsNeedDisplay;
- (void)displayIfNeeded;
- (void)display;

@property BOOL preservesContentDuringLiveResize;

- (void)update;

- (BOOL)makeFirstResponder:(nullable NSResponder *)responder;
/* firstResponder is Key Value Observing (KVO) compliant. */
@property (readonly, weak) NSResponder *firstResponder;

@property (readonly) NSEventModifierFlags resizeFlags;
- (void)close;
@property (getter=isReleasedWhenClosed) BOOL releasedWhenClosed;
- (void)miniaturize:(nullable id)sender;
- (void)deminiaturize:(nullable id)sender;
@property (getter=isZoomed, readonly) BOOL zoomed;
- (void)zoom:(nullable id)sender;
@property (getter=isMiniaturized, readonly) BOOL miniaturized;
- (BOOL)tryToPerform:(SEL)action with:(nullable id)object;
- (nullable id)validRequestorForSendType:(nullable NSPasteboardType)sendType returnType:(nullable NSPasteboardType)returnType;
@property (null_resettable, copy) NSColor *backgroundColor;

/* Indicates the thickness of a given border of the window. NSMinYEdge is the bottom edge of the window, while NSMaxYEdge is the top edge of the window. This method may throw an exception for values that don't apply to the current window styleMask; specifically, passing NSMaxYEdge for a non-textured window will always raise. The contentBorder does not include the titlebar or toolbar.
 */
- (void)setContentBorderThickness:(CGFloat)thickness forEdge:(NSRectEdge)edge API_AVAILABLE(macos(10.5));
- (CGFloat)contentBorderThicknessForEdge:(NSRectEdge)edge API_AVAILABLE(macos(10.5));

- (void)setAutorecalculatesContentBorderThickness:(BOOL)flag forEdge:(NSRectEdge)edge API_AVAILABLE(macos(10.5));
- (BOOL)autorecalculatesContentBorderThicknessForEdge:(NSRectEdge)edge API_AVAILABLE(macos(10.5));

/* Calling -setMovable with a flag of NO will disable server-side dragging of the window via titlebar or background.  -setMovableByWindowBackground:YES is ignored on a window that returns NO from -isMovable.  When a window returns NO for -isMovable, it can be assigned to a different space with its relative screen position preserved.  Note that a resizable window may still be resized, and the window frame may be changed programmatically.  Applications may choose to enable application-controlled window dragging after disabling server-side dragging (perhaps to achieve snapping or pinning) by handling the mouseDown/mouseDragged/mouseUp sequence in -sendEvent: in an NSWindow subclass.  Note that a non movable window will also not be moved (or resized) by the system in response to a display reconfiguration. */
@property (getter=isMovable) BOOL movable API_AVAILABLE(macos(10.6));

@property (getter=isMovableByWindowBackground) BOOL movableByWindowBackground;

@property BOOL hidesOnDeactivate;

/// Indicates whether a window can be hidden during `-[NSApplication hide:]`.  Default is \c YES.
@property BOOL canHide;

- (void)center;
- (void)makeKeyAndOrderFront:(nullable id)sender;
- (void)orderFront:(nullable id)sender;
- (void)orderBack:(nullable id)sender;
- (void)orderOut:(nullable id)sender;
- (void)orderWindow:(NSWindowOrderingMode)place relativeTo:(NSInteger)otherWin;
- (void)orderFrontRegardless;

@property (nullable, strong) NSImage *miniwindowImage;
@property (null_resettable, copy) NSString *miniwindowTitle;

@property (readonly, strong) NSDockTile *dockTile API_AVAILABLE(macos(10.5));

@property (getter=isDocumentEdited) BOOL documentEdited;
@property (getter=isVisible, readonly) BOOL visible;
@property (getter=isKeyWindow, readonly) BOOL keyWindow;
@property (getter=isMainWindow, readonly) BOOL mainWindow;
@property (readonly) BOOL canBecomeKeyWindow;
@property (readonly) BOOL canBecomeMainWindow;

/*! Makes the window key and main if eligible, updating NSAppication's `-keyWindow` and `-mainWindow` properties.
 */
- (void)makeKeyWindow;

/*! Makes the window main if eligible. Updates NSApplication's `-mainWindow` property.
 */
- (void)makeMainWindow;

/*! Informs the window that it has become the key window. This method exists as an override point. Do not invoke directly. Instead, invoke `-makeKeyWindow`.
 */
- (void)becomeKeyWindow;

/*! Informs the window that it has stopped being the key window. This method exists as an override point. Do not invoke directly. Windows automatically receive this message when deactivating or when another window has become key.
 */
- (void)resignKeyWindow;

/*! Informs the window that it has become the main window. This method exists as an override point. Do not invoke directly. Instead, invoke `-makeMainWindow`.
 */
- (void)becomeMainWindow;

/*! Informs the window that it has stopped being the main window. This method exists as an override point. Do not invoke directly. Windows automatically receive this message when deactivating or when another window has become main.
 */
- (void)resignMainWindow;

@property (readonly) BOOL worksWhenModal;

/*! Normally, application termination is prohibited when a modal window or sheet is open, without consulting the application delegate.  Some windows like the open panel or toolbar customization sheet should not prevent application termination.  `-setPreventsApplicationTerminationWhenModal:NO` on a modal window or sheet will override the default behavior and allow application termination to proceed, either through the sudden termination path if enabled, or on to the next step of consulting the application delegate.  By default, `-preventsApplicationTerminationWhenModal` returns \c YES
 */
@property BOOL preventsApplicationTerminationWhenModal API_AVAILABLE(macos(10.6));

/* Methods to convert window coordinates to screen coordinates */
- (NSRect)convertRectToScreen:(NSRect)rect API_AVAILABLE(macos(10.7));
- (NSRect)convertRectFromScreen:(NSRect)rect API_AVAILABLE(macos(10.7));
- (NSPoint)convertPointToScreen:(NSPoint)point API_AVAILABLE(macos(10.12));
- (NSPoint)convertPointFromScreen:(NSPoint)point API_AVAILABLE(macos(10.12));

/* Methods to convert to/from a pixel integral backing store space */
- (NSRect)convertRectToBacking:(NSRect)rect API_AVAILABLE(macos(10.7));
- (NSRect)convertRectFromBacking:(NSRect)rect API_AVAILABLE(macos(10.7));
- (NSPoint)convertPointToBacking:(NSPoint)point API_AVAILABLE(macos(10.14));
- (NSPoint)convertPointFromBacking:(NSPoint)point API_AVAILABLE(macos(10.14));

/// Use `NSIntegralRectWithOptions()` to produce a backing store pixel aligned rectangle from the given input rectangle in window coordinates.
- (NSRect)backingAlignedRect:(NSRect)rect options:(NSAlignmentOptions)options API_AVAILABLE(macos(10.7));

/// Returns the scale factor representing the number of backing store pixels corresponding to each linear unit in window space on this \c NSWindow. This method is provided for rare cases when the explicit scale factor is needed. Please use `-convert*ToBacking:` methods whenever possible.
@property (readonly) CGFloat backingScaleFactor API_AVAILABLE(macos(10.7)); 

- (void)performClose:(nullable id)sender;
- (void)performMiniaturize:(nullable id)sender;
- (void)performZoom:(nullable id)sender;
- (NSData *)dataWithEPSInsideRect:(NSRect)rect;
- (NSData *)dataWithPDFInsideRect:(NSRect)rect;
- (void)print:(nullable id)sender;

/*!
 * Default is \c NO. Set to \c YES to allow a window to display tooltips even when the application is in the background.  Note that, enabling tooltips in an inactive application will cause the app to do work any time the mouse passes over the window.  This can degrade system performance.
 * Returns \c YES if this window displays tooltips even when the application is in the background.  To configure this setting you should call `-setAllowsToolTipsWhenApplicationIsInactive:` instead of overriding `-allowsToolTipsWhenApplicationIsInactive`.
 */
@property BOOL allowsToolTipsWhenApplicationIsInactive;

@property NSBackingStoreType backingType;
@property NSWindowLevel level;
@property NSWindowDepth depthLimit;
- (void)setDynamicDepthLimit:(BOOL)flag;
@property (readonly) BOOL hasDynamicDepthLimit;

/*! The screen property returns the best screen for the window. If the window only intersects one screen, it returns that screen. If it intersects more than one screen, then it resolves the tie through based on what space it is mostly on. It may return nil if there are no available screens, or it is completely off screen.
 */
@property (nullable, readonly, strong) NSScreen *screen;
@property (nullable, readonly, strong) NSScreen *deepestScreen;

@property BOOL hasShadow;
- (void)invalidateShadow;
@property CGFloat alphaValue;
@property (getter=isOpaque) BOOL opaque;


/*! `-setSharingType:` specifies whether the window content can be read and/or written from another process.  The default sharing type is \c NSWindowSharingReadOnly, which means other processes can read the window content (eg. for window capture) but cannot modify it.  If you set your window sharing type to \c NSWindowSharingNone, so that the content cannot be captured, your window will also not be able to participate in a number of system services, so this setting should be used with caution.  If you set your window sharing type to \c NSWindowSharingReadWrite, other processes can both read and modify the window content.
*/
@property NSWindowSharingType sharingType API_AVAILABLE(macos(10.5));

/*! Controls whether threading of view drawing should be enabled for this window.  Defaults to \c YES.  When this is set to \c YES, AppKit's view system is allowed to perform `-drawRect:` activity for the window's views on threads other than the main thread, for views that have `canDrawConcurrently == YES`.  When this is set to \c NO, the window's views will be drawn serially as on 10.5 and earlier, even though some of the views may have `canDrawConcurrently == YES`.
 */
@property BOOL allowsConcurrentViewDrawing API_AVAILABLE(macos(10.6));

@property BOOL displaysWhenScreenProfileChanges;

/*!
 In recent macOS versions this method does not do anything and should not be called. 
 */
- (void)disableScreenUpdatesUntilFlush;

/*! This API controls whether the receiver is permitted onscreen before the user has logged in.  This property is off by default.  Alert panels and windows presented by input managers are examples of windows which should have this property set.
*/
@property BOOL canBecomeVisibleWithoutLogin API_AVAILABLE(macos(10.5));


@property NSWindowCollectionBehavior collectionBehavior API_AVAILABLE(macos(10.5));


/*! Provides for per-window control over automatic orderFront/orderOut animation behaviors added in 10.7.  Can be set to \c NSWindowAnimationBehaviorNone to disable Appkit's automatic animations for a given window, or to one of the other non-Default \c NSWindowAnimationBehavior values to override AppKit's automatic inference of appropriate animation behavior based on the window's apparent type.
*/
@property NSWindowAnimationBehavior animationBehavior API_AVAILABLE(macos(10.7));


/*! Returns \c YES if this window is associated with the active space.  For visible windows, this API indicates whether the window is currently visible on the active space.  For offscreen windows, it indicates whether ordering the window onscreen would make it bring it onto the active space */
@property (getter=isOnActiveSpace, readonly) BOOL onActiveSpace API_AVAILABLE(macos(10.6));

/*! `-toggleFullScreen:` enters or exits for full screen. A window must have \c NSWindowCollectionBehaviorFullScreenAuxiliary or \c NSWindowCollectionBehaviorFullScreenPrimary included in the \c collectionBehavior property; if it does not, this method may simply do nothing.
 */
- (void)toggleFullScreen:(nullable id)sender API_AVAILABLE(macos(10.7));

@property (readonly, copy) NSWindowPersistableFrameDescriptor stringWithSavedFrame;
- (void)setFrameFromString:(NSWindowPersistableFrameDescriptor)string;
- (void)saveFrameUsingName:(NSWindowFrameAutosaveName)name;
// Set force=YES to use setFrameUsingName on a non-resizable window
- (BOOL)setFrameUsingName:(NSWindowFrameAutosaveName)name force:(BOOL)force;
- (BOOL)setFrameUsingName:(NSWindowFrameAutosaveName)name;
- (BOOL)setFrameAutosaveName:(NSWindowFrameAutosaveName)name;
@property (readonly, copy) NSWindowFrameAutosaveName frameAutosaveName;
+ (void)removeFrameUsingName:(NSWindowFrameAutosaveName)name;

/* NOTE: minSize/contentMinSize and maxSize/contentMaxSize are ignored when using autolayout.
 */
@property NSSize minSize;
@property NSSize maxSize;
@property NSSize contentMinSize;
@property NSSize contentMaxSize;

/* These are the min and max values for a full screen tiled window.
 
 In general, one should not need to explicitly set the min/maxFullScreenContentSize. If an application does not change its window content upon entering full screen, then the normal auto layout min and max size will be sufficient, and one should not set these values. If an application does significantly rework the UI in full screen, then it may be necessary to set a min/maxFullScreenContentSize. This size is what is used to determine if a window can fit when it is in full screen in a tile. This property may be used even if the window does not support full screen, but are implicitly opted into supporting a full screen tile based on resizing behavior and window properties (see the collectionBehavior property). By default, the system uses auto layout to determine the min and max sizes. If auto layout is not used, contentMinSize and contentMaxSize are queried.
 */
@property NSSize minFullScreenContentSize API_AVAILABLE(macos(10.11));
@property NSSize maxFullScreenContentSize API_AVAILABLE(macos(10.11));

@property (readonly, copy) NSDictionary<NSDeviceDescriptionKey, id> *deviceDescription;

@property (nullable, weak) __kindof NSWindowController *windowController;

/*
 This API presents modal-sheets on this window. It replaces NSApp's -beginSheet:modalForWindow:modalDelegate:didEndSelector:contextInfo:.
 
 If the window already has a presented sheet, it will queue up sheets presented after that. Once the presented sheet is dismissed, the next queued sheet will be presented, and so forth.
 Critical sheets will skip this queuing process and be immediately presented on top of existing sheets. The presented sheet will be temporarily disabled and be able to be interacted with after the critical sheet is dismissed, and will then continue as normal. Critical sheets should only be used for time-critical or important events, when the presentation of the sheet needs to be guaranteed (Critical Alerts will automatically use this API).
*/
- (void)beginSheet:(NSWindow *)sheetWindow completionHandler:(void (^ _Nullable)(NSModalResponse returnCode))handler API_AVAILABLE(macos(10.9));
- (void)beginCriticalSheet:(NSWindow *)sheetWindow completionHandler:(void (^ _Nullable)(NSModalResponse returnCode))handler API_AVAILABLE(macos(10.9));
- (void)endSheet:(NSWindow *)sheetWindow API_AVAILABLE(macos(10.9));
- (void)endSheet:(NSWindow *)sheetWindow returnCode:(NSModalResponse)returnCode API_AVAILABLE(macos(10.9));

/// An ordered array of the sheets on the window. This consists of the presented sheets in top-to-bottom order, followed by queued sheets in the order they were queued. This does not include nested/sub-sheets.
@property (readonly, copy) NSArray<__kindof NSWindow *> *sheets API_AVAILABLE(macos(10.9));

/// Returns the top-most sheet if there is one or more sheets, or nil if there is no sheet.
@property (nullable, readonly, strong) NSWindow *attachedSheet;

@property (getter=isSheet, readonly) BOOL sheet;

/*!
 * Returns the window that the sheet is directly attached to. This is based on the logical attachment of the sheet, not visual attachment.
 * This relationship exists starting when the sheet is begun (using \c NSApplication's `-beginSheet:modalForWindow:modalDelegate:didEndSelector:contextInfo: or NSWindow's -beginSheet:completionHandler:`), and ending once it is ordered out.
 * Returns nil if the window is not a sheet or has no sheet parent.
 */
@property (nullable, readonly, strong) NSWindow *sheetParent API_AVAILABLE(macos(10.9));


+ (nullable NSButton *)standardWindowButton:(NSWindowButton)b forStyleMask:(NSWindowStyleMask)styleMask;
- (nullable NSButton *)standardWindowButton:(NSWindowButton)b;

- (void)addChildWindow:(NSWindow *)childWin ordered:(NSWindowOrderingMode)place;
- (void)removeChildWindow:(NSWindow *)childWin;
