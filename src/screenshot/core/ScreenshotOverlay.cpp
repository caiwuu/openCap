#include "ScreenshotOverlay.h" // 包含截图覆盖层头文件

// Qt核心头文件
#include <QApplication> // 包含Qt应用程序框架
#include <QClipboard>   // 包含Qt剪切板功能
#include <QCursor>      // 包含Qt光标功能
#include <QDateTime>    // 包含Qt日期时间功能
#include <QDebug>       // 包含Qt调试输出功能
#include <QEnterEvent>  // 包含Qt鼠标进入事件类
#include <QPaintEvent>  // 包含Qt绘制事件定义
#include <QPixmap>      // 包含Qt像素图类
#include <QPoint>       // 包含Qt点坐标类
#include <QRect>        // 包含Qt矩形类
#include <QScreen>      // 包含Qt屏幕相关功能
#include <QTimer>       // 包含Qt定时器功能
#include <QWidget>      // 包含Qt窗口部件基类
#include <QtGlobal>     // 包含Qt全局定义

// 项目头文件
#include "../managers/PerformanceManager.h"  // 包含性能管理器头文件
#include "../managers/ScreenshotProcessor.h" // 包含截图处理器头文件
#include "../managers/SelectionManager.h"    // 包含选择管理器头文件
#include "../managers/UIManager.h"           // 包含UI管理器头文件
#include "../managers/WindowLevelManager.h"  // 包含窗口层级管理器头文件
#include "../ui/ScreenshotRenderer.h"        // 包含截图渲染器头文件
#include "../ui/ScreenshotToolbar.h"         // 包含截图工具栏头文件
#include "managers/CursorManager.h"          // 包含光标管理器头文件
#include "managers/EventHandler.h"           // 包含事件处理器头文件

#ifdef Q_OS_MACOS    // 如果是macOS平台
  #include <QWindow> // 包含Qt窗口功能
#else
// 非macOS平台的空实现
typedef quintptr WId; // 定义WId类型
#endif

// 截图覆盖层构造函数，接收截图和父窗口参数
ScreenshotOverlay::ScreenshotOverlay(const QPixmap& screenshot, QWidget* parent)
  : QWidget(parent),                                // 调用QWidget基类构造函数
    m_screenshot(screenshot),                       // 保存传入的截图
    m_renderer(new ScreenshotRenderer(screenshot)), // 创建渲染器对象
    m_toolbar(nullptr),                             // 初始化工具栏为nullptr
    m_selectionManager(nullptr),                    // 初始化选择管理器为nullptr
    m_cursorManager(nullptr),                       // 初始化光标管理器为nullptr
    m_screenshotProcessor(nullptr),                 // 初始化截图处理器为nullptr
    m_performanceManager(nullptr),                  // 初始化性能管理器为nullptr
    m_windowManager(nullptr),                       // 初始化窗口管理器为nullptr
    m_uiManager(nullptr),                           // 初始化UI管理器为nullptr
    m_eventHandler(nullptr),                        // 初始化事件处理器为nullptr
    m_mousePos(QPoint(0, 0))                        // 初始化鼠标位置为(0,0)
{
  // 立即获取当前鼠标位置，避免从(0,0)闪烁
  QPoint globalMousePos = QCursor::pos(); // 获取全局鼠标位置
  m_mousePos = globalMousePos;            // 先用全局坐标，等窗口创建后再转换

  // 初始化所有管理器
  initializeManagers();

  // 设置窗口管理器
  m_windowManager->setupWindow();

  // 手动设置窗口几何覆盖整个屏幕，包括状态栏
  QScreen* screen = QApplication::primaryScreen(); // 获取主屏幕
  if (screen)                                      // 如果屏幕存在
  {
    // 获取完整屏幕区域（包含状态栏），但要考虑设备像素比
    QRect fullGeometry = screen->geometry();             // 获取屏幕完整几何信息
    qreal devicePixelRatio = screen->devicePixelRatio(); // 获取设备像素比

    qDebug() << "屏幕几何信息:";                           // 输出调试信息
    qDebug() << "  - screen->geometry():" << fullGeometry; // 输出屏幕几何信息
    qDebug() << "  - screen->availableGeometry():"
             << screen->availableGeometry();                 // 输出可用屏幕区域
    qDebug() << "  - devicePixelRatio:" << devicePixelRatio; // 输出设备像素比
    qDebug() << "  - 截图尺寸:" << screenshot.size();        // 输出截图尺寸

    // 窗口几何设置后，将全局鼠标坐标转换为本地坐标
    QPoint globalMousePos = QCursor::pos();     // 再次获取全局鼠标位置
    m_mousePos = mapFromGlobal(globalMousePos); // 转换为本地坐标
    qDebug() << "初始鼠标位置:" << m_mousePos;  // 输出初始鼠标位置
  }

  // 设置光标
  m_cursorManager->setDefaultCursor();

  // 创建UI组件
  createInfoLabels();
  createToolbar();

  // 设置信号连接
  setupConnections();

  // 启动窗口焦点监控
  m_windowManager->startFocusMonitoring();

  qDebug() << "截图覆盖窗口已创建"; // 输出创建成功信息
}

// 析构函数，清理资源
ScreenshotOverlay::~ScreenshotOverlay()
{
  // 清理管理器
  cleanupManagers();

  // 清理渲染器
  delete m_renderer; // 删除渲染器对象

  qDebug() << "截图覆盖窗口已销毁"; // 输出销毁信息
}

// 初始化管理器
void ScreenshotOverlay::initializeManagers()
{
  // 创建各个管理器
  m_selectionManager = new SelectionManager();
  m_cursorManager = new CursorManager(this);
  m_screenshotProcessor = new ScreenshotProcessor(m_screenshot, this);
  m_performanceManager = new PerformanceManager(this);
  m_windowManager = new WindowLevelManager(this, this);
  m_uiManager = new UIManager(this);
  m_eventHandler = new EventHandler(this);

  // 设置事件处理器的截图处理器
  m_eventHandler->setScreenshotProcessor(m_screenshotProcessor);

  qDebug() << "所有管理器已初始化";
}

// 设置信号连接
void ScreenshotOverlay::setupConnections()
{
  // 连接事件处理器信号
  connect(m_eventHandler,
          &EventHandler::screenshotFinished,
          this,
          [this](const QRect& region)
          {
            // 如果是确定按钮触发（region为空），先复制到剪切板
            if (region.isEmpty())
            {
              copyToClipboard();
            }
            // 然后发射完成信号
            emit screenshotFinished(region);
          });
  connect(m_eventHandler,
          &EventHandler::screenshotCancelled,
          this,
          &ScreenshotOverlay::screenshotCancelled);
  connect(m_eventHandler,
          &EventHandler::saveRequested,
          this,
          [this]()
          {
            QRect selectionRect = m_selectionManager->getSelectionRect();
            emit screenshotFinished(selectionRect);
          });

  // 连接截图处理器信号
  connect(m_screenshotProcessor,
          &ScreenshotProcessor::processingFinished,
          this,
          [this]()
          { QTimer::singleShot(0, this, [this]() { emit screenshotFinished(QRect()); }); });

  qDebug() << "信号连接已设置";
}

// 清理管理器
void ScreenshotOverlay::cleanupManagers()
{
  delete m_selectionManager;
  delete m_cursorManager;
  delete m_screenshotProcessor;
  delete m_performanceManager;
  delete m_windowManager;
  delete m_uiManager;
  delete m_eventHandler;

  // 设置为nullptr避免悬空指针
  m_selectionManager = nullptr;
  m_cursorManager = nullptr;
  m_screenshotProcessor = nullptr;
  m_performanceManager = nullptr;
  m_windowManager = nullptr;
  m_uiManager = nullptr;
  m_eventHandler = nullptr;
}

// 绘制事件处理函数
void ScreenshotOverlay::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event) // 标记未使用的参数

  QPainter painter(this);                        // 创建绘制器对象
  painter.setRenderHint(QPainter::Antialiasing); // 启用抗锯齿

  // 绘制背景截图
  m_renderer->drawBackground(painter); // 使用渲染器绘制背景

  // 绘制半透明遮罩
  QRect selectionRect = m_selectionManager->getSelectionRect();
  m_renderer->drawOverlay(painter, selectionRect, m_selectionManager->hasSelection());

  // 绘制选择框
  if (m_selectionManager->hasSelection() && !m_toolbar->m_isEditing)
  {
    m_renderer->drawSelectionBox(painter, selectionRect);

    // 如果选择完成，绘制调整锚点
    if (m_selectionManager->isSelectionFinished())
    {
      m_renderer->drawResizeHandles(painter, selectionRect);
    }
  }

  // 绘制信息文字
  drawInfo(painter);

  // 绘制放大镜（如果有有效的鼠标位置）
  if (m_mousePos.x() >= 0 && m_mousePos.y() >= 0 && !m_selectionManager->isSelectionFinished() &&
      !m_toolbar->m_isEditing)
  {
    m_renderer->drawMagnifier(painter, m_mousePos, width(), height());
  }
}

// 绘制信息文字
void ScreenshotOverlay::drawInfo(QPainter& painter)
{
  Q_UNUSED(painter) // 标记未使用的参数，因为现在使用QLabel

  // 更新坐标信息标签
  if (m_selectionManager->hasSelection())
  {
    QRect selectionRect = m_selectionManager->getSelectionRect();
    m_uiManager->updateCoordinateLabel(selectionRect, width());
    m_uiManager->showCoordinateLabel();
  }
  else
  {
    m_uiManager->hideCoordinateLabel();
  }
}

// 鼠标按下事件处理
void ScreenshotOverlay::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) // 如果是左键按下
  {
    if (m_toolbar->m_isEditing)
    {
      return;
    }

    if (m_selectionManager->isSelectionFinished()) // 如果选择已完成
    {
      // 检查是否点击了锚点
      SelectionManager::ResizeHandle handle = m_selectionManager->getResizeHandle(event->pos());
      if (handle != SelectionManager::ResizeHandle::None) // 如果点击了锚点
      {
        m_selectionManager->startResize(handle, event->pos());
        hideToolbar(); // 调整开始时隐藏工具栏
        return;
      }
      else if (m_selectionManager->isInsideSelection(event->pos())) // 如果点击了选择框内部
      {
        // 开始移动选择框
        m_selectionManager->startMove(event->pos());
        hideToolbar(); // 移动开始时隐藏工具栏
        return;
      }
    }
    else
    {
      // 开始新的选择
      m_selectionManager->startSelection(event->pos());
    }

    m_performanceManager->triggerOptimizedUpdate();
  }
}

// 鼠标移动事件处理
void ScreenshotOverlay::mouseMoveEvent(QMouseEvent* event)
{
  // 始终更新鼠标位置以便绘制放大镜
  QPoint newMousePos = event->pos();
  if (m_toolbar->m_isEditing)
  {
  }
  else if (m_selectionManager->isResizing()) // 如果正在调整大小
  {
    m_selectionManager->updateResize(event->pos());
    m_performanceManager->triggerOptimizedUpdate();
  }
  else if (m_selectionManager->isMoving()) // 如果正在移动选择框
  {
    m_selectionManager->updateMove(event->pos(), width(), height());
    m_performanceManager->triggerOptimizedUpdate();
  }
  else if (m_selectionManager->isSelecting()) // 如果正在选择
  {
    m_selectionManager->updateSelection(event->pos());
    m_performanceManager->triggerOptimizedUpdate();
  }
  else if (m_selectionManager->isSelectionFinished()) // 如果选择完成，更新光标
  {
    m_cursorManager->updateCursor(event->pos(), m_selectionManager);
    // 对于光标更新，不需要重绘界面
  }
  else
  {
    // 只有鼠标位置真正变化时才更新放大镜区域
    if (newMousePos != m_mousePos)
    {
      m_mousePos = newMousePos;
      m_performanceManager->updateMagnifierRegion();
      return;
    }
  }

  // 更新鼠标位置
  m_mousePos = newMousePos;
}

// 鼠标释放事件处理
void ScreenshotOverlay::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) // 如果是左键释放
  {
    if (m_selectionManager->isSelecting()) // 如果正在选择
    {
      m_selectionManager->finishSelection();

      if (m_selectionManager->isValidSelection())
      {
        m_cursorManager->setArrowCursor();
        showToolbar();
        m_performanceManager->invalidateMagnifierCache();
        qDebug() << "选择区域完成:" << m_selectionManager->getSelectionRect();
      }

      m_performanceManager->triggerOptimizedUpdate();
    }
    else if (m_selectionManager->isResizing()) // 如果正在调整大小
    {
      m_selectionManager->finishResize();
      m_cursorManager->updateCursor(event->pos(), m_selectionManager);
      showToolbar(); // 调整完成后更新工具栏位置
      m_performanceManager->triggerOptimizedUpdate();
    }
    else if (m_selectionManager->isMoving()) // 如果正在移动选择框
    {
      m_selectionManager->finishMove();
      m_cursorManager->updateCursor(event->pos(), m_selectionManager);
      showToolbar(); // 移动完成后更新工具栏位置
      m_performanceManager->triggerOptimizedUpdate();
    }
  }
}

// 键盘按键事件处理
void ScreenshotOverlay::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Escape) // 如果按下ESC键
  {
    qDebug() << "用户取消截图"; // 输出取消信息
    m_eventHandler->handleCancelButton();
    return;
  }
  else if (event->key() == Qt::Key_C &&
           !m_selectionManager->isSelectionFinished()) // 如果按下C键且未完成选择
  {
    // 复制当前鼠标位置的色值到剪切板
    if (m_renderer)
    {
      QColor pixelColor = m_renderer->getPixelColor(m_mousePos);
      QString hexColor = m_renderer->colorToHex(pixelColor);

      QClipboard* clipboard = QApplication::clipboard();
      clipboard->setText(hexColor);
    }
    return;
  }
  QWidget::keyPressEvent(event); // 调用基类事件处理
}

// 鼠标进入事件处理
void ScreenshotOverlay::enterEvent(QEnterEvent* event)
{
  // 确保鼠标进入窗口时位置正确
  QPoint newMousePos = event->position().toPoint();

  // 只有位置真正变化时才更新
  if (newMousePos != m_mousePos)
  {
    m_mousePos = newMousePos;
    m_performanceManager->updateMagnifierRegion();
  }

  QWidget::enterEvent(event); // 调用基类事件处理
}

// 鼠标离开事件处理
void ScreenshotOverlay::leaveEvent(QEvent* event)
{
  // 鼠标离开窗口时保持最后的位置，不隐藏放大镜
  QWidget::leaveEvent(event); // 调用基类事件处理
}

// 设置窗口层级
void ScreenshotOverlay::setWindowLevel()
{
  m_windowManager->setWindowLevel();
  m_performanceManager->forceFullRedraw();
}

// 显示工具栏
void ScreenshotOverlay::showToolbar()
{
  if (!m_selectionManager->hasSelection() || !m_toolbar)
    return;

  QRect selection = m_selectionManager->getSelectionRect();
  int toolbarWidth = m_toolbar->width();
  int toolbarHeight = m_toolbar->height();

  // 工具栏位置：选择框下方居中，留出一些间距
  int toolbarX = selection.center().x() - toolbarWidth / 2;
  int toolbarY = selection.bottom() + 15;

  // 确保工具栏不超出屏幕边界
  toolbarX = qMax(10, qMin(toolbarX, width() - toolbarWidth - 10));
  if (toolbarY + toolbarHeight > height() - 10)
  {
    toolbarY = selection.top() - toolbarHeight - 15; // 如果下方空间不够，放到上方
  }

  // 确保不会超出上边界
  toolbarY = qMax(10, toolbarY);

  m_toolbar->showAt(QPoint(toolbarX, toolbarY));
}

// 隐藏工具栏
void ScreenshotOverlay::hideToolbar()
{
  if (m_toolbar)
  {
    m_toolbar->hideToolbar();
  }
}

// 确定按钮点击槽函数
void ScreenshotOverlay::onOkButtonClicked()
{
  // 确定按钮功能：复制到剪切板
  copyToClipboard();
}

// 取消按钮点击槽函数
void ScreenshotOverlay::onCancelButtonClicked()
{
  m_eventHandler->handleCancelButton();
}

// 复制到剪切板
void ScreenshotOverlay::copyToClipboard()
{
  QRect selectionRect = m_selectionManager->getSelectionRect();
  m_screenshotProcessor->copyToClipboard(selectionRect);
}

// 保存到文件
void ScreenshotOverlay::saveToFile()
{
  QRect selectionRect = m_selectionManager->getSelectionRect();
  m_screenshotProcessor->saveToFile(selectionRect);
}

// 创建信息标签
void ScreenshotOverlay::createInfoLabels()
{
  m_uiManager->createInfoLabels();

  // 设置信息标签位置
  m_uiManager->positionInfoLabel(width());
}

// 创建工具栏
void ScreenshotOverlay::createToolbar()
{
  // 创建工具栏组件
  m_toolbar = new ScreenshotToolbar(this);

  // 连接工具栏信号到事件处理器
  connect(m_toolbar,
          &ScreenshotToolbar::toolSelected,
          m_eventHandler,
          &EventHandler::handleToolSelected);
  connect(m_toolbar,
          &ScreenshotToolbar::actionTriggered,
          m_eventHandler,
          &EventHandler::handleActionTriggered);
}

// 工具栏信号处理槽函数
void ScreenshotOverlay::onToolbarToolSelected(ScreenshotToolbar::ToolType toolType)
{
  m_eventHandler->handleToolSelected(toolType);
}

void ScreenshotOverlay::onToolbarActionTriggered(ScreenshotToolbar::ActionType actionType)
{
  m_eventHandler->handleActionTriggered(actionType);
}