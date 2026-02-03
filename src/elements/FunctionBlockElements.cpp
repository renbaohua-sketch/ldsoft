#include "FunctionBlockElements.h"
#include <QPainter>

namespace LadderDiagram {

// 上升沿检测功能块 (R_TRIG)
RTrig::RTrig(QGraphicsItem* parent)
    : LadderElement(ElementType::RTrig, parent) {
    m_size = QSizeF(70, 50);
    m_name = "R_TRIG";
}

QList<ConnectionPoint> RTrig::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, -10), ConnectionType::PowerIn, "CLK"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, -10), ConnectionType::PowerOut, "Q"));
    return points;
}

void RTrig::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制方框
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制功能块名称
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(-25, 5, "R_TRIG");
    
    // 绘制上升沿符号
    painter->drawLine(-15, -20, -10, -25);
    painter->drawLine(-10, -25, -5, -20);
    
    // 绘制连接线
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(-m_size.width() / 2, -10, -m_size.width() / 2 + 5, -10);
    painter->drawLine(m_size.width() / 2 - 5, -10, m_size.width() / 2, -10);
}

QMap<QString, QVariant> RTrig::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "r_trig";
    return map;
}

void RTrig::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* RTrig::clone() const {
    auto* element = new RTrig();
    element->fromMap(toMap());
    return element;
}

// 下降沿检测功能块 (F_TRIG)
FTrig::FTrig(QGraphicsItem* parent)
    : LadderElement(ElementType::FTrig, parent) {
    m_size = QSizeF(70, 50);
    m_name = "F_TRIG";
}

QList<ConnectionPoint> FTrig::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, -10), ConnectionType::PowerIn, "CLK"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, -10), ConnectionType::PowerOut, "Q"));
    return points;
}

void FTrig::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制方框
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制功能块名称
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(-25, 5, "F_TRIG");
    
    // 绘制下降沿符号
    painter->drawLine(-15, -20, -10, -15);
    painter->drawLine(-10, -15, -5, -20);
    
    // 绘制连接线
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(-m_size.width() / 2, -10, -m_size.width() / 2 + 5, -10);
    painter->drawLine(m_size.width() / 2 - 5, -10, m_size.width() / 2, -10);
}

QMap<QString, QVariant> FTrig::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "f_trig";
    return map;
}

void FTrig::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* FTrig::clone() const {
    auto* element = new FTrig();
    element->fromMap(toMap());
    return element;
}

// 置位优先触发器 (RS)
RS::RS(QGraphicsItem* parent)
    : LadderElement(ElementType::RS, parent) {
    m_size = QSizeF(70, 60);
    m_name = "RS";
}

QList<ConnectionPoint> RS::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, -15), ConnectionType::PowerIn, "S"));
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 15), ConnectionType::Input, "R"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "Q"));
    return points;
}

void RS::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制方框
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制功能块名称
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(12);
    painter->setFont(font);
    painter->drawText(-12, 5, "RS");
    
    // 绘制输入标签
    font.setPointSize(8);
    painter->setFont(font);
    painter->drawText(-m_size.width() / 2 + 8, -12, "S");
    painter->drawText(-m_size.width() / 2 + 8, 18, "R");
    painter->drawText(m_size.width() / 2 - 15, 5, "Q");
    
    // 绘制连接线
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(-m_size.width() / 2, -15, -m_size.width() / 2 + 5, -15);
    painter->drawLine(-m_size.width() / 2, 15, -m_size.width() / 2 + 5, 15);
    painter->drawLine(m_size.width() / 2 - 5, 0, m_size.width() / 2, 0);
}

QMap<QString, QVariant> RS::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "rs";
    return map;
}

void RS::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* RS::clone() const {
    auto* element = new RS();
    element->fromMap(toMap());
    return element;
}

// 复位优先触发器 (SR)
SR::SR(QGraphicsItem* parent)
    : LadderElement(ElementType::SR, parent) {
    m_size = QSizeF(70, 60);
    m_name = "SR";
}

QList<ConnectionPoint> SR::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, -15), ConnectionType::PowerIn, "S"));
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 15), ConnectionType::Input, "R"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "Q"));
    return points;
}

void SR::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制方框
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制功能块名称
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(12);
    painter->setFont(font);
    painter->drawText(-12, 5, "SR");
    
    // 绘制输入标签
    font.setPointSize(8);
    painter->setFont(font);
    painter->drawText(-m_size.width() / 2 + 8, -12, "S");
    painter->drawText(-m_size.width() / 2 + 8, 18, "R");
    painter->drawText(m_size.width() / 2 - 15, 5, "Q");
    
    // 绘制连接线
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(-m_size.width() / 2, -15, -m_size.width() / 2 + 5, -15);
    painter->drawLine(-m_size.width() / 2, 15, -m_size.width() / 2 + 5, 15);
    painter->drawLine(m_size.width() / 2 - 5, 0, m_size.width() / 2, 0);
}

QMap<QString, QVariant> SR::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "sr";
    return map;
}

void SR::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* SR::clone() const {
    auto* element = new SR();
    element->fromMap(toMap());
    return element;
}

} // namespace LadderDiagram
