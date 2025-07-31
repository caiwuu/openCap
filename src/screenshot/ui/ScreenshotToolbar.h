#ifndef SCREENSHOTTOOLBAR_H
#define SCREENSHOTTOOLBAR_H

#include <QButtonGroup>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QPushButton>
#include <QStyle>
#include <QWidget>

/**
 * 截图工具栏组件
 * 提供截图编辑和操作的工具按钮界面
 */
class ScreenshotToolbar : public QWidget
{
  Q_OBJECT

public:
  // 工具类型枚举
  enum class ToolType
  {
    Rectangle = 0, // 矩形/椭圆工具
    Step = 1,      // 步骤标注工具
    Arrow = 2,     // 箭头工具
    Pen = 3,       // 画笔工具
    Text = 4,      // 文字工具
    Mosaic = 5,    // 马赛克工具
    Marker = 6     // 区域高亮工具
  };

  // 功能类型枚举
  enum class ActionType
  {
    Undo,  // 撤销
    Pin,   // 钉在桌面
    Save,  // 保存
    Ok,    // 确定
    Cancel // 取消
  };
  bool m_isEditing;

  explicit ScreenshotToolbar(QWidget* parent = nullptr);
  ~ScreenshotToolbar();

  // 显示和隐藏工具栏
  void showAt(const QPoint& position);
  void hideToolbar();

  // 获取当前选中的工具
  ToolType getCurrentTool() const;

  // 设置按钮可用状态
  void setUndoEnabled(bool enabled);

signals:
  // 工具按钮点击信号
  void toolSelected(ToolType tool);

  // 功能按钮点击信号
  void actionTriggered(ActionType action);

private slots:
  // 内部槽函数
  void onToolButtonClicked(QAbstractButton* button);
  void onUndoButtonClicked();
  void onPinButtonClicked();
  void onSaveButtonClicked();
  void onOkButtonClicked();
  void onCancelButtonClicked();

private:
  // 初始化方法
  void initializeUI();
  void createToolButtons();
  void createFunctionButtons();
  void setupLayout();

  // 图标创建方法
  QIcon createToolIcon(const QString& iconFileName);
  QIcon createWhiteSvgIcon(const QString& svgPath);

  // UI组件
  QHBoxLayout* m_mainLayout;

  // 工具按钮组
  QButtonGroup* m_toolButtonGroup;
  QPushButton* m_rectButton;   // 矩形/椭圆工具
  QPushButton* m_stepButton;   // 步骤标注工具
  QPushButton* m_arrowButton;  // 箭头工具
  QPushButton* m_penButton;    // 画笔工具
  QPushButton* m_textButton;   // 文字工具
  QPushButton* m_mosaicButton; // 马赛克工具
  QPushButton* m_markerButton; // 区域高亮工具

  // 功能按钮
  QPushButton* m_undoButton;   // 撤销按钮
  QPushButton* m_pinButton;    // 钉在桌面按钮
  QPushButton* m_saveButton;   // 保存按钮
  QPushButton* m_okButton;     // 确定按钮
  QPushButton* m_cancelButton; // 取消按钮

  // 分隔线
  QFrame* m_separator1;
  QFrame* m_separator2;

  // 样式常量
  static const QString BUTTON_STYLE;
  static const QString SEPARATOR_STYLE;
};

#endif // SCREENSHOTTOOLBAR_H