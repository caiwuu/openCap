#include "UIUtils.h"

#include "../screenshot/ui/IconProvider.h"

namespace UIUtilsConstants
{
const QString DEFAULT_BUTTON_STYLE = R"(
QPushButton {
  background-color: transparent;
  color: white;
  font-size: 14px;
  border: none;
  border-radius: 4px;
  width: 32px;
  height: 32px;
  padding: 4px;
}
QPushButton:hover {
  background-color: rgba(255, 255, 255, 0.15);
  color: white;
}
QPushButton:pressed {
  background-color: rgba(255, 255, 255, 0.25);
  color: white;
}
QPushButton:checked {
  background-color: rgba(64, 158, 255, 0.8);
  color: white;
}
)";

const QString DEFAULT_SEPARATOR_STYLE = R"(
QFrame {
  background-color: rgba(255, 255, 255, 0.2);
  border: none;
}
)";

const QString DEFAULT_TOOLBAR_STYLE = R"(
ScreenshotToolbar {
  background-color: rgba(42, 42, 42, 240);
  border-radius: 8px;
  border: 1px solid rgba(255, 255, 255, 0.1);
}
)";
} // namespace UIUtilsConstants


QPushButton* UIUtils::createToolbarButton(ToolBarButtonParams params)
{
  auto& [parent, iconName, toolTip, checkable, buttonStyle] = params;
  QPushButton* button = new QPushButton(parent);
  button->setIcon(IconProvider::createToolIcon(iconName));
  button->setToolTip(toolTip);
  button->setStyleSheet(buttonStyle);
  button->setFixedSize(32, 32);

  if (checkable)
  {
    button->setCheckable(true);
  }

  return button;
}