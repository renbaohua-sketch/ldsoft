#include "MathElements.h"
#include <QPainter>

namespace LadderDiagram {

// 比较指令 (CMP)
Comparison::Comparison(QGraphicsItem* parent)
    : LadderElement(ElementType::Comparison, parent) {
    m_size = QSizeF(70, 50);
    m_name = "CMP";
    setProperty("compare_op", static_cast<int>(EQ));
}

QList<ConnectionPoint> Comparison::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, -10), ConnectionType::PowerIn, "IN"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, -10), ConnectionType::PowerOut, "OUT"));
    return points;
}

void Comparison::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制方框
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制比较操作符
    QString opStr;
    switch (m_compareOp) {
        case EQ: opStr = "=="; break;
        case NE: opStr = "<>"; break;
        case GT: opStr = ">"; break;
        case GE: opStr = ">="; break;
        case LT: opStr = "<"; break;
        case LE: opStr = "<="; break;
    }
    
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(12);
    painter->setFont(font);
    painter->drawText(-10, 5, opStr);
    
    // 绘制连接线
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(-m_size.width() / 2, -10, -m_size.width() / 2 + 5, -10);
    painter->drawLine(m_size.width() / 2 - 5, -10, m_size.width() / 2, -10);
}

void Comparison::setCompareOp(CompareOp op) {
    m_compareOp = op;
    setProperty("compare_op", static_cast<int>(op));
    update();
}

Comparison::CompareOp Comparison::compareOp() const {
    return m_compareOp;
}

QMap<QString, QVariant> Comparison::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "comparison";
    map["compare_op"] = static_cast<int>(m_compareOp);
    return map;
}

void Comparison::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
    m_compareOp = static_cast<CompareOp>(map["compare_op"].toInt());
}

LadderElement* Comparison::clone() const {
    auto* element = new Comparison();
    element->fromMap(toMap());
    return element;
}

// 数学运算 (ADD, SUB, MUL, DIV)
MathOperation::MathOperation(QGraphicsItem* parent)
    : LadderElement(ElementType::MathOperation, parent) {
    m_size = QSizeF(70, 60);
    m_name = "MATH";
    setProperty("math_op", static_cast<int>(ADD));
}

QList<ConnectionPoint> MathOperation::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, -15), ConnectionType::PowerIn, "IN1"));
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 15), ConnectionType::Input, "IN2"));
    points.append(ConnectionPoint(QPointF(m_size.width() / 2, 0), ConnectionType::PowerOut, "OUT"));
    return points;
}

void MathOperation::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制方框
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制运算符号
    QString opStr;
    switch (m_mathOp) {
        case ADD: opStr = "+"; break;
        case SUB: opStr = "-"; break;
        case MUL: opStr = "×"; break;
        case DIV: opStr = "÷"; break;
    }
    
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->drawText(-6, 6, opStr);
    
    // 绘制连接线
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(-m_size.width() / 2, -15, -m_size.width() / 2 + 5, -15);
    painter->drawLine(-m_size.width() / 2, 15, -m_size.width() / 2 + 5, 15);
    painter->drawLine(m_size.width() / 2 - 5, 0, m_size.width() / 2, 0);
}

void MathOperation::setMathOp(MathOp op) {
    m_mathOp = op;
    setProperty("math_op", static_cast<int>(op));
    update();
}

MathOperation::MathOp MathOperation::mathOp() const {
    return m_mathOp;
}

QMap<QString, QVariant> MathOperation::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "math_operation";
    map["math_op"] = static_cast<int>(m_mathOp);
    return map;
}

void MathOperation::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
    m_mathOp = static_cast<MathOp>(map["math_op"].toInt());
}

LadderElement* MathOperation::clone() const {
    auto* element = new MathOperation();
    element->fromMap(toMap());
    return element;
}

} // namespace LadderDiagram
