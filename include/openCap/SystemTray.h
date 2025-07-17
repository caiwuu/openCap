#ifndef SYSTEMTRAY_H // 防止头文件重复包含的宏定义开始
#define SYSTEMTRAY_H // 定义头文件标识符

#include <QSystemTrayIcon> // 包含Qt系统托盘图标类
#include <QMenu>           // 包含Qt菜单类
#include <QAction>         // 包含Qt动作类

// 系统托盘类，继承自QSystemTrayIcon
class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT // Qt元对象系统宏，支持信号槽机制

        public :
        // 构造函数：创建系统托盘，接收父对象参数
        explicit SystemTray(QObject *parent = nullptr);

signals:
    // 信号：请求截图
    void screenshotRequested();
    // 信号：请求退出应用程序
    void exitRequested();

private slots:
    // 私有槽函数：处理托盘图标激活事件
    void onTrayActivated(QSystemTrayIcon::ActivationReason reason);
    // 私有槽函数：处理截图动作触发
    void onScreenshotAction();
    // 私有槽函数：处理退出动作触发
    void onExitAction();

private:
    // 私有成员函数：创建托盘图标
    void createTrayIcon();
    // 私有成员函数：创建右键菜单
    void createMenu();

    // 私有成员变量
    QMenu *m_trayMenu;           // 托盘右键菜单
    QAction *m_screenshotAction; // 截图动作
    QAction *m_exitAction;       // 退出动作
};

#endif // SYSTEMTRAY_H      // 防止头文件重复包含的宏定义结束