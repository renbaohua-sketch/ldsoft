#include "ContactElements.h"
#include <QPainter>

namespace LadderDiagram {

// 常开触点
NormallyOpenContact::NormallyOpenContact(QGraphicsItem* parent)
    : LadderElement(ElementType::NormallyOpen, parent) {
    m_size = QSizeF(50, 35);
    m_name = "X0";
}

QList<ConnectionPoint> NormallyOpenContact::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 0), ConnectionType::PowerIn, "IN"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void NormallyOpenContact::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制左连接线
    painter->drawLine(-m_size.width() / 2, 0, -15, 0);
    
    // 绘制右连接线
    painter->drawLine(15, 0, m_size.width() / 2, 0);
    
    // 绘制两个竖线（常开触点）
    painter->drawLine(-10, -10, -10, 10);
    painter->drawLine(10, -10, 10, 10);
    
    // 绘制斜线表示常开
    // painter->drawLine(-15, 15, 15, -15);
}

QMap<QString, QVariant> NormallyOpenContact::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "normally_open";
    return map;
}

void NormallyOpenContact::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* NormallyOpenContact::clone() const {
    auto* element = new NormallyOpenContact();
    element->fromMap(toMap());
    return element;
}

// 常闭触点
NormallyClosedContact::NormallyClosedContact(QGraphicsItem* parent)
    : LadderElement(ElementType::NormallyClosed, parent) {
    m_size = QSizeF(50, 35);
    m_name = "X0";
}

QList<ConnectionPoint> NormallyClosedContact::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 0), ConnectionType::PowerIn, "IN"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void NormallyClosedContact::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制左连接线
    painter->drawLine(-m_size.width() / 2, 0, -15, 0);
    
    // 绘制右连接线
    painter->drawLine(15, 0, m_size.width() / 2, 0);
    
    // 绘制两个竖线
    painter->drawLine(-10, -10, -10, 10);
    painter->drawLine(10, -10, 10, 10);
    
    // 绘制斜线表示常闭
    painter->drawLine(-10, 10, 10, -3);
}

QMap<QString, QVariant> NormallyClosedContact::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "normally_closed";
    return map;
}

void NormallyClosedContact::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* NormallyClosedContact::clone() const {
    auto* element = new NormallyClosedContact();
    element->fromMap(toMap());
    return element;
}

// 输出线圈
OutputCoil::OutputCoil(QGraphicsItem* parent)
    : LadderElement(ElementType::OutputCoil, parent) {
    m_size = QSizeF(50, 35);
    m_name = "Y0";
}

QList<ConnectionPoint> OutputCoil::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 0), ConnectionType::PowerIn, "IN"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void OutputCoil::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制左连接线
    painter->drawLine(-m_size.width() / 2, 0, -20, 0);
    
    // 绘制右连接线
    painter->drawLine(20, 0, m_size.width() / 2, 0);
    
    // 绘制圆圈
    painter->drawEllipse(-20, -15, 40, 30);
}

QMap<QString, QVariant> OutputCoil::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "output_coil";
    return map;
}

void OutputCoil::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* OutputCoil::clone() const {
    auto* element = new OutputCoil();
    element->fromMap(toMap());
    return element;
}

// 置位线圈
SetCoil::SetCoil(QGraphicsItem* parent)
    : LadderElement(ElementType::SetCoil, parent) {
    m_size = QSizeF(50, 35);
    m_name = "Y0";
}

QList<ConnectionPoint> SetCoil::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 0), ConnectionType::PowerIn, "IN"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void SetCoil::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制左连接线
    painter->drawLine(-m_size.width() / 2, 0, -20, 0);
    
    // 绘制右连接线
    painter->drawLine(20, 0, m_size.width() / 2, 0);
    
    // 绘制圆圈
    painter->drawEllipse(-20, -15, 40, 30);
    
    // 绘制S字符
    painter->setPen(QPen(m_borderColor, 2));
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->drawText(-6, 6, "S");
}

QMap<QString, QVariant> SetCoil::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "set_coil";
    return map;
}

void SetCoil::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* SetCoil::clone() const {
    auto* element = new SetCoil();
    element->fromMap(toMap());
    return element;
}

// 复位线圈
ResetCoil::ResetCoil(QGraphicsItem* parent)
    : LadderElement(ElementType::ResetCoil, parent) {
    m_size = QSizeF(50, 35);
    m_name = "Y0";
}

QList<ConnectionPoint> ResetCoil::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 0), ConnectionType::PowerIn, "IN"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void ResetCoil::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制左连接线
    painter->drawLine(-m_size.width() / 2, 0, -20, 0);
    
    // 绘制右连接线
    painter->drawLine(20, 0, m_size.width() / 2, 0);
    
    // 绘制圆圈
    painter->drawEllipse(-20, -15, 40, 30);
    
    // 绘制R字符
    painter->setPen(QPen(m_borderColor, 2));
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->drawText(-6, 6, "R");
}

QMap<QString, QVariant> ResetCoil::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "reset_coil";
    return map;
}

void ResetCoil::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* ResetCoil::clone() const {
    auto* element = new ResetCoil();
    element->fromMap(toMap());
    return element;
}

// 正边沿触点 (--|P|--)
PositiveEdgeContact::PositiveEdgeContact(QGraphicsItem* parent)
    : LadderElement(ElementType::PositiveEdge, parent) {
    m_size = QSizeF(50, 35);
    m_name = "X0";
}

QList<ConnectionPoint> PositiveEdgeContact::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 0), ConnectionType::PowerIn, "IN"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void PositiveEdgeContact::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制左连接线
    painter->drawLine(-m_size.width() / 2, 0, -15, 0);
    
    // 绘制右连接线
    painter->drawLine(15, 0, m_size.width() / 2, 0);
    
    // 绘制两个竖线（触点）
    painter->drawLine(-10, -10, -10, 10);
    painter->drawLine(10, -10, 10, 10);
    
    // 绘制P字符表示正边沿
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(-4, -12, "P");
    
    // 绘制上升沿箭头
    painter->drawLine(-5, -18, 0, -23);
    painter->drawLine(0, -23, 5, -18);
}

QMap<QString, QVariant> PositiveEdgeContact::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "positive_edge";
    return map;
}

void PositiveEdgeContact::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* PositiveEdgeContact::clone() const {
    auto* element = new PositiveEdgeContact();
    element->fromMap(toMap());
    return element;
}

// 负边沿触点 (--|N|--)
NegativeEdgeContact::NegativeEdgeContact(QGraphicsItem* parent)
    : LadderElement(ElementType::NegativeEdge, parent) {
    m_size = QSizeF(50, 35);
    m_name = "X0";
}

QList<ConnectionPoint> NegativeEdgeContact::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 0), ConnectionType::PowerIn, "IN"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void NegativeEdgeContact::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制左连接线
    painter->drawLine(-m_size.width() / 2, 0, -15, 0);
    
    // 绘制右连接线
    painter->drawLine(15, 0, m_size.width() / 2, 0);
    
    // 绘制两个竖线（触点）
    painter->drawLine(-10, -10, -10, 10);
    painter->drawLine(10, -10, 10, 10);
    
    // 绘制N字符表示负边沿
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(-4, -12, "N");
    
    // 绘制下降沿箭头
    painter->drawLine(-5, -18, 0, -13);
    painter->drawLine(0, -13, 5, -18);
}

QMap<QString, QVariant> NegativeEdgeContact::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "negative_edge";
    return map;
}

void NegativeEdgeContact::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* NegativeEdgeContact::clone() const {
    auto* element = new NegativeEdgeContact();
    element->fromMap(toMap());
    return element;
}

// 取反线圈 (--(/)--)
InvertedCoil::InvertedCoil(QGraphicsItem* parent)
    : LadderElement(ElementType::InvertedCoil, parent) {
    m_size = QSizeF(50, 35);
    m_name = "Y0";
}

QList<ConnectionPoint> InvertedCoil::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 0), ConnectionType::PowerIn, "IN"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void InvertedCoil::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制左连接线
    painter->drawLine(-m_size.width() / 2, 0, -20, 0);
    
    // 绘制右连接线
    painter->drawLine(20, 0, m_size.width() / 2, 0);
    
    // 绘制圆圈
    painter->drawEllipse(-20, -15, 40, 30);
    
    // 绘制斜线表示取反
    painter->drawLine(-10, -8, 10, 8);
}

QMap<QString, QVariant> InvertedCoil::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "inverted_coil";
    return map;
}

void InvertedCoil::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* InvertedCoil::clone() const {
    auto* element = new InvertedCoil();
    element->fromMap(toMap());
    return element;
}

// 正边沿线圈 (--(P)--)
PositiveEdgeCoil::PositiveEdgeCoil(QGraphicsItem* parent)
    : LadderElement(ElementType::PositiveEdgeCoil, parent) {
    m_size = QSizeF(50, 35);
    m_name = "Y0";
}

QList<ConnectionPoint> PositiveEdgeCoil::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 0), ConnectionType::PowerIn, "IN"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void PositiveEdgeCoil::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制左连接线
    painter->drawLine(-m_size.width() / 2, 0, -20, 0);
    
    // 绘制右连接线
    painter->drawLine(20, 0, m_size.width() / 2, 0);
    
    // 绘制圆圈
    painter->drawEllipse(-20, -15, 40, 30);
    
    // 绘制P字符
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(12);
    painter->setFont(font);
    painter->drawText(-6, 5, "P");
    
    // 绘制上升沿箭头
    painter->drawLine(12, -10, 17, -15);
    painter->drawLine(17, -15, 22, -10);
}

QMap<QString, QVariant> PositiveEdgeCoil::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "positive_edge_coil";
    return map;
}

void PositiveEdgeCoil::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* PositiveEdgeCoil::clone() const {
    auto* element = new PositiveEdgeCoil();
    element->fromMap(toMap());
    return element;
}

// 负边沿线圈 (--(N)--)
NegativeEdgeCoil::NegativeEdgeCoil(QGraphicsItem* parent)
    : LadderElement(ElementType::NegativeEdgeCoil, parent) {
    m_size = QSizeF(50, 35);
    m_name = "Y0";
}

QList<ConnectionPoint> NegativeEdgeCoil::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 0), ConnectionType::PowerIn, "IN"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void NegativeEdgeCoil::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制左连接线
    painter->drawLine(-m_size.width() / 2, 0, -20, 0);
    
    // 绘制右连接线
    painter->drawLine(20, 0, m_size.width() / 2, 0);
    
    // 绘制圆圈
    painter->drawEllipse(-20, -15, 40, 30);
    
    // 绘制N字符
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(12);
    painter->setFont(font);
    painter->drawText(-6, 5, "N");
    
    // 绘制下降沿箭头
    painter->drawLine(12, -10, 17, -5);
    painter->drawLine(17, -5, 22, -10);
}

QMap<QString, QVariant> NegativeEdgeCoil::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "negative_edge_coil";
    return map;
}

void NegativeEdgeCoil::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* NegativeEdgeCoil::clone() const {
    auto* element = new NegativeEdgeCoil();
    element->fromMap(toMap());
    return element;
}

// 左电源轨
LeftPowerRail::LeftPowerRail(QGraphicsItem* parent)
    : LadderElement(ElementType::LeftPowerRail, parent) {
    m_size = QSizeF(20, 200);
    m_name = "LEFT";
}

QList<ConnectionPoint> LeftPowerRail::connectionPoints() const {
    QList<ConnectionPoint> points;
    // 提供多个输出连接点
    for (int i = -4; i <= 4; ++i) {
        points.append(ConnectionPoint(
            QPointF(m_size.width() / 2, i * 20),
            ConnectionType::PowerOut,
            QString("OUT_%1").arg(i + 4)
        ));
    }
    return points;
}

void LeftPowerRail::drawElement(QPainter* painter) {
    painter->setPen(QPen(Qt::darkGray, 3));
    
    // 绘制粗的竖线
    painter->drawLine(m_size.width() / 2 - 3, -m_size.height() / 2,
                      m_size.width() / 2 - 3, m_size.height() / 2);
    painter->drawLine(m_size.width() / 2 + 3, -m_size.height() / 2,
                      m_size.width() / 2 + 3, m_size.height() / 2);
}

QMap<QString, QVariant> LeftPowerRail::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "left_power_rail";
    return map;
}

void LeftPowerRail::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* LeftPowerRail::clone() const {
    auto* element = new LeftPowerRail();
    element->fromMap(toMap());
    return element;
}

// 右电源轨
RightPowerRail::RightPowerRail(QGraphicsItem* parent)
    : LadderElement(ElementType::RightPowerRail, parent) {
    m_size = QSizeF(20, 200);
    m_name = "RIGHT";
}

QList<ConnectionPoint> RightPowerRail::connectionPoints() const {
    QList<ConnectionPoint> points;
    // 提供多个输入连接点
    for (int i = -4; i <= 4; ++i) {
        points.append(ConnectionPoint(
            QPointF(-m_size.width() / 2, i * 20),
            ConnectionType::PowerIn,
            QString("IN_%1").arg(i + 4)
        ));
    }
    return points;
}

void RightPowerRail::drawElement(QPainter* painter) {
    painter->setPen(QPen(Qt::darkGray, 3));
    
    // 绘制粗的竖线
    painter->drawLine(-m_size.width() / 2 - 3, -m_size.height() / 2,
                      -m_size.width() / 2 - 3, m_size.height() / 2);
    painter->drawLine(-m_size.width() / 2 + 3, -m_size.height() / 2,
                      -m_size.width() / 2 + 3, m_size.height() / 2);
}

QMap<QString, QVariant> RightPowerRail::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "right_power_rail";
    return map;
}

void RightPowerRail::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* RightPowerRail::clone() const {
    auto* element = new RightPowerRail();
    element->fromMap(toMap());
    return element;
}

// 定时器
Timer::Timer(QGraphicsItem* parent)
    : LadderElement(ElementType::Timer, parent) {
    m_size = QSizeF(60, 40);
    m_name = "T0";
    setProperty("preset", 100);  // 预设值
    setProperty("current", 0);   // 当前值
    setProperty("timer_type", static_cast<int>(TON));
}

QList<ConnectionPoint> Timer::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, -10), ConnectionType::PowerIn, "IN"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, -10), ConnectionType::PowerOut, "OUT"));
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 10), ConnectionType::Input, "RESET"));
    return points;
}

void Timer::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制方框
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制定时器类型
    QString typeStr;
    switch (m_timerType) {
        case TON: typeStr = "TON"; break;
        case TOF: typeStr = "TOF"; break;
        case TP: typeStr = "TP"; break;
    }
    
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(-15, 5, typeStr);
    
    // 绘制连接线
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(-m_size.width() / 2, -10, -m_size.width() / 2 + 5, -10);
    painter->drawLine(m_size.width() / 2 - 5, -10, m_size.width() / 2, -10);
    painter->drawLine(-m_size.width() / 2, 10, -m_size.width() / 2 + 5, 10);
}

void Timer::setTimerType(TimerType type) {
    m_timerType = type;
    setProperty("timer_type", static_cast<int>(type));
    update();
}

Timer::TimerType Timer::timerType() const {
    return m_timerType;
}

QMap<QString, QVariant> Timer::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "timer";
    map["timer_type"] = static_cast<int>(m_timerType);
    return map;
}

void Timer::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
    m_timerType = static_cast<TimerType>(map["timer_type"].toInt());
}

LadderElement* Timer::clone() const {
    auto* element = new Timer();
    element->fromMap(toMap());
    return element;
}

// 计数器
Counter::Counter(QGraphicsItem* parent)
    : LadderElement(ElementType::Counter, parent) {
    m_size = QSizeF(60, 40);
    m_name = "C0";
    setProperty("preset", 10);   // 预设值
    setProperty("current", 0);   // 当前值
    setProperty("counter_type", static_cast<int>(CTU));
}

QList<ConnectionPoint> Counter::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, -10), ConnectionType::PowerIn, "CU"));
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 10), ConnectionType::Input, "CD"));
    points.append(ConnectionPoint(QPointF(0, m_size.height() / 2), ConnectionType::Input, "RESET"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void Counter::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制方框
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制计数器类型
    QString typeStr;
    switch (m_counterType) {
        case CTU: typeStr = "CTU"; break;
        case CTD: typeStr = "CTD"; break;
        case CTUD: typeStr = "CTUD"; break;
    }
    
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(-15, 5, typeStr);
    
    // 绘制连接线
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(-m_size.width() / 2, -10, -m_size.width() / 2 + 5, -10);
    painter->drawLine(-m_size.width() / 2, 10, -m_size.width() / 2 + 5, 10);
    painter->drawLine(0, m_size.height() / 2 - 5, 0, m_size.height() / 2);
    painter->drawLine(m_size.width() / 2 - 5, 0, m_size.width() / 2, 0);
}

void Counter::setCounterType(CounterType type) {
    m_counterType = type;
    setProperty("counter_type", static_cast<int>(type));
    update();
}

Counter::CounterType Counter::counterType() const {
    return m_counterType;
}

QMap<QString, QVariant> Counter::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "counter";
    map["counter_type"] = static_cast<int>(m_counterType);
    return map;
}

void Counter::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
    m_counterType = static_cast<CounterType>(map["counter_type"].toInt());
}

LadderElement* Counter::clone() const {
    auto* element = new Counter();
    element->fromMap(toMap());
    return element;
}

} // namespace LadderDiagram
