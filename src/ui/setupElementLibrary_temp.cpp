void RibbonMainWindow::setupElementLibrary() {
    // 设置树形控件样式 - 只显示一列，tooltip显示说明
    m_elementLibrary->setHeaderHidden(true);
    
    // 辅助函数：创建一级分类节点
    auto createCategory = [&](const QString& name, const QString& tooltip, QStyle::StandardPixmap icon) {
        QTreeWidgetItem* item = new QTreeWidgetItem(m_elementLibrary);
        item->setText(0, name);
        item->setToolTip(0, tooltip);
        item->setIcon(0, QApplication::style()->standardIcon(icon));
        item->setExpanded(false);
        return item;
    };
    
    // 辅助函数：创建二级元件节点
    auto createElement = [&](QTreeWidgetItem* parent, const QString& name, const QString& tooltip, ElementType type) {
        QTreeWidgetItem* item = new QTreeWidgetItem(parent);
        item->setText(0, "  " + name);  // 缩进显示
        item->setToolTip(0, tooltip);
        item->setData(0, Qt::UserRole, static_cast<int>(type));
        return item;
    };
    
    // ========== 1. 电源轨线 ==========
    QTreeWidgetItem* powerRail = createCategory(tr("电源轨线"), tr("能流边界 - 电源轨线"), QStyle::SP_ComputerIcon);
    createElement(powerRail, tr("左电源轨"), tr("| 能流起点，状态始终ON (TRUE)"), ElementType::LeftPowerRail);
    createElement(powerRail, tr("右电源轨"), tr("| 能流终点"), ElementType::RightPowerRail);
    
    // ========== 2. 触点 ==========
    QTreeWidgetItem* contact = createCategory(tr("触点"), tr("输入元件 - 控制能流通断"), QStyle::SP_DialogYesButton);
    createElement(contact, tr("常开触点"), tr("--| |-- 变量TRUE时导通，FALSE时断开"), ElementType::NormallyOpen);
    createElement(contact, tr("常闭触点"), tr("--|/|-- 变量FALSE时导通，TRUE时断开"), ElementType::NormallyClosed);
    createElement(contact, tr("正边沿触点"), tr("--|P|-- 检测到FALSE→TRUE上升沿时导通一个扫描周期"), ElementType::PositiveEdge);
    createElement(contact, tr("负边沿触点"), tr("--|N|-- 检测到TRUE→FALSE下降沿时导通一个扫描周期"), ElementType::NegativeEdge);
    
    // ========== 3. 线圈 ==========
    QTreeWidgetItem* coil = createCategory(tr("线圈"), tr("输出元件 - 将能流状态写入布尔变量"), QStyle::SP_DriveHDIcon);
    createElement(coil, tr("一般线圈"), tr("--( )-- 将左侧能流状态直接赋给变量"), ElementType::OutputCoil);
    createElement(coil, tr("取反线圈"), tr("--(/)-- 将左侧能流状态取反后赋给变量"), ElementType::InvertedCoil);
    createElement(coil, tr("置位线圈"), tr("--(S)-- 能流ON时将变量置TRUE并保持(锁存)"), ElementType::SetCoil);
    createElement(coil, tr("复位线圈"), tr("--(R)-- 能流ON时将变量置FALSE并解除锁存"), ElementType::ResetCoil);
    createElement(coil, tr("正边沿线圈"), tr("--(P)-- 检测到OFF→ON时使变量在一个扫描周期内为TRUE"), ElementType::PositiveEdgeCoil);
    createElement(coil, tr("负边沿线圈"), tr("--(N)-- 检测到ON→OFF时使变量在一个扫描周期内为TRUE"), ElementType::NegativeEdgeCoil);
    
    // ========== 4. 定时器 ==========
    QTreeWidgetItem* timer = createCategory(tr("定时器"), tr("时间控制功能块"), QStyle::SP_ComputerIcon);
    createElement(timer, tr("TON"), tr("通电延时定时器 - 输入ON后延时输出ON"), ElementType::Timer);
    createElement(timer, tr("TOF"), tr("断电延时定时器 - 输入OFF后延时输出OFF"), ElementType::TimerTOF);
    createElement(timer, tr("TP"), tr("脉冲定时器 - 输入ON后输出固定时长脉冲"), ElementType::TimerTP);
    
    // ========== 5. 计数器 ==========
    QTreeWidgetItem* counter = createCategory(tr("计数器"), tr("计数功能块"), QStyle::SP_FileDialogListView);
    createElement(counter, tr("CTU"), tr("加计数器 - 递增计数，到达预设值输出ON"), ElementType::Counter);
    createElement(counter, tr("CTD"), tr("减计数器 - 递减计数，到达零输出ON"), ElementType::CounterCTD);
    createElement(counter, tr("CTUD"), tr("加减计数器 - 可递增递减计数"), ElementType::CounterCTUD);
    
    // ========== 6. 边沿检测 ==========
    QTreeWidgetItem* edge = createCategory(tr("边沿检测"), tr("边沿检测功能块"), QStyle::SP_ArrowForward);
    createElement(edge, tr("R_TRIG"), tr("上升沿检测 - 检测到FALSE→TRUE时输出一个周期"), ElementType::RTrig);
    createElement(edge, tr("F_TRIG"), tr("下降沿检测 - 检测到TRUE→FALSE时输出一个周期"), ElementType::FTrig);
    
    // ========== 7. 双稳态 ==========
    QTreeWidgetItem* bistable = createCategory(tr("双稳态"), tr("RS/SR双稳态触发器"), QStyle::SP_DialogYesButton);
    createElement(bistable, tr("RS"), tr("置位优先触发器 - Set优先于Reset"), ElementType::RS);
    createElement(bistable, tr("SR"), tr("复位优先触发器 - Reset优先于Set"), ElementType::SR);
    
    // ========== 8. 逻辑运算 ==========
    QTreeWidgetItem* logic = createCategory(tr("逻辑运算"), tr("AND/OR/NOT逻辑运算"), QStyle::SP_DialogYesButton);
    createElement(logic, tr("AND"), tr("逻辑与 - 所有输入为TRUE时输出TRUE"), ElementType::LogicAND);
    createElement(logic, tr("OR"), tr("逻辑或 - 任一输入为TRUE时输出TRUE"), ElementType::LogicOR);
    createElement(logic, tr("NOT"), tr("逻辑非 - 输入取反"), ElementType::LogicNOT);
    
    // ========== 9. 运算功能 ==========
    QTreeWidgetItem* func = createCategory(tr("运算功能"), tr("比较和算术运算"), QStyle::SP_DialogNoButton);
    createElement(func, tr("比较"), tr("CMP = / <> / < / > / <= / >= 比较运算"), ElementType::Comparison);
    createElement(func, tr("算术"), tr("ADD/SUB/MUL/DIV/MOD算术运算"), ElementType::MathOperation);
    
    // ========== 10. 程序控制 ==========
    QTreeWidgetItem* control = createCategory(tr("程序控制"), tr("跳转/返回/标签控制"), QStyle::SP_ArrowRight);
    createElement(control, tr("跳转"), tr(">>LABEL 当条件TRUE时跳转到指定标签"), ElementType::Jump);
    createElement(control, tr("返回"), tr("RETURN 当条件TRUE时从当前POU返回"), ElementType::Return);
    createElement(control, tr("标签"), tr("LABEL: 作为跳转目标的标识符"), ElementType::Label);
    
    // 双击添加元件
    connect(m_elementLibrary, &QTreeWidget::itemDoubleClicked, this, [this](QTreeWidgetItem* item, int column) {
        Q_UNUSED(column)
        if (item && item->data(0, Qt::UserRole).isValid()) {
            ElementType type = static_cast<ElementType>(item->data(0, Qt::UserRole).toInt());
            addElementToScene(type);
        }
    });
}