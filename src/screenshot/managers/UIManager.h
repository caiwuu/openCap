#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QLabel>
#include <QRect>
#include <QWidget>

// UI管理器类 - 负责管理UI标签和信息显示
class UIManager : public QObject
{
  Q_OBJECT

public:
  // 构造函数和析构函数
  explicit UIManager(QWidget* parent);
  ~UIManager();

  // 标签管理
  void createInfoLabels();                                                 // 创建信息标签
  void updateCoordinateLabel(const QRect& selectionRect, int widgetWidth); // 更新坐标标签
  void showCoordinateLabel();                                              // 显示坐标标签
  void hideCoordinateLabel();                                              // 隐藏坐标标签

  // 信息提示管理
  void showInfoMessage(const QString& message);   // 显示信息提示
  void hideInfoMessage();                         // 隐藏信息提示
  void updateInfoMessage(const QString& message); // 更新信息提示

  // 标签位置管理
  void positionCoordinateLabel(const QRect& selectionRect, int widgetWidth, int widgetHeight);
  void positionInfoLabel(int widgetWidth);

  // 样式管理
  void setCoordinateLabelStyle(const QString& style); // 设置坐标标签样式
  void setInfoLabelStyle(const QString& style);       // 设置信息标签样式

private:
  QWidget* m_parentWidget; // 父窗口部件
  QLabel* m_infoLabel;     // 提示信息标签
  QLabel* m_coordLabel;    // 坐标信息标签

  // 常量定义
  static constexpr int MARGIN_TEXT_AND_BG = 8; // 文字和背景的垂直内边距

  // 默认样式
  static const QString DEFAULT_INFO_STYLE;
  static const QString DEFAULT_COORD_STYLE;
};

#endif // UIMANAGER_H