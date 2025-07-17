#ifndef MACWINDOWLEVEL_H // 防止头文件重复包含的宏定义开始
#define MACWINDOWLEVEL_H // 定义头文件标识符

#include <QtGlobal> // 包含Qt全局定义

// macOS窗口层级控制类
class MacWindowLevel
{
public:
  // 设置窗口层级高于状态栏
  static void setWindowAboveStatusBar(quintptr winId);

  // 设置窗口层级为弹出菜单级别（最高层级之一）
  static void setWindowPopupLevel(quintptr winId);

  // 获取窗口当前层级
  static int getWindowLevel(quintptr winId);

  // 禁用系统手势（如状态栏下拉、Mission Control等）
  static void disableSystemGestures(quintptr winId);

  // 临时禁用系统热角和手势
  static void disableSystemHotCorners();

  // 恢复系统热角和手势
  static void restoreSystemHotCorners();
};

#endif // MACWINDOWLEVEL_H   // 防止头文件重复包含的宏定义结束