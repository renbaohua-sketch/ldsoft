#pragma once

#include "../core/LadderElement.h"

namespace LadderDiagram {

// 连接线类
class ConnectionLine : public QGraphicsItem {
public:
    ConnectionLine(QGraphicsItem* parent = nullptr);
    
    // 设置起点和终点
    void setStartPoint(const QPointF& point);
    void setEndPoint(const QPointF& point);
    QPointF startPoint() const { return m_startPoint; }
    QPointF endPoint() const { return m_endPoint; }
    
    // 设置连接的元素
    void setStartElement(LadderElement* element, int connectionIndex);
    void setEndElement(LadderElement* element, int connectionIndex);
    LadderElement* startElement() const { return m_startElement; }
    LadderElement* endElement() const { return m_endElement; }
    int startConnectionIndex() const { return m_startConnectionIndex; }
    int endConnectionIndex() const { return m_endConnectionIndex; }
    
    // 更新连接位置
    void updateConnection();
    
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
               QWidget* widget = nullptr) override;
    
    QPainterPath shape() const override;
    
    // 序列化
    QMap<QString, QVariant> toMap() const;
    void fromMap(const QMap<QString, QVariant>& map);
    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    
private:
    QPointF m_startPoint;
    QPointF m_endPoint;
    LadderElement* m_startElement = nullptr;
    LadderElement* m_endElement = nullptr;
    int m_startConnectionIndex = -1;
    int m_endConnectionIndex = -1;
    
    QColor m_color = Qt::black;
    int m_width = 2;
    bool m_isSelected = false;
    
    QPainterPath createPath() const;
};

} // namespace LadderDiagram
