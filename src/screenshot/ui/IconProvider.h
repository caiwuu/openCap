#ifndef ICONPROVIDER_H
#define ICONPROVIDER_H

#include <QIcon>
#include <QString>

// 图标提供器类 - 负责创建和管理图标
class IconProvider
{
public:
  // 静态方法
  static QIcon createToolIcon(const QString& iconFileName);
  static QIcon createWhiteSvgIcon(const QString& svgPath);

private:
  // 私有构造函数（静态类）
  IconProvider() = delete;

  // 私有辅助方法
  static bool fileExists(const QString& filePath);
};

#endif // ICONPROVIDER_H