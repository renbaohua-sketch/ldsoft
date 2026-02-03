/*
 * 工业软件风格全局样式表
 * Industrial Software Style - Global Stylesheet
 * 
 * 配色方案：
 * - 主背景: #2D2D2D (深灰色)
 * - 次背景: #3C3C3C (中灰色)
 * - 高亮: #007ACC (工业蓝)
 * - 文字: #E0E0E0 (浅灰白)
 * - 边框: #1E1E1E (深黑灰)
 * - 选中: #094771 (深蓝)
 * - 悬停: #4A4A4A (亮灰)
 */

#ifndef INDUSTRIAL_STYLES_H
#define INDUSTRIAL_STYLES_H

#include <QString>

namespace LadderDiagram {

// 工业风格配色
namespace IndustrialColors {
    // 背景色
    static const QString BACKGROUND_DARK = "#2D2D2D";      // 主背景
    static const QString BACKGROUND_MID = "#3C3C3C";       // 次背景
    static const QString BACKGROUND_LIGHT = "#454545";     // 面板背景
    
    // 强调色
    static const QString ACCENT_BLUE = "#007ACC";          // 工业蓝
    static const QString ACCENT_BLUE_DARK = "#005A9E";     // 深蓝
    static const QString ACCENT_BLUE_LIGHT = "#1177BB";    // 亮蓝
    
    // 文字色
    static const QString TEXT_PRIMARY = "#E0E0E0";         // 主文字
    static const QString TEXT_SECONDARY = "#A0A0A0";       // 次文字
    static const QString TEXT_DISABLED = "#666666";        // 禁用文字
    
    // 边框色
    static const QString BORDER_DARK = "#1E1E1E";          // 深边框
    static const QString BORDER_MID = "#555555";           // 中边框
    static const QString BORDER_LIGHT = "#666666";         // 亮边框
    
    // 状态色
    static const QString HOVER_BG = "#4A4A4A";             // 悬停背景
    static const QString SELECTED_BG = "#094771";          // 选中背景
    static const QString PRESSED_BG = "#2D5A8A";           // 按下背景
    
    // 功能色
    static const QString SUCCESS = "#28A745";              // 成功/运行
    static const QString WARNING = "#FFC107";              // 警告
    static const QString ERROR = "#DC3545";                // 错误/停止
}

// 全局样式表
inline QString getIndustrialStyleSheet() {
    using namespace IndustrialColors;
    
    return QString(R"(
        /* ===== 主窗口 ===== */
        QMainWindow {
            background-color: %1;
            color: %3;
        }
        
        /* ===== 中央部件 ===== */
        QWidget {
            background-color: %1;
            color: %3;
            font-family: "Segoe UI", "Microsoft YaHei", sans-serif;
            font-size: 13px;
        }
        
        /* ===== Ribbon 容器 ===== */
        #ribbonContainer {
            background-color: %2;
            border-bottom: 1px solid %5;
        }
        
        #toolbarFrame {
            background-color: %2;
            border-bottom: 1px solid %5;
        }
        
        #tabBarFrame {
            background-color: %1;
            border-bottom: 1px solid %5;
        }
        
        #tabStack {
            background-color: %2;
        }
        
        /* ===== Ribbon 工具按钮 ===== */
        QToolButton {
            background-color: transparent;
            border: none;
            border-radius: 2px;
            color: %3;
            padding: 4px 8px;
            font-size: 12px;
        }
        
        QToolButton:hover {
            background-color: %8;
        }
        
        QToolButton:pressed {
            background-color: %10;
        }
        
        QToolButton:checked {
            background-color: %9;
            color: %3;
        }
        
        QToolButton:disabled {
            color: %4;
        }
        
        /* ===== Tab 按钮（特殊样式） ===== */
        #tabBarFrame QToolButton {
            background-color: transparent;
            border: none;
            border-radius: 2px;
            color: %3;
            padding: 4px 12px;
            font-size: 12px;
            font-weight: normal;
        }
        
        #tabBarFrame QToolButton:hover {
            background-color: %8;
        }
        
        #tabBarFrame QToolButton:checked {
            background-color: %6;
            color: #FFFFFF;
            font-weight: bold;
        }
        
        /* ===== 运行按钮（绿色强调） ===== */
        #runButton {
            background-color: %11;
            color: #FFFFFF;
            font-weight: bold;
        }
        
        #runButton:hover {
            background-color: #218838;
        }
        
        /* ===== 停止按钮（红色强调） ===== */
        #stopButton {
            background-color: %13;
            color: #FFFFFF;
            font-weight: bold;
        }
        
        #stopButton:hover {
            background-color: #C82333;
        }
        
        /* ===== 侧边栏（元件库） ===== */
        #elementLibrary {
            background-color: %2;
            border: none;
            border-right: 1px solid %5;
            color: %3;
            font-size: 13px;
            outline: none;
        }
        
        #elementLibrary::item {
            padding: 6px 8px;
            border-radius: 2px;
            margin: 1px 4px;
            min-height: 24px;
        }
        
        #elementLibrary::item:selected {
            background-color: %9;
            color: #FFFFFF;
        }
        
        #elementLibrary::item:hover:!selected {
            background-color: %8;
        }
        
        #elementLibrary QHeaderView::section {
            background-color: %1;
            color: %3;
            border: none;
            border-bottom: 1px solid %5;
            padding: 8px;
            font-weight: bold;
        }
        
        /* ===== 属性编辑器 ===== */
        #propertyEditor {
            background-color: %2;
            border: none;
            border-left: 1px solid %5;
            color: %3;
        }
        
        #propertyEditor QLabel {
            color: %3;
            font-size: 12px;
            padding: 4px 0;
        }
        
        #propertyEditor QLineEdit,
        #propertyEditor QTextEdit {
            background-color: %1;
            border: 1px solid %6;
            border-radius: 2px;
            color: %3;
            padding: 4px 6px;
            font-size: 12px;
        }
        
        #propertyEditor QLineEdit:focus,
        #propertyEditor QTextEdit:focus {
            border-color: %6;
            border-width: 2px;
        }
        
        #propertyEditor QLineEdit:disabled,
        #propertyEditor QTextEdit:disabled {
            background-color: %2;
            color: %4;
        }
        
        /* ===== 表格 ===== */
        QTableWidget {
            background-color: %2;
            border: 1px solid %5;
            border-radius: 2px;
            color: %3;
            gridline-color: %5;
        }
        
        QTableWidget::item {
            padding: 6px;
        }
        
        QTableWidget::item:selected {
            background-color: %9;
            color: #FFFFFF;
        }
        
        QHeaderView::section {
            background-color: %1;
            color: %3;
            border: none;
            border-bottom: 1px solid %5;
            border-right: 1px solid %5;
            padding: 6px;
            font-weight: bold;
        }
        
        /* ===== 状态栏 ===== */
        #statusBar {
            background-color: %1;
            color: %3;
            border-top: 1px solid %5;
            padding: 4px 12px;
            font-size: 12px;
        }
        
        /* ===== 菜单栏 ===== */
        QMenuBar {
            background-color: %1;
            color: %3;
            border-bottom: 1px solid %5;
        }
        
        QMenuBar::item {
            background-color: transparent;
            padding: 6px 12px;
        }
        
        QMenuBar::item:selected {
            background-color: %8;
        }
        
        QMenuBar::item:pressed {
            background-color: %9;
        }
        
        /* ===== 菜单 ===== */
        QMenu {
            background-color: %2;
            color: %3;
            border: 1px solid %5;
            padding: 4px;
        }
        
        QMenu::item {
            padding: 6px 24px;
            border-radius: 2px;
        }
        
        QMenu::item:selected {
            background-color: %9;
            color: #FFFFFF;
        }
        
        QMenu::separator {
            height: 1px;
            background-color: %5;
            margin: 4px 8px;
        }
        
        /* ===== 滚动条 ===== */
        QScrollBar:vertical {
            background-color: %1;
            width: 12px;
            border-radius: 0px;
        }
        
        QScrollBar::handle:vertical {
            background-color: %6;
            border-radius: 6px;
            min-height: 30px;
            margin: 2px;
        }
        
        QScrollBar::handle:vertical:hover {
            background-color: %7;
        }
        
        QScrollBar:horizontal {
            background-color: %1;
            height: 12px;
            border-radius: 0px;
        }
        
        QScrollBar::handle:horizontal {
            background-color: %6;
            border-radius: 6px;
            min-width: 30px;
            margin: 2px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background-color: %7;
        }
        
        /* ===== 分割器 ===== */
        QSplitter::handle {
            background-color: %5;
        }
        
        QSplitter::handle:horizontal {
            width: 2px;
        }
        
        QSplitter::handle:vertical {
            height: 2px;
        }
        
        /* ===== 工具提示 ===== */
        QToolTip {
            background-color: %2;
            color: %3;
            border: 1px solid %6;
            border-radius: 2px;
            padding: 6px 10px;
            font-size: 12px;
        }
        
        /* ===== 对话框 ===== */
        QDialog {
            background-color: %2;
            color: %3;
        }
        
        QDialog QPushButton {
            background-color: %6;
            color: #FFFFFF;
            border: none;
            border-radius: 2px;
            padding: 6px 16px;
            font-size: 12px;
            min-width: 80px;
        }
        
        QDialog QPushButton:hover {
            background-color: %7;
        }
        
        QDialog QPushButton:pressed {
            background-color: %10;
        }
        
        QDialog QPushButton:default {
            background-color: %6;
            font-weight: bold;
        }
        
        /* ===== 分组框 ===== */
        QGroupBox {
            background-color: %2;
            color: %3;
            border: 1px solid %5;
            border-radius: 2px;
            margin-top: 12px;
            padding-top: 12px;
            font-weight: bold;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 8px;
            padding: 0 4px;
        }
        
        /* ===== 复选框 ===== */
        QCheckBox {
            color: %3;
            spacing: 6px;
        }
        
        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border-radius: 2px;
            border: 1px solid %6;
            background-color: %1;
        }
        
        QCheckBox::indicator:checked {
            background-color: %6;
            border-color: %6;
        }
        
        /* ===== 单选按钮 ===== */
        QRadioButton {
            color: %3;
            spacing: 6px;
        }
        
        QRadioButton::indicator {
            width: 16px;
            height: 16px;
            border-radius: 8px;
            border: 1px solid %6;
            background-color: %1;
        }
        
        QRadioButton::indicator:checked {
            background-color: %6;
            border-color: %6;
        }
        
        /* ===== 下拉框 ===== */
        QComboBox {
            background-color: %1;
            color: %3;
            border: 1px solid %6;
            border-radius: 2px;
            padding: 4px 8px;
            min-width: 80px;
        }
        
        QComboBox:hover {
            border-color: %7;
        }
        
        QComboBox::drop-down {
            border: none;
            width: 24px;
        }
        
        QComboBox QAbstractItemView {
            background-color: %2;
            color: %3;
            border: 1px solid %5;
            selection-background-color: %9;
        }
        
        /* ===== 标签页（QTabWidget） ===== */
        QTabWidget::pane {
            border: 1px solid %5;
            background-color: %2;
        }
        
        QTabBar::tab {
            background-color: %1;
            color: %3;
            border: 1px solid %5;
            border-bottom: none;
            border-top-left-radius: 2px;
            border-top-right-radius: 2px;
            padding: 6px 16px;
            margin-right: 2px;
        }
        
        QTabBar::tab:selected {
            background-color: %2;
            border-bottom: 1px solid %2;
        }
        
        QTabBar::tab:hover:!selected {
            background-color: %8;
        }
    )").arg(
        BACKGROUND_DARK,    // %1
        BACKGROUND_MID,     // %2
        TEXT_PRIMARY,       // %3
        TEXT_DISABLED,      // %4
        BORDER_DARK,        // %5
        ACCENT_BLUE,        // %6
        ACCENT_BLUE_LIGHT,  // %7
        HOVER_BG,           // %8
        SELECTED_BG,        // %9
        PRESSED_BG,         // %10
        SUCCESS,            // %11
        WARNING,            // %12
        ERROR               // %13
    );
}

} // namespace LadderDiagram

#endif // INDUSTRIAL_STYLES_H
