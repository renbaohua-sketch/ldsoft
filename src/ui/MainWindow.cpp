#include "MainWindow.h"
#include "../elements/ContactElements.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>
#include <QSplitter>
#include <QHBoxLayout>

namespace LadderDiagram {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setupUI();
    setupMenuBar();
    setupToolBar();
    setupDockWidgets();
    setupStatusBar();
    setupConnections();
    
    setWindowTitle(tr("梯形图编辑器"));
    resize(1200, 800);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    // 创建中央部件
    QWidget* centralWidget = new QWidget();
    QHBoxLayout* centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    
    // 创建场景和视图
    m_scene = new LadderScene(this);
    m_view = new LadderView(this);
    m_view->setScene(m_scene);
    
    centralLayout->addWidget(m_view);
    setCentralWidget(centralWidget);
}

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);
    
    // 文件菜单
    QMenu* fileMenu = menuBar->addMenu(tr("文件(&F)"));
    m_actionNew = fileMenu->addAction(tr("新建(&N)"), this, &MainWindow::onNewFile, QKeySequence::New);
    m_actionOpen = fileMenu->addAction(tr("打开(&O)..."), this, &MainWindow::onOpenFile, QKeySequence::Open);
    m_actionSave = fileMenu->addAction(tr("保存(&S)"), this, &MainWindow::onSaveFile, QKeySequence::Save);
    m_actionSaveAs = fileMenu->addAction(tr("另存为(&A)..."), this, &MainWindow::onSaveFileAs, QKeySequence::SaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("退出(&X)"), this, &MainWindow::onExit, QKeySequence::Quit);
    
    // 编辑菜单
    QMenu* editMenu = menuBar->addMenu(tr("编辑(&E)"));
    m_actionUndo = editMenu->addAction(tr("撤销(&U)"), this, &MainWindow::onUndo, QKeySequence::Undo);
    m_actionRedo = editMenu->addAction(tr("重做(&R)"), this, &MainWindow::onRedo, QKeySequence::Redo);
    editMenu->addSeparator();
    m_actionDelete = editMenu->addAction(tr("删除(&D)"), this, &MainWindow::onDelete, QKeySequence::Delete);
    editMenu->addAction(tr("全选(&A)"), this, &MainWindow::onSelectAll, QKeySequence::SelectAll);
    
    // 视图菜单
    QMenu* viewMenu = menuBar->addMenu(tr("视图(&V)"));
    m_actionZoomIn = viewMenu->addAction(tr("放大(&I)"), this, &MainWindow::onZoomIn, QKeySequence::ZoomIn);
    m_actionZoomOut = viewMenu->addAction(tr("缩小(&O)"), this, &MainWindow::onZoomOut, QKeySequence::ZoomOut);
    m_actionZoomReset = viewMenu->addAction(tr("重置缩放(&R)"), this, &MainWindow::onZoomReset);
    viewMenu->addSeparator();
    m_actionToggleGrid = viewMenu->addAction(tr("显示网格(&G)"), this, &MainWindow::onToggleGrid);
    m_actionToggleGrid->setCheckable(true);
    m_actionToggleGrid->setChecked(true);
    viewMenu->addAction(tr("属性编辑器(&P)"), m_propertyDock, &QDockWidget::show);
    viewMenu->addAction(tr("元件库(&L)"), m_libraryDock, &QDockWidget::show);
    
    // 工具菜单
    QMenu* toolsMenu = menuBar->addMenu(tr("工具(&T)"));
    m_actionRun = toolsMenu->addAction(tr("运行仿真(&R)"), this, &MainWindow::onRunSimulation);
    m_actionStop = toolsMenu->addAction(tr("停止仿真(&S)"), this, &MainWindow::onStopSimulation);
    m_actionStop->setEnabled(false);
    toolsMenu->addSeparator();
    toolsMenu->addAction(tr("生成代码(&G)"), this, &MainWindow::onGenerateCode);
    
    // 帮助菜单
    QMenu* helpMenu = menuBar->addMenu(tr("帮助(&H)"));
    helpMenu->addAction(tr("关于(&A)"), this, &MainWindow::onAbout);
    
    setMenuBar(menuBar);
}

void MainWindow::setupToolBar() {
    QToolBar* fileToolBar = addToolBar(tr("文件"));
    fileToolBar->addAction(m_actionNew);
    fileToolBar->addAction(m_actionOpen);
    fileToolBar->addAction(m_actionSave);
    
    QToolBar* editToolBar = addToolBar(tr("编辑"));
    editToolBar->addAction(m_actionUndo);
    editToolBar->addAction(m_actionRedo);
    editToolBar->addAction(m_actionDelete);
    
    QToolBar* viewToolBar = addToolBar(tr("视图"));
    viewToolBar->addAction(m_actionZoomIn);
    viewToolBar->addAction(m_actionZoomOut);
    viewToolBar->addAction(m_actionZoomReset);
    viewToolBar->addSeparator();
    viewToolBar->addAction(m_actionToggleGrid);
    
    QToolBar* toolsToolBar = addToolBar(tr("工具"));
    m_actionConnectionMode = toolsToolBar->addAction(tr("连线模式"), this, &MainWindow::onToggleConnectionMode);
    m_actionConnectionMode->setCheckable(true);
    toolsToolBar->addSeparator();
    toolsToolBar->addAction(m_actionRun);
    toolsToolBar->addAction(m_actionStop);
}

void MainWindow::setupDockWidgets() {
    // 元件库Dock
    m_libraryDock = new QDockWidget(tr("元件库"), this);
    m_elementLibrary = new QTreeWidget();
    m_elementLibrary->setHeaderHidden(true);
    createElementLibrary();
    m_libraryDock->setWidget(m_elementLibrary);
    addDockWidget(Qt::LeftDockWidgetArea, m_libraryDock);
    
    // 属性编辑器Dock
    m_propertyDock = new QDockWidget(tr("属性"), this);
    m_propertyEditor = new PropertyEditor();
    m_propertyDock->setWidget(m_propertyEditor);
    addDockWidget(Qt::RightDockWidgetArea, m_propertyDock);
}

void MainWindow::setupStatusBar() {
    QStatusBar* statusBar = new QStatusBar(this);
    statusBar->showMessage(tr("就绪"));
    setStatusBar(statusBar);
}

void MainWindow::setupConnections() {
    // 撤销重做
    connect(m_scene->undoStack(), &QUndoStack::canUndoChanged, m_actionUndo, &QAction::setEnabled);
    connect(m_scene->undoStack(), &QUndoStack::canRedoChanged, m_actionRedo, &QAction::setEnabled);
    
    // 场景选择变化
    connect(m_scene, &QGraphicsScene::selectionChanged, this, &MainWindow::onSceneSelectionChanged);
    
    // 元件库双击
    connect(m_elementLibrary, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onElementLibraryItemDoubleClicked);
    
    m_actionUndo->setEnabled(false);
    m_actionRedo->setEnabled(false);
}

void MainWindow::createElementLibrary() {
    // 基本元件
    QTreeWidgetItem* basicItem = new QTreeWidgetItem(m_elementLibrary);
    basicItem->setText(0, tr("基本元件"));
    basicItem->setExpanded(true);
    
    QTreeWidgetItem* leftRail = new QTreeWidgetItem(basicItem);
    leftRail->setText(0, tr("左电源轨"));
    leftRail->setData(0, Qt::UserRole, static_cast<int>(ElementType::LeftPowerRail));
    
    QTreeWidgetItem* rightRail = new QTreeWidgetItem(basicItem);
    rightRail->setText(0, tr("右电源轨"));
    rightRail->setData(0, Qt::UserRole, static_cast<int>(ElementType::RightPowerRail));
    
    // 触点
    QTreeWidgetItem* contactItem = new QTreeWidgetItem(m_elementLibrary);
    contactItem->setText(0, tr("触点"));
    contactItem->setExpanded(true);
    
    QTreeWidgetItem* noContact = new QTreeWidgetItem(contactItem);
    noContact->setText(0, tr("常开触点"));
    noContact->setData(0, Qt::UserRole, static_cast<int>(ElementType::NormallyOpen));
    
    QTreeWidgetItem* ncContact = new QTreeWidgetItem(contactItem);
    ncContact->setText(0, tr("常闭触点"));
    ncContact->setData(0, Qt::UserRole, static_cast<int>(ElementType::NormallyClosed));
    
    // 线圈
    QTreeWidgetItem* coilItem = new QTreeWidgetItem(m_elementLibrary);
    coilItem->setText(0, tr("线圈"));
    coilItem->setExpanded(true);
    
    QTreeWidgetItem* outCoil = new QTreeWidgetItem(coilItem);
    outCoil->setText(0, tr("输出线圈"));
    outCoil->setData(0, Qt::UserRole, static_cast<int>(ElementType::OutputCoil));
    
    QTreeWidgetItem* setCoil = new QTreeWidgetItem(coilItem);
    setCoil->setText(0, tr("置位线圈"));
    setCoil->setData(0, Qt::UserRole, static_cast<int>(ElementType::SetCoil));
    
    QTreeWidgetItem* resetCoil = new QTreeWidgetItem(coilItem);
    resetCoil->setText(0, tr("复位线圈"));
    resetCoil->setData(0, Qt::UserRole, static_cast<int>(ElementType::ResetCoil));
    
    // 指令
    QTreeWidgetItem* instructionItem = new QTreeWidgetItem(m_elementLibrary);
    instructionItem->setText(0, tr("指令"));
    instructionItem->setExpanded(true);
    
    QTreeWidgetItem* timer = new QTreeWidgetItem(instructionItem);
    timer->setText(0, tr("定时器"));
    timer->setData(0, Qt::UserRole, static_cast<int>(ElementType::Timer));
    
    QTreeWidgetItem* counter = new QTreeWidgetItem(instructionItem);
    counter->setText(0, tr("计数器"));
    counter->setData(0, Qt::UserRole, static_cast<int>(ElementType::Counter));
}

void MainWindow::onElementLibraryItemClicked(QTreeWidgetItem* item, int column) {
    Q_UNUSED(column)
    
    if (item && item->data(0, Qt::UserRole).isValid()) {
        ElementType type = static_cast<ElementType>(item->data(0, Qt::UserRole).toInt());
        addElementToScene(type);
    }
}

void MainWindow::onElementLibraryItemDoubleClicked(QTreeWidgetItem* item, int column) {
    onElementLibraryItemClicked(item, column);
}

void MainWindow::addElementToScene(ElementType type, const QPointF& pos) {
    LadderElement* element = nullptr;
    
    switch (type) {
        case ElementType::LeftPowerRail:
            element = new LeftPowerRail();
            break;
        case ElementType::RightPowerRail:
            element = new RightPowerRail();
            break;
        case ElementType::NormallyOpen:
            element = new NormallyOpenContact();
            break;
        case ElementType::NormallyClosed:
            element = new NormallyClosedContact();
            break;
        case ElementType::OutputCoil:
            element = new OutputCoil();
            break;
        case ElementType::SetCoil:
            element = new SetCoil();
            break;
        case ElementType::ResetCoil:
            element = new ResetCoil();
            break;
        case ElementType::Timer:
            element = new Timer();
            break;
        case ElementType::Counter:
            element = new Counter();
            break;
        default:
            return;
    }
    
    if (element) {
        QPointF targetPos = pos.isNull() ? m_view->mapToScene(m_view->viewport()->rect().center()) : pos;
        element->setPos(m_scene->snapToGrid(targetPos));
        m_scene->addElement(element);
        m_modified = true;
    }
}

void MainWindow::onNewFile() {
    if (maybeSave()) {
        m_scene->clearScene();
        m_currentFile.clear();
        m_modified = false;
        setWindowTitle(tr("梯形图编辑器"));
    }
}

void MainWindow::onOpenFile() {
    if (!maybeSave()) return;
    
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), QString(),
                                                    tr("梯形图文件 (*.ldjson);;所有文件 (*.*)"));
    if (!fileName.isEmpty()) {
        loadFile(fileName);
    }
}

void MainWindow::onSaveFile() {
    if (m_currentFile.isEmpty()) {
        onSaveFileAs();
    } else {
        saveFile(m_currentFile);
    }
}

void MainWindow::onSaveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存文件"), QString(),
                                                    tr("梯形图文件 (*.ldjson);;所有文件 (*.*)"));
    if (!fileName.isEmpty()) {
        saveFile(fileName);
    }
}

void MainWindow::onExit() {
    if (maybeSave()) {
        QApplication::quit();
    }
}

void MainWindow::onUndo() {
    m_scene->undoStack()->undo();
}

void MainWindow::onRedo() {
    m_scene->undoStack()->redo();
}

void MainWindow::onElementSelected() {
    // 元件选择处理已在 onSceneSelectionChanged 中实现
}

void MainWindow::onDelete() {
    // 由场景处理删除
}

void MainWindow::onSelectAll() {
    QGraphicsScene* scene = m_scene;
    for (auto* item : scene->items()) {
        item->setSelected(true);
    }
}

void MainWindow::onZoomIn() {
    m_view->scale(1.2, 1.2);
}

void MainWindow::onZoomOut() {
    m_view->scale(1.0 / 1.2, 1.0 / 1.2);
}

void MainWindow::onZoomReset() {
    m_view->resetTransform();
}

void MainWindow::onToggleGrid() {
    m_scene->setGridEnabled(m_actionToggleGrid->isChecked());
}

void MainWindow::onToggleConnectionMode() {
    m_scene->setConnectionMode(m_actionConnectionMode->isChecked());
    if (m_actionConnectionMode->isChecked()) {
        statusBar()->showMessage(tr("连线模式：点击元件的连接点开始和结束连线"));
    } else {
        statusBar()->showMessage(tr("就绪"));
    }
}

void MainWindow::onRunSimulation() {
    statusBar()->showMessage(tr("仿真运行中..."));
    m_actionRun->setEnabled(false);
    m_actionStop->setEnabled(true);
}

void MainWindow::onStopSimulation() {
    statusBar()->showMessage(tr("仿真已停止"));
    m_actionRun->setEnabled(true);
    m_actionStop->setEnabled(false);
}

void MainWindow::onGenerateCode() {
    QMessageBox::information(this, tr("生成代码"), tr("代码生成功能正在开发中"));
}

void MainWindow::onAbout() {
    QMessageBox::about(this, tr("关于梯形图编辑器"),
                       tr("<h2>梯形图编辑器 1.0</h2>"
                          "<p>基于 Qt 6.8.3 开发的梯形图组态软件</p>"
                          "<p>支持基本的梯形图元件编辑、连接和仿真功能</p>"));
}

void MainWindow::onSceneSelectionChanged() {
    auto selectedItems = m_scene->selectedItems();
    if (selectedItems.size() == 1) {
        if (auto* element = dynamic_cast<LadderElement*>(selectedItems.first())) {
            m_propertyEditor->setElement(element);
            return;
        }
    }
    m_propertyEditor->clear();
}

bool MainWindow::maybeSave() {
    if (!m_modified) return true;
    
    QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("梯形图编辑器"),
                                                           tr("文档已被修改，是否保存？"),
                                                           QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    
    switch (ret) {
        case QMessageBox::Save:
            return saveFile(m_currentFile);
        case QMessageBox::Cancel:
            return false;
        default:
            return true;
    }
}

bool MainWindow::saveFile(const QString& path) {
    QString filePath = path;
    if (filePath.isEmpty()) {
        filePath = QFileDialog::getSaveFileName(this, tr("保存文件"), QString(),
                                                tr("梯形图文件 (*.ldjson)"));
        if (filePath.isEmpty()) return false;
    }
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("错误"), tr("无法保存文件 %1").arg(filePath));
        return false;
    }
    
    file.write(m_scene->toJson());
    file.close();
    
    setCurrentFile(filePath);
    m_modified = false;
    statusBar()->showMessage(tr("文件已保存"), 2000);
    return true;
}

bool MainWindow::loadFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("错误"), tr("无法打开文件 %1").arg(path));
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    if (!m_scene->fromJson(data)) {
        QMessageBox::warning(this, tr("错误"), tr("无法解析文件 %1").arg(path));
        return false;
    }
    
    setCurrentFile(path);
    m_modified = false;
    statusBar()->showMessage(tr("文件已加载"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString& path) {
    m_currentFile = path;
    QString shownName = m_currentFile.isEmpty() ? tr("未命名") : QFileInfo(m_currentFile).fileName();
    setWindowTitle(tr("%1 - 梯形图编辑器").arg(shownName));
}

} // namespace LadderDiagram
