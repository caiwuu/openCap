#ifndef SCREENSHOTOVERLAY_H // 防止头文件重复包含的宏定义开始
#define SCREENSHOTOVERLAY_H // 定义头文件标识符

#include <QWidget>       // 包含Qt窗口部件基类
#include <QPixmap>       // 包含Qt像素图类
#include <QPoint>        // 包含Qt点坐标类
#include <QRect>         // 包含Qt矩形类
#include <QPainter>      // 包含Qt绘制器类
#include <QMouseEvent>   // 包含Qt鼠标事件类
#include <QKeyEvent>     // 包含Qt键盘事件类
#include <QEnterEvent>   // 包含Qt鼠标进入事件类
#include <QColor>        // 包含Qt颜色类
#include <QTimer>        // 包含Qt定时器类
#include <QElapsedTimer> // 包含Qt高精度计时器类
#include <QLabel>        // 包含Qt标签类

// 前向声明
class ScreenshotRenderer;

// 包含工具栏头文件以使用枚举类型
#include "ScreenshotToolbar.h"

// 截图覆盖层类，继承自QWidget
class ScreenshotOverlay : public QWidget
{
    Q_OBJECT // Qt元对象系统宏，支持信号槽机制

        public :
        // 锚点调整相关枚举（移到public部分）
        enum class ResizeHandle {
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

    // 构造函数：创建截图覆盖层，接收截图和父窗口参数
    explicit ScreenshotOverlay(const QPixmap &screenshot, QWidget *parent = nullptr);
    ~ScreenshotOverlay();  // 析构函数：清理资源
    void setWindowLevel(); // 设置窗口层级为最高

signals:
    // 信号：截图完成，传递选中的区域
    void screenshotFinished(const QRect &region);
    // 信号：截图取消
    void screenshotCancelled();

private slots:
    // 工具栏信号处理槽函数
    void onToolbarToolSelected(ScreenshotToolbar::ToolType toolType);        // 工具选择槽函数
    void onToolbarActionTriggered(ScreenshotToolbar::ActionType actionType); // 功能按钮槽函数

    // 内部按钮处理函数
    void onOkButtonClicked();     // 确定按钮点击槽函数
    void onCancelButtonClicked(); // 取消按钮点击槽函数

    // 截图处理函数
    void copyToClipboard(); // 复制到剪切板
    void saveToFile();      // 保存到文件

protected:
    // 重写的事件处理函数
    void paintEvent(QPaintEvent *event) override;        // 绘制事件处理
    void mousePressEvent(QMouseEvent *event) override;   // 鼠标按下事件处理
    void mouseMoveEvent(QMouseEvent *event) override;    // 鼠标移动事件处理
    void mouseReleaseEvent(QMouseEvent *event) override; // 鼠标释放事件处理
    void keyPressEvent(QKeyEvent *event) override;       // 键盘按键事件处理
    void enterEvent(QEnterEvent *event) override;        // 鼠标进入事件处理
    void leaveEvent(QEvent *event) override;             // 鼠标离开事件处理

private:
    // 私有绘制函数
    void drawInfo(QPainter &painter); // 绘制信息文字

    // 私有工具函数
    QRect getSelectionRect() const;                        // 获取选择区域矩形
    void updateCursor();                                   // 更新光标样式
    void updateCursor(const QPoint &pos);                  // 根据鼠标位置更新光标样式
    void updateCursor(ResizeHandle handle);                // 根据锚点更新光标样式
    void ensureWindowActive();                             // 确保窗口保持活跃状态
    void createInfoLabels();                               // 创建信息标签
    void createToolbar();                                  // 创建工具栏
    ResizeHandle getResizeHandle(const QPoint &pos) const; // 获取鼠标位置对应的锚点
    QRect getHandleRect(ResizeHandle handle) const;        // 获取锚点区域
    void updateSelection(const QPoint &pos);               // 根据锚点调整选择区域
    bool isInsideSelection(const QPoint &pos) const;       // 检测鼠标是否在选择框内部
    void moveSelection(const QPoint &pos);                 // 移动选择区域
    void showToolbar();                                    // 显示工具栏
    void hideToolbar();                                    // 隐藏工具栏

    // 性能优化相关函数
    void updateMagnifierRegion();    // 更新放大镜区域（使用节流更新）
    void optimizedUpdate();          // 智能更新函数（自适应节流）
    void invalidateMagnifierCache(); // 清除放大镜缓存

    // 私有成员变量
    QPixmap m_screenshot;           // 存储全屏截图
    ScreenshotRenderer *m_renderer; // 渲染器对象

    // 选择相关变量
    bool m_isSelecting;         // 是否正在选择区域
    bool m_hasSelection;        // 是否已有选择区域
    bool m_isSelectionFinished; // 是否完成选择
    QPoint m_startPoint;        // 选择起始点
    QPoint m_currentPoint;      // 当前选择点
    QPoint m_mousePos;          // 当前鼠标位置

    // 调整大小相关变量
    ResizeHandle m_currentHandle; // 当前锚点
    bool m_isResizing;            // 是否正在调整大小
    QPoint m_resizeStartPos;      // 调整开始位置
    QRect m_resizeStartRect;      // 调整开始时的选择区域

    // 移动选择框相关变量
    bool m_isMoving;       // 是否正在移动选择框
    QPoint m_moveStartPos; // 移动开始位置
    QRect m_moveStartRect; // 移动开始时的选择区域

    // UI组件
    QTimer *m_focusTimer;         // 焦点检查定时器
    QLabel *m_infoLabel;          // 提示信息标签
    QLabel *m_coordLabel;         // 坐标信息标签
    ScreenshotToolbar *m_toolbar; // 工具栏组件

    // 性能优化缓存
    QPixmap m_cachedMagnifierPixmap; // 缓存的放大镜像素图
    QPoint m_cachedMagnifierPos;     // 缓存的放大镜位置
    QString m_cachedColorText;       // 缓存的颜色文字
    QPoint m_lastMousePos;           // 上次鼠标位置
    bool m_needsFullRedraw;          // 是否需要完整重绘
    QRect m_lastMagnifierRect;       // 上次放大镜区域
    QElapsedTimer m_updateTimer;     // 高性能更新计时器
    qint64 m_lastUpdateTime;         // 上次更新时间（使用实例变量避免静态变量问题）

    // 常量定义
    static constexpr int MIN_SELECTION_SIZE = 10; // 最小选择区域大小
    static constexpr int HANDLE_SIZE = 8;         // 锚点尺寸
    static constexpr int UPDATE_THROTTLE_MS = 8;  // 更新节流时间（约120FPS，更高响应性）
};

#endif // SCREENSHOTOVERLAY_H      // 防止头文件重复包含的宏定义结束