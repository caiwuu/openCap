#include "CursorManager.h"

#include <QCursor>
#include <QDebug>

// 构造函数
CursorManager::CursorManager(QWidget* widget) : m_widget(widget)
{
  if (!m_widget)
  {
    qWarning() << "CursorManager: widget不能为空";
  }
}

// 析构函数
CursorManager::~CursorManager() {}

// 根据状态更新光标
void CursorManager::updateCursor(bool isSelecting, bool isSelectionFinished)
{
  if (!m_widget)
    return;

  if (isSelecting || !isSelectionFinished)
  {
    m_widget->setCursor(Qt::ArrowCursor); // 选择时使用十字光标
  }
  else
  {
    m_widget->setCursor(Qt::ArrowCursor); // 默认箭头光标
  }
}

// 根据鼠标位置更新光标
void CursorManager::updateCursor(const QPoint& pos, SelectionManager* selectionManager)
{
  if (!m_widget || !selectionManager)
    return;

  if (!selectionManager->isSelectionFinished())
  {
    m_widget->setCursor(Qt::ArrowCursor); // 选择时使用十字光标
    return;
  }

  // 检查是否在锚点上
  SelectionManager::ResizeHandle handle = selectionManager->getResizeHandle(pos);
  if (handle != SelectionManager::ResizeHandle::None)
  {
    updateCursor(handle); // 使用锚点对应的光标
    return;
  }

  // 检查是否在选择框内部
  if (selectionManager->isInsideSelection(pos))
  {
    m_widget->setCursor(Qt::SizeAllCursor); // 在选择框内部使用移动光标
    return;
  }

  // 默认光标
  m_widget->setCursor(Qt::ArrowCursor);
}

// 根据锚点更新光标
void CursorManager::updateCursor(SelectionManager::ResizeHandle handle)
{
  if (!m_widget)
    return;

  switch (handle)
  {
    case SelectionManager::ResizeHandle::TopLeft:
    case SelectionManager::ResizeHandle::BottomRight:
      m_widget->setCursor(Qt::SizeFDiagCursor); // nwse-resize: 西北-东南调整
      break;
    case SelectionManager::ResizeHandle::TopRight:
    case SelectionManager::ResizeHandle::BottomLeft:
      m_widget->setCursor(Qt::SizeBDiagCursor); // nesw-resize: 东北-西南调整
      break;
    case SelectionManager::ResizeHandle::TopCenter:
    case SelectionManager::ResizeHandle::BottomCenter:
      m_widget->setCursor(Qt::SizeVerCursor); // ns-resize: 南北调整（垂直）
      break;
    case SelectionManager::ResizeHandle::MiddleLeft:
    case SelectionManager::ResizeHandle::MiddleRight:
      m_widget->setCursor(Qt::SizeHorCursor); // ew-resize: 东西调整（水平）
      break;
    default:
      m_widget->setCursor(Qt::ArrowCursor); // 默认箭头光标
      break;
  }
}

// 设置默认光标
void CursorManager::setDefaultCursor()
{
  if (m_widget)
  {
    m_widget->setCursor(Qt::ArrowCursor);
  }
}

// 设置十字光标
void CursorManager::setCrossCursor()
{
  if (m_widget)
  {
    m_widget->setCursor(Qt::ArrowCursor);
  }
}

// 设置箭头光标
void CursorManager::setArrowCursor()
{
  if (m_widget)
  {
    m_widget->setCursor(Qt::ArrowCursor);
  }
}