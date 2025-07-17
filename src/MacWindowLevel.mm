#include "jietu/MacWindowLevel.h"      // 包含macOS窗口层级控制头文件
#include <Cocoa/Cocoa.h>         // 包含Cocoa框架（macOS原生UI框架）
#include <CoreGraphics/CoreGraphics.h>  // 包含CoreGraphics框架（图形和显示服务）
#include <QDebug>                // 包含Qt调试输出功能

// 设置窗口层级高于状态栏
void MacWindowLevel::setWindowAboveStatusBar(quintptr winId)
{
    if (winId == 0) {            // 检查窗口ID是否有效
        qWarning() << "MacWindowLevel: 无效的窗口ID";  // 输出警告信息
        return;                  // 直接返回
    }
    
    // 从Qt窗口ID获取NSView，然后获取NSWindow
    NSView *view = reinterpret_cast<NSView *>(winId);  // 将Qt窗口ID转换为NSView指针
    NSWindow *window = [view window];                  // 从NSView获取NSWindow对象
    
    if (!window) {               // 检查NSWindow是否存在
        qWarning() << "MacWindowLevel: 无法获取NSWindow";  // 输出警告信息
        return;                  // 直接返回
    }
    
    // 使用更高的窗口层级来确保在所有系统UI之上
    // NSScreenSaverWindowLevel = 1000, 这比状态栏和所有系统手势都高
    const NSInteger screenSaverLevel = NSScreenSaverWindowLevel;  // 获取屏保窗口层级常量
    
    [window setLevel:screenSaverLevel];  // 设置窗口层级为屏保级别
    
    qDebug() << "MacWindowLevel: 窗口层级已设置为屏保级别" << screenSaverLevel << "（高于所有系统UI）";  // 输出调试信息
}

// 设置窗口层级为弹出菜单级别
void MacWindowLevel::setWindowPopupLevel(quintptr winId)
{
    if (winId == 0) {            // 检查窗口ID是否有效
        qWarning() << "MacWindowLevel: 无效的窗口ID";  // 输出警告信息
        return;                  // 直接返回
    }
    
    NSView *view = reinterpret_cast<NSView *>(winId);  // 将Qt窗口ID转换为NSView指针
    NSWindow *window = [view window];                  // 从NSView获取NSWindow对象
    
    if (!window) {               // 检查NSWindow是否存在
        qWarning() << "MacWindowLevel: 无法获取NSWindow";  // 输出警告信息
        return;                  // 直接返回
    }
    
    // 设置为弹出菜单层级（NSPopUpMenuWindowLevel = 101）
    [window setLevel:NSPopUpMenuWindowLevel];  // 设置窗口层级为弹出菜单级别
    
    qDebug() << "MacWindowLevel: 窗口层级已设置为弹出菜单级别" << NSPopUpMenuWindowLevel;  // 输出调试信息
}

// 获取窗口当前层级
int MacWindowLevel::getWindowLevel(quintptr winId)
{
    if (winId == 0) {            // 检查窗口ID是否有效
        qWarning() << "MacWindowLevel: 无效的窗口ID";  // 输出警告信息
        return -1;               // 返回无效值
    }
    
    NSView *view = reinterpret_cast<NSView *>(winId);  // 将Qt窗口ID转换为NSView指针
    NSWindow *window = [view window];                  // 从NSView获取NSWindow对象
    
    if (!window) {               // 检查NSWindow是否存在
        qWarning() << "MacWindowLevel: 无法获取NSWindow";  // 输出警告信息
        return -1;               // 返回无效值
    }
    
    NSInteger level = [window level];  // 获取当前窗口层级
    qDebug() << "MacWindowLevel: 当前窗口层级为" << level;  // 输出调试信息
    
    return static_cast<int>(level);  // 转换为int类型并返回
}

// 禁用系统手势
void MacWindowLevel::disableSystemGestures(quintptr winId)
{
    if (winId == 0) {            // 检查窗口ID是否有效
        qWarning() << "MacWindowLevel: 无效的窗口ID";  // 输出警告信息
        return;                  // 直接返回
    }
    
    NSView *view = reinterpret_cast<NSView *>(winId);  // 将Qt窗口ID转换为NSView指针
    NSWindow *window = [view window];                  // 从NSView获取NSWindow对象
    
    if (!window) {               // 检查NSWindow是否存在
        qWarning() << "MacWindowLevel: 无法获取NSWindow";  // 输出警告信息
        return;                  // 直接返回
    }
    
    // 设置更强的窗口行为以完全禁用系统手势
    NSWindowCollectionBehavior behavior = 
        NSWindowCollectionBehaviorStationary |              // 不参与Exposé
        NSWindowCollectionBehaviorIgnoresCycle |            // 不参与窗口循环
        NSWindowCollectionBehaviorCanJoinAllSpaces |        // 出现在所有空间
        NSWindowCollectionBehaviorFullScreenAuxiliary |     // 全屏辅助窗口
        NSWindowCollectionBehaviorFullScreenDisallowsTiling |// 禁用平铺
        NSWindowCollectionBehaviorFullScreenPrimary;        // 主全屏窗口
    
    [window setCollectionBehavior:behavior];  // 设置窗口收集行为
    
    // 额外设置：禁用窗口动画和自动隐藏
    [window setAnimationBehavior:NSWindowAnimationBehaviorNone];  // 禁用窗口动画
    
    // 设置窗口为非激活状态，这样可以避免一些系统手势
    [window setHidesOnDeactivate:NO];  // 失去焦点时不隐藏
    
    // 确保窗口接收所有鼠标事件
    [window setAcceptsMouseMovedEvents:YES];  // 接收鼠标移动事件
    
    // 设置窗口为最前端，阻止系统手势
    [window makeKeyAndOrderFront:nil];  // 使窗口成为关键窗口并移到前面
    
    // 设置窗口样式掩码，移除可能触发系统行为的标志
    [window setStyleMask:NSWindowStyleMaskBorderless];  // 设置为无边框样式
    
    // 强制窗口保持在最前面
    [window setLevel:NSScreenSaverWindowLevel];  // 再次确认层级
    
    qDebug() << "MacWindowLevel: 已完全禁用系统手势和动画，窗口行为设置为" << behavior;  // 输出调试信息
}

// 临时禁用系统热角
void MacWindowLevel::disableSystemHotCorners()
{
    // 通过系统事件来临时禁用热角
    // 这个方法会在截图开始时调用
    
    // 创建一个透明的全屏窗口来覆盖整个屏幕边缘
    // 这样可以阻止系统手势的触发区域
    NSScreen *mainScreen = [NSScreen mainScreen];  // 获取主屏幕
    NSRect screenRect = [mainScreen frame];        // 获取屏幕矩形区域
    
    // 创建顶部条带（阻止状态栏下拉）- 只覆盖顶部2像素
    NSWindow *topBar = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, screenRect.size.height - 2, screenRect.size.width, 2)  // 创建顶部条带窗口
                                                   styleMask:NSWindowStyleMaskBorderless  // 无边框样式
                                                     backing:NSBackingStoreBuffered       // 缓冲备份
                                                       defer:NO];                         // 不延迟创建
    [topBar setLevel:NSScreenSaverWindowLevel + 1];  // 设置最高层级
    [topBar setOpaque:NO];                           // 设置为非不透明
    [topBar setBackgroundColor:[NSColor clearColor]]; // 设置透明背景
    [topBar setIgnoresMouseEvents:YES];              // 关键：让鼠标事件穿透，只阻挡系统手势检测
    [topBar setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces | NSWindowCollectionBehaviorStationary];  // 设置窗口行为
    [topBar orderFront:nil];                         // 显示窗口
    
    // 将窗口引用存储在用户默认值中（简单的全局存储方式）
    [[NSUserDefaults standardUserDefaults] setValue:@((long long)topBar) forKey:@"ScreenshotTopBarWindow"];  // 存储窗口引用
    
    qDebug() << "MacWindowLevel: 已创建顶部阻挡条（穿透模式），禁用状态栏下拉";  // 输出调试信息
}

// 恢复系统热角
void MacWindowLevel::restoreSystemHotCorners()
{
    // 恢复系统热角功能
    // 关闭之前创建的阻挡窗口
    
    long long topBarPtr = [[[NSUserDefaults standardUserDefaults] valueForKey:@"ScreenshotTopBarWindow"] longLongValue];  // 获取存储的窗口引用
    if (topBarPtr != 0) {        // 如果窗口引用有效
        NSWindow *topBar = (NSWindow *)topBarPtr;  // 转换为NSWindow指针
        [topBar close];          // 关闭窗口
        [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"ScreenshotTopBarWindow"];  // 移除存储的引用
        qDebug() << "MacWindowLevel: 已移除顶部阻挡条，恢复系统手势";  // 输出调试信息
   }
}