#include "ControlElements.h"
#include <QPainter>

namespace LadderDiagram {

// 跳转指令 (Jump)
Jump::Jump(QGraphicsItem* parent)
    : LadderElement(ElementType::Jump, parent) {
    m_size = QSizeF(60, 40);
    m_name = "JMP";
    m_targetLabel = "";
}

QList<ConnectionPoint> Jump::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 0), ConnectionType::PowerIn, "IN"));
    return points;
}

void Jump::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制方框
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制JMP文字
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(-15, 5, "JMP");
    
    // 绘制目标标签（如果有）
    if (!m_targetLabel.isEmpty()) {
        font.setPointSize(8);
        painter->setFont(font);
        painter->drawText(-m_size.width() / 2 + 8, -m_size.height() / 2 - 5, m_targetLabel);
    }
    
    // 绘制连接线
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(-m_size.width() / 2, 0, -m_size.width() / 2 + 5, 0);
}

void Jump::setTargetLabel(const QString& label) {
    m_targetLabel = label;
    setProperty("target_label", label);
    update();
}

QString Jump::targetLabel() const {
    return m_targetLabel;
}

QMap<QString, QVariant> Jump::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "jump";
    map["target_label"] = m_targetLabel;
    return map;
}

void Jump::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
    m_targetLabel = map["target_label"].toString();
}

LadderElement* Jump::clone() const {
    auto* element = new Jump();
    element->fromMap(toMap());
    return element;
}

// 返回指令 (Return)
Return::Return(QGraphicsItem* parent)
    : LadderElement(ElementType::Return, parent) {
    m_size = QSizeF(60, 40);
    m_name = "RET";
}

QList<ConnectionPoint> Return::connectionPoints() const {
    QList<ConnectionPoint> points;
    points.append(ConnectionPoint(QPointF(-m_size.width() / 2, 0), ConnectionType::PowerIn, "IN"));
    return points;
}

void Return::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制方框
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制RET文字
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(-12, 5, "RET");
    
    // 绘制连接线
    painter->setPen(QPen(m_borderColor, 1));
    painter->drawLine(-m_size.width() / 2, 0, -m_size.width() / 2 + 5, 0);
}

QMap<QString, QVariant> Return::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "return";
    return map;
}

void Return::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* Return::clone() const {
    auto* element = new Return();
    element->fromMap(toMap());
    return element;
}

// 网络标签 (Label)
Label::Label(QGraphicsItem* parent)
    : LadderElement(ElementType::Label, parent) {
    m_size = QSizeF(80, 30);
    m_name = "LBL";
}

QList<ConnectionPoint> Label::connectionPoints() const {
    QList<ConnectionPoint> points;
    // 标签没有连接点，只是一个标记
    return points;
}

void Label::drawElement(QPainter* painter) {
    painter->setPen(QPen(m_borderColor, 2));
    
    // 绘制标签背景
    painter->setBrush(QBrush(QColor(240, 240, 240)));
    painter->drawRect(-m_size.width() / 2 + 5, -m_size.height() / 2 + 5, 
                      m_size.width() - 10, m_size.height() - 10);
    
    // 绘制标签文字
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(-30, 5, "LBL: " + m_name);
}

QMap<QString, QVariant> Label::toMap() const {
    auto map = LadderElement::toMap();
    map["element_subtype"] = "label";
    return map;
}

void Label::fromMap(const QMap<QString, QVariant>& map) {
    LadderElement::fromMap(map);
}

LadderElement* Label::clone() const {
    auto* element = new Label();
    element->fromMap(toMap());
    return element;
}

} // namespace LadderDiagram
