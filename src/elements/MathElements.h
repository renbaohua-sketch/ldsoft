#pragma once

#include "../core/LadderElement.h"

namespace LadderDiagram {

// 比较指令 (CMP)
class Comparison : public LadderElement {
public:
    explicit Comparison(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
    // 比较操作符类型
    enum CompareOp {
        EQ,     // ==
        NE,     // <>
        GT,     // >
        GE,     // >=
        LT,     // <
        LE      // <=
    };
    
    void setCompareOp(CompareOp op);
    CompareOp compareOp() const;
    
protected:
    void drawElement(QPainter* painter) override;
    
private:
    CompareOp m_compareOp = EQ;
};

// 数学运算 (ADD, SUB, MUL, DIV)
class MathOperation : public LadderElement {
public:
    explicit MathOperation(QGraphicsItem* parent = nullptr);
    
    QList<ConnectionPoint> connectionPoints() const override;
    QMap<QString, QVariant> toMap() const override;
    void fromMap(const QMap<QString, QVariant>& map) override;
    LadderElement* clone() const override;
    
    // 数学运算类型
    enum MathOp {
        ADD,    // 加法
        SUB,    // 减法
        MUL,    // 乘法
        DIV     // 除法
    };
    
    void setMathOp(MathOp op);
    MathOp mathOp() const;
    
protected:
    void drawElement(QPainter* painter) override;
    
private:
    MathOp m_mathOp = ADD;
};

} // namespace LadderDiagram
