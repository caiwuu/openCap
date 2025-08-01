#ifndef UIUTILS_H
#define UIUTILS_H

#include <QPushButton>
#include <QString>
#include <QWidget>

/**
 * 默认样式常量
 */
namespace UIUtilsConstants
{
extern const QString DEFAULT_BUTTON_STYLE;
extern const QString DEFAULT_SEPARATOR_STYLE;
extern const QString DEFAULT_TOOLBAR_STYLE;
} // namespace UIUtilsConstants

/**
 * 工具栏按钮参数结构体
 */
struct ToolBarButtonParams
{
  QWidget* parent = nullptr;
  const QString& iconName;
  const QString& toolTip;
  bool checkable = true;
  const QString& buttonStyle = UIUtilsConstants::DEFAULT_BUTTON_STYLE;
};

/**
 * UI工具函数类
 * 提供通用的UI组件创建方法
 */
class UIUtils
{
public:
  // 私有构造函数（静态类）
  UIUtils() = delete;

  /**
   * 创建工具栏按钮
   * @param ToolBarButtonParams 按钮参数结构体
   * @return 创建的按钮指针
   */
  static QPushButton* createToolbarButton(ToolBarButtonParams params);
};

#endif // UIUTILS_H