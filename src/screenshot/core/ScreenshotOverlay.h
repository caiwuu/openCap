#ifndef SCREENSHOTOVERLAY_H // 防止头文件重复包含的宏定义开始
#define SCREENSHOTOVERLAY_H // 定义头文件标识符

#include <QEnterEvent> // 包含Qt鼠标进入事件类
#include <QKeyEvent>   // 包含Qt键盘事件类
#include <QLabel>      // 包含Qt标签类
#include <QMouseEvent> // 包含Qt鼠标事件类
#include <QPainter>    // 包含Qt绘制器类
#include <QPixmap>     // 包含Qt像素图类
#include <QPoint>      // 包含Qt点坐标类
#include <QRect>       // 包含Qt矩形类
#include <QTimer>      // 包含Qt定时器类
#include <QWidget>     // 包含Qt窗口部件基类

// 包含工具栏头文件以使用枚举类型
#include "ScreenshotToolbar.h"

// 前向声明
class ScreenshotRenderer;
class SelectionManager;
class CursorManager;
class ScreenshotProcessor;
class PerformanceManager;
class WindowLevelManager;
class UIManager;
class EventHandler;

// 截图覆盖层类，继承自QWidget
class ScreenshotOverlay : public QWidget
{
Q_OBJECT // Qt元对象系统宏，支持信号槽机制

    public :
  // 构造函数：创建截图覆盖层，接收截图和父窗口参数
  explicit ScreenshotOverlay(const QPixmap& screenshot, QWidget* parent = nullptr);
  ~ScreenshotOverlay();  // 析构函数：清理资源
  void setWindowLevel(); // 设置窗口层级为最高

signals:
  // 信号：截图完成，传递选中的区域
  void screenshotFinished(const QRect& region);
  // 信号：截图取消
  void screenshotCancelled();

private slots:
  // 工具栏信号处理槽函数
  void onToolbarToolSelected(ScreenshotToolbar::ToolType toolType);        // 工具选择槽函数
  void onToolbarActionTriggered(ScreenshotToolbar::ActionType actionType); // 功能按钮槽函数

  // 内部按钮处理函数
  void onOkButtonClicked();     // 确定按钮点击槽函数
  void onCancelButtonClicked(); // 取消按钮点击槽函数

  // 截图处理函数
  void copyToClipboard(); // 复制到剪切板
  void saveToFile();      // 保存到文件

protected:
  // 重写的事件处理函数
  void paintEvent(QPaintEvent* event) override;        // 绘制事件处理
  void mousePressEvent(QMouseEvent* event) override;   // 鼠标按下事件处理
  void mouseMoveEvent(QMouseEvent* event) override;    // 鼠标移动事件处理
  void mouseReleaseEvent(QMouseEvent* event) override; // 鼠标释放事件处理
  void keyPressEvent(QKeyEvent* event) override;       // 键盘按键事件处理
  void enterEvent(QEnterEvent* event) override;        // 鼠标进入事件处理
  void leaveEvent(QEvent* event) override;             // 鼠标离开事件处理

private:
  // 私有绘制函数
  void drawInfo(QPainter& painter); // 绘制信息文字

  // 私有工具函数
  void createInfoLabels(); // 创建信息标签
  void createToolbar();    // 创建工具栏
  void showToolbar();      // 显示工具栏
  void hideToolbar();      // 隐藏工具栏

  // 初始化相关
  void initializeManagers(); // 初始化管理器
  void setupConnections();   // 设置信号连接
  void cleanupManagers();    // 清理管理器

  // 私有成员变量
  QPixmap m_screenshot;           // 存储全屏截图
  ScreenshotRenderer* m_renderer; // 渲染器对象
  ScreenshotToolbar* m_toolbar;   // 工具栏组件

  // 管理器对象
  SelectionManager* m_selectionManager;       // 选择区域管理器
  CursorManager* m_cursorManager;             // 光标管理器
  ScreenshotProcessor* m_screenshotProcessor; // 截图处理器
  PerformanceManager* m_performanceManager;   // 性能管理器
  WindowLevelManager* m_windowManager;        // 窗口层级管理器
  UIManager* m_uiManager;                     // UI管理器
  EventHandler* m_eventHandler;               // 事件处理器

  // 当前鼠标位置（仍需要在主类中维护）
  QPoint m_mousePos; // 当前鼠标位置

  // 常量定义（已移动到各个管理器中，此处保留为空或删除）
};

#endif // SCREENSHOTOVERLAY_H      // 防止头文件重复包含的宏定义结束