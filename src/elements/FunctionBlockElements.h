#pragma once

#include "../core/LadderElement.h"

namespace LadderDiagram {

// 上升沿检测功能块 (R_TRIG)
class RTrig : public LadderElement {
public:
    explicit RTrig(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 下降沿检测功能块 (F_TRIG)
class FTrig : public LadderElement {
public:
    explicit FTrig(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 置位优先触发器 (RS)
class RS : public LadderElement {
public:
    explicit RS(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 复位优先触发器 (SR)
class SR : public LadderElement {
public:
    explicit SR(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

} // namespace LadderDiagram
