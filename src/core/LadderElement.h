#pragma once

#include <QGraphicsItem>
#include <QString>
#include <QMap>
#include <QVariant>
#include <memory>

namespace LadderDiagram {

// 元件类型枚举 - 符合 IEC 61131-3:2013 / GB/T 15969.3 标准
enum class ElementType {
    Unknown,
    
    // 电源轨线 (Power Rails)
    LeftPowerRail,      // 左电源轨 - 能流起点
    RightPowerRail,     // 右电源轨 - 能流终点
    
    // 触点 (Contacts)
    NormallyOpen,       // 常开触点 (NO) --| |--
    NormallyClosed,     // 常闭触点 (NC) --|/|--
    PositiveEdge,       // 正边沿检测触点 (P) --|P|--
    NegativeEdge,       // 负边沿检测触点 (N) --|N|--
    ComparisonContact,  // 比较触点
    
    // 线圈 (Coils)
    OutputCoil,         // 一般线圈 --( )--
    InvertedCoil,       // 取反线圈 --(/)--
    SetCoil,            // 置位线圈 --(S)--
    ResetCoil,          // 复位线圈 --(R)--
    PositiveEdgeCoil,   // 正边沿线圈 --(P)--
    NegativeEdgeCoil,   // 负边沿线圈 --(N)--
    
    // 定时器 (Timers)
    Timer,              // TON - 通电延时
    TimerTOF,           // TOF - 断电延时
    TimerTP,            // TP - 脉冲定时器
    
    // 计数器 (Counters)
    Counter,            // CTU - 加计数器
    CounterCTD,         // CTD - 减计数器
    CounterCTUD,        // CTUD - 加减计数器
    
    // 功能块 (Function Blocks)
    RTrig,              // 上升沿检测功能块
    FTrig,              // 下降沿检测功能块
    RS,                 // 置位优先触发器
    SR,                 // 复位优先触发器
    
    // 运算功能
    Comparison,         // 比较指令
    MathOperation,      // 数学运算
    LogicAND,           // 逻辑与
    LogicOR,            // 逻辑或
    LogicNOT,           // 逻辑非
    
    // 程序控制
    Jump,               // 跳转
    Return,             // 返回
    Label,              // 网络标签
    
    // 连接线
    ConnectionLine      // 连接线
};

// 连接点类型
enum class ConnectionType {
    Input,      // 输入
    Output,     // 输出
    PowerIn,    // 电源输入
    PowerOut    // 电源输出
};

// 连接点结构
struct ConnectionPoint {
    QPointF position;
    ConnectionType type;
    QString name;
    bool isConnected = false;
    
    ConnectionPoint(const QPointF& pos, ConnectionType t, const QString& n)
        : position(pos), type(t), name(n) {}
};

// 梯形图元件基类
class LadderElement : public QGraphicsItem {
public:
    LadderElement(ElementType type, QGraphicsItem* parent = nullptr);
    virtual ~LadderElement();
    
    // 获取元件类型
    ElementType elementType() const { return m_type; }
    
    // 获取/设置元件名称
    QString name() const { return m_name; }
    void setName(const QString& name);
    
    // 获取/设置地址
    QString address() const { return m_address; }
    void setAddress(const QString& address);
    
    // 获取/设置注释
    QString comment() const { return m_comment; }
    void setComment(const QString& comment);
    
    // 获取连接点
    virtual QList<ConnectionPoint> connectionPoints() const = 0;
    
    // 属性管理
    QVariant getProperty(const QString& key) const;
    void setProperty(const QString& key, const QVariant& value);
    QMap<QString, QVariant> properties() const { return m_properties; }
    
    // 序列化
    virtual QMap<QString, QVariant> toMap() const;
    virtual void fromMap(const QMap<QString, QVariant>& map);
    
    // 获取边界矩形
    QRectF boundingRect() const override;
    
    // 绘制元件
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, 
               QWidget* widget = nullptr) override;
    
    // 克隆元件
    virtual LadderElement* clone() const = 0;
    
    // 是否可以被连接
    virtual bool canConnect(const ConnectionPoint& point, const ConnectionPoint& other) const;
    
protected:
    // 绘制元件主体（子类实现）
    virtual void drawElement(QPainter* painter) = 0;
    
    // 绘制连接点
    void drawConnectionPoints(QPainter* painter);
    
    // 绘制标签
    void drawLabel(QPainter* painter);
    
    ElementType m_type;
    QString m_name;
    QString m_address;
    QString m_comment;
    QMap<QString, QVariant> m_properties;
    
    QSizeF m_size;
    QColor m_fillColor;
    QColor m_borderColor;
    QColor m_textColor;
    
    // 选中状态
    bool m_isSelected = false;
    
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
};

} // namespace LadderDiagram
