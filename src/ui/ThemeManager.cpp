#include "ThemeManager.h"
#include <QStyleFactory>
#include <QFile>

namespace LadderDiagram {

ThemeManager& ThemeManager::instance() {
    static ThemeManager instance;
    return instance;
}

void ThemeManager::init() {
    m_settings = new QSettings("LadderDiagram", "Theme", this);
    
    // 从配置文件加载主题，默认浅色
    QString savedTheme = m_settings->value("theme", "light").toString();
    if (savedTheme == "dark") {
        m_currentTheme = ThemeType::Dark;
    } else {
        m_currentTheme = ThemeType::Light;
    }
}

void ThemeManager::setTheme(ThemeType type) {
    m_currentTheme = type;
    saveSettings();
}

void ThemeManager::toggleTheme() {
    if (m_currentTheme == ThemeType::Light) {
        m_currentTheme = ThemeType::Dark;
    } else {
        m_currentTheme = ThemeType::Light;
    }
    saveSettings();
}

ThemeColors ThemeManager::getCurrentColors() const {
    if (m_currentTheme == ThemeType::Dark) {
        return getDarkColors();
    }
    return getLightColors();
}

QString ThemeManager::getStyleSheet() const {
    return generateStyleSheet(getCurrentColors());
}

void ThemeManager::applyTheme(QApplication* app) {
    if (!app) return;
    app->setStyleSheet(getStyleSheet());
}

void ThemeManager::applyToWidget(QWidget* widget) {
    if (!widget) return;
    widget->setStyleSheet(getStyleSheet());
}

void ThemeManager::saveSettings() {
    if (!m_settings) return;
    
    if (m_currentTheme == ThemeType::Dark) {
        m_settings->setValue("theme", "dark");
    } else {
        m_settings->setValue("theme", "light");
    }
    m_settings->sync();
}

ThemeColors ThemeManager::getLightColors() const {
    ThemeColors colors;
    
    // 浅色主题 - 工业软件风格
    // 使用蓝色系作为主色调，统一协调
    colors.backgroundDark = "#F0F2F5";      // 主背景 - 淡蓝灰
    colors.backgroundMid = "#FFFFFF";       // 次背景 - 纯白
    colors.backgroundLight = "#F8F9FA";     // 面板背景
    
    colors.accentBlue = "#1890FF";          // 强调蓝 - Ant Design蓝
    colors.accentBlueDark = "#096DD9";      // 深蓝
    colors.accentBlueLight = "#40A9FF";     // 亮蓝
    
    colors.textPrimary = "#000000";         // 主文字 - 纯黑
    colors.textSecondary = "#000000";       // 次文字 - 纯黑
    colors.textDisabled = "#000000";        // 禁用文字 - 纯黑
    
    colors.borderDark = "#D9D9D9";          // 边框
    colors.borderMid = "#E8E8E8";
    colors.borderLight = "#F0F0F0";
    
    colors.hoverBg = "#E6F7FF";             // 悬停 - 淡蓝
    colors.selectedBg = "#1890FF";          // 选中 - 蓝色
    colors.pressedBg = "#096DD9";           // 按下
    
    colors.success = "#52C41A";             // 成功绿
    colors.warning = "#FAAD14";             // 警告黄
    colors.error = "#F5222D";               // 错误红
    
    return colors;
}

ThemeColors ThemeManager::getDarkColors() const {
    ThemeColors colors;
    
    // 暗色主题 - 深色工业风格
    colors.backgroundDark = "#1F1F1F";      // 主背景 - 深黑
    colors.backgroundMid = "#2A2A2A";       // 次背景 - 中黑
    colors.backgroundLight = "#3A3A3A";     // 面板背景
    
    colors.accentBlue = "#177DDC";          // 强调蓝
    colors.accentBlueDark = "#0958D9";      // 深蓝
    colors.accentBlueLight = "#4FAFF5";     // 亮蓝
    
    colors.textPrimary = "#E0E0E0";         // 主文字 - 浅灰
    colors.textSecondary = "#A6A6A6";       // 次文字
    colors.textDisabled = "#595959";        // 禁用文字
    
    colors.borderDark = "#434343";          // 边框
    colors.borderMid = "#3A3A3A";
    colors.borderLight = "#2A2A2A";
    
    colors.hoverBg = "#2C2C2C";             // 悬停
    colors.selectedBg = "#177DDC";          // 选中
    colors.pressedBg = "#0958D9";           // 按下
    
    colors.success = "#49AA19";
    colors.warning = "#D89614";
    colors.error = "#D84A4A";
    
    return colors;
}

QString ThemeManager::generateStyleSheet(const ThemeColors& colors) const {
    // 使用颜色占位符
    QString style = QString(R"(
        /* ===== 全局变量定义 ===== */
        * {
            font-family: "Microsoft YaHei", "SimHei", "Segoe UI", sans-serif;
            font-size: 13px;
            outline: none;
        }
        
        /* ===== 主窗口 ===== */
        QMainWindow {
            background-color: %1;
            color: #000000;
            border: none;
        }
        
        /* ===== 中央部件 ===== */
        QWidget {
            background-color: %1;
            color: #000000;
        }
        
        /* ===== Ribbon 容器 - 一体化设计 ===== */
        #ribbonContainer {
            background-color: %2;
            border: none;
        }
        
        /* Tab标签栏 - Excel绿色风格 */
        #tabBarFrame {
            background-color: #217346;
            border: none;
        }
        
        /* 内容面板 - 使用浅灰背景与Tab栏协调 */
        #tabStack {
            background-color: #FAFAFA;
            border: none;
            border-top: 1px solid #E0E0E0;
        }
        
        /* ===== Ribbon工具栏按钮 - 与整体协调 ===== */
        #tabStack QToolButton {
            background-color: transparent;
            border: none;
            border-radius: 4px;
            color: #262626;
            padding: 2px 8px;
            font-size: 12px;
            margin: 1px 0px;
            height: 34px;
        }

        #tabStack QToolButton:hover {
            background-color: #C8E6C9;
            border: none;
            border-radius: 4px;
            box-shadow: 0 2px 8px rgba(0, 0, 0, 0.15);
            height: 34px;
        }
        
        #tabStack QToolButton:pressed {
            background-color: #E8E8E8;
            border-color: #BFBFBF;
        }
        
        #tabStack QToolButton:checked {
            background-color: #1890FF;
            border-color: #1890FF;
            color: #FFFFFF;
        }
        
        #tabStack QToolButton:disabled {
            color: #BFBFBF;
            background-color: transparent;
            border-color: transparent;
        }
        
        /* ===== Tab 按钮 - Excel绿色风格 ===== */
        #tabBarFrame QToolButton {
            background-color: transparent;
            border: 2px solid transparent;
            border-bottom: none;
            border-radius: 6px 6px 0 0;
            color: #FFFFFF;
            padding: 4px 8px;
            font-size: 13px;
            font-weight: 500;
            margin: 0;
            min-width: 50px;
            max-width: 60px;
        }

        #tabBarFrame QToolButton:hover {
            background-color: #1E663D;
            border-color: #1E663D;
            color: #FFFFFF;
        }

        #tabBarFrame QToolButton:checked {
            background-color: #FFFFFF;
            border-color: #FFFFFF;
            border-bottom: 2px solid #FFFFFF;
            color: #217346;
            font-weight: 600;
        }

        #tabBarFrame QToolButton:checked:hover {
            background-color: #F0F0F0;
            border-color: #F0F0F0;
        }
        
        /* ===== 运行/停止按钮 ===== */
        #runButton {
            background-color: %16;
            color: #FFFFFF;
            font-weight: 600;
            border-color: %16;
        }
        
        #runButton:hover {
            background-color: #389E0D;
            border-color: #389E0D;
        }
        
        #stopButton {
            background-color: %18;
            color: #FFFFFF;
            font-weight: 600;
            border-color: %18;
        }
        
        #stopButton:hover {
            background-color: #D9363E;
            border-color: #D9363E;
        }
        
        /* ===== 按钮 ===== */
        QPushButton {
            background-color: %2;
            border: 1px solid %10;
            border-radius: 4px;
            color: #000000;
            padding: 6px 16px;
            font-size: 13px;
            min-width: 80px;
        }
        
        QPushButton:hover {
            background-color: %13;
            border-color: %6;
        }
        
        QPushButton:pressed {
            background-color: %15;
            border-color: %5;
        }
        
        QPushButton:disabled {
            background-color: %1;
            color: %9;
            border-color: %11;
        }
        
        QPushButton#primaryButton {
            background-color: %4;
            color: #FFFFFF;
            border-color: %4;
        }
        
        QPushButton#primaryButton:hover {
            background-color: %6;
            border-color: %6;
        }
        
        /* ===== 工具按钮 ===== */
        QToolButton {
            background-color: transparent;
            border: 1px solid transparent;
            border-radius: 4px;
            color: #000000;
            padding: 4px 8px;
        }
        
        QToolButton:hover {
            background-color: %13;
            border-color: %11;
        }
        
        QToolButton:pressed {
            background-color: %15;
            border-color: %5;
        }
        
        QToolButton:checked {
            background-color: %14;
            border-color: %4;
            color: #FFFFFF;
        }
        
        /* ===== 输入框 ===== */
        QLineEdit, QTextEdit, QPlainTextEdit {
            background-color: %1;
            border: 1px solid %10;
            border-radius: 4px;
            color: #000000;
            padding: 5px 8px;
            font-size: 13px;
        }
        
        QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus {
            border-color: %4;
            border-width: 2px;
        }
        
        QLineEdit:hover, QTextEdit:hover, QPlainTextEdit:hover {
            border-color: %6;
        }
        
        QLineEdit:disabled, QTextEdit:disabled, QPlainTextEdit:disabled {
            background-color: %1;
            color: %9;
            border-color: %11;
        }
        
        /* ===== 下拉框 ===== */
        QComboBox {
            background-color: %1;
            border: 1px solid %10;
            border-radius: 4px;
            color: #000000;
            padding: 5px 8px;
            min-width: 100px;
        }
        
        QComboBox:hover {
            border-color: %6;
        }
        
        QComboBox:focus {
            border-color: %4;
            border-width: 2px;
        }
        
        QComboBox::drop-down {
            border: none;
            width: 24px;
        }
        
        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 5px solid %7;
            width: 0;
            height: 0;
        }
        
        QComboBox QAbstractItemView {
            background-color: %2;
            border: 1px solid %10;
            border-radius: 4px;
            selection-background-color: %14;
            selection-color: #FFFFFF;
        }
        
        /* ===== 标签 ===== */
        QLabel {
            color: #000000;
            font-size: 13px;
        }
        
        /* ===== 分组框 ===== */
        QGroupBox {
            background-color: %2;
            border: 1px solid %10;
            border-radius: 6px;
            margin-top: 12px;
            padding-top: 16px;
            font-weight: 600;
            color: #000000;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 16px;
            padding: 0 8px;
            color: %8;
        }
        
        /* ===== 选项卡 ===== */
        QTabWidget::pane {
            background-color: %2;
            border: 1px solid %10;
            border-radius: 4px;
            top: -1px;
        }
        
        QTabBar::tab {
            background-color: %1;
            border: 1px solid %10;
            border-bottom: none;
            border-radius: 4px 4px 0 0;
            color: #000000;
            padding: 8px 16px;
            margin-right: 2px;
        }
        
        QTabBar::tab:selected {
            background-color: %2;
            border-bottom: 1px solid %2;
            color: #000000;
            font-weight: 600;
        }
        
        QTabBar::tab:hover:!selected {
            background-color: %13;
        }
        
        /* ===== 列表 ===== */
        QListWidget {
            background-color: %2;
            border: 1px solid %10;
            border-radius: 4px;
            color: #000000;
            outline: none;
        }
        
        QListWidget::item {
            padding: 8px 12px;
            border-radius: 4px;
            margin: 2px 6px;
        }
        
        QListWidget::item:selected {
            background-color: %14;
            color: #FFFFFF;
        }
        
        QListWidget::item:hover:!selected {
            background-color: %13;
        }
        
        /* ===== 树形控件 - 通用样式 ===== */
        QTreeWidget {
            background-color: %2;
            border: none;
            color: #000000;
            font-size: 13px;
            outline: none;
        }
        
        QTreeWidget::item {
            padding: 5px 8px;
            border-radius: 4px;
            margin: 2px 6px;
            min-height: 24px;
            border: 1px solid transparent;
        }
        
        QTreeWidget::item:selected {
            background-color: %14;
            color: #FFFFFF;
            border-color: #333333;
        }
        
        QTreeWidget::item:hover:!selected {
            background-color: %13;
            border-color: %11;
        }
        
        QTreeWidget QHeaderView::section {
            background-color: %1;
            color: #000000;
            border: none;
            border-bottom: 1px solid %11;
            padding: 8px;
            font-weight: 600;
            font-size: 13px;
        }
        
        /* ===== 元件库树 - 简约风格 (特定覆盖) ===== */
        #elementLibrary {
            background-color: %2;
            border: none;
            color: #000000;
            font-size: 13px;
            outline: none;
            selection-background-color: transparent;
        }
        
        #elementLibrary::item {
            padding: 5px 8px;
            border-radius: 4px;
            margin: 2px 6px;
            min-height: 24px;
            border: 1px solid transparent;
        }
        
        #elementLibrary::item:selected {
            background-color: %14;
            color: #FFFFFF;
            border-color: #333333;
        }
        
        #elementLibrary::item:hover:!selected {
            background-color: %13;
            border-color: %11;
        }
        
        #elementLibrary QHeaderView::section {
            background-color: %1;
            color: #000000;
            border: none;
            border-bottom: 1px solid %11;
            padding: 8px;
            font-weight: 600;
            font-size: 13px;
        }
        
        /* ===== 属性编辑器 ===== */
        #propertyEditor {
            background-color: %2;
            border: none;
            color: #000000;
        }
        
        #propertyEditor QLabel {
            color: #000000;
            color: #000000;
            font-size: 12px;
            padding: 4px 0;
            font-weight: 500;
        }
        
        #propertyEditor QLineEdit,
        #propertyEditor QTextEdit {
            background-color: %1;
            border: 1px solid #555555;
            border-radius: 4px;
            color: #000000;
            padding: 5px 8px;
            font-size: 12px;
        }
        
        #propertyEditor QLineEdit:focus,
        #propertyEditor QTextEdit:focus {
            border-color: #333333;
            border-width: 2px;
        }
        
        #propertyEditor QLineEdit:hover,
        #propertyEditor QTextEdit:hover {
            border-color: #666666;
        }
        
        /* ===== 表格 ===== */
        QTableWidget {
            background-color: %2;
            border: none;
            color: #000000;
            gridline-color: %12;
            selection-background-color: %14;
            selection-color: #FFFFFF;
        }
        
        QTableWidget::item {
            padding: 6px;
            border-bottom: 1px solid %12;
            border-right: 1px solid %12;
        }
        
        QHeaderView::section {
            background-color: %1;
            color: #000000;
            border: none;
            border-bottom: 1px solid %11;
            padding: 8px;
            font-weight: 600;
        }
        
        /* ===== 状态栏 ===== */
        #statusBar {
            background-color: %1;
            color: #000000;
            border-top: 1px solid %12;
            padding: 6px 16px;
            font-size: 12px;
        }
        
        /* ===== 菜单 ===== */
        QMenuBar {
            background-color: %2;
            color: #000000;
            border: none;
            border-bottom: 1px solid %12;
        }
        
        QMenuBar::item {
            background-color: transparent;
            padding: 8px 16px;
        }
        
        QMenuBar::item:selected {
            background-color: %13;
            border-radius: 4px;
        }
        
        QMenu {
            background-color: %2;
            color: #000000;
            border: 1px solid %11;
            border-radius: 4px;
            padding: 4px;
        }
        
        QMenu::item {
            padding: 8px 24px;
            border-radius: 4px;
        }
        
        QMenu::item:selected {
            background-color: %14;
            color: #FFFFFF;
        }
        
        QMenu::separator {
            height: 1px;
            background-color: %12;
            margin: 4px 12px;
        }
        
        /* ===== 滚动条 - 绿色系风格 ===== */
        QScrollBar:vertical {
            background-color: transparent;
            width: 10px;
            border-radius: 0px;
        }

        QScrollBar::handle:vertical {
            background-color: #808080;
            border-radius: 5px;
            min-height: 40px;
            margin: 2px 4px;
        }

        QScrollBar::handle:vertical:hover {
            background-color: #555555;
        }

        QScrollBar:horizontal {
            background-color: transparent;
            height: 10px;
            border-radius: 0px;
        }

        QScrollBar::handle:horizontal {
            background-color: #808080;
            border-radius: 5px;
            min-width: 40px;
            margin: 4px 2px;
        }

        QScrollBar::handle:horizontal:hover {
            background-color: #555555;
        }

        QScrollBar::sub-line:vertical, QScrollBar::add-line:vertical {
            background-color: transparent;
            border: none;
            height: 0px;
        }

        QScrollBar::sub-line:horizontal, QScrollBar::add-line:horizontal {
            background-color: transparent;
            border: none;
            width: 0px;
        }

        QScrollBar::track:vertical, QScrollBar::track:horizontal {
            background-color: transparent;
            border: none;
        }

        /* ===== 分割器 - 完全透明无缝风格 ===== */
        QSplitter::handle {
            background-color: transparent;
            border: none;
        }

        QSplitter::handle:horizontal {
            width: 6px;
            background-color: transparent;
            border: none;
            margin: 0;
        }

        QSplitter::handle:vertical {
            height: 6px;
            background-color: transparent;
            border: none;
            margin: 0;
        }

        QSplitter::handle:hover {
            background-color: %13;
        }
        
        /* ===== 工具提示 - 现代风格 ===== */
        QToolTip {
            background-color: %2;
            color: #000000;
            border: 1px solid %11;
            border-radius: 4px;
            padding: 8px 12px;
            font-size: 12px;
        }
        
        /* ===== 对话框 ===== */
        QDialog {
            background-color: %2;
            color: #000000;
        }
        
        QDialog QPushButton {
            background-color: %4;
            color: #FFFFFF;
            border-color: %4;
            min-width: 100px;
        }
        
        QDialog QPushButton:hover {
            background-color: %6;
            border-color: %6;
        }
        
        QDialog QPushButton:pressed {
            background-color: %5;
            border-color: %5;
        }
        
        /* ===== 复选框和单选按钮 ===== */
        QCheckBox, QRadioButton {
            color: #000000;
            spacing: 8px;
        }
        
        QCheckBox::indicator, QRadioButton::indicator {
            width: 16px;
            height: 16px;
        }
        
        QCheckBox::indicator:unchecked {
            border: 2px solid %10;
            border-radius: 3px;
            background-color: %1;
        }
        
        QCheckBox::indicator:checked {
            border: 2px solid %4;
            border-radius: 3px;
            background-color: %4;
        }
        
        QCheckBox::indicator:hover:unchecked {
            border-color: %6;
        }
        
        QRadioButton::indicator:unchecked {
            border: 2px solid %10;
            border-radius: 8px;
            background-color: %1;
        }
        
        QRadioButton::indicator:checked {
            border: 2px solid %4;
            border-radius: 8px;
            background-color: %4;
        }
        
        QRadioButton::indicator:hover:unchecked {
            border-color: %6;
        }
        
        /* ===== 滑块 ===== */
        QSlider::groove:horizontal {
            height: 4px;
            background-color: %11;
            border-radius: 2px;
        }
        
        QSlider::sub-page:horizontal {
            background-color: %4;
            border-radius: 2px;
        }
        
        QSlider::handle:horizontal {
            width: 16px;
            height: 16px;
            margin: -6px 0;
            background-color: %2;
            border: 2px solid %4;
            border-radius: 8px;
        }
        
        QSlider::handle:horizontal:hover {
            background-color: %13;
            border-color: %6;
        }
        
        QSlider::groove:vertical {
            width: 4px;
            background-color: %11;
            border-radius: 2px;
        }
        
        QSlider::sub-page:vertical {
            background-color: %4;
            border-radius: 2px;
        }
        
        QSlider::handle:vertical {
            width: 16px;
            height: 16px;
            margin: 0 -6px;
            background-color: %2;
            border: 2px solid %4;
            border-radius: 8px;
        }
        
        QSlider::handle:vertical:hover {
            background-color: %13;
            border-color: %6;
        }
        
        /* ===== 进度条 ===== */
        QProgressBar {
            background-color: %11;
            border: none;
            border-radius: 4px;
            height: 8px;
            text-align: center;
            color: #000000;
        }
        
        QProgressBar::chunk {
            background-color: %4;
            border-radius: 4px;
        }
        
        /* ===== 旋转框 ===== */
        QSpinBox, QDoubleSpinBox {
            background-color: %1;
            border: 1px solid %10;
            border-radius: 4px;
            color: #000000;
            padding: 5px 8px;
        }
        
        QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: %4;
            border-width: 2px;
        }
        
        QSpinBox::up-button, QDoubleSpinBox::up-button,
        QSpinBox::down-button, QDoubleSpinBox::down-button {
            width: 20px;
            background-color: %2;
            border: 1px solid %10;
        }
        
        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,
        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background-color: %13;
        }
        
        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
            image: none;
            border-left: 4px solid transparent;
            border-right: 4px solid transparent;
            border-bottom: 6px solid %7;
        }
        
        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            image: none;
            border-left: 4px solid transparent;
            border-right: 4px solid transparent;
            border-top: 6px solid %7;
        }
    )");
    
    // 替换颜色占位符
    style = style.arg(
        colors.backgroundDark,    // %1
        colors.backgroundMid,     // %2
        colors.backgroundLight,   // %3
        colors.accentBlue,        // %4
        colors.accentBlueDark,    // %5
        colors.accentBlueLight,   // %6
        colors.textPrimary,       // %7
        colors.textSecondary,     // %8
        colors.textDisabled,      // %9
        colors.borderDark,        // %10
        colors.borderMid,         // %11
        colors.borderLight,       // %12
        colors.hoverBg,           // %13
        colors.selectedBg,        // %14
        colors.pressedBg,         // %15
        colors.success,           // %16
        colors.warning,           // %17
        colors.error              // %18
    );
    
    return style;
}

} // namespace LadderDiagram
