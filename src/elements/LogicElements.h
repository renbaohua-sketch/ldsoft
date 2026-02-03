#pragma once

#include "../core/LadderElement.h"

namespace LadderDiagram {

// 逻辑与 (Logic AND)
class LogicAND : public LadderElement {
public:
    explicit LogicAND(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 逻辑或 (Logic OR)
class LogicOR : public LadderElement {
public:
    explicit LogicOR(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 逻辑非 (Logic NOT)
class LogicNOT : public LadderElement {
public:
    explicit LogicNOT(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

} // namespace LadderDiagram
