#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QFrame>
#include <QStackedWidget>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QTreeWidget>
#include "LadderScene.h"
#include "PropertyEditor.h"

namespace LadderDiagram {

// Ribbon 分组
class RibbonGroup : public QFrame {
    Q_OBJECT

public:
    explicit RibbonGroup(const QString& title, QWidget* parent = nullptr);
    
    // 添加按钮
    QToolButton* addButton(const QString& text, const QString& iconPath, 
                          const QString& tooltip = QString());
    QToolButton* addLargeButton(const QString& text, const QString& iconPath,
                               const QString& tooltip = QString());
    
    // 添加分隔线
    void addSeparator();
    
    // 添加自定义控件
    void addWidget(QWidget* widget);
    
    // 获取布局
    QHBoxLayout* layout() { return m_layout; }

private:
    QHBoxLayout* m_layout;
    QLabel* m_titleLabel;
    QString m_title;
};

// Ribbon 页面
class RibbonPage : public QFrame {
    Q_OBJECT

public:
    explicit RibbonPage(QWidget* parent = nullptr);
    
    // 添加分组
    RibbonGroup* addGroup(const QString& title);
    
private:
    QHBoxLayout* m_layout;
};

// Ribbon 主窗口
class RibbonMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit RibbonMainWindow(QWidget* parent = nullptr);
    ~RibbonMainWindow();

private slots:
    // 文件操作
    void onNewFile();
    void onOpenFile();
    void onSaveFile();
    void onSaveFileAs();
    void onExit();
    
    // 编辑操作
    void onUndo();
    void onRedo();
    void onDelete();
    void onCut();
    void onCopy();
    void onPaste();
    void onSelectAll();
    
    // 视图操作
    void onZoomIn();
    void onZoomOut();
    void onZoomReset();
    void onToggleGrid();
    void onToggleConnectionMode();
    
    // 元件操作
    void onAddContactNO();
    void onAddContactNC();
    void onAddOutputCoil();
    void onAddSetCoil();
    void onAddResetCoil();
    void onAddTimer();
    void onAddCounter();
    void onAddLeftRail();
    void onAddRightRail();
    
    // 工具
    void onRunSimulation();
    void onStopSimulation();
    void onGenerateCode();
    
    // 帮助
    void onAbout();
    
    // 场景选择变化
    void onSceneSelectionChanged();
    
    // Tab 切换
    void onRibbonTabChanged(int index);
    
    // 主题切换
    void onToggleTheme();

private:
    void setupUI();
    void setupRibbon();
    void setupStatusBar();
    void setupSidePanels();
    void setupElementLibrary();
    void setupConnections();
    
    // Tab 面板创建
    QWidget* createFilePanel();
    QWidget* createHomePanel();
    QWidget* createInsertPanel();
    QWidget* createViewPanel();
    QWidget* createRunPanel();
    
    // 添加元件到场景
    void addElementToScene(ElementType type);
    
    // 文件操作辅助
    bool maybeSave();
    bool saveFile(const QString& path);
    bool loadFile(const QString& path);
    void setCurrentFile(const QString& path);
    
    // 更新按钮状态
    void updateActionStates();

    // 核心组件
    LadderScene* m_scene = nullptr;
    LadderView* m_view = nullptr;
    PropertyEditor* m_propertyEditor = nullptr;
    
    // Ribbon 组件
    QFrame* m_ribbonContainer = nullptr;
    
    // Tab 相关按钮
    QToolButton* m_fileTabBtn = nullptr;
    QToolButton* m_homeTabBtn = nullptr;
    QToolButton* m_insertTabBtn = nullptr;
    QToolButton* m_viewTabBtn = nullptr;
    QToolButton* m_runTabBtn = nullptr;
    
    // Tab 内容堆叠
    QStackedWidget* m_tabStack = nullptr;
    
    // 元件库
    QTreeWidget* m_elementLibrary = nullptr;
    
    // 当前文件
    QString m_currentFile;
    bool m_modified = false;
    
    // 按钮集合
    struct {
        QToolButton* newFile = nullptr;
        QToolButton* openFile = nullptr;
        QToolButton* saveFile = nullptr;
        QToolButton* undo = nullptr;
        QToolButton* redo = nullptr;
        QToolButton* cut = nullptr;
        QToolButton* copy = nullptr;
        QToolButton* paste = nullptr;
        QToolButton* deleteItem = nullptr;
        QToolButton* zoomIn = nullptr;
        QToolButton* zoomOut = nullptr;
        QToolButton* zoomReset = nullptr;
        QToolButton* toggleGrid = nullptr;
        QToolButton* connectionMode = nullptr;
        QToolButton* runSim = nullptr;
        QToolButton* stopSim = nullptr;
    } m_buttons;
    
    // Tab 切换
    void switchToTab(int index);
    
    // 页面
    RibbonPage* m_filePage = nullptr;
    RibbonPage* m_homePage = nullptr;
    RibbonPage* m_insertPage = nullptr;
    RibbonPage* m_viewPage = nullptr;
    RibbonPage* m_toolsPage = nullptr;
};

} // namespace LadderDiagram
