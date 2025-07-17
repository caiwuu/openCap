#include "jietu/ScreenshotOverlay.h"  // 包含截图覆盖层头文件
#include "jietu/ScreenshotRenderer.h" // 包含截图渲染器头文件
#include "jietu/ScreenshotToolbar.h"  // 包含截图工具栏头文件
#include <QApplication>               // 包含Qt应用程序框架
#include <QScreen>                    // 包含Qt屏幕相关功能
#include <QClipboard>                 // 包含Qt剪切板功能
#include <QPaintEvent>                // 包含Qt绘制事件定义
#include <QFont>                      // 包含Qt字体功能
#include <QFontMetrics>               // 包含Qt字体度量功能
#include <QDebug>                     // 包含Qt调试输出功能
#include <QTimer>                     // 包含Qt定时器功能
#include <QDateTime>                  // 包含Qt日期时间功能

#ifdef Q_OS_MACOS                 // 如果是macOS平台
#include <QWindow>                // 包含Qt窗口功能
#include "jietu/MacWindowLevel.h" // 包含macOS窗口层级控制
#else
// 非macOS平台的空实现
class MacWindowLevel
{
public:
  static int getWindowLevel(WId) { return 0; }
  static void setWindowAboveStatusBar(WId) {}
  static void disableSystemGestures(WId) {}
};
#endif
constexpr int MARGIN_TEXT_AND_BG = 8; // 文字和背景的垂直内边距

// 截图覆盖层构造函数，接收截图和父窗口参数
ScreenshotOverlay::ScreenshotOverlay(const QPixmap &screenshot, QWidget *parent)
    : QWidget(parent),                                // 调用QWidget基类构造函数
      m_screenshot(screenshot),                       // 保存传入的截图
      m_renderer(new ScreenshotRenderer(screenshot)), // 创建渲染器对象
      m_isSelecting(false),                           // 初始化选择状态为false
      m_hasSelection(false),                          // 初始化是否有选择区域为false
      m_isSelectionFinished(false),                   // 初始化选择完成状态为false
      m_mousePos(QPoint(0, 0)),                       // 初始化鼠标位置为(0,0)
      m_currentHandle(ResizeHandle::None),            // 初始化当前锚点为无
      m_isResizing(false),                            // 初始化调整状态为false
      m_isMoving(false),                              // 初始化移动状态为false
      m_lastMousePos(QPoint(-1, -1)),                 // 初始化上次鼠标位置
      m_needsFullRedraw(true),                        // 初始需要完整重绘
      m_lastUpdateTime(0)                             // 初始化上次更新时间
{
  // 立即获取当前鼠标位置，避免从(0,0)闪烁
  QPoint globalMousePos = QCursor::pos(); // 获取全局鼠标位置
  m_mousePos = globalMousePos;            // 先用全局坐标，等窗口创建后再转换

  // 启动高精度计时器用于性能优化
  m_updateTimer.start(); // 启动计时器

  // 设置窗口属性 - 使用稳定的标志组合，避免Popup的异常行为
  setWindowFlags(Qt::FramelessWindowHint |     // 无边框窗口
                 Qt::WindowStaysOnTopHint |    // 窗口置顶
                 Qt::BypassWindowManagerHint | // 绕过窗口管理器
                 Qt::Tool);                    // 工具窗口类型
  setAttribute(Qt::WA_DeleteOnClose);          // 关闭时自动删除
  setAttribute(Qt::WA_NoSystemBackground);     // 不使用系统背景
  setAttribute(Qt::WA_OpaquePaintEvent);       // 不透明绘制事件

  // 手动设置窗口几何覆盖整个屏幕，包括状态栏
  QScreen *screen = QApplication::primaryScreen(); // 获取主屏幕
  if (screen)                                      // 如果屏幕存在
  {
    // 获取完整屏幕区域（包含状态栏），但要考虑设备像素比
    QRect fullGeometry = screen->geometry();             // 获取屏幕完整几何信息
    qreal devicePixelRatio = screen->devicePixelRatio(); // 获取设备像素比

    qDebug() << "屏幕几何信息:";                                                   // 输出调试信息
    qDebug() << "  - screen->geometry():" << fullGeometry;                         // 输出屏幕几何信息
    qDebug() << "  - screen->availableGeometry():" << screen->availableGeometry(); // 输出可用屏幕区域
    qDebug() << "  - devicePixelRatio:" << devicePixelRatio;                       // 输出设备像素比
    qDebug() << "  - 截图尺寸:" << screenshot.size();                              // 输出截图尺寸
    // 设置窗口覆盖完整屏幕，从(0,0)开始
    setGeometry(fullGeometry); // 设置窗口几何为完整屏幕

    qDebug() << "设置窗口几何:" << fullGeometry; // 输出设置的窗口几何

    // 窗口几何设置后，将全局鼠标坐标转换为本地坐标
    QPoint globalMousePos = QCursor::pos();     // 再次获取全局鼠标位置
    m_mousePos = mapFromGlobal(globalMousePos); // 转换为本地坐标
    qDebug() << "初始鼠标位置:" << m_mousePos;  // 输出初始鼠标位置
  }

  // 设置鼠标追踪和焦点
  setMouseTracking(true);          // 启用鼠标追踪
  setFocusPolicy(Qt::StrongFocus); // 设置强焦点策略

  // 确保窗口始终保持活跃状态
  setAttribute(Qt::WA_AlwaysShowToolTips); // 始终显示工具提示
  setAttribute(Qt::WA_MouseTracking);      // 启用鼠标追踪属性

  // 创建定时器来定期检查和恢复窗口状态
  m_focusTimer = new QTimer(this);                                                       // 创建定时器对象
  connect(m_focusTimer, &QTimer::timeout, this, &ScreenshotOverlay::ensureWindowActive); // 连接定时器超时信号
  m_focusTimer->start(250);                                                              // 每250ms检查一次（降低频率）

  // 设置窗口光标属性（虽然被隐藏，但需要用于事件处理）
  setCursor(Qt::ArrowCursor); // 设置十字光标

  // 创建信息标签和工具栏
  createInfoLabels();
  createToolbar();

  qDebug() << "截图覆盖窗口已创建，避免Popup窗口类型"; // 输出创建成功信息
}

// 析构函数，清理资源
ScreenshotOverlay::~ScreenshotOverlay()
{
  // 停止定时器
  if (m_focusTimer) // 如果定时器存在
  {
    m_focusTimer->stop(); // 停止定时器
  }

  // 清理渲染器
  delete m_renderer; // 删除渲染器对象

  // 恢复系统鼠标行为
  // #ifdef Q_OS_MACOS                               // 如果是macOS平台
  // MacWindowLevel::restoreSystemMouseBehavior(); // 恢复系统鼠标行为
  // #endif

  qDebug() << "截图覆盖窗口已销毁"; // 输出销毁信息
}

// 绘制事件处理函数
void ScreenshotOverlay::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event) // 标记未使用的参数

  QPainter painter(this);                        // 创建绘制器对象
  painter.setRenderHint(QPainter::Antialiasing); // 启用抗锯齿

  // 绘制背景截图
  m_renderer->drawBackground(painter); // 使用渲染器绘制背景

  // 绘制半透明遮罩
  QRect selectionRect = getSelectionRect();
  m_renderer->drawOverlay(painter, selectionRect, m_hasSelection); // 使用渲染器绘制遮罩

  // 绘制选择框
  if (m_hasSelection) // 如果有选择区域
  {
    m_renderer->drawSelectionBox(painter, selectionRect); // 使用渲染器绘制选择框

    // 如果选择完成，绘制调整锚点
    if (m_isSelectionFinished)
    {
      m_renderer->drawResizeHandles(painter, selectionRect); // 使用渲染器绘制调整锚点
    }
  }

  // 绘制信息文字
  drawInfo(painter); // 绘制信息文字

  // 绘制放大镜（如果有有效的鼠标位置）
  if (m_mousePos.x() >= 0 && m_mousePos.y() >= 0 && !m_isSelectionFinished) // 如果鼠标位置有效
  {
    m_renderer->drawMagnifier(painter, m_mousePos, width(), height()); // 使用渲染器绘制放大镜
  }
}

// 绘制信息文字
void ScreenshotOverlay::drawInfo(QPainter &painter)
{
  Q_UNUSED(painter) // 标记未使用的参数，因为现在使用QLabel

  // 更新坐标信息标签
  if (m_hasSelection) // 如果有选择区域
  {
    QRect selectionRect = getSelectionRect(); // 获取选择区域
    QString coordText = QString("%1 × %2")
                            .arg(selectionRect.width())   // 插入宽度
                            .arg(selectionRect.height()); // 插入高度

    m_coordLabel->setText(coordText);
    m_coordLabel->adjustSize();

    // 计算坐标信息在选择框左上角的位置
    int coordX = selectionRect.x();                      // X坐标对齐选择框左边
    int coordY = selectionRect.y() - MARGIN_TEXT_AND_BG; // Y坐标在选择框上方

    // 如果信息框会超出屏幕上边界，则显示在选择框内部左上角
    if (coordY - m_coordLabel->height() < 4)
    {
      coordY = selectionRect.y() + m_coordLabel->height(); // 显示在选择框内部顶部
    }

    // 确保信息框不会超出屏幕右边界
    if (coordX + m_coordLabel->width() > width())
    {
      coordX = width() - m_coordLabel->width() - 4; // 右对齐显示
    }

    m_coordLabel->move(coordX, coordY - m_coordLabel->height());
    m_coordLabel->show();
  }
  else
  {
    m_coordLabel->hide(); // 隐藏坐标标签
  }
}

// 鼠标按下事件处理
void ScreenshotOverlay::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) // 如果是左键按下
  {
    if (m_isSelectionFinished) // 如果选择已完成
    {
      // 检查是否点击了锚点
      ResizeHandle handle = getResizeHandle(event->pos());
      if (handle != ResizeHandle::None) // 如果点击了锚点
      {
        m_isResizing = true;                    // 开始调整大小
        m_currentHandle = handle;               // 记录当前锚点
        m_resizeStartPos = event->pos();        // 记录开始位置
        m_resizeStartRect = getSelectionRect(); // 记录开始选择区域
        hideToolbar();                          // 调整开始时隐藏工具栏
        return;                                 // 不执行后续逻辑
      }
      else if (isInsideSelection(event->pos())) // 如果点击了选择框内部
      {
        // 开始移动选择框
        m_isMoving = true;                    // 开始移动状态
        m_moveStartPos = event->pos();        // 记录移动开始位置
        m_moveStartRect = getSelectionRect(); // 记录移动开始时的选择区域
        hideToolbar();                        // 移动开始时隐藏工具栏
        return;                               // 不执行后续逻辑
      }
      else
      {
        // 点击了其他地方，重新开始选择
        m_isSelectionFinished = false; // 重置选择完成状态
        setCursor(Qt::ArrowCursor);    // 恢复十字光标
        hideToolbar();                 // 隐藏工具栏
      }
    }

    // 开始新的选择
    m_startPoint = event->pos();   // 记录起始点
    m_currentPoint = event->pos(); // 记录当前点
    m_isSelecting = true;          // 设置选择状态为true
    m_hasSelection = false;        // 重置选择区域状态
    optimizedUpdate();             // 使用优化的更新
  }
}

// 鼠标移动事件处理
void ScreenshotOverlay::mouseMoveEvent(QMouseEvent *event)
{
  // 始终更新鼠标位置以便绘制放大镜
  QPoint newMousePos = event->pos();

  if (m_isResizing) // 如果正在调整大小
  {
    updateSelection(event->pos()); // 根据锚点调整选择区域
    optimizedUpdate();             // 使用优化的更新
  }
  else if (m_isMoving) // 如果正在移动选择框
  {
    moveSelection(event->pos()); // 移动选择区域
    optimizedUpdate();           // 使用优化的更新
  }
  else if (m_isSelecting) // 如果正在选择
  {
    m_currentPoint = event->pos(); // 更新当前点
    m_hasSelection = true;         // 设置有选择区域
    optimizedUpdate();             // 使用优化的更新
  }
  else if (m_isSelectionFinished) // 如果选择完成，更新光标
  {
    updateCursor(event->pos()); // 根据鼠标位置更新光标
    // 对于光标更新，不需要重绘界面
  }
  else
  {
    // 只有鼠标位置真正变化时才更新放大镜区域
    if (newMousePos != m_lastMousePos)
    {
      m_mousePos = newMousePos;
      m_lastMousePos = newMousePos;
      updateMagnifierRegion(); // 更新放大镜区域
      return;                  // 提前返回，避免重复设置鼠标位置
    }
  }

  // 更新鼠标位置
  m_mousePos = newMousePos;
  m_lastMousePos = newMousePos;
}

// 鼠标释放事件处理
void ScreenshotOverlay::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) // 如果是左键释放
  {
    if (m_isSelecting) // 如果正在选择
    {
      m_isSelecting = false; // 结束选择状态

      QRect selectionRect = getSelectionRect(); // 获取选择区域

      // 检查选择区域是否足够大
      if (selectionRect.width() >= MIN_SELECTION_SIZE && // 宽度足够
          selectionRect.height() >= MIN_SELECTION_SIZE)  // 高度足够
      {
        m_isSelectionFinished = true;                 // 标记选择完成
        setCursor(Qt::ArrowCursor);                   // 设置默认箭头光标
        showToolbar();                                // 显示工具栏
        invalidateMagnifierCache();                   // 清除放大镜缓存
        qDebug() << "选择区域完成:" << selectionRect; // 输出选择区域信息
      }
      else // 选择区域太小
      {
        // 选择区域太小，重置
        m_hasSelection = false; // 重置选择区域状态
      }
      optimizedUpdate(); // 使用优化的更新
    }
    else if (m_isResizing) // 如果正在调整大小
    {
      m_isResizing = false;                 // 结束调整状态
      m_currentHandle = ResizeHandle::None; // 清除当前锚点
      updateCursor(event->pos());           // 根据当前位置更新光标
      showToolbar();                        // 调整完成后更新工具栏位置
      optimizedUpdate();                    // 使用优化的更新
    }
    else if (m_isMoving) // 如果正在移动选择框
    {
      m_isMoving = false;         // 结束移动状态
      updateCursor(event->pos()); // 根据当前位置更新光标
      showToolbar();              // 移动完成后更新工具栏位置
      optimizedUpdate();          // 使用优化的更新
    }
  }
}

// 键盘按键事件处理
void ScreenshotOverlay::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Escape) // 如果按下ESC键
  {
    qDebug() << "用户取消截图"; // 输出取消信息
    // 使用延迟信号发射，避免在事件处理过程中删除窗口
    QTimer::singleShot(0, this, [this]()                 // 延迟发射信号
                       { emit screenshotCancelled(); }); // 发射取消信号
    return;                                              // 不调用父类的事件处理
  }
  else if (event->key() == Qt::Key_C && !m_isSelectionFinished) // 如果按下C键且未完成选择
  {
    // 复制当前鼠标位置的色值到剪切板
    if (m_renderer)
    {
      QColor pixelColor = m_renderer->getPixelColor(m_mousePos);
      QString hexColor = m_renderer->colorToHex(pixelColor);

      QClipboard *clipboard = QApplication::clipboard();
      clipboard->setText(hexColor);

      qDebug() << "色值已复制到剪切板:" << hexColor;
    }
    return;
  }
  QWidget::keyPressEvent(event); // 调用基类事件处理
}

// 鼠标进入事件处理
void ScreenshotOverlay::enterEvent(QEnterEvent *event)
{
  // 确保鼠标进入窗口时位置正确
  QPoint newMousePos = event->position().toPoint(); // 更新鼠标位置

  // 注意：不再需要设置系统光标，使用自定义绘制

  // 只有位置真正变化时才更新
  if (newMousePos != m_mousePos)
  {
    m_mousePos = newMousePos;
    m_lastMousePos = newMousePos;
    updateMagnifierRegion(); // 只更新放大镜区域
  }

  QWidget::enterEvent(event); // 调用基类事件处理
}

// 鼠标离开事件处理
void ScreenshotOverlay::leaveEvent(QEvent *event)
{
  // 鼠标离开窗口时保持最后的位置，不隐藏放大镜
  // 这样在屏幕边缘时放大镜仍然可见
  QWidget::leaveEvent(event); // 调用基类事件处理
}

// 获取选择区域
QRect ScreenshotOverlay::getSelectionRect() const
{
  if (!m_hasSelection && !m_isSelecting) // 如果没有选择且不在选择中
  {
    return QRect(); // 返回空矩形
  }

  int x = qMin(m_startPoint.x(), m_currentPoint.x());       // 计算左上角X坐标
  int y = qMin(m_startPoint.y(), m_currentPoint.y());       // 计算左上角Y坐标
  int width = qAbs(m_currentPoint.x() - m_startPoint.x());  // 计算宽度
  int height = qAbs(m_currentPoint.y() - m_startPoint.y()); // 计算高度

  return QRect(x, y, width, height); // 返回选择矩形
}

// 更新光标
void ScreenshotOverlay::updateCursor()
{
  // 根据状态更新光标
  if (m_isSelecting) // 如果正在选择
  {
    setCursor(Qt::ArrowCursor); // 设置十字光标
  }
  else // 否则
  {
    setCursor(Qt::ArrowCursor); // 设置箭头光标
  }
}

// 根据鼠标位置更新光标
void ScreenshotOverlay::updateCursor(const QPoint &pos)
{
  if (!m_isSelectionFinished)
  {
    setCursor(Qt::ArrowCursor); // 选择时使用十字光标
    return;
  }

  // 检查是否在锚点上
  ResizeHandle handle = getResizeHandle(pos);
  if (handle != ResizeHandle::None)
  {
    updateCursor(handle); // 使用锚点对应的光标
    return;
  }

  // 检查是否在选择框内部
  if (isInsideSelection(pos))
  {
    setCursor(Qt::SizeAllCursor); // 在选择框内部使用移动光标
    return;
  }

  // 默认光标
  setCursor(Qt::ArrowCursor);
}

// 根据锚点更新光标
void ScreenshotOverlay::updateCursor(ResizeHandle handle)
{
  switch (handle)
  {
  case ResizeHandle::TopLeft:
  case ResizeHandle::BottomRight:
    setCursor(Qt::SizeFDiagCursor); // nwse-resize: 西北-东南调整
    break;
  case ResizeHandle::TopRight:
  case ResizeHandle::BottomLeft:
    setCursor(Qt::SizeBDiagCursor); // nesw-resize: 东北-西南调整
    break;
  case ResizeHandle::TopCenter:
  case ResizeHandle::BottomCenter:
    setCursor(Qt::SizeVerCursor); // ns-resize: 南北调整（垂直）
    break;
  case ResizeHandle::MiddleLeft:
  case ResizeHandle::MiddleRight:
    setCursor(Qt::SizeHorCursor); // ew-resize: 东西调整（水平）
    break;
  default:
    setCursor(Qt::ArrowCursor); // 默认箭头光标
    break;
  }
}

// 设置窗口层级
void ScreenshotOverlay::setWindowLevel()
{
  qDebug() << "提升窗口层级"; // 输出调试信息

  // 简单而稳定的窗口层级提升
  raise();          // 提升窗口
  activateWindow(); // 激活窗口
  setFocus();       // 设置焦点

  // 在macOS上使用原生API设置窗口层级高于状态栏
#ifdef Q_OS_MACOS                                // 如果是macOS平台
  if (windowHandle() && windowHandle()->winId()) // 如果窗口句柄存在
  {
    qDebug() << "使用原生API设置窗口层级高于状态栏"; // 输出调试信息

    // 首先获取当前窗口层级用于调试
    int currentLevel = MacWindowLevel::getWindowLevel(windowHandle()->winId()); // 获取当前窗口层级
    qDebug() << "当前窗口层级:" << currentLevel;                                // 输出当前层级

    // 设置窗口层级高于状态栏
    MacWindowLevel::setWindowAboveStatusBar(windowHandle()->winId()); // 设置窗口层级

    // 禁用系统手势（防止状态栏下拉等）
    MacWindowLevel::disableSystemGestures(windowHandle()->winId()); // 禁用系统手势

    // 验证设置结果
    int newLevel = MacWindowLevel::getWindowLevel(windowHandle()->winId()); // 获取新的窗口层级
    qDebug() << "新的窗口层级:" << newLevel;                                // 输出新层级
  }
  else // 如果窗口句柄不存在
  {
    qWarning() << "无法获取窗口句柄，回退到Qt方法"; // 输出警告信息
  }
#else // 如果不是macOS平台
  qDebug() << "非macOS平台，使用Qt标准方法"; // 输出调试信息
#endif

  qDebug() << "窗口状态:";                         // 输出窗口状态信息
  qDebug() << "  - 几何:" << geometry();           // 输出几何信息
  qDebug() << "  - 大小:" << size();               // 输出大小信息
  qDebug() << "  - 是否可见:" << isVisible();      // 输出可见性
  qDebug() << "  - 是否激活:" << isActiveWindow(); // 输出激活状态

  // 强制重绘
  m_needsFullRedraw = true;
  optimizedUpdate(); // 使用优化的更新
}

// 确保窗口保持活跃
void ScreenshotOverlay::ensureWindowActive()
{
  // 使用全局鼠标位置更新，即使窗口失去焦点也能跟踪鼠标
  QPoint globalPos = QCursor::pos();          // 获取全局鼠标位置
  QPoint localPos = mapFromGlobal(globalPos); // 转换为本地坐标

  // 如果鼠标位置变化了，更新并重绘
  if (localPos != m_mousePos) // 如果位置发生变化
  {
    m_mousePos = localPos;   // 更新鼠标位置
    updateMagnifierRegion(); // 只更新放大镜区域
  }

  // 检查窗口是否仍然是活动窗口
  if (!isActiveWindow() || !hasFocus()) // 如果窗口不活跃或失去焦点
  {
    qDebug() << "窗口失去焦点，正在恢复..."; // 输出调试信息

    // 重新激活窗口
    raise();          // 提升窗口
    activateWindow(); // 激活窗口
    setFocus();       // 设置焦点

    // 确保光标属性正确
    setCursor(Qt::ArrowCursor); // 设置十字光标

    // 在macOS上重新设置窗口层级
#ifdef Q_OS_MACOS                                  // 如果是macOS平台
    if (windowHandle() && windowHandle()->winId()) // 如果窗口句柄存在
    {
      MacWindowLevel::setWindowAboveStatusBar(windowHandle()->winId()); // 设置窗口层级
      MacWindowLevel::disableSystemGestures(windowHandle()->winId());   // 禁用系统手势
    }
#endif

    // 重绘窗口
    m_needsFullRedraw = true;
    optimizedUpdate(); // 使用优化的更新
  }
}

// 获取鼠标位置对应的锚点
ScreenshotOverlay::ResizeHandle ScreenshotOverlay::getResizeHandle(const QPoint &pos) const
{
  if (!m_hasSelection)
    return ResizeHandle::None;

  QRect selection = getSelectionRect();
  const int tolerance = HANDLE_SIZE / 2 + 2; // 增加一点容错

  // 检查各个锚点
  if (QRect(selection.left() - tolerance, selection.top() - tolerance,
            tolerance * 2, tolerance * 2)
          .contains(pos))
    return ResizeHandle::TopLeft;

  if (QRect(selection.center().x() - tolerance, selection.top() - tolerance,
            tolerance * 2, tolerance * 2)
          .contains(pos))
    return ResizeHandle::TopCenter;

  if (QRect(selection.right() - tolerance, selection.top() - tolerance,
            tolerance * 2, tolerance * 2)
          .contains(pos))
    return ResizeHandle::TopRight;

  if (QRect(selection.left() - tolerance, selection.center().y() - tolerance,
            tolerance * 2, tolerance * 2)
          .contains(pos))
    return ResizeHandle::MiddleLeft;

  if (QRect(selection.right() - tolerance, selection.center().y() - tolerance,
            tolerance * 2, tolerance * 2)
          .contains(pos))
    return ResizeHandle::MiddleRight;

  if (QRect(selection.left() - tolerance, selection.bottom() - tolerance,
            tolerance * 2, tolerance * 2)
          .contains(pos))
    return ResizeHandle::BottomLeft;

  if (QRect(selection.center().x() - tolerance, selection.bottom() - tolerance,
            tolerance * 2, tolerance * 2)
          .contains(pos))
    return ResizeHandle::BottomCenter;

  if (QRect(selection.right() - tolerance, selection.bottom() - tolerance,
            tolerance * 2, tolerance * 2)
          .contains(pos))
    return ResizeHandle::BottomRight;

  return ResizeHandle::None;
}

// 根据锚点调整选择区域
void ScreenshotOverlay::updateSelection(const QPoint &pos)
{
  if (!m_isResizing || m_currentHandle == ResizeHandle::None)
    return;

  QRect newRect = m_resizeStartRect;
  QPoint delta = pos - m_resizeStartPos;

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
    // 移除频繁的工具栏更新，在移动结束时才更新
  }
}

// 检测鼠标是否在选择框内部（但不在锚点上）
bool ScreenshotOverlay::isInsideSelection(const QPoint &pos) const
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

// 移动选择区域
void ScreenshotOverlay::moveSelection(const QPoint &pos)
{
  if (!m_isMoving)
    return;

  QPoint delta = pos - m_moveStartPos;
  QRect newRect = m_moveStartRect;
  newRect.translate(delta);

  // 确保选择区域不会超出屏幕边界
  if (newRect.left() < 0)
    newRect.moveLeft(0);
  if (newRect.top() < 0)
    newRect.moveTop(0);
  if (newRect.right() >= width())
    newRect.moveRight(width() - 1);
  if (newRect.bottom() >= height())
    newRect.moveBottom(height() - 1);

  m_startPoint = newRect.topLeft();
  m_currentPoint = newRect.bottomRight();
  // 移除频繁的工具栏更新，在移动结束时才更新
}

// 显示工具栏
void ScreenshotOverlay::showToolbar()
{
  if (!m_hasSelection || !m_toolbar)
    return;

  QRect selection = getSelectionRect();
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
  qDebug() << "用户取消截图";

  // 发射取消信号
  QTimer::singleShot(0, this, [this]()
                     { emit screenshotCancelled(); });
}

// 复制到剪切板
void ScreenshotOverlay::copyToClipboard()
{
  QRect selectionRect = getSelectionRect();
  qDebug() << "复制截图到剪切板，逻辑区域:" << selectionRect;

  if (selectionRect.isValid() && !selectionRect.isEmpty())
  {
    // 获取设备像素比
    QScreen *screen = QApplication::primaryScreen();
    qreal devicePixelRatio = screen ? screen->devicePixelRatio() : 1.0;

    qDebug() << "设备像素比:" << devicePixelRatio;
    qDebug() << "原始截图尺寸:" << m_screenshot.size();

    // 如果设备像素比不是1，需要调整选择区域坐标
    QRect actualRect = selectionRect;
    if (devicePixelRatio != 1.0)
    {
      // 将逻辑坐标转换为物理像素坐标
      actualRect = QRect(
          static_cast<int>(selectionRect.x() * devicePixelRatio),
          static_cast<int>(selectionRect.y() * devicePixelRatio),
          static_cast<int>(selectionRect.width() * devicePixelRatio),
          static_cast<int>(selectionRect.height() * devicePixelRatio));
      qDebug() << "调整后的物理像素区域:" << actualRect;
    }

    // 确保裁剪区域不超出截图边界
    QRect screenshotRect = m_screenshot.rect();
    actualRect = actualRect.intersected(screenshotRect);

    qDebug() << "最终裁剪区域:" << actualRect;

    // 从原始截图中裁剪选择区域
    QPixmap croppedPixmap = m_screenshot.copy(actualRect);

    // 获取系统剪切板
    QClipboard *clipboard = QApplication::clipboard();

    // 将裁剪后的图片保存到剪切板
    clipboard->setPixmap(croppedPixmap);

    qDebug() << "截图已保存到剪切板，最终尺寸:" << croppedPixmap.size();

    // 发射截图完成信号（传递空区域，因为已经处理完成）
    QTimer::singleShot(0, this, [this]()
                       { emit screenshotFinished(QRect()); });
  }
  else
  {
    qWarning() << "无效的选择区域，无法复制到剪切板";
  }
}

// 保存到文件
void ScreenshotOverlay::saveToFile()
{
  QRect selectionRect = getSelectionRect();
  qDebug() << "保存截图到文件，区域:" << selectionRect;

  // 发射截图完成信号，让上层处理保存逻辑
  QTimer::singleShot(0, this, [this, selectionRect]()
                     { emit screenshotFinished(selectionRect); });
}

// 创建信息标签
void ScreenshotOverlay::createInfoLabels()
{
  // 创建顶部提示信息标签
  m_infoLabel = new QLabel(this);
  m_infoLabel->setText("拖拽选择截图区域，按 ESC 键取消");
  m_infoLabel->setStyleSheet(
      "QLabel {"
      "  background-color: rgba(125, 125, 125, 180);"
      "  color: white;"
      "  font-family: Arial;"
      "  font-size: 14px;"
      "  padding: 8px 20px;"
      "  border-radius: 4px;"
      "}");
  m_infoLabel->setAlignment(Qt::AlignCenter);
  m_infoLabel->adjustSize();
  m_infoLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

  // 将标签移动到屏幕顶部居中位置
  int x = (width() - m_infoLabel->width()) / 2;
  int y = 30;
  m_infoLabel->move(x, y);
  m_infoLabel->show();

  // 创建坐标尺寸信息标签（初始隐藏）
  m_coordLabel = new QLabel(this);
  m_coordLabel->setStyleSheet(
      "QLabel {"
      "  background-color: #292c33;"
      "  color: white;"
      "  font-family: Arial;"
      "  font-size: 10px;"
      "  padding: 8px 20px;"
      "  border-radius: 6px;"
      "}");
  m_coordLabel->setAlignment(Qt::AlignCenter);
  m_coordLabel->hide(); // 初始隐藏
}

// 创建工具栏
void ScreenshotOverlay::createToolbar()
{
  // 创建工具栏组件
  m_toolbar = new ScreenshotToolbar(this);

  // 连接工具栏信号到槽函数
  connect(m_toolbar, &ScreenshotToolbar::toolSelected,
          this, &ScreenshotOverlay::onToolbarToolSelected);
  connect(m_toolbar, &ScreenshotToolbar::actionTriggered,
          this, &ScreenshotOverlay::onToolbarActionTriggered);
}

// 性能优化：更新放大镜区域（使用节流的全屏更新）
void ScreenshotOverlay::updateMagnifierRegion()
{
  if (m_isSelectionFinished)
    return; // 选择完成后不显示放大镜

  // 使用优化的更新函数，它包含节流机制
  optimizedUpdate();
}

// 性能优化：智能更新函数
void ScreenshotOverlay::optimizedUpdate()
{
  // 使用高精度计时器，性能更优且不受系统时间影响
  qint64 elapsed = m_updateTimer.elapsed();

  int throttleTime = 16;

  // 检查自上次更新以来是否已过足够时间（使用实例变量避免静态变量问题）
  if (elapsed - m_lastUpdateTime < throttleTime)
  {
    return;
  }

  m_lastUpdateTime = elapsed;
  update(); // 触发重绘
}

// 性能优化：清除放大镜缓存
void ScreenshotOverlay::invalidateMagnifierCache()
{
  m_cachedMagnifierPixmap = QPixmap();
  m_cachedMagnifierPos = QPoint(-1, -1);
  m_cachedColorText.clear();

  // 同时清除渲染器缓存
  if (m_renderer)
  {
    m_renderer->clearCache();
  }
}

// 工具栏信号处理槽函数
void ScreenshotOverlay::onToolbarToolSelected(ScreenshotToolbar::ToolType toolType)
{
  // 根据工具类型执行相应操作
  switch (toolType)
  {
  case ScreenshotToolbar::ToolType::Rectangle:
    qDebug() << "选择矩形工具";
    // TODO: 实现矩形绘制功能
    break;
  case ScreenshotToolbar::ToolType::Step:
    qDebug() << "选择步骤标注工具";
    // TODO: 实现步骤标注功能
    break;
  case ScreenshotToolbar::ToolType::Arrow:
    qDebug() << "选择箭头工具";
    // TODO: 实现箭头绘制功能
    break;
  case ScreenshotToolbar::ToolType::Pen:
    qDebug() << "选择画笔工具";
    // TODO: 实现画笔绘制功能
    break;
  case ScreenshotToolbar::ToolType::Text:
    qDebug() << "选择文字工具";
    // TODO: 实现文字添加功能
    break;
  case ScreenshotToolbar::ToolType::Mosaic:
    qDebug() << "选择马赛克工具";
    // TODO: 实现马赛克功能
    break;
  case ScreenshotToolbar::ToolType::Marker:
    qDebug() << "选择区域高亮工具";
    // TODO: 实现区域高亮功能
    break;
  }
}

void ScreenshotOverlay::onToolbarActionTriggered(ScreenshotToolbar::ActionType actionType)
{
  // 根据功能类型执行相应操作
  switch (actionType)
  {
  case ScreenshotToolbar::ActionType::Undo:
    qDebug() << "执行撤销操作";
    // TODO: 实现撤销功能
    break;
  case ScreenshotToolbar::ActionType::Pin:
    qDebug() << "钉在桌面";
    // TODO: 实现钉在桌面功能
    break;
  case ScreenshotToolbar::ActionType::Save:
    qDebug() << "保存截图到文件";
    saveToFile();
    break;
  case ScreenshotToolbar::ActionType::Ok:
    onOkButtonClicked();
    break;
  case ScreenshotToolbar::ActionType::Cancel:
    onCancelButtonClicked();
    break;
  }
}
