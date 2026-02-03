#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMap>
#include <QUndoStack>
#include "../core/LadderElement.h"
#include "../elements/ConnectionLine.h"

namespace LadderDiagram {

class LadderScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit LadderScene(QObject* parent = nullptr);
    ~LadderScene();
    
    // 添加元件
    void addElement(LadderElement* element);
    void removeElement(LadderElement* element);
    
    // 添加连接线
    void addConnection(ConnectionLine* connection);
    void removeConnection(ConnectionLine* connection);
    
    // 获取所有元件
    QList<LadderElement*> elements() const;
    
    // 获取所有连接线
    QList<ConnectionLine*> connections() const;
    
    // 获取元件ID（用于序列化）
    QString getElementId(LadderElement* element) const;
    LadderElement* getElementById(const QString& id) const;
    
    // 序列化
    QByteArray toJson() const;
    bool fromJson(const QByteArray& json);
    
    // 清除所有
    void clearScene();
    
    // 网格设置
    void setGridEnabled(bool enabled);
    bool isGridEnabled() const { return m_gridEnabled; }
    void setGridSize(int size);
    int gridSize() const { return m_gridSize; }
    
    // 对齐到网格
    QPointF snapToGrid(const QPointF& point) const;
    
    // 连接模式
    void setConnectionMode(bool enabled);
    bool isConnectionMode() const { return m_connectionMode; }
    
    // 开始/结束连接
    void startConnection(LadderElement* element, int connectionIndex);
    void endConnection(LadderElement* element, int connectionIndex);
    void cancelConnection();
    bool isConnecting() const { return m_isConnecting; }
    
    // 获取撤销栈
    QUndoStack* undoStack() { return m_undoStack; }

protected:
    void drawBackground(QPainter* painter, const QRectF& rect) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void drawGrid(QPainter* painter, const QRectF& rect);
    void updateTemporaryConnection(const QPointF& point);
    void completeConnection(LadderElement* element, int connectionIndex);
    
    bool m_gridEnabled = true;
    int m_gridSize = 20;
    
    // 连接模式
    bool m_connectionMode = false;
    bool m_isConnecting = false;
    LadderElement* m_startElement = nullptr;
    int m_startConnectionIndex = -1;
    ConnectionLine* m_tempConnection = nullptr;
    
    // 元素ID映射
    QMap<QString, LadderElement*> m_elementMap;
    int m_nextElementId = 1;
    
    // 撤销栈
    QUndoStack* m_undoStack;
};

// 梯形图视图
class LadderView : public QGraphicsView {
    Q_OBJECT

public:
    explicit LadderView(QWidget* parent = nullptr);
    
    void setScene(LadderScene* scene);
    LadderScene* ladderScene() const;

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void setupViewport();
    
    bool m_panning = false;
    QPoint m_panStart;
    LadderScene* m_ladderScene = nullptr;
};

} // namespace LadderDiagram
