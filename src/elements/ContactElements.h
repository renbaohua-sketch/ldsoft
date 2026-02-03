#pragma once

#include "../core/LadderElement.h"

namespace LadderDiagram {

// 常开触点
class NormallyOpenContact : public LadderElement {
public:
    explicit NormallyOpenContact(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 常闭触点
class NormallyClosedContact : public LadderElement {
public:
    explicit NormallyClosedContact(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 输出线圈
class OutputCoil : public LadderElement {
public:
    explicit OutputCoil(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 置位线圈
class SetCoil : public LadderElement {
public:
    explicit SetCoil(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 复位线圈
class ResetCoil : public LadderElement {
public:
    explicit ResetCoil(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 正边沿触点 (--|P|--)
class PositiveEdgeContact : public LadderElement {
public:
    explicit PositiveEdgeContact(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 负边沿触点 (--|N|--)
class NegativeEdgeContact : public LadderElement {
public:
    explicit NegativeEdgeContact(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 取反线圈 (--(/)--)
class InvertedCoil : public LadderElement {
public:
    explicit InvertedCoil(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 正边沿线圈 (--(P)--)
class PositiveEdgeCoil : public LadderElement {
public:
    explicit PositiveEdgeCoil(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 负边沿线圈 (--(N)--)
class NegativeEdgeCoil : public LadderElement {
public:
    explicit NegativeEdgeCoil(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 左电源轨
class LeftPowerRail : public LadderElement {
public:
    explicit LeftPowerRail(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 右电源轨
class RightPowerRail : public LadderElement {
public:
    explicit RightPowerRail(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
protected:
    void drawElement(QPainter* painter) override;
};

// 定时器
class Timer : public LadderElement {
public:
    explicit Timer(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
    // 定时器类型
    enum TimerType {
        TON,    // 通电延时
        TOF,    // 断电延时
        TP      // 脉冲定时器
    };
    
    void setTimerType(TimerType type);
    TimerType timerType() const;
    
protected:
    void drawElement(QPainter* painter) override;
    
private:
    TimerType m_timerType = TON;
};

// 计数器
class Counter : public LadderElement {
public:
    explicit Counter(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
    // 计数器类型
    enum CounterType {
        CTU,    // 增计数
        CTD,    // 减计数
        CTUD    // 增减计数
    };
    
    void setCounterType(CounterType type);
    CounterType counterType() const;
    
protected:
    void drawElement(QPainter* painter) override;
    
private:
    CounterType m_counterType = CTU;
};

} // namespace LadderDiagram
