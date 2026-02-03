#pragma once

#include "../core/LadderElement.h"

namespace LadderDiagram {

// 跳转指令 (Jump)
class Jump : public LadderElement {
public:
    explicit Jump(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
    void setTargetLabel(const QString& label);
    QString targetLabel() const;
    
protected:
    void drawElement(QPainter* painter) override;
    
private:
    QString m_targetLabel;
};

// 返回指令 (Return)
class Return : public LadderElement {
public:
    explicit Return(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 网络标签 (Label)
class Label : public LadderElement {
public:
    explicit Label(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

} // namespace LadderDiagram
