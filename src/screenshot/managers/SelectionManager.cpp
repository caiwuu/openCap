#include "SelectionManager.h"

#include <QDebug>
#include <algorithm>

// 构造函数
SelectionManager::SelectionManager()
  : m_isSelecting(false),
    m_hasSelection(false),
    m_isSelectionFinished(false),
    m_startPoint(QPoint(0, 0)),
    m_currentPoint(QPoint(0, 0)),
    m_currentHandle(ResizeHandle::None),
    m_isResizing(false),
    m_resizeStartPos(QPoint(0, 0)),
    m_resizeStartRect(QRect()),
    m_isMoving(false),
    m_moveStartPos(QPoint(0, 0)),
    m_moveStartRect(QRect())
{
}

// 析构函数
SelectionManager::~SelectionManager() {}

// 开始选择
void SelectionManager::startSelection(const QPoint& startPoint)
{
  m_startPoint = startPoint;
  m_currentPoint = startPoint;
  m_isSelecting = true;
  m_hasSelection = false;
  m_isSelectionFinished = false;

  // 重置调整和移动状态
  m_isResizing = false;
  m_isMoving = false;
  m_currentHandle = ResizeHandle::None;

  qDebug() << "开始选择，起始点:" << startPoint;
}

// 更新选择
void SelectionManager::updateSelection(const QPoint& currentPoint)
{
  if (!m_isSelecting)
    return;

  m_currentPoint = currentPoint;
  m_hasSelection = true;

  qDebug() << "更新选择，当前点:" << currentPoint;
}

// 完成选择
void SelectionManager::finishSelection()
{
  if (!m_isSelecting)
    return;

  m_isSelecting = false;

  QRect selectionRect = getSelectionRect();

  // 检查选择区域是否足够大
  if (selectionRect.width() >= MIN_SELECTION_SIZE && selectionRect.height() >= MIN_SELECTION_SIZE)
  {
    m_isSelectionFinished = true;
    qDebug() << "选择完成，区域:" << selectionRect;
  }
  else
  {
    // 选择区域太小，重置
    m_hasSelection = false;
    qDebug() << "选择区域太小，已重置";
  }
}

// 取消选择
void SelectionManager::cancelSelection()
{
  reset();
  qDebug() << "选择已取消";
}

// 获取选择区域
QRect SelectionManager::getSelectionRect() const
{
  if (!m_hasSelection && !m_isSelecting)
  {
    return QRect();
  }

  int x = std::min(m_startPoint.x(), m_currentPoint.x());
  int y = std::min(m_startPoint.y(), m_currentPoint.y());
  int width = std::abs(m_currentPoint.x() - m_startPoint.x());
  int height = std::abs(m_currentPoint.y() - m_startPoint.y());

  return QRect(x, y, width, height);
}

// 是否有选择区域
bool SelectionManager::hasSelection() const
{
  return m_hasSelection;
}

// 是否选择完成
bool SelectionManager::isSelectionFinished() const
{
  return m_isSelectionFinished;
}

// 是否正在选择
bool SelectionManager::isSelecting() const
{
  return m_isSelecting;
}

// 开始调整大小
void SelectionManager::startResize(ResizeHandle handle, const QPoint& startPos)
{
  if (!m_hasSelection || handle == ResizeHandle::None)
    return;

  m_isResizing = true;
  m_currentHandle = handle;
  m_resizeStartPos = startPos;
  m_resizeStartRect = getSelectionRect();

  qDebug() << "开始调整大小，锚点:" << static_cast<int>(handle);
}

// 更新调整大小
void SelectionManager::updateResize(const QPoint& currentPos)
{
  if (!m_isResizing || m_currentHandle == ResizeHandle::None)
    return;

  QRect newRect = m_resizeStartRect;
  QPoint delta = currentPos - m_resizeStartPos;

  switch (m_currentHandle)
  {
    case ResizeHandle::TopLeft:
      newRect.setTopLeft(newRect.topLeft() + delta);
      break;
    case ResizeHandle::TopCenter:
      newRect.setTop(newRect.top() + delta.y());
      break;
    case ResizeHandle::TopRight:
      newRect.setTopRight(newRect.topRight() + delta);
      break;
    case ResizeHandle::MiddleLeft:
      newRect.setLeft(newRect.left() + delta.x());
      break;
    case ResizeHandle::MiddleRight:
      newRect.setRight(newRect.right() + delta.x());
      break;
    case ResizeHandle::BottomLeft:
      newRect.setBottomLeft(newRect.bottomLeft() + delta);
      break;
    case ResizeHandle::BottomCenter:
      newRect.setBottom(newRect.bottom() + delta.y());
      break;
    case ResizeHandle::BottomRight:
      newRect.setBottomRight(newRect.bottomRight() + delta);
      break;
    default:
      return;
  }

  // 确保选择区域不会太小
  if (newRect.width() >= MIN_SELECTION_SIZE && newRect.height() >= MIN_SELECTION_SIZE)
  {
    m_startPoint = newRect.topLeft();
    m_currentPoint = newRect.bottomRight();
  }
}

// 完成调整大小
void SelectionManager::finishResize()
{
  m_isResizing = false;
  m_currentHandle = ResizeHandle::None;
  qDebug() << "调整大小完成";
}

// 是否正在调整大小
bool SelectionManager::isResizing() const
{
  return m_isResizing;
}

// 获取当前锚点
SelectionManager::ResizeHandle SelectionManager::getCurrentHandle() const
{
  return m_currentHandle;
}

// 获取鼠标位置对应的锚点
SelectionManager::ResizeHandle SelectionManager::getResizeHandle(const QPoint& pos) const
{
  if (!m_hasSelection)
    return ResizeHandle::None;

  QRect selection = getSelectionRect();
  const int tolerance = HANDLE_SIZE / 2 + 2; // 增加一点容错

  // 检查各个锚点
  if (QRect(selection.left() - tolerance, selection.top() - tolerance, tolerance * 2, tolerance * 2)
          .contains(pos))
    return ResizeHandle::TopLeft;

  if (QRect(selection.center().x() - tolerance,
            selection.top() - tolerance,
            tolerance * 2,
            tolerance * 2)
          .contains(pos))
    return ResizeHandle::TopCenter;

  if (QRect(
          selection.right() - tolerance, selection.top() - tolerance, tolerance * 2, tolerance * 2)
          .contains(pos))
    return ResizeHandle::TopRight;

  if (QRect(selection.left() - tolerance,
            selection.center().y() - tolerance,
            tolerance * 2,
            tolerance * 2)
          .contains(pos))
    return ResizeHandle::MiddleLeft;

  if (QRect(selection.right() - tolerance,
            selection.center().y() - tolerance,
            tolerance * 2,
            tolerance * 2)
          .contains(pos))
    return ResizeHandle::MiddleRight;

  if (QRect(selection.left() - tolerance,
            selection.bottom() - tolerance,
            tolerance * 2,
            tolerance * 2)
          .contains(pos))
    return ResizeHandle::BottomLeft;

  if (QRect(selection.center().x() - tolerance,
            selection.bottom() - tolerance,
            tolerance * 2,
            tolerance * 2)
          .contains(pos))
    return ResizeHandle::BottomCenter;

  if (QRect(selection.right() - tolerance,
            selection.bottom() - tolerance,
            tolerance * 2,
            tolerance * 2)
          .contains(pos))
    return ResizeHandle::BottomRight;

  return ResizeHandle::None;
}

// 开始移动选择框
void SelectionManager::startMove(const QPoint& startPos)
{
  if (!m_hasSelection || !m_isSelectionFinished)
    return;

  m_isMoving = true;
  m_moveStartPos = startPos;
  m_moveStartRect = getSelectionRect();

  qDebug() << "开始移动选择框";
}

// 更新移动选择框
void SelectionManager::updateMove(const QPoint& currentPos, int screenWidth, int screenHeight)
{
  if (!m_isMoving)
    return;

  QPoint delta = currentPos - m_moveStartPos;
  QRect newRect = m_moveStartRect;
  newRect.translate(delta);

  // 确保选择区域不会超出屏幕边界
  if (newRect.left() < 0)
    newRect.moveLeft(0);
  if (newRect.top() < 0)
    newRect.moveTop(0);
  if (newRect.right() >= screenWidth)
    newRect.moveRight(screenWidth - 1);
  if (newRect.bottom() >= screenHeight)
    newRect.moveBottom(screenHeight - 1);

  m_startPoint = newRect.topLeft();
  m_currentPoint = newRect.bottomRight();
}

// 完成移动选择框
void SelectionManager::finishMove()
{
  m_isMoving = false;
  qDebug() << "移动选择框完成";
}

// 是否正在移动
bool SelectionManager::isMoving() const
{
  return m_isMoving;
}

// 检测鼠标是否在选择框内部（但不在锚点上）
bool SelectionManager::isInsideSelection(const QPoint& pos) const
{
  if (!m_hasSelection || !m_isSelectionFinished)
    return false;

  QRect selection = getSelectionRect();

  // 检查是否在选择区域内
  if (!selection.contains(pos))
    return false;

  // 检查是否在锚点上，如果在锚点上则不算内部
  return getResizeHandle(pos) == ResizeHandle::None;
}

// 是否有效选择
bool SelectionManager::isValidSelection() const
{
  if (!m_hasSelection)
    return false;

  QRect rect = getSelectionRect();
  return rect.width() >= MIN_SELECTION_SIZE && rect.height() >= MIN_SELECTION_SIZE;
}

// 重置所有状态
void SelectionManager::reset()
{
  m_isSelecting = false;
  m_hasSelection = false;
  m_isSelectionFinished = false;
  m_isResizing = false;
  m_isMoving = false;
  m_currentHandle = ResizeHandle::None;
  m_startPoint = QPoint(0, 0);
  m_currentPoint = QPoint(0, 0);
}

// 计算锚点区域
QRect SelectionManager::getHandleRect(ResizeHandle handle) const
{
  if (!m_hasSelection || handle == ResizeHandle::None)
    return QRect();

  QRect selection = getSelectionRect();
  QPoint center;

  switch (handle)
  {
    case ResizeHandle::TopLeft:
      center = selection.topLeft();
      break;
    case ResizeHandle::TopCenter:
      center = QPoint(selection.center().x(), selection.top());
      break;
    case ResizeHandle::TopRight:
      center = selection.topRight();
      break;
    case ResizeHandle::MiddleLeft:
      center = QPoint(selection.left(), selection.center().y());
      break;
    case ResizeHandle::MiddleRight:
      center = QPoint(selection.right(), selection.center().y());
      break;
    case ResizeHandle::BottomLeft:
      center = selection.bottomLeft();
      break;
    case ResizeHandle::BottomCenter:
      center = QPoint(selection.center().x(), selection.bottom());
      break;
    case ResizeHandle::BottomRight:
      center = selection.bottomRight();
      break;
    default:
      return QRect();
  }

  return QRect(
      center.x() - HANDLE_SIZE / 2, center.y() - HANDLE_SIZE / 2, HANDLE_SIZE, HANDLE_SIZE);
}