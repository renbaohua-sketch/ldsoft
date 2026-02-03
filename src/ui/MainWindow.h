#pragma once

#include <QMainWindow>
#include <QToolBar>
#include <QDockWidget>
#include <QAction>
#include <QSplitter>
#include <QTreeWidget>
#include "LadderScene.h"
#include "PropertyEditor.h"

namespace LadderDiagram {

// 主窗口
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

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
    void onSelectAll();
    
    // 视图操作
    void onZoomIn();
    void onZoomOut();
    void onZoomReset();
    void onToggleGrid();
    void onToggleConnectionMode();
    
    // 工具
    void onRunSimulation();
    void onStopSimulation();
    void onGenerateCode();
    
    // 帮助
    void onAbout();
    
    // 元件选择
    void onElementSelected();
    void onSceneSelectionChanged();

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupDockWidgets();
    void setupStatusBar();
    void setupConnections();
    
    // 元件库面板
    void createElementLibrary();
    void onElementLibraryItemClicked(QTreeWidgetItem* item, int column);
    void onElementLibraryItemDoubleClicked(QTreeWidgetItem* item, int column);
    
    // 文件操作辅助
    bool maybeSave();
    bool saveFile(const QString& path);
    bool loadFile(const QString& path);
    void setCurrentFile(const QString& path);
    
    // 添加元件到场景
    void addElementToScene(ElementType type, const QPointF& pos = QPointF(0, 0));
    
    // 核心组件
    LadderScene* m_scene = nullptr;
    LadderView* m_view = nullptr;
    PropertyEditor* m_propertyEditor = nullptr;
    
    // 元件库
    QTreeWidget* m_elementLibrary = nullptr;
    
    // Dock窗口
    QDockWidget* m_libraryDock = nullptr;
    QDockWidget* m_propertyDock = nullptr;
    
    // 当前文件
    QString m_currentFile;
    bool m_modified = false;
    
    // 动作
    QAction* m_actionNew = nullptr;
    QAction* m_actionOpen = nullptr;
    QAction* m_actionSave = nullptr;
    QAction* m_actionSaveAs = nullptr;
    QAction* m_actionUndo = nullptr;
    QAction* m_actionRedo = nullptr;
    QAction* m_actionDelete = nullptr;
    QAction* m_actionZoomIn = nullptr;
    QAction* m_actionZoomOut = nullptr;
    QAction* m_actionZoomReset = nullptr;
    QAction* m_actionToggleGrid = nullptr;
    QAction* m_actionConnectionMode = nullptr;
    QAction* m_actionRun = nullptr;
    QAction* m_actionStop = nullptr;
};

} // namespace LadderDiagram
