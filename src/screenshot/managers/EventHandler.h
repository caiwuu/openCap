#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QObject>

#include "ScreenshotToolbar.h"

// 前向声明
class ScreenshotProcessor;

// 事件处理器类 - 负责处理工具栏和用户交互事件
class EventHandler : public QObject
{
  Q_OBJECT

public:
  // 构造函数和析构函数
  explicit EventHandler(QObject* parent = nullptr);
  ~EventHandler();

  // 设置相关组件
  void setScreenshotProcessor(ScreenshotProcessor* processor);

  // 工具栏事件处理
  void handleToolSelected(ScreenshotToolbar::ToolType toolType);
  void handleActionTriggered(ScreenshotToolbar::ActionType actionType);

  // 按钮事件处理
  void handleOkButton();
  void handleCancelButton();
  void handleSaveButton();

signals:
  // 向外发射的信号
  void screenshotFinished(const QRect& region);
  void screenshotCancelled();
  void saveRequested();

private:
  ScreenshotProcessor* m_processor; // 截图处理器引用
  QWidget* m_widget;
};

#endif // EVENTHANDLER_H