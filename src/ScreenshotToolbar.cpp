#include "openCap/ScreenshotToolbar.h"

#include <QDebug>
#include <QFileInfo>
#include <QPainter>
#include <QSvgRenderer>

// 样式常量定义
const QString ScreenshotToolbar::BUTTON_STYLE = "QPushButton {"
                                                "  background-color: transparent;"
                                                "  color: white;"
                                                "  font-size: 14px;"
                                                "  border: none;"
                                                "  border-radius: 4px;"
                                                "  width: 32px;"
                                                "  height: 32px;"
                                                "  padding: 4px;"
                                                "}"
                                                "QPushButton:hover {"
                                                "  background-color: rgba(255, 255, 255, 0.15);"
                                                "  color: white;"
                                                "}"
                                                "QPushButton:pressed {"
                                                "  background-color: rgba(255, 255, 255, 0.25);"
                                                "  color: white;"
                                                "}"
                                                "QPushButton:checked {"
                                                "  background-color: rgba(64, 158, 255, 0.8);"
                                                "  color: white;"
                                                "}";

const QString ScreenshotToolbar::SEPARATOR_STYLE = "QFrame {"
                                                   "  background-color: rgba(255, 255, 255, 0.2);"
                                                   "  border: none;"
                                                   "}";

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
    m_separator1(nullptr),
    m_separator2(nullptr)
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
  setStyleSheet("ScreenshotToolbar {"
                "  background-color: rgba(42, 42, 42, 240);"
                "  border-radius: 8px;"
                "  border: 1px solid rgba(255, 255, 255, 0.1);"
                "}");

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
  m_rectButton = new QPushButton(this);
  m_rectButton->setIcon(createToolIcon("geometry_24.svg"));
  m_rectButton->setToolTip("矩形/椭圆");
  m_rectButton->setCheckable(true);
  m_rectButton->setStyleSheet(BUTTON_STYLE);
  m_rectButton->setFixedSize(32, 32);
  m_toolButtonGroup->addButton(m_rectButton, static_cast<int>(ToolType::Rectangle));

  m_stepButton = new QPushButton(this);
  m_stepButton->setIcon(createToolIcon("one_circle_24.svg"));
  m_stepButton->setToolTip("步骤标注");
  m_stepButton->setCheckable(true);
  m_stepButton->setStyleSheet(BUTTON_STYLE);
  m_stepButton->setFixedSize(32, 32);
  m_toolButtonGroup->addButton(m_stepButton, static_cast<int>(ToolType::Step));

  m_arrowButton = new QPushButton(this);
  m_arrowButton->setIcon(createToolIcon("arrows_24.svg"));
  m_arrowButton->setToolTip("绘制箭头");
  m_arrowButton->setCheckable(true);
  m_arrowButton->setStyleSheet(BUTTON_STYLE);
  m_arrowButton->setFixedSize(32, 32);
  m_toolButtonGroup->addButton(m_arrowButton, static_cast<int>(ToolType::Arrow));

  m_penButton = new QPushButton(this);
  m_penButton->setIcon(createToolIcon("edit_24.svg"));
  m_penButton->setToolTip("自由画笔");
  m_penButton->setCheckable(true);
  m_penButton->setStyleSheet(BUTTON_STYLE);
  m_penButton->setFixedSize(32, 32);
  m_toolButtonGroup->addButton(m_penButton, static_cast<int>(ToolType::Pen));

  m_textButton = new QPushButton(this);
  m_textButton->setIcon(createToolIcon("text_24.svg"));
  m_textButton->setToolTip("添加文字");
  m_textButton->setCheckable(true);
  m_textButton->setStyleSheet(BUTTON_STYLE);
  m_textButton->setFixedSize(32, 32);
  m_toolButtonGroup->addButton(m_textButton, static_cast<int>(ToolType::Text));

  m_mosaicButton = new QPushButton(this);
  m_mosaicButton->setIcon(createToolIcon("mosaic_24.svg"));
  m_mosaicButton->setToolTip("马赛克");
  m_mosaicButton->setCheckable(true);
  m_mosaicButton->setStyleSheet(BUTTON_STYLE);
  m_mosaicButton->setFixedSize(32, 32);
  m_toolButtonGroup->addButton(m_mosaicButton, static_cast<int>(ToolType::Mosaic));

  m_markerButton = new QPushButton(this);
  m_markerButton->setIcon(createToolIcon("highlight_24.svg"));
  m_markerButton->setToolTip("区域高亮");
  m_markerButton->setCheckable(true);
  m_markerButton->setStyleSheet(BUTTON_STYLE);
  m_markerButton->setFixedSize(32, 32);
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
  m_undoButton = new QPushButton(this);
  m_undoButton->setIcon(createToolIcon("recall_24.svg"));
  m_undoButton->setToolTip("撤销上一步");
  m_undoButton->setStyleSheet(BUTTON_STYLE);
  m_undoButton->setFixedSize(32, 32);
  connect(m_undoButton, &QPushButton::clicked, this, &ScreenshotToolbar::onUndoButtonClicked);

  m_pinButton = new QPushButton(this);
  m_pinButton->setIcon(createToolIcon("pin_24.svg"));
  m_pinButton->setToolTip("钉在桌面");
  m_pinButton->setStyleSheet(BUTTON_STYLE);
  m_pinButton->setFixedSize(32, 32);
  connect(m_pinButton, &QPushButton::clicked, this, &ScreenshotToolbar::onPinButtonClicked);

  m_saveButton = new QPushButton(this);
  m_saveButton->setIcon(createToolIcon("download_24.svg"));
  m_saveButton->setToolTip("保存图片");
  m_saveButton->setStyleSheet(BUTTON_STYLE);
  m_saveButton->setFixedSize(32, 32);
  connect(m_saveButton, &QPushButton::clicked, this, &ScreenshotToolbar::onSaveButtonClicked);

  m_okButton = new QPushButton(this);
  m_okButton->setIcon(createToolIcon("tick_24.svg"));
  m_okButton->setToolTip("复制到剪切板");
  m_okButton->setStyleSheet(BUTTON_STYLE);
  m_okButton->setFixedSize(32, 32);
  connect(m_okButton, &QPushButton::clicked, this, &ScreenshotToolbar::onOkButtonClicked);

  m_cancelButton = new QPushButton(this);
  m_cancelButton->setIcon(createToolIcon("close_24.svg"));
  m_cancelButton->setToolTip("取消");
  m_cancelButton->setStyleSheet(BUTTON_STYLE);
  m_cancelButton->setFixedSize(32, 32);
  connect(m_cancelButton, &QPushButton::clicked, this, &ScreenshotToolbar::onCancelButtonClicked);
}

void ScreenshotToolbar::setupLayout()
{
  // 创建分隔线
  m_separator1 = new QFrame(this);
  m_separator1->setFrameShape(QFrame::VLine);
  m_separator1->setFixedSize(1, 24);
  m_separator1->setStyleSheet(SEPARATOR_STYLE);

  m_separator2 = new QFrame(this);
  m_separator2->setFrameShape(QFrame::VLine);
  m_separator2->setFixedSize(1, 24);
  m_separator2->setStyleSheet(SEPARATOR_STYLE);

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
  // 构建图标文件路径
  QString iconPath = QString("icons/%1").arg(iconFileName);

  // 检查SVG文件是否存在
  if (QFileInfo::exists(iconPath))
  {
    return createWhiteSvgIcon(iconPath);
  }
  return QIcon();
}

QIcon ScreenshotToolbar::createWhiteSvgIcon(const QString& svgPath)
{
  // 使用QPainter着色模式（最安全，保持原始结构）
  QSvgRenderer svgRenderer(svgPath);
  if (!svgRenderer.isValid())
  {
    return QIcon();
  }

  // 渲染为不同尺寸的图标（支持高DPI）
  QIcon icon;
  for (int size : {16, 24, 32, 48})
  {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // 先正常渲染SVG
    svgRenderer.render(&painter);

    // 使用CompositionMode将图标着色为白色，保持形状不变
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), Qt::white);

    icon.addPixmap(pixmap);
  }

  return icon;
}

// 槽函数实现
void ScreenshotToolbar::onToolButtonClicked(QAbstractButton* button)
{
  int buttonId = m_toolButtonGroup->id(button);
  ToolType tool = static_cast<ToolType>(buttonId);

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