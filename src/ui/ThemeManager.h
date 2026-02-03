/*
 * 主题样式管理器
 * Theme Manager - 支持浅色/暗色主题切换
 * 
 * 默认：浅色主题 (Light Theme)
 * 可选：暗色主题 (Dark Theme)
 */

#ifndef THEME_MANAGER_H
#define THEME_MANAGER_H

#include <QString>
#include <QApplication>
#include <QWidget>
#include <QSettings>
#include <QObject>

namespace LadderDiagram {

// 主题类型
enum class ThemeType {
    Light,  // 浅色主题（默认）
    Dark    // 暗色主题
};

// 主题颜色结构
struct ThemeColors {
    // 背景色
    QString backgroundDark;
    QString backgroundMid;
    QString backgroundLight;
    
    // 强调色
    QString accentBlue;
    QString accentBlueDark;
    QString accentBlueLight;
    
    // 文字色
    QString textPrimary;
    QString textSecondary;
    QString textDisabled;
    
    // 边框色
    QString borderDark;
    QString borderMid;
    QString borderLight;
    
    // 状态色
    QString hoverBg;
    QString selectedBg;
    QString pressedBg;
    
    // 功能色
    QString success;
    QString warning;
    QString error;
};

// 主题管理器类
class ThemeManager : public QObject {
    Q_OBJECT
public:
    // 获取单例实例
    static ThemeManager& instance();
    
    // 初始化主题（从配置文件加载或设置默认）
    void init();
    
    // 设置主题
    void setTheme(ThemeType type);
    
    // 切换主题
    void toggleTheme();
    
    // 获取当前主题
    ThemeType currentTheme() const { return m_currentTheme; }
    
    // 获取当前主题颜色
    ThemeColors getCurrentColors() const;
    
    // 获取当前主题样式表
    QString getStyleSheet() const;
    
    // 应用主题到应用程序
    void applyTheme(QApplication* app);
    
    // 应用主题到指定窗口
    void applyToWidget(QWidget* widget);
    
    // 保存主题设置
    void saveSettings();
    
private:
    ThemeManager() = default;
    ~ThemeManager() = default;
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;
    
    // 生成浅色主题颜色
    ThemeColors getLightColors() const;
    
    // 生成暗色主题颜色
    ThemeColors getDarkColors() const;
    
    // 生成样式表
    QString generateStyleSheet(const ThemeColors& colors) const;
    
    ThemeType m_currentTheme = ThemeType::Light;
    QSettings* m_settings = nullptr;
};

// 便捷函数：获取全局样式表
inline QString getGlobalStyleSheet() {
    return ThemeManager::instance().getStyleSheet();
}

// 便捷函数：应用主题
inline void applyGlobalTheme(QApplication* app) {
    ThemeManager::instance().applyTheme(app);
}

} // namespace LadderDiagram

#endif // THEME_MANAGER_H
