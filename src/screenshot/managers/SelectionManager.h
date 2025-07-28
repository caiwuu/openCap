#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H

#include <QPoint>
#include <QRect>

// 选择区域管理器类 - 负责管理截图选择区域的所有逻辑
class SelectionManager
{
public:
  // 锚点调整相关枚举
  enum class ResizeHandle
  {
    None = 0,
    TopLeft,
    TopCenter,
    TopRight,
    MiddleLeft,
    MiddleRight,
    BottomLeft,
    BottomCenter,
    BottomRight
  };

  // 构造函数和析构函数
  explicit SelectionManager();
  ~SelectionManager();

  // 选择状态管理
  void startSelection(const QPoint& startPoint);
  void updateSelection(const QPoint& currentPoint);
  void finishSelection();
  void cancelSelection();

  // 选择区域操作
  QRect getSelectionRect() const;
  bool hasSelection() const;
  bool isSelectionFinished() const;
  bool isSelecting() const;

  // 调整大小相关
  void startResize(ResizeHandle handle, const QPoint& startPos);
  void updateResize(const QPoint& currentPos);
  void finishResize();
  bool isResizing() const;
  ResizeHandle getCurrentHandle() const;
  ResizeHandle getResizeHandle(const QPoint& pos) const;

  // 移动选择框相关
  void startMove(const QPoint& startPos);
  void updateMove(const QPoint& currentPos, int screenWidth, int screenHeight);
  void finishMove();
  bool isMoving() const;
  bool isInsideSelection(const QPoint& pos) const;

  // 工具函数
  bool isValidSelection() const;
  void reset();

private:
  // 常量定义
  static constexpr int MIN_SELECTION_SIZE = 10; // 最小选择区域大小
  static constexpr int HANDLE_SIZE = 8;         // 锚点尺寸

  // 计算锚点区域
  QRect getHandleRect(ResizeHandle handle) const;

  // 选择相关变量
  bool m_isSelecting;         // 是否正在选择区域
  bool m_hasSelection;        // 是否已有选择区域
  bool m_isSelectionFinished; // 是否完成选择
  QPoint m_startPoint;        // 选择起始点
  QPoint m_currentPoint;      // 当前选择点

  // 调整大小相关变量
  ResizeHandle m_currentHandle; // 当前锚点
  bool m_isResizing;            // 是否正在调整大小
  QPoint m_resizeStartPos;      // 调整开始位置
  QRect m_resizeStartRect;      // 调整开始时的选择区域

  // 移动选择框相关变量
  bool m_isMoving;       // 是否正在移动选择框
  QPoint m_moveStartPos; // 移动开始位置
  QRect m_moveStartRect; // 移动开始时的选择区域
};

#endif // SELECTIONMANAGER_H