#include "UIManager.h"

#include <QDebug>

// 默认样式定义
const QString UIManager::DEFAULT_INFO_STYLE = "QLabel {"
                                              "  background-color: rgba(125, 125, 125, 180);"
                                              "  color: white;"
                                              "  font-family: Arial;"
                                              "  font-size: 14px;"
                                              "  padding: 8px 20px;"
                                              "  border-radius: 4px;"
                                              "}";

const QString UIManager::DEFAULT_COORD_STYLE = "QLabel {"
                                               "  background-color: #292c33;"
                                               "  color: white;"
                                               "  font-family: Arial;"
                                               "  font-size: 10px;"
                                               "  padding: 8px 20px;"
                                               "  border-radius: 6px;"
                                               "}";

// 构造函数
UIManager::UIManager(QWidget* parent)
  : QObject(parent), m_parentWidget(parent), m_infoLabel(nullptr), m_coordLabel(nullptr)
{
  if (!m_parentWidget)
  {
    qWarning() << "UIManager: 父窗口不能为空";
  }
}

// 析构函数
UIManager::~UIManager()
{
  // QLabel会被Qt自动清理，因为它们是m_parentWidget的子对象
}

// 创建信息标签
void UIManager::createInfoLabels()
{
  if (!m_parentWidget)
  {
    qWarning() << "UIManager: 无法创建标签，父窗口为空";
    return;
  }

  // 创建顶部提示信息标签
  m_infoLabel = new QLabel(m_parentWidget);
  m_infoLabel->setText("拖拽选择截图区域，按 ESC 键取消");
  m_infoLabel->setStyleSheet(DEFAULT_INFO_STYLE);
  m_infoLabel->setAlignment(Qt::AlignCenter);
  m_infoLabel->adjustSize();
  m_infoLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

  // 创建坐标尺寸信息标签（初始隐藏）
  m_coordLabel = new QLabel(m_parentWidget);
  m_coordLabel->setStyleSheet(DEFAULT_COORD_STYLE);
  m_coordLabel->setAlignment(Qt::AlignCenter);
  m_coordLabel->hide(); // 初始隐藏

  qDebug() << "UI标签已创建";
}

// 更新坐标标签
void UIManager::updateCoordinateLabel(const QRect& selectionRect, int widgetWidth)
{
  if (!m_coordLabel || !selectionRect.isValid())
    return;

  QString coordText = QString("%1 × %2").arg(selectionRect.width()).arg(selectionRect.height());

  m_coordLabel->setText(coordText);
  m_coordLabel->adjustSize();

  // 计算坐标信息在选择框左上角的位置
  int coordX = selectionRect.x();
  int coordY = selectionRect.y() - MARGIN_TEXT_AND_BG;

  // 如果信息框会超出屏幕上边界，则显示在选择框内部左上角
  if (coordY - m_coordLabel->height() < 4)
  {
    coordY = selectionRect.y() + m_coordLabel->height();
  }

  // 确保信息框不会超出屏幕右边界
  if (coordX + m_coordLabel->width() > widgetWidth)
  {
    coordX = widgetWidth - m_coordLabel->width() - 4;
  }

  m_coordLabel->move(coordX, coordY - m_coordLabel->height());
}

// 显示坐标标签
void UIManager::showCoordinateLabel()
{
  if (m_coordLabel)
  {
    m_coordLabel->show();
  }
}

// 隐藏坐标标签
void UIManager::hideCoordinateLabel()
{
  if (m_coordLabel)
  {
    m_coordLabel->hide();
  }
}

// 显示信息提示
void UIManager::showInfoMessage(const QString& message)
{
  if (m_infoLabel)
  {
    m_infoLabel->setText(message);
    m_infoLabel->adjustSize();
    m_infoLabel->show();

    // 重新定位到屏幕顶部居中
    if (m_parentWidget)
    {
      positionInfoLabel(m_parentWidget->width());
    }
  }
}

// 隐藏信息提示
void UIManager::hideInfoMessage()
{
  if (m_infoLabel)
  {
    m_infoLabel->hide();
  }
}

// 更新信息提示
void UIManager::updateInfoMessage(const QString& message)
{
  if (m_infoLabel)
  {
    m_infoLabel->setText(message);
    m_infoLabel->adjustSize();

    // 重新定位
    if (m_parentWidget)
    {
      positionInfoLabel(m_parentWidget->width());
    }
  }
}

// 定位坐标标签
void UIManager::positionCoordinateLabel(const QRect& selectionRect,
                                        int widgetWidth,
                                        int widgetHeight)
{
  if (!m_coordLabel || !selectionRect.isValid())
    return;

  updateCoordinateLabel(selectionRect, widgetWidth);
}

// 定位信息标签
void UIManager::positionInfoLabel(int widgetWidth)
{
  if (!m_infoLabel)
    return;

  // 将标签移动到屏幕顶部居中位置
  int x = (widgetWidth - m_infoLabel->width()) / 2;
  int y = 40;
  m_infoLabel->move(x, y);
}

// 设置坐标标签样式
void UIManager::setCoordinateLabelStyle(const QString& style)
{
  if (m_coordLabel)
  {
    m_coordLabel->setStyleSheet(style);
    qDebug() << "坐标标签样式已更新";
  }
}

// 设置信息标签样式
void UIManager::setInfoLabelStyle(const QString& style)
{
  if (m_infoLabel)
  {
    m_infoLabel->setStyleSheet(style);
    qDebug() << "信息标签样式已更新";
  }
}