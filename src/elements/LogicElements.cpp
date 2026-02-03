#include "LogicElements.h"
#include <QPainter>

namespace LadderDiagram {

// 逻辑与 (Logic AND)
LogicAND::LogicAND(QGraphicsItem* parent)
    : LadderElement(ElementType::LogicAND, parent) {
    m_size = QSizeF(60, 50);
    m_name = "AND";
}

QList<ConnectionPoint> LogicAND::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, -10), ConnectionType::PowerIn, "IN1"));
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 10), ConnectionType::Input, "IN2"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void LogicAND::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制方框
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制AND符号 (&)
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->drawText(-8, 6, "&");
    
    // 绘制连接线
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(-m_size.width() / 2, -10, -m_size.width() / 2 + 5, -10);
    painter->drawLine(-m_size.width() / 2, 10, -m_size.width() / 2 + 5, 10);
    painter->drawLine(m_size.width() / 2 - 5, 0, m_size.width() / 2, 0);
}

QMap<QString, QVariant> LogicAND::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "logic_and";
    return map;
}

void LogicAND::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* LogicAND::clone() const {
    auto* element = new LogicAND();
    element->fromMap(toMap());
    return element;
}

// 逻辑或 (Logic OR)
LogicOR::LogicOR(QGraphicsItem* parent)
    : LadderElement(ElementType::LogicOR, parent) {
    m_size = QSizeF(60, 50);
    m_name = "OR";
}

QList<ConnectionPoint> LogicOR::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, -10), ConnectionType::PowerIn, "IN1"));
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 10), ConnectionType::Input, "IN2"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void LogicOR::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制方框
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制OR符号 (>=1)
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(-12, 5, ">=1");
    
    // 绘制连接线
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(-m_size.width() / 2, -10, -m_size.width() / 2 + 5, -10);
    painter->drawLine(-m_size.width() / 2, 10, -m_size.width() / 2 + 5, 10);
    painter->drawLine(m_size.width() / 2 - 5, 0, m_size.width() / 2, 0);
}

QMap<QString, QVariant> LogicOR::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "logic_or";
    return map;
}

void LogicOR::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* LogicOR::clone() const {
    auto* element = new LogicOR();
    element->fromMap(toMap());
    return element;
}

// 逻辑非 (Logic NOT)
LogicNOT::LogicNOT(QGraphicsItem* parent)
    : LadderElement(ElementType::LogicNOT, parent) {
    m_size = QSizeF(50, 40);
    m_name = "NOT";
}

QList<ConnectionPoint> LogicNOT::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 0), ConnectionType::PowerIn, "IN"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void LogicNOT::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制方框
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制NOT符号 (1 with circle)
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(12);
    painter->setFont(font);
    painter->drawText(-8, 5, "1");
    
    // 绘制小圆圈表示取反
    painter->drawEllipse(8, -4, 8, 8);
    
    // 绘制连接线
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(-m_size.width() / 2, 0, -m_size.width() / 2 + 5, 0);
    painter->drawLine(m_size.width() / 2 - 5, 0, m_size.width() / 2, 0);
}

QMap<QString, QVariant> LogicNOT::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "logic_not";
    return map;
}

void LogicNOT::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* LogicNOT::clone() const {
    auto* element = new LogicNOT();
    element->fromMap(toMap());
    return element;
}

} // namespace LadderDiagram
