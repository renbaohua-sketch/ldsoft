#include "LadderElement.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

namespace LadderDiagram {

LadderElement::LadderElement(ElementType type, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , m_type(type)
    , m_size(100, 60)
    , m_fillColor(QColor("#2D2D2D"))
    , m_borderColor(QColor("#007ACC"))
    , m_textColor(QColor("#E0E0E0"))
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
}

LadderElement::~LadderElement() = default;

void LadderElement::setName(const QString& name) {
    m_name = name;
    update();
}

void LadderElement::setAddress(const QString& address) {
    m_address = address;
    update();
}

void LadderElement::setComment(const QString& comment) {
    m_comment = comment;
    update();
}

QVariant LadderElement::getProperty(const QString& key) const {
    return m_properties.value(key);
}

void LadderElement::setProperty(const QString& key, const QVariant& value) {
    m_properties[key] = value;
}

QMap<QString, QVariant> LadderElement::toMap() const {
    QMap<QString, QVariant> map;
    map["type"] = static_cast<int>(m_type);
    map["name"] = m_name;
    map["address"] = m_address;
    map["comment"] = m_comment;
    map["x"] = pos().x();
    map["y"] = pos().y();
    map["properties"] = m_properties;
    return map;
}

void LadderElement::fromMap(const QMap<QString, QVariant>& map) {
    m_name = map["name"].toString();
    m_address = map["address"].toString();
    m_comment = map["comment"].toString();
    setPos(map["x"].toReal(), map["y"].toReal());
    m_properties = map["properties"].toMap();
}

QRectF LadderElement::boundingRect() const {
    return QRectF(-m_size.width() / 2 - 5, -m_size.height() / 2 - 5,
                  m_size.width() + 10, m_size.height() + 10);
}

void LadderElement::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                          QWidget* widget) {
    painter->setRenderHint(QPainter::Antialiasing);
    
    // 绘制选中效果
    if (isSelected()) {
        painter->setPen(QPen(Qt::blue, 2, Qt::DashLine));
        painter->drawRect(boundingRect());
    }
    
    // 绘制元件主体
    drawElement(painter);
    
    // 绘制连接点
    drawConnectionPoints(painter);
    
    // 绘制标签
    if (!m_name.isEmpty() || !m_address.isEmpty()) {
        drawLabel(painter);
    }
}

void LadderElement::drawConnectionPoints(QPainter* painter) {
    auto points = connectionPoints();
    for (const auto& point : points) {
        painter->setBrush(point.isConnected ? Qt::green : Qt::white);
        painter->setPen(QPen(Qt::black, 1));
        painter->drawEllipse(point.position, 4, 4);
    }
}

void LadderElement::drawLabel(QPainter* painter) {
    painter->setPen(m_textColor);
    QFont font = painter->font();
    font.setPointSize(8);
    painter->setFont(font);
    
    QString label;
    if (!m_name.isEmpty() && !m_address.isEmpty()) {
        label = QString("%1 (%2)").arg(m_name, m_address);
    } else if (!m_name.isEmpty()) {
        label = m_name;
    } else {
        label = m_address;
    }
    
    QRectF textRect(-m_size.width() / 2, m_size.height() / 2 + 2, 
                    m_size.width(), 20);
    painter->drawText(textRect, Qt::AlignCenter, label);
}

bool LadderElement::canConnect(const ConnectionPoint& point, const ConnectionPoint& other) const {
    // 同类型不能连接（输入连输出，电源入连电源出）
    if (point.type == other.type) {
        return false;
    }
    
    // 检查类型兼容性
    bool isInputOutput = (point.type == ConnectionType::Input && other.type == ConnectionType::Output) ||
                         (point.type == ConnectionType::Output && other.type == ConnectionType::Input);
    bool isPower = (point.type == ConnectionType::PowerIn && other.type == ConnectionType::PowerOut) ||
                   (point.type == ConnectionType::PowerOut && other.type == ConnectionType::PowerIn);
    
    return isInputOutput || isPower;
}

void LadderElement::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mousePressEvent(event);
    update();
}

void LadderElement::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseReleaseEvent(event);
    // 对齐到网格
    QPointF pos = this->pos();
    qreal x = qRound(pos.x() / 20) * 20;
    qreal y = qRound(pos.y() / 20) * 20;
    setPos(x, y);
}

void LadderElement::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    QGraphicsItem::hoverEnterEvent(event);
    setCursor(Qt::OpenHandCursor);
}

void LadderElement::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    QGraphicsItem::hoverLeaveEvent(event);
    unsetCursor();
}

} // namespace LadderDiagram
