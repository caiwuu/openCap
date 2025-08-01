#include "ScreenshotToolbar.h"

#include <QDebug>

#include "../../utils/UIUtils.h"
#include "IconProvider.h"


ScreenshotToolbar::ScreenshotToolbar(QWidget* parent)
  : QWidget(parent),
    m_mainLayout(nullptr),
    m_toolButtonGroup(nullptr),
    m_rectButton(nullptr),
    m_stepButton(nullptr),
    m_arrowButton(nullptr),
    m_penButton(nullptr),
    m_textButton(nullptr),
    m_mosaicButton(nullptr),
    m_markerButton(nullptr),
    m_undoButton(nullptr),
    m_pinButton(nullptr),
    m_saveButton(nullptr),
    m_okButton(nullptr),
    m_cancelButton(nullptr),
    // 分割线
    m_separator1(nullptr),
    m_separator2(nullptr),
    // 是否正在编辑
    m_isEditing(false)
{
  initializeUI();
}

ScreenshotToolbar::~ScreenshotToolbar()
{
  // Qt 会自动清理子对象，无需手动删除
}

void ScreenshotToolbar::initializeUI()
{
  // 设置对象名称用于样式选择器
  setObjectName("ScreenshotToolbar");

  // 确保背景会被绘制
  setAttribute(Qt::WA_StyledBackground, true);

  // 设置工具栏样式
  setStyleSheet(UIUtilsConstants::DEFAULT_TOOLBAR_STYLE);

  // 创建各个组件
  createToolButtons();
  createFunctionButtons();
  setupLayout();

  // 调整工具栏大小
  adjustSize();
  hide(); // 初始隐藏
}

void ScreenshotToolbar::createToolButtons()
{
  // 创建按钮组用于工具按钮的互斥选择
  m_toolButtonGroup = new QButtonGroup(this);
  m_toolButtonGroup->setExclusive(true);

  // 创建工具按钮
  m_rectButton = UIUtils::createToolbarButton({this, "geometry_24.svg", "矩形/椭圆"});
  m_toolButtonGroup->addButton(m_rectButton, static_cast<int>(ToolType::Rectangle));

  m_stepButton = UIUtils::createToolbarButton({this, "one_circle_24.svg", "步骤标注"});
  m_toolButtonGroup->addButton(m_stepButton, static_cast<int>(ToolType::Step));

  m_arrowButton = UIUtils::createToolbarButton({this, "arrows_24.svg", "绘制箭头"});
  m_toolButtonGroup->addButton(m_arrowButton, static_cast<int>(ToolType::Arrow));

  m_penButton = UIUtils::createToolbarButton({this, "edit_24.svg", "自由画笔"});
  m_toolButtonGroup->addButton(m_penButton, static_cast<int>(ToolType::Pen));

  m_textButton = UIUtils::createToolbarButton({this, "text_24.svg", "添加文字"});
  m_toolButtonGroup->addButton(m_textButton, static_cast<int>(ToolType::Text));

  m_mosaicButton = UIUtils::createToolbarButton({this, "mosaic_24.svg", "马赛克"});
  m_toolButtonGroup->addButton(m_mosaicButton, static_cast<int>(ToolType::Mosaic));

  m_markerButton = UIUtils::createToolbarButton({
      this,
      "highlight_24.svg",
      "区域高亮",
  });
  m_toolButtonGroup->addButton(m_markerButton, static_cast<int>(ToolType::Marker));

  // 连接按钮组信号
  connect(m_toolButtonGroup,
          &QButtonGroup::buttonClicked,
          this,
          &ScreenshotToolbar::onToolButtonClicked);
}

void ScreenshotToolbar::createFunctionButtons()
{
  // 创建功能按钮
  m_undoButton = UIUtils::createToolbarButton({
      this,
      "recall_24.svg",
      "撤销上一步",
  });
  connect(m_undoButton, &QPushButton::clicked, this, &ScreenshotToolbar::onUndoButtonClicked);

  m_pinButton = UIUtils::createToolbarButton({
      this,
      "pin_24.svg",
      "钉在桌面",
  });
  connect(m_pinButton, &QPushButton::toggled, this, &ScreenshotToolbar::onPinButtonClicked);

  m_saveButton = UIUtils::createToolbarButton({
      this,
      "download_24.svg",
      "保存图片",
  });
  connect(m_saveButton, &QPushButton::clicked, this, &ScreenshotToolbar::onSaveButtonClicked);

  m_okButton = UIUtils::createToolbarButton({
      this,
      "tick_24.svg",
      "复制到剪切板",
  });
  connect(m_okButton, &QPushButton::clicked, this, &ScreenshotToolbar::onOkButtonClicked);

  m_cancelButton = UIUtils::createToolbarButton({
      this,
      "close_24.svg",
      "取消",
  });
  connect(m_cancelButton, &QPushButton::clicked, this, &ScreenshotToolbar::onCancelButtonClicked);
}

void ScreenshotToolbar::setupLayout()
{
  // 创建分隔线
  m_separator1 = new QFrame(this);
  m_separator1->setFrameShape(QFrame::VLine);
  m_separator1->setFixedSize(1, 24);
  m_separator1->setStyleSheet(UIUtilsConstants::DEFAULT_SEPARATOR_STYLE);

  m_separator2 = new QFrame(this);
  m_separator2->setFrameShape(QFrame::VLine);
  m_separator2->setFixedSize(1, 24);
  m_separator2->setStyleSheet(UIUtilsConstants::DEFAULT_SEPARATOR_STYLE);

  // 设置主布局
  m_mainLayout = new QHBoxLayout(this);
  m_mainLayout->setContentsMargins(8, 4, 8, 4);
  m_mainLayout->setSpacing(4);

  // 添加工具按钮
  m_mainLayout->addWidget(m_rectButton);
  m_mainLayout->addWidget(m_stepButton);
  m_mainLayout->addWidget(m_arrowButton);
  m_mainLayout->addWidget(m_penButton);
  m_mainLayout->addWidget(m_textButton);
  m_mainLayout->addWidget(m_mosaicButton);
  m_mainLayout->addWidget(m_markerButton);

  // 添加分隔线1
  m_mainLayout->addWidget(m_separator1);

  // 添加功能按钮
  m_mainLayout->addWidget(m_undoButton);
  m_mainLayout->addWidget(m_pinButton);
  m_mainLayout->addWidget(m_saveButton);

  // 添加分隔线2
  m_mainLayout->addWidget(m_separator2);

  // 添加确定和取消按钮
  m_mainLayout->addWidget(m_cancelButton);
  m_mainLayout->addWidget(m_okButton);
}

void ScreenshotToolbar::showAt(const QPoint& position)
{
  move(position);
  show();
}

void ScreenshotToolbar::hideToolbar()
{
  hide();
}

ScreenshotToolbar::ToolType ScreenshotToolbar::getCurrentTool() const
{
  if (!m_toolButtonGroup)
  {
    return ToolType::Rectangle; // 默认工具
  }

  QAbstractButton* checkedButton = m_toolButtonGroup->checkedButton();
  if (checkedButton)
  {
    int buttonId = m_toolButtonGroup->id(checkedButton);
    return static_cast<ToolType>(buttonId);
  }

  return ToolType::Rectangle; // 默认工具
}

void ScreenshotToolbar::setUndoEnabled(bool enabled)
{
  if (m_undoButton)
  {
    m_undoButton->setEnabled(enabled);
  }
}

QIcon ScreenshotToolbar::createToolIcon(const QString& iconFileName)
{
  return IconProvider::createToolIcon(iconFileName);
}

QIcon ScreenshotToolbar::createWhiteSvgIcon(const QString& svgPath)
{
  return IconProvider::createWhiteSvgIcon(svgPath);
}

// 槽函数实现
void ScreenshotToolbar::onToolButtonClicked(QAbstractButton* button)
{
  int buttonId = m_toolButtonGroup->id(button);
  ToolType tool = static_cast<ToolType>(buttonId);
  m_isEditing = true;

  qDebug() << "工具栏：选择工具" << buttonId;
  emit toolSelected(tool);
}

void ScreenshotToolbar::onUndoButtonClicked()
{
  qDebug() << "工具栏：执行撤销操作";
  emit actionTriggered(ActionType::Undo);
}

void ScreenshotToolbar::onPinButtonClicked()
{
  qDebug() << "工具栏：钉在桌面";
  emit actionTriggered(ActionType::Pin);
}

void ScreenshotToolbar::onSaveButtonClicked()
{
  qDebug() << "工具栏：保存截图";
  emit actionTriggered(ActionType::Save);
}

void ScreenshotToolbar::onOkButtonClicked()
{
  qDebug() << "工具栏：确定";
  emit actionTriggered(ActionType::Ok);
}

void ScreenshotToolbar::onCancelButtonClicked()
{
  qDebug() << "工具栏：取消";
  emit actionTriggered(ActionType::Cancel);
}