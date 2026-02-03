#include "ConnectionLine.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

namespace LadderDiagram {

ConnectionLine::ConnectionLine(QGraphicsItem* parent)
    : QGraphicsItem(parent) {
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(-1);  // 确保线在元件下方
}

void ConnectionLine::setStartPoint(const QPointF& point) {
    m_startPoint = point;
    update();
}

void ConnectionLine::setEndPoint(const QPointF& point) {
    m_endPoint = point;
    update();
}

void ConnectionLine::setStartElement(LadderElement* element, int connectionIndex) {
    m_startElement = element;
    m_startConnectionIndex = connectionIndex;
    updateConnection();
}

void ConnectionLine::setEndElement(LadderElement* element, int connectionIndex) {
    m_endElement = element;
    m_endConnectionIndex = connectionIndex;
    updateConnection();
}

void ConnectionLine::updateConnection() {
    if (m_startElement) {
        auto points = m_startElement->connectionPoints();
        if (m_startConnectionIndex >= 0 && m_startConnectionIndex < points.size()) {
            QPointF localPos = points[m_startConnectionIndex].position;
            m_startPoint = m_startElement->mapToScene(localPos);
        }
    }
    
    if (m_endElement) {
        auto points = m_endElement->connectionPoints();
        if (m_endConnectionIndex >= 0 && m_endConnectionIndex < points.size()) {
            QPointF localPos = points[m_endConnectionIndex].position;
            m_endPoint = m_endElement->mapToScene(localPos);
        }
    }
    
    update();
}

QRectF ConnectionLine::boundingRect() const {
    qreal minX = qMin(m_startPoint.x(), m_endPoint.x());
    qreal minY = qMin(m_startPoint.y(), m_endPoint.y());
    qreal maxX = qMax(m_startPoint.x(), m_endPoint.x());
    qreal maxY = qMax(m_startPoint.y(), m_endPoint.y());
    
    return QRectF(minX - 5, minY - 5, maxX - minX + 10, maxY - minY + 10);
}

void ConnectionLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                          QWidget* widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)
    
    painter->setRenderHint(QPainter::Antialiasing);
    
    // 绘制选中效果
    if (isSelected()) {
        QPen pen(Qt::blue, m_width + 2, Qt::DashLine);
        painter->setPen(pen);
        painter->drawPath(createPath());
    }
    
    // 绘制连接线
    QPen pen(m_color, m_width);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawPath(createPath());
}

QPainterPath ConnectionLine::shape() const {
    QPainterPathStroker stroker;
    stroker.setWidth(8);
    return stroker.createStroke(createPath());
}

QPainterPath ConnectionLine::createPath() const {
    QPainterPath path;
    path.moveTo(m_startPoint);
    
    // 计算中点，创建直角连接线
    qreal midX = (m_startPoint.x() + m_endPoint.x()) / 2;
    
    if (qAbs(m_startPoint.x() - m_endPoint.x()) < 10) {
        // 直接垂直连接
        path.lineTo(m_endPoint);
    } else {
        // 使用直角连接
        path.lineTo(midX, m_startPoint.y());
        path.lineTo(midX, m_endPoint.y());
        path.lineTo(m_endPoint);
    }
    
    return path;
}

QMap<QString, QVariant> ConnectionLine::toMap() const {
    QMap<QString, QVariant> map;
    map["type"] = "connection_line";
    map["start_x"] = m_startPoint.x();
    map["start_y"] = m_startPoint.y();
    map["end_x"] = m_endPoint.x();
    map["end_y"] = m_endPoint.y();
    
    if (m_startElement) {
        // 这里需要存储元素的标识符，实际实现中应该使用唯一ID
        map["start_element"] = m_startElement->name();
        map["start_connection_index"] = m_startConnectionIndex;
    }
    
    if (m_endElement) {
        map["end_element"] = m_endElement->name();
        map["end_connection_index"] = m_endConnectionIndex;
    }
    
    return map;
}

void ConnectionLine::fromMap(const QMap<QString, QVariant>& map) {
    m_startPoint = QPointF(map["start_x"].toReal(), map["start_y"].toReal());
    m_endPoint = QPointF(map["end_x"].toReal(), map["end_y"].toReal());
    // 元素连接需要在加载后重新建立
}

void ConnectionLine::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mousePressEvent(event);
}

void ConnectionLine::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseReleaseEvent(event);
}

} // namespace LadderDiagram
