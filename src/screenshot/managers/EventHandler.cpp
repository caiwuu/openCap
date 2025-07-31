#include "EventHandler.h"

#include <QDebug>
#include <QTimer>
#include <QWidget>

#include "ScreenshotProcessor.h"

// 构造函数
EventHandler::EventHandler(QObject* parent)
  : QObject(parent), m_processor(nullptr), m_widget(qobject_cast<QWidget*>(parent))
{
}

// 析构函数
EventHandler::~EventHandler() {}

// 设置截图处理器
void EventHandler::setScreenshotProcessor(ScreenshotProcessor* processor)
{
  m_processor = processor;
}

// 处理工具选择
void EventHandler::handleToolSelected(ScreenshotToolbar::ToolType toolType)
{
  // 更新一次渲染
  m_widget->update();
  // 根据工具类型执行相应操作
  switch (toolType)
  {
    case ScreenshotToolbar::ToolType::Rectangle:
      qDebug() << "选择矩形工具";
      // TODO: 实现矩形绘制功能
      break;
    case ScreenshotToolbar::ToolType::Step:
      qDebug() << "选择步骤标注工具";
      // TODO: 实现步骤标注功能
      break;
    case ScreenshotToolbar::ToolType::Arrow:
      qDebug() << "选择箭头工具";
      // TODO: 实现箭头绘制功能
      break;
    case ScreenshotToolbar::ToolType::Pen:
      qDebug() << "选择画笔工具";
      // TODO: 实现画笔绘制功能
      break;
    case ScreenshotToolbar::ToolType::Text:
      qDebug() << "选择文字工具";
      // TODO: 实现文字添加功能
      break;
    case ScreenshotToolbar::ToolType::Mosaic:
      qDebug() << "选择马赛克工具";
      // TODO: 实现马赛克功能
      break;
    case ScreenshotToolbar::ToolType::Marker:
      qDebug() << "选择区域高亮工具";
      // TODO: 实现区域高亮功能
      break;
  }
}

// 处理功能触发
void EventHandler::handleActionTriggered(ScreenshotToolbar::ActionType actionType)
{
  // 根据功能类型执行相应操作
  switch (actionType)
  {
    case ScreenshotToolbar::ActionType::Undo:
      qDebug() << "执行撤销操作";
      // TODO: 实现撤销功能
      break;
    case ScreenshotToolbar::ActionType::Pin:
      qDebug() << "钉在桌面";
      // TODO: 实现钉在桌面功能
      break;
    case ScreenshotToolbar::ActionType::Save:
      qDebug() << "保存截图到文件";
      handleSaveButton();
      break;
    case ScreenshotToolbar::ActionType::Ok:
      handleOkButton();
      break;
    case ScreenshotToolbar::ActionType::Cancel:
      handleCancelButton();
      break;
  }
}

// 处理确定按钮
void EventHandler::handleOkButton()
{
  qDebug() << "用户点击确定按钮";

  // 先执行复制到剪切板，然后发射完成信号
  if (m_processor)
  {
    // 这里需要获取选择区域，但EventHandler没有直接访问SelectionManager
    // 我们改为通过信号来处理这个问题
  }

  // 发射截图完成信号（传递空区域，因为复制到剪切板不需要区域信息）
  QTimer::singleShot(0, this, [this]() { emit screenshotFinished(QRect()); });
}

// 处理取消按钮
void EventHandler::handleCancelButton()
{
  qDebug() << "用户取消截图";

  // 发射取消信号
  QTimer::singleShot(0, this, [this]() { emit screenshotCancelled(); });
}

// 处理保存按钮
void EventHandler::handleSaveButton()
{
  qDebug() << "用户请求保存截图";

  // 发射保存请求信号
  emit saveRequested();
}