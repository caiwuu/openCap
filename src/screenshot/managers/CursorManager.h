#ifndef CURSORMANAGER_H
#define CURSORMANAGER_H

#include <QPoint>
#include <QWidget>

#include "SelectionManager.h"

// 光标管理器类 - 负责管理光标样式
class CursorManager
{
public:
  // 构造函数和析构函数
  explicit CursorManager(QWidget* widget);
  ~CursorManager();

  // 光标更新方法
  void updateCursor(bool isSelecting, bool isSelectionFinished);
  void updateCursor(const QPoint& pos, SelectionManager* selectionManager);
  void updateCursor(SelectionManager::ResizeHandle handle);

  // 设置默认光标
  void setDefaultCursor();
  void setCrossCursor();
  void setArrowCursor();

private:
  QWidget* m_widget; // 关联的窗口部件
};

#endif // CURSORMANAGER_H