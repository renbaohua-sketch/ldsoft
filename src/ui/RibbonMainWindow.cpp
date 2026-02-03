#include "RibbonMainWindow.h"
#include "../elements/ContactElements.h"
#include "../elements/FunctionBlockElements.h"
#include "../elements/LogicElements.h"
#include "../elements/MathElements.h"
#include "../elements/ControlElements.h"
#include "../codegen/STCodeGenerator.h"
#include "ThemeManager.h"
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QSplitter>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QStatusBar>

namespace LadderDiagram {

RibbonGroup::RibbonGroup(const QString& title, QWidget* parent)
    : QFrame(parent), m_title(title) {
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(0);
    m_layout = layout;
    setMaximumHeight(36);
}

QToolButton* RibbonGroup::addButton(const QString& text, const QString& iconPath, const QString& tooltip) {
    QToolButton* btn = new QToolButton(this);
    btn->setText(text);
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn->setFixedHeight(26);
    btn->setMinimumWidth(60);
    btn->setIconSize(QSize(14, 14));
    btn->setToolTip(tooltip.isEmpty() ? text : tooltip);
    
    m_layout->addWidget(btn);
    return btn;
}

QToolButton* RibbonGroup::addLargeButton(const QString& text, const QString& iconPath, const QString& tooltip) {
    return addButton(text, iconPath, tooltip);
}

void RibbonGroup::addSeparator() {
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::VLine);
    line->setFixedWidth(1);
    line->setFixedHeight(32);
    m_layout->addWidget(line);
}

void RibbonGroup::addWidget(QWidget* widget) {
    m_layout->addWidget(widget);
}

RibbonPage::RibbonPage(QWidget* parent) : QFrame(parent) {
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(2, 2, 2, 2);
    m_layout->setSpacing(2);
}

RibbonGroup* RibbonPage::addGroup(const QString& title) {
    RibbonGroup* group = new RibbonGroup(title, this);
    m_layout->addWidget(group);
    return group;
}

RibbonMainWindow::RibbonMainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(tr("梯形图编辑器"));
    resize(1400, 900);
    
    // 初始化主题管理器
    ThemeManager::instance().init();
    
    // 先创建所有UI控件
    setupUI();
    setupRibbon();
    setupStatusBar();
    setupConnections();
    
    // 所有控件创建完成后再应用主题（确保样式表能应用到所有子控件）
    // 使用 QApplication 级别的样式表确保全局生效
    qApp->setStyleSheet(ThemeManager::instance().getStyleSheet());
}

RibbonMainWindow::~RibbonMainWindow() = default;

void RibbonMainWindow::setupUI() {
    QWidget* mainWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Ribbon - 高度 76px (36px Tab栏 + 40px 内容面板)
    m_ribbonContainer = new QFrame(this);
    m_ribbonContainer->setFixedHeight(76);
    m_ribbonContainer->setObjectName("ribbonContainer");
    m_ribbonContainer->setFrameStyle(QFrame::NoFrame);
    
    // 添加Ribbon整体阴影效果，使其悬浮
    QGraphicsDropShadowEffect* ribbonShadow = new QGraphicsDropShadowEffect(m_ribbonContainer);
    ribbonShadow->setBlurRadius(20);
    ribbonShadow->setColor(QColor(0, 0, 0, 80));
    ribbonShadow->setOffset(0, 6);
    m_ribbonContainer->setGraphicsEffect(ribbonShadow);
    
    mainLayout->addWidget(m_ribbonContainer);
    
    // 主内容区
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    
    m_elementLibrary = new QTreeWidget();
    m_elementLibrary->setObjectName("elementLibrary");
    m_elementLibrary->setHeaderHidden(true);
    m_elementLibrary->setMinimumWidth(180);
    m_elementLibrary->setMaximumWidth(260);
    setupElementLibrary();
    
    m_scene = new LadderScene(this);
    m_view = new LadderView(this);
    m_view->setScene(m_scene);
    
    m_propertyEditor = new PropertyEditor();
    m_propertyEditor->setObjectName("propertyEditor");
    m_propertyEditor->setMinimumWidth(200);
    m_propertyEditor->setMaximumWidth(300);
    
    splitter->addWidget(m_elementLibrary);
    splitter->addWidget(m_view);
    splitter->addWidget(m_propertyEditor);
    splitter->setSizes({200, 1000, 220});
    
    mainLayout->addWidget(splitter, 1);
    setCentralWidget(mainWidget);
}

void RibbonMainWindow::setupElementLibrary() {
    // 设置树形控件样式和列数
    m_elementLibrary->setColumnCount(2);
    m_elementLibrary->setHeaderLabels({tr("元件库"), tr("说明")});
    m_elementLibrary->setColumnWidth(0, 140);
    m_elementLibrary->setColumnWidth(1, 180);
    
    // ========== 1. 电源轨线 (Power Rails) ==========
    QTreeWidgetItem* powerRailItem = new QTreeWidgetItem(m_elementLibrary);
    powerRailItem->setText(0, tr("电源轨线"));
    powerRailItem->setText(1, "");
    powerRailItem->setToolTip(0, tr("能流边界"));
    powerRailItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_ToolBarVerticalExtensionButton));
    powerRailItem->setExpanded(false);

    QTreeWidgetItem* leftRail = new QTreeWidgetItem(powerRailItem);
    leftRail->setText(0, tr("  左电源轨"));
    leftRail->setText(1, tr("|"));
    leftRail->setToolTip(0, tr("能流起点，状态始终ON"));
    leftRail->setData(0, Qt::UserRole, static_cast<int>(ElementType::LeftPowerRail));

    QTreeWidgetItem* rightRail = new QTreeWidgetItem(powerRailItem);
    rightRail->setText(0, tr("  右电源轨"));
    rightRail->setText(1, tr("|"));
    rightRail->setToolTip(0, tr("能流终点"));
    rightRail->setData(0, Qt::UserRole, static_cast<int>(ElementType::RightPowerRail));
    
    // ========== 2. 触点 (Contacts) ==========
    QTreeWidgetItem* contactItem = new QTreeWidgetItem(m_elementLibrary);
    contactItem->setText(0, tr("触点"));
    contactItem->setText(1, "");
    contactItem->setToolTip(0, tr("输入元件"));
    contactItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_TitleBarMinButton));
    contactItem->setExpanded(false);

    QTreeWidgetItem* noContact = new QTreeWidgetItem(contactItem);
    noContact->setText(0, tr("  常开触点"));
    noContact->setText(1, tr("--| |--"));
    noContact->setToolTip(0, tr("变量TRUE时导通，FALSE时断开"));
    noContact->setData(0, Qt::UserRole, static_cast<int>(ElementType::NormallyOpen));

    QTreeWidgetItem* ncContact = new QTreeWidgetItem(contactItem);
    ncContact->setText(0, tr("  常闭触点"));
    ncContact->setText(1, tr("--|/|--"));
    ncContact->setToolTip(0, tr("变量FALSE时导通，TRUE时断开"));
    ncContact->setData(0, Qt::UserRole, static_cast<int>(ElementType::NormallyClosed));

    QTreeWidgetItem* posEdge = new QTreeWidgetItem(contactItem);
    posEdge->setText(0, tr("  正边沿触点"));
    posEdge->setText(1, tr("--|P|--"));
    posEdge->setToolTip(0, tr("检测变量从FALSE变为TRUE的上升沿"));
    posEdge->setData(0, Qt::UserRole, static_cast<int>(ElementType::PositiveEdge));

    QTreeWidgetItem* negEdge = new QTreeWidgetItem(contactItem);
    negEdge->setText(0, tr("  负边沿触点"));
    negEdge->setText(1, tr("--|N|--"));
    negEdge->setToolTip(0, tr("检测变量从TRUE变为FALSE的下降沿"));
    negEdge->setData(0, Qt::UserRole, static_cast<int>(ElementType::NegativeEdge));
    
    // ========== 3. 线圈 (Coils) ==========
    QTreeWidgetItem* coilItem = new QTreeWidgetItem(m_elementLibrary);
    coilItem->setText(0, tr("线圈"));
    coilItem->setText(1, "");
    coilItem->setToolTip(0, tr("输出元件"));
    coilItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    coilItem->setExpanded(false);

    QTreeWidgetItem* outputCoil = new QTreeWidgetItem(coilItem);
    outputCoil->setText(0, tr("  一般线圈"));
    outputCoil->setText(1, tr("--( )--"));
    outputCoil->setToolTip(0, tr("直接将能流状态赋值给变量"));
    outputCoil->setData(0, Qt::UserRole, static_cast<int>(ElementType::OutputCoil));

    QTreeWidgetItem* invCoil = new QTreeWidgetItem(coilItem);
    invCoil->setText(0, tr("  取反线圈"));
    invCoil->setText(1, tr("--(/)--"));
    invCoil->setToolTip(0, tr("将能流状态取反后赋值给变量"));
    invCoil->setData(0, Qt::UserRole, static_cast<int>(ElementType::InvertedCoil));

    QTreeWidgetItem* setCoil = new QTreeWidgetItem(coilItem);
    setCoil->setText(0, tr("  置位线圈"));
    setCoil->setText(1, tr("--(S)--"));
    setCoil->setToolTip(0, tr("能流为TRUE时置位变量为TRUE"));
    setCoil->setData(0, Qt::UserRole, static_cast<int>(ElementType::SetCoil));

    QTreeWidgetItem* resetCoil = new QTreeWidgetItem(coilItem);
    resetCoil->setText(0, tr("  复位线圈"));
    resetCoil->setText(1, tr("--(R)--"));
    resetCoil->setToolTip(0, tr("能流为TRUE时复位变量为FALSE"));
    resetCoil->setData(0, Qt::UserRole, static_cast<int>(ElementType::ResetCoil));

    QTreeWidgetItem* posEdgeCoil = new QTreeWidgetItem(coilItem);
    posEdgeCoil->setText(0, tr("  正边沿线圈"));
    posEdgeCoil->setText(1, tr("--(P)--"));
    posEdgeCoil->setToolTip(0, tr("检测能流上升沿，输出一个扫描周期脉冲"));
    posEdgeCoil->setData(0, Qt::UserRole, static_cast<int>(ElementType::PositiveEdgeCoil));

    QTreeWidgetItem* negEdgeCoil = new QTreeWidgetItem(coilItem);
    negEdgeCoil->setText(0, tr("  负边沿线圈"));
    negEdgeCoil->setText(1, tr("--(N)--"));
    negEdgeCoil->setToolTip(0, tr("检测能流下降沿，输出一个扫描周期脉冲"));
    negEdgeCoil->setData(0, Qt::UserRole, static_cast<int>(ElementType::NegativeEdgeCoil));
    
    // ========== 4. 定时器 (Timers) ==========
    QTreeWidgetItem* timerItem = new QTreeWidgetItem(m_elementLibrary);
    timerItem->setText(0, tr("定时器"));
    timerItem->setText(1, "");
    timerItem->setToolTip(0, tr("时间控制"));
    timerItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_MediaSeekForward));
    timerItem->setExpanded(false);

    QTreeWidgetItem* tonTimer = new QTreeWidgetItem(timerItem);
    tonTimer->setText(0, tr("  TON"));
    tonTimer->setText(1, tr("[TON]"));
    tonTimer->setToolTip(0, tr("通电延时定时器：输入为TRUE且持续设定时间后输出TRUE"));
    tonTimer->setData(0, Qt::UserRole, static_cast<int>(ElementType::Timer));

    QTreeWidgetItem* tofTimer = new QTreeWidgetItem(timerItem);
    tofTimer->setText(0, tr("  TOF"));
    tofTimer->setText(1, tr("[TOF]"));
    tofTimer->setToolTip(0, tr("断电延时定时器：输入变为FALSE后延迟设定时间才输出FALSE"));
    tofTimer->setData(0, Qt::UserRole, static_cast<int>(ElementType::TimerTOF));

    QTreeWidgetItem* tpTimer = new QTreeWidgetItem(timerItem);
    tpTimer->setText(0, tr("  TP"));
    tpTimer->setText(1, tr("[TP]"));
    tpTimer->setToolTip(0, tr("脉冲定时器：输入为TRUE时输出固定时长脉冲"));
    tpTimer->setData(0, Qt::UserRole, static_cast<int>(ElementType::TimerTP));
    
    // ========== 5. 计数器 (Counters) ==========
    QTreeWidgetItem* counterItem = new QTreeWidgetItem(m_elementLibrary);
    counterItem->setText(0, tr("计数器"));
    counterItem->setText(1, "");
    counterItem->setToolTip(0, tr("计数功能"));
    counterItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
    counterItem->setExpanded(false);

    QTreeWidgetItem* ctuCounter = new QTreeWidgetItem(counterItem);
    ctuCounter->setText(0, tr("  CTU"));
    ctuCounter->setText(1, tr("[CTU]"));
    ctuCounter->setToolTip(0, tr("加计数器：输入脉冲上升沿时计数值加1，达到设定值输出TRUE"));
    ctuCounter->setData(0, Qt::UserRole, static_cast<int>(ElementType::Counter));

    QTreeWidgetItem* ctdCounter = new QTreeWidgetItem(counterItem);
    ctdCounter->setText(0, tr("  CTD"));
    ctdCounter->setText(1, tr("[CTD]"));
    ctdCounter->setToolTip(0, tr("减计数器：输入脉冲上升沿时计数值减1，达到0输出TRUE"));
    ctdCounter->setData(0, Qt::UserRole, static_cast<int>(ElementType::CounterCTD));

    QTreeWidgetItem* ctudCounter = new QTreeWidgetItem(counterItem);
    ctudCounter->setText(0, tr("  CTUD"));
    ctudCounter->setText(1, tr("[CTUD]"));
    ctudCounter->setToolTip(0, tr("加减计数器：支持加计数和减计数，双向计数功能"));
    ctudCounter->setData(0, Qt::UserRole, static_cast<int>(ElementType::CounterCTUD));
    
    // ========== 6. 边沿检测 (Edge Detection) ==========
    QTreeWidgetItem* edgeItem = new QTreeWidgetItem(m_elementLibrary);
    edgeItem->setText(0, tr("边沿检测"));
    edgeItem->setText(1, "");
    edgeItem->setToolTip(0, tr("触发器"));
    edgeItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_ArrowUp));
    edgeItem->setExpanded(false);

    QTreeWidgetItem* rTrig = new QTreeWidgetItem(edgeItem);
    rTrig->setText(0, tr("  R_TRIG"));
    rTrig->setText(1, tr("[R_TRIG]"));
    rTrig->setToolTip(0, tr("上升沿检测：检测输入从FALSE变为TRUE时输出一个扫描周期脉冲"));
    rTrig->setData(0, Qt::UserRole, static_cast<int>(ElementType::RTrig));

    QTreeWidgetItem* fTrig = new QTreeWidgetItem(edgeItem);
    fTrig->setText(0, tr("  F_TRIG"));
    fTrig->setText(1, tr("[F_TRIG]"));
    fTrig->setToolTip(0, tr("下降沿检测：检测输入从TRUE变为FALSE时输出一个扫描周期脉冲"));
    fTrig->setData(0, Qt::UserRole, static_cast<int>(ElementType::FTrig));
    
    // ========== 7. 双稳态功能块 (Bistable) ==========
    QTreeWidgetItem* bistableItem = new QTreeWidgetItem(m_elementLibrary);
    bistableItem->setText(0, tr("双稳态"));
    bistableItem->setText(1, "");
    bistableItem->setToolTip(0, tr("RS/SR触发器"));
    bistableItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
    bistableItem->setExpanded(false);

    QTreeWidgetItem* rsBlock = new QTreeWidgetItem(bistableItem);
    rsBlock->setText(0, tr("  RS"));
    rsBlock->setText(1, tr("[RS]"));
    rsBlock->setToolTip(0, tr("置位优先触发器：S和R同时为TRUE时输出置位状态(TRUE)"));
    rsBlock->setData(0, Qt::UserRole, static_cast<int>(ElementType::RS));

    QTreeWidgetItem* srBlock = new QTreeWidgetItem(bistableItem);
    srBlock->setText(0, tr("  SR"));
    srBlock->setText(1, tr("[SR]"));
    srBlock->setToolTip(0, tr("复位优先触发器：S和R同时为TRUE时输出复位状态(FALSE)"));
    srBlock->setData(0, Qt::UserRole, static_cast<int>(ElementType::SR));
    
    // ========== 8. 逻辑运算 (Logic Operations) ==========
    QTreeWidgetItem* logicItem = new QTreeWidgetItem(m_elementLibrary);
    logicItem->setText(0, tr("逻辑运算"));
    logicItem->setText(1, "");
    logicItem->setToolTip(0, tr("布尔运算"));
    logicItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_CommandLink));
    logicItem->setExpanded(false);

    QTreeWidgetItem* andFunc = new QTreeWidgetItem(logicItem);
    andFunc->setText(0, tr("  AND"));
    andFunc->setText(1, tr("[AND]"));
    andFunc->setToolTip(0, tr("逻辑与：所有输入为TRUE时输出TRUE"));
    andFunc->setData(0, Qt::UserRole, static_cast<int>(ElementType::LogicAND));

    QTreeWidgetItem* orFunc = new QTreeWidgetItem(logicItem);
    orFunc->setText(0, tr("  OR"));
    orFunc->setText(1, tr("[OR]"));
    orFunc->setToolTip(0, tr("逻辑或：任一输入为TRUE时输出TRUE"));
    orFunc->setData(0, Qt::UserRole, static_cast<int>(ElementType::LogicOR));

    QTreeWidgetItem* notFunc = new QTreeWidgetItem(logicItem);
    notFunc->setText(0, tr("  NOT"));
    notFunc->setText(1, tr("[NOT]"));
    notFunc->setToolTip(0, tr("逻辑非：输入取反后输出"));
    notFunc->setData(0, Qt::UserRole, static_cast<int>(ElementType::LogicNOT));
    
    // ========== 9. 比较与运算 (Comparison & Math) ==========
    QTreeWidgetItem* funcItem = new QTreeWidgetItem(m_elementLibrary);
    funcItem->setText(0, tr("功能运算"));
    funcItem->setText(1, "");
    funcItem->setToolTip(0, tr("比较/算术"));
    funcItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DialogYesButton));
    funcItem->setExpanded(false);

    QTreeWidgetItem* cmpFunc = new QTreeWidgetItem(funcItem);
    cmpFunc->setText(0, tr("  比较"));
    cmpFunc->setText(1, tr("[CMP]"));
    cmpFunc->setToolTip(0, tr("比较运算：支持等于(=)、不等于(<>)、小于(<)、大于(>)等操作"));
    cmpFunc->setData(0, Qt::UserRole, static_cast<int>(ElementType::Comparison));

    QTreeWidgetItem* mathFunc = new QTreeWidgetItem(funcItem);
    mathFunc->setText(0, tr("  算术运算"));
    mathFunc->setText(1, tr("[MATH]"));
    mathFunc->setToolTip(0, tr("算术运算：支持加(ADD)、减(SUB)、乘(MUL)、除(DIV)等操作"));
    mathFunc->setData(0, Qt::UserRole, static_cast<int>(ElementType::MathOperation));
    
    // ========== 10. 程序控制 (Program Control) ==========
    QTreeWidgetItem* controlItem = new QTreeWidgetItem(m_elementLibrary);
    controlItem->setText(0, tr("程序控制"));
    controlItem->setText(1, "");
    controlItem->setToolTip(0, tr("跳转/返回"));
    controlItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_ArrowForward));
    controlItem->setExpanded(false);

    QTreeWidgetItem* jumpCtrl = new QTreeWidgetItem(controlItem);
    jumpCtrl->setText(0, tr("  跳转"));
    jumpCtrl->setText(1, tr(">>"));
    jumpCtrl->setToolTip(0, tr("条件跳转：能流为TRUE时跳转到指定标签位置"));
    jumpCtrl->setData(0, Qt::UserRole, static_cast<int>(ElementType::Jump));

    QTreeWidgetItem* retCtrl = new QTreeWidgetItem(controlItem);
    retCtrl->setText(0, tr("  返回"));
    retCtrl->setText(1, tr("[RET]"));
    retCtrl->setToolTip(0, tr("条件返回：能流为TRUE时从当前函数返回"));
    retCtrl->setData(0, Qt::UserRole, static_cast<int>(ElementType::Return));

    QTreeWidgetItem* labelCtrl = new QTreeWidgetItem(controlItem);
    labelCtrl->setText(0, tr("  标签"));
    labelCtrl->setText(1, tr("[LBL]"));
    labelCtrl->setToolTip(0, tr("跳转目标：定义跳转指令的目标位置"));
    labelCtrl->setData(0, Qt::UserRole, static_cast<int>(ElementType::Label));
    
    // 双击添加元件
    connect(m_elementLibrary, &QTreeWidget::itemDoubleClicked, this, [this](QTreeWidgetItem* item, int column) {
        Q_UNUSED(column)
        if (item && item->data(0, Qt::UserRole).isValid()) {
            ElementType type = static_cast<ElementType>(item->data(0, Qt::UserRole).toInt());
            addElementToScene(type);
        }
    });
}

void RibbonMainWindow::setupRibbon() {
    QVBoxLayout* ribbonLayout = new QVBoxLayout(m_ribbonContainer);
    ribbonLayout->setContentsMargins(0, 0, 0, 0);
    ribbonLayout->setSpacing(0);
    
    // ========== 第1层：Tab 标签栏（36px）==========
    QFrame* tabBarFrame = new QFrame(this);
    tabBarFrame->setFixedHeight(36);
    tabBarFrame->setObjectName("tabBarFrame");
    
    QHBoxLayout* tabLayout = new QHBoxLayout(tabBarFrame);
    tabLayout->setContentsMargins(4, 2, 12, 2);
    tabLayout->setSpacing(0);
    tabLayout->setAlignment(Qt::AlignLeft);
    
    // Tab 按钮样式
    auto createTabBtn = [&](const QString& text, bool isPurple = false) {
        QToolButton* btn = new QToolButton(tabBarFrame);
        btn->setText(text);
        btn->setFixedHeight(28);
        btn->setMinimumWidth(50);
        btn->setMaximumWidth(70);
        btn->setCheckable(true);
        btn->setAutoExclusive(true);
        Q_UNUSED(isPurple)
        return btn;
    };
    
    // 文件 Tab（紫色）
    QToolButton* fileTabBtn = createTabBtn(tr("文件"), true);
    tabLayout->addWidget(fileTabBtn);
    
    // 主页 Tab
    QToolButton* homeTabBtn = createTabBtn(tr("主页"), false);
    homeTabBtn->setChecked(true);
    tabLayout->addWidget(homeTabBtn);
    
    // 插入 Tab
    QToolButton* insertTabBtn = createTabBtn(tr("插入"), false);
    tabLayout->addWidget(insertTabBtn);
    
    // 视图 Tab
    QToolButton* viewTabBtn = createTabBtn(tr("视图"), false);
    tabLayout->addWidget(viewTabBtn);

    // 运行 Tab
    QToolButton* runTabBtn = createTabBtn(tr("运行"), false);
    tabLayout->addWidget(runTabBtn);
    
    // 保存 Tab 按钮指针用于切换
    m_fileTabBtn = fileTabBtn;
    m_homeTabBtn = homeTabBtn;
    m_insertTabBtn = insertTabBtn;
    m_viewTabBtn = viewTabBtn;
    m_runTabBtn = runTabBtn;
    
    // Tab 切换连接
    connect(fileTabBtn, &QToolButton::clicked, this, [this]() { switchToTab(0); });
    connect(homeTabBtn, &QToolButton::clicked, this, [this]() { switchToTab(1); });
    connect(insertTabBtn, &QToolButton::clicked, this, [this]() { switchToTab(2); });
    connect(viewTabBtn, &QToolButton::clicked, this, [this]() { switchToTab(3); });
    connect(runTabBtn, &QToolButton::clicked, this, [this]() { switchToTab(4); });
    
    ribbonLayout->addWidget(tabBarFrame);
    
    // ========== 第2层：Tab 内容面板（40px）==========
    m_tabStack = new QStackedWidget(this);
    m_tabStack->setObjectName("tabStack");
    m_tabStack->setFixedHeight(40);
    // 设置工具栏背景样式
    m_tabStack->setStyleSheet("QStackedWidget#tabStack { background-color: #FAFAFA; border-top: 1px solid #E0E0E0; }");
    
    // 添加工具栏阴影效果
    QGraphicsDropShadowEffect* toolbarShadow = new QGraphicsDropShadowEffect(m_tabStack);
    toolbarShadow->setBlurRadius(6);
    toolbarShadow->setColor(QColor(0, 0, 0, 40));
    toolbarShadow->setOffset(0, 2);
    m_tabStack->setGraphicsEffect(toolbarShadow);
    
    // 文件面板
    QWidget* filePanel = createFilePanel();
    m_tabStack->addWidget(filePanel);
    
    // 主页面板
    QWidget* homePanel = createHomePanel();
    m_tabStack->addWidget(homePanel);
    
    // 插入面板
    QWidget* insertPanel = createInsertPanel();
    m_tabStack->addWidget(insertPanel);
    
    // 视图面板
    QWidget* viewPanel = createViewPanel();
    m_tabStack->addWidget(viewPanel);
    
    // 运行面板
    QWidget* runPanel = createRunPanel();
    m_tabStack->addWidget(runPanel);
    
    // 默认显示主页
    m_tabStack->setCurrentIndex(1);
    
    ribbonLayout->addWidget(m_tabStack);
}

QWidget* RibbonMainWindow::createFilePanel() {
    QFrame* panel = new QFrame();
    panel->setStyleSheet("QFrame { background-color: transparent; border: none; }");
    QHBoxLayout* layout = new QHBoxLayout(panel);
    layout->setContentsMargins(32, 4, 32, 4);
    layout->setSpacing(8);
    
    // 文件操作组
    RibbonGroup* fileGroup = new RibbonGroup(tr("文件操作"), panel);
    m_buttons.newFile = fileGroup->addButton(tr("新建"), "", tr("新建文件"));
    m_buttons.newFile->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    connect(m_buttons.newFile, &QToolButton::clicked, this, &RibbonMainWindow::onNewFile);
    
    m_buttons.openFile = fileGroup->addButton(tr("打开"), "", tr("打开文件"));
    m_buttons.openFile->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon));
    connect(m_buttons.openFile, &QToolButton::clicked, this, &RibbonMainWindow::onOpenFile);
    
    m_buttons.saveFile = fileGroup->addButton(tr("保存"), "", tr("保存文件"));
    m_buttons.saveFile->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton));
    connect(m_buttons.saveFile, &QToolButton::clicked, this, &RibbonMainWindow::onSaveFile);
    
    QToolButton* saveAsBtn = fileGroup->addButton(tr("另存为"), "", tr("另存为"));
    saveAsBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton));
    connect(saveAsBtn, &QToolButton::clicked, this, &RibbonMainWindow::onSaveFileAs);
    
    fileGroup->addSeparator();
    
    // 编辑操作组
    m_buttons.undo = fileGroup->addButton(tr("撤销"), "", tr("撤销"));
    m_buttons.undo->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowBack));
    m_buttons.undo->setEnabled(false);
    connect(m_buttons.undo, &QToolButton::clicked, this, &RibbonMainWindow::onUndo);
    
    m_buttons.redo = fileGroup->addButton(tr("重做"), "", tr("重做"));
    m_buttons.redo->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowForward));
    m_buttons.redo->setEnabled(false);
    connect(m_buttons.redo, &QToolButton::clicked, this, &RibbonMainWindow::onRedo);
    
    m_buttons.deleteItem = fileGroup->addButton(tr("删除"), "", tr("删除"));
    m_buttons.deleteItem->setIcon(QApplication::style()->standardIcon(QStyle::SP_TrashIcon));
    connect(m_buttons.deleteItem, &QToolButton::clicked, this, &RibbonMainWindow::onDelete);
    
    fileGroup->addSeparator();
    
    // 缩放操作组
    m_buttons.zoomIn = fileGroup->addButton(tr("放大"), "", tr("放大"));
    m_buttons.zoomIn->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowUp));
    connect(m_buttons.zoomIn, &QToolButton::clicked, this, &RibbonMainWindow::onZoomIn);
    
    m_buttons.zoomOut = fileGroup->addButton(tr("缩小"), "", tr("缩小"));
    m_buttons.zoomOut->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowDown));
    connect(m_buttons.zoomOut, &QToolButton::clicked, this, &RibbonMainWindow::onZoomOut);
    
    layout->addWidget(fileGroup);
    layout->addStretch();
    return panel;
}

QWidget* RibbonMainWindow::createHomePanel() {
    QFrame* panel = new QFrame();
    panel->setStyleSheet("QFrame { background-color: transparent; border: none; }");
    QHBoxLayout* layout = new QHBoxLayout(panel);
    layout->setContentsMargins(32, 4, 32, 4);
    layout->setSpacing(8);
    
    RibbonGroup* editGroup = new RibbonGroup(tr("编辑"), panel);
    
    QToolButton* cutBtn = editGroup->addButton(tr("剪切"), "", tr("剪切"));
    cutBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_CommandLink));
    connect(cutBtn, &QToolButton::clicked, this, &RibbonMainWindow::onCut);
    
    QToolButton* copyBtn = editGroup->addButton(tr("复制"), "", tr("复制"));
    copyBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogContentsView));
    connect(copyBtn, &QToolButton::clicked, this, &RibbonMainWindow::onCopy);
    
    QToolButton* pasteBtn = editGroup->addButton(tr("粘贴"), "", tr("粘贴"));
    pasteBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
    connect(pasteBtn, &QToolButton::clicked, this, &RibbonMainWindow::onPaste);
    
    QToolButton* selectAllBtn = editGroup->addButton(tr("全选"), "", tr("全选"));
    selectAllBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogListView));
    connect(selectAllBtn, &QToolButton::clicked, this, &RibbonMainWindow::onSelectAll);
    
    layout->addWidget(editGroup);
    layout->addStretch();
    return panel;
}

QWidget* RibbonMainWindow::createInsertPanel() {
    QFrame* panel = new QFrame();
    panel->setStyleSheet("QFrame { background-color: transparent; border: none; }");
    QHBoxLayout* layout = new QHBoxLayout(panel);
    layout->setContentsMargins(32, 4, 32, 4);
    layout->setSpacing(8);
    
    // 触点组
    RibbonGroup* contactGroup = new RibbonGroup(tr("触点"), panel);
    QToolButton* noBtn = contactGroup->addButton(tr("常开"), "", tr("常开触点"));
    noBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogYesButton));
    connect(noBtn, &QToolButton::clicked, this, &RibbonMainWindow::onAddContactNO);
    
    QToolButton* ncBtn = contactGroup->addButton(tr("常闭"), "", tr("常闭触点"));
    ncBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogNoButton));
    connect(ncBtn, &QToolButton::clicked, this, &RibbonMainWindow::onAddContactNC);
    
    // 线圈组
    RibbonGroup* coilGroup = new RibbonGroup(tr("线圈"), panel);
    QToolButton* outBtn = coilGroup->addButton(tr("输出"), "", tr("输出线圈"));
    outBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon));
    connect(outBtn, &QToolButton::clicked, this, &RibbonMainWindow::onAddOutputCoil);
    
    QToolButton* setBtn = coilGroup->addButton(tr("置位"), "", tr("置位线圈"));
    setBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
    connect(setBtn, &QToolButton::clicked, this, &RibbonMainWindow::onAddSetCoil);
    
    QToolButton* resetBtn = coilGroup->addButton(tr("复位"), "", tr("复位线圈"));
    resetBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
    connect(resetBtn, &QToolButton::clicked, this, &RibbonMainWindow::onAddResetCoil);
    
    // 电源组
    RibbonGroup* railGroup = new RibbonGroup(tr("电源"), panel);
    QToolButton* leftBtn = railGroup->addButton(tr("左轨"), "", tr("左电源轨"));
    leftBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowLeft));
    connect(leftBtn, &QToolButton::clicked, this, &RibbonMainWindow::onAddLeftRail);
    
    QToolButton* rightBtn = railGroup->addButton(tr("右轨"), "", tr("右电源轨"));
    rightBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowRight));
    connect(rightBtn, &QToolButton::clicked, this, &RibbonMainWindow::onAddRightRail);
    
    // 指令组
    RibbonGroup* instrGroup = new RibbonGroup(tr("指令"), panel);
    QToolButton* timerBtn = instrGroup->addButton(tr("定时器"), "", tr("定时器"));
    timerBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
    connect(timerBtn, &QToolButton::clicked, this, &RibbonMainWindow::onAddTimer);
    
    QToolButton* counterBtn = instrGroup->addButton(tr("计数器"), "", tr("计数器"));
    counterBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogListView));
    connect(counterBtn, &QToolButton::clicked, this, &RibbonMainWindow::onAddCounter);
    
    layout->addWidget(contactGroup);
    layout->addWidget(coilGroup);
    layout->addWidget(railGroup);
    layout->addWidget(instrGroup);
    layout->addStretch();
    return panel;
}

QWidget* RibbonMainWindow::createViewPanel() {
    QFrame* panel = new QFrame();
    panel->setStyleSheet("QFrame { background-color: transparent; border: none; }");
    QHBoxLayout* layout = new QHBoxLayout(panel);
    layout->setContentsMargins(32, 4, 32, 4);
    layout->setSpacing(8);
    
    RibbonGroup* displayGroup = new RibbonGroup(tr("显示"), panel);
    
    m_buttons.toggleGrid = displayGroup->addButton(tr("网格"), "", tr("显示网格"));
    m_buttons.toggleGrid->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    m_buttons.toggleGrid->setCheckable(true);
    m_buttons.toggleGrid->setChecked(true);
    connect(m_buttons.toggleGrid, &QToolButton::clicked, this, &RibbonMainWindow::onToggleGrid);
    
    m_buttons.connectionMode = displayGroup->addButton(tr("连线"), "", tr("连线模式"));
    m_buttons.connectionMode->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOkButton));
    m_buttons.connectionMode->setCheckable(true);
    connect(m_buttons.connectionMode, &QToolButton::clicked, this, &RibbonMainWindow::onToggleConnectionMode);
    
    layout->addWidget(displayGroup);
    
    layout->addSpacing(16);
    
    // 主题切换组
    RibbonGroup* themeGroup = new RibbonGroup(tr("主题"), panel);
    QToolButton* themeBtn = themeGroup->addButton(tr("切换"), "", tr("切换浅色/暗色主题"));
    themeBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
    connect(themeBtn, &QToolButton::clicked, this, &RibbonMainWindow::onToggleTheme);
    
    layout->addWidget(themeGroup);
    layout->addStretch();
    return panel;
}

QWidget* RibbonMainWindow::createRunPanel() {
    QFrame* panel = new QFrame();
    panel->setStyleSheet("QFrame { background-color: transparent; border: none; }");
    QHBoxLayout* layout = new QHBoxLayout(panel);
    layout->setContentsMargins(32, 4, 32, 4);
    layout->setSpacing(8);
    
    RibbonGroup* runGroup = new RibbonGroup(tr("仿真控制"), panel);
    
    m_buttons.runSim = runGroup->addButton(tr("开始"), "", tr("开始仿真"));
    m_buttons.runSim->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
    m_buttons.runSim->setObjectName("runButton");
    connect(m_buttons.runSim, &QToolButton::clicked, this, &RibbonMainWindow::onRunSimulation);
    
    m_buttons.stopSim = runGroup->addButton(tr("停止"), "", tr("停止仿真"));
    m_buttons.stopSim->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaStop));
    m_buttons.stopSim->setObjectName("stopButton");
    m_buttons.stopSim->setEnabled(false);
    connect(m_buttons.stopSim, &QToolButton::clicked, this, &RibbonMainWindow::onStopSimulation);
    
    layout->addWidget(runGroup);
    
    // 代码生成组
    RibbonGroup* codeGroup = new RibbonGroup(tr("代码生成"), panel);
    QToolButton* genCodeBtn = codeGroup->addButton(tr("生成ST"), "", tr("生成IEC 61131-3 ST语言代码"));
    genCodeBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    connect(genCodeBtn, &QToolButton::clicked, this, &RibbonMainWindow::onGenerateCode);
    
    layout->addWidget(codeGroup);
    layout->addStretch();
    return panel;
}

void RibbonMainWindow::switchToTab(int index) {
    m_tabStack->setCurrentIndex(index);
    
    // 更新 Tab 按钮状态
    m_fileTabBtn->setChecked(index == 0);
    m_homeTabBtn->setChecked(index == 1);
    m_insertTabBtn->setChecked(index == 2);
    m_viewTabBtn->setChecked(index == 3);
    m_runTabBtn->setChecked(index == 4);
}

void RibbonMainWindow::setupStatusBar() {
    QStatusBar* bar = new QStatusBar(this);
    bar->setObjectName("statusBar");
    bar->showMessage(tr("就绪"));
    setStatusBar(bar);
}

void RibbonMainWindow::setupConnections() {
    connect(m_scene->undoStack(), &QUndoStack::canUndoChanged, m_buttons.undo, &QToolButton::setEnabled);
    connect(m_scene->undoStack(), &QUndoStack::canRedoChanged, m_buttons.redo, &QToolButton::setEnabled);
    connect(m_scene, &QGraphicsScene::selectionChanged, this, &RibbonMainWindow::onSceneSelectionChanged);
}



void RibbonMainWindow::addElementToScene(ElementType type) {
    LadderElement* element = nullptr;
    
    switch (type) {
        // 电源轨线
        case ElementType::LeftPowerRail: element = new LeftPowerRail(); break;
        case ElementType::RightPowerRail: element = new RightPowerRail(); break;
        
        // 触点
        case ElementType::NormallyOpen: element = new NormallyOpenContact(); break;
        case ElementType::NormallyClosed: element = new NormallyClosedContact(); break;
        case ElementType::PositiveEdge: element = new PositiveEdgeContact(); break;
        case ElementType::NegativeEdge: element = new NegativeEdgeContact(); break;
        
        // 线圈
        case ElementType::OutputCoil: element = new OutputCoil(); break;
        case ElementType::InvertedCoil: element = new InvertedCoil(); break;
        case ElementType::SetCoil: element = new SetCoil(); break;
        case ElementType::ResetCoil: element = new ResetCoil(); break;
        case ElementType::PositiveEdgeCoil: element = new PositiveEdgeCoil(); break;
        case ElementType::NegativeEdgeCoil: element = new NegativeEdgeCoil(); break;
        
        // 定时器
        case ElementType::Timer: element = new Timer(); break;
        case ElementType::TimerTOF: {
            auto* timer = new Timer();
            timer->setTimerType(Timer::TOF);
            element = timer;
            break;
        }
        case ElementType::TimerTP: {
            auto* timer = new Timer();
            timer->setTimerType(Timer::TP);
            element = timer;
            break;
        }
        
        // 计数器
        case ElementType::Counter: element = new Counter(); break;
        case ElementType::CounterCTD: {
            auto* counter = new Counter();
            counter->setCounterType(Counter::CTD);
            element = counter;
            break;
        }
        case ElementType::CounterCTUD: {
            auto* counter = new Counter();
            counter->setCounterType(Counter::CTUD);
            element = counter;
            break;
        }
        
        // 功能块
        case ElementType::RTrig: element = new RTrig(); break;
        case ElementType::FTrig: element = new FTrig(); break;
        case ElementType::RS: element = new RS(); break;
        case ElementType::SR: element = new SR(); break;
        
        // 逻辑运算
        case ElementType::LogicAND: element = new LogicAND(); break;
        case ElementType::LogicOR: element = new LogicOR(); break;
        case ElementType::LogicNOT: element = new LogicNOT(); break;
        
        // 数学/比较
        case ElementType::Comparison: element = new Comparison(); break;
        case ElementType::MathOperation: element = new MathOperation(); break;
        
        // 程序控制
        case ElementType::Jump: element = new Jump(); break;
        case ElementType::Return: element = new Return(); break;
        case ElementType::Label: element = new Label(); break;
        
        default: return;
    }
    
    if (element) {
        QPointF pos = m_view->mapToScene(m_view->viewport()->rect().center());
        element->setPos(m_scene->snapToGrid(pos));
        m_scene->addElement(element);
        m_modified = true;
    }
}

// 槽函数实现
void RibbonMainWindow::onNewFile() {
    if (maybeSave()) {
        m_scene->clearScene();
        m_currentFile.clear();
        m_modified = false;
        setCurrentFile(QString());
    }
}

void RibbonMainWindow::onOpenFile() {
    if (!maybeSave()) return;
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), QString(),
                                                    tr("梯形图文件 (*.ldjson);;所有文件 (*.*)"));
    if (!fileName.isEmpty()) loadFile(fileName);
}

void RibbonMainWindow::onSaveFile() {
    if (m_currentFile.isEmpty()) onSaveFileAs();
    else saveFile(m_currentFile);
}

void RibbonMainWindow::onSaveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存文件"), QString(),
                                                    tr("梯形图文件 (*.ldjson);;所有文件 (*.*)"));
    if (!fileName.isEmpty()) saveFile(fileName);
}

void RibbonMainWindow::onExit() {
    if (maybeSave()) QApplication::quit();
}

void RibbonMainWindow::onUndo() { m_scene->undoStack()->undo(); }
void RibbonMainWindow::onRedo() { m_scene->undoStack()->redo(); }

void RibbonMainWindow::onDelete() {
    for (auto* item : m_scene->selectedItems()) {
        if (auto* element = dynamic_cast<LadderElement*>(item)) {
            m_scene->removeElement(element);
            delete element;
        } else if (auto* conn = dynamic_cast<ConnectionLine*>(item)) {
            m_scene->removeConnection(conn);
            delete conn;
        }
    }
}

void RibbonMainWindow::onCut() {}
void RibbonMainWindow::onCopy() {}
void RibbonMainWindow::onPaste() {}

void RibbonMainWindow::onSelectAll() {
    for (auto* item : m_scene->items()) item->setSelected(true);
}

void RibbonMainWindow::onZoomIn() { m_view->scale(1.2, 1.2); }
void RibbonMainWindow::onZoomOut() { m_view->scale(1.0 / 1.2, 1.0 / 1.2); }
void RibbonMainWindow::onZoomReset() { m_view->resetTransform(); }

void RibbonMainWindow::onToggleGrid() {
    m_scene->setGridEnabled(m_buttons.toggleGrid->isChecked());
}

void RibbonMainWindow::onToggleConnectionMode() {
    m_scene->setConnectionMode(m_buttons.connectionMode->isChecked());
    statusBar()->showMessage(m_buttons.connectionMode->isChecked() ? 
        tr("连线模式：点击元件的连接点开始和结束连线") : tr("就绪"));
}

void RibbonMainWindow::onAddContactNO() { addElementToScene(ElementType::NormallyOpen); }
void RibbonMainWindow::onAddContactNC() { addElementToScene(ElementType::NormallyClosed); }
void RibbonMainWindow::onAddOutputCoil() { addElementToScene(ElementType::OutputCoil); }
void RibbonMainWindow::onAddSetCoil() { addElementToScene(ElementType::SetCoil); }
void RibbonMainWindow::onAddResetCoil() { addElementToScene(ElementType::ResetCoil); }
void RibbonMainWindow::onAddTimer() { addElementToScene(ElementType::Timer); }
void RibbonMainWindow::onAddCounter() { addElementToScene(ElementType::Counter); }
void RibbonMainWindow::onAddLeftRail() { addElementToScene(ElementType::LeftPowerRail); }
void RibbonMainWindow::onAddRightRail() { addElementToScene(ElementType::RightPowerRail); }

void RibbonMainWindow::onRunSimulation() {
    statusBar()->showMessage(tr("仿真运行中..."));
    m_buttons.runSim->setEnabled(false);
    m_buttons.stopSim->setEnabled(true);
}

void RibbonMainWindow::onStopSimulation() {
    statusBar()->showMessage(tr("仿真已停止"));
    m_buttons.runSim->setEnabled(true);
    m_buttons.stopSim->setEnabled(false);
}

void RibbonMainWindow::onGenerateCode() {
    // 获取保存路径
    QString filePath = QFileDialog::getSaveFileName(this, tr("生成ST代码"), QString(),
                                                    tr("ST代码文件 (*.st);;所有文件 (*.*)"));
    if (filePath.isEmpty()) return;
    
    // 确保有.st扩展名
    if (!filePath.endsWith(".st", Qt::CaseInsensitive)) {
        filePath += ".st";
    }
    
    // 创建简单的ST代码
    QString stCode = "(* 梯形图生成的ST代码 *)\n";
    stCode += "PROGRAM LadderProgram\n\n";
    stCode += "VAR\n";
    stCode += "    Start : BOOL;\n";
    stCode += "    Stop : BOOL;\n";
    stCode += "    Motor : BOOL;\n";
    stCode += "END_VAR\n\n";
    stCode += "(* 主程序 *)\n";
    stCode += "Motor := Start AND NOT Stop;\n\n";
    stCode += "END_PROGRAM\n";
    
    // 保存到文件
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Utf8);
        stream << stCode;
        file.close();
        
        statusBar()->showMessage(tr("ST代码已生成: %1").arg(filePath), 5000);
        QMessageBox::information(this, tr("代码生成成功"), 
                                 tr("ST代码已保存到:\n%1").arg(filePath));
    } else {
        QMessageBox::warning(this, tr("生成失败"), tr("无法保存文件: %1").arg(filePath));
    }
}

void RibbonMainWindow::onAbout() {
    QMessageBox::about(this, tr("关于梯形图编辑器"),
                       tr("<h2>梯形图编辑器 1.0</h2>"
                          "<p>基于 Qt 6.8.3 开发的梯形图组态软件</p>"));
}

void RibbonMainWindow::onSceneSelectionChanged() {
    auto selectedItems = m_scene->selectedItems();
    if (selectedItems.size() == 1) {
        if (auto* element = dynamic_cast<LadderElement*>(selectedItems.first())) {
            m_propertyEditor->setElement(element);
            return;
        }
    }
    m_propertyEditor->clear();
}

void RibbonMainWindow::onToggleTheme() {
    ThemeManager::instance().toggleTheme();
    ThemeManager::instance().applyToWidget(this);
    ThemeManager::instance().saveSettings();
}

void RibbonMainWindow::onRibbonTabChanged(int index) { Q_UNUSED(index) }

bool RibbonMainWindow::maybeSave() {
    if (!m_modified) return true;
    
    auto ret = QMessageBox::warning(this, tr("梯形图编辑器"),
                                    tr("文档已被修改，是否保存？"),
                                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    
    switch (ret) {
        case QMessageBox::Save: return saveFile(m_currentFile);
        case QMessageBox::Cancel: return false;
        default: return true;
    }
}

bool RibbonMainWindow::saveFile(const QString& path) {
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

bool RibbonMainWindow::loadFile(const QString& path) {
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

void RibbonMainWindow::setCurrentFile(const QString& path) {
    m_currentFile = path;
    QString shownName = m_currentFile.isEmpty() ? tr("未命名") : QFileInfo(m_currentFile).fileName();
    setWindowTitle(tr("%1 - 梯形图编辑器").arg(shownName));
}

void RibbonMainWindow::updateActionStates() {}

} // namespace LadderDiagram
