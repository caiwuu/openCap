#include <QDir>           // 包含Qt目录操作功能
#include <QScreen>        // 包含Qt屏幕相关功能
#include <QStandardPaths> // 包含Qt标准路径功能

#include <QtWidgets/QApplication> // 包含Qt应用程序框架

#include "screenshot/core/ScreenshotApp.h" // 包含截图应用程序头文件

// 应用程序主入口函数
int main(int argc, char* argv[])
{
  QApplication app(argc, argv); // 创建Qt应用程序实例，传入命令行参数

  // 设置应用程序信息
  app.setApplicationName("截图工具");   // 设置应用程序名称
  app.setApplicationVersion("1.0.0");   // 设置应用程序版本
  app.setOrganizationName("截图工具");  // 设置组织名称
  app.setQuitOnLastWindowClosed(false); // 不在最后一个窗口关闭时退出应用程序

  // 创建并启动截图应用
  ScreenshotApp screenshotApp; // 创建截图应用程序实例

  return app.exec(); // 启动Qt事件循环，程序在此处等待事件
}