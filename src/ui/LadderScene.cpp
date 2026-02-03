#include "LadderScene.h"
#include "../elements/ContactElements.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUndoCommand>
#include <QScrollBar>

namespace LadderDiagram {

// 添加元件命令
class AddElementCommand : public QUndoCommand {
public:
    AddElementCommand(LadderScene* scene, LadderElement* element)
        : m_scene(scene), m_element(element) {
        setText(QObject::tr("添加元件"));
    }
    
    void undo() override {
        m_scene->removeElement(m_element);
    }
    
    void redo() override {
        m_scene->addElement(m_element);
    }
    
private:
    LadderScene* m_scene;
    LadderElement* m_element;
};

// 删除元件命令
class RemoveElementCommand : public QUndoCommand {
public:
    RemoveElementCommand(LadderScene* scene, LadderElement* element)
        : m_scene(scene), m_element(element) {
        setText(QObject::tr("删除元件"));
    }
    
    void undo() override {
        m_scene->addElement(m_element);
    }
    
    void redo() override {
        m_scene->removeElement(m_element);
    }
    
private:
    LadderScene* m_scene;
    LadderElement* m_element;
};

LadderScene::LadderScene(QObject* parent)
    : QGraphicsScene(parent)
    , m_undoStack(new QUndoStack(this)) {
    setSceneRect(-5000, -5000, 10000, 10000);
}

LadderScene::~LadderScene() = default;

void LadderScene::addElement(LadderElement* element) {
    // 生成唯一ID
    QString id = QString("E%1").arg(m_nextElementId++);
    m_elementMap[id] = element;
    
    // 对齐到网格
    element->setPos(snapToGrid(element->pos()));
    
    addItem(element);
    
    // 使用场景的选择变化来更新连接（简化方案）
    connect(this, &QGraphicsScene::selectionChanged, this, [this]() {
        for (auto* item : items()) {
            if (auto* conn = dynamic_cast<ConnectionLine*>(item)) {
                if (conn->startElement() && conn->endElement()) {
                    conn->updateConnection();
                }
            }
        }
    });
}

void LadderScene::removeElement(LadderElement* element) {
    // 删除相关连接
    QList<ConnectionLine*> connectionsToRemove;
    for (auto* item : items()) {
        if (auto* conn = dynamic_cast<ConnectionLine*>(item)) {
            if (conn->startElement() == element || conn->endElement() == element) {
                connectionsToRemove.append(conn);
            }
        }
    }
    
    for (auto* conn : connectionsToRemove) {
        removeConnection(conn);
        delete conn;
    }
    
    // 从映射中移除
    QString idToRemove;
    for (auto it = m_elementMap.begin(); it != m_elementMap.end(); ++it) {
        if (it.value() == element) {
            idToRemove = it.key();
            break;
        }
    }
    if (!idToRemove.isEmpty()) {
        m_elementMap.remove(idToRemove);
    }
    
    removeItem(element);
}

void LadderScene::addConnection(ConnectionLine* connection) {
    addItem(connection);
    connection->setZValue(-1);
}

void LadderScene::removeConnection(ConnectionLine* connection) {
    removeItem(connection);
}

QList<LadderElement*> LadderScene::elements() const {
    QList<LadderElement*> result;
    for (auto* item : items()) {
        if (auto* element = dynamic_cast<LadderElement*>(item)) {
            result.append(element);
        }
    }
    return result;
}

QList<ConnectionLine*> LadderScene::connections() const {
    QList<ConnectionLine*> result;
    for (auto* item : items()) {
        if (auto* conn = dynamic_cast<ConnectionLine*>(item)) {
            result.append(conn);
        }
    }
    return result;
}

QString LadderScene::getElementId(LadderElement* element) const {
    for (auto it = m_elementMap.begin(); it != m_elementMap.end(); ++it) {
        if (it.value() == element) {
            return it.key();
        }
    }
    return QString();
}

LadderElement* LadderScene::getElementById(const QString& id) const {
    return m_elementMap.value(id, nullptr);
}

QByteArray LadderScene::toJson() const {
    QJsonObject root;
    
    // 保存元件
    QJsonArray elementsArray;
    for (auto* element : elements()) {
        QJsonObject elemObj;
        auto map = element->toMap();
        for (auto it = map.begin(); it != map.end(); ++it) {
            elemObj[it.key()] = QJsonValue::fromVariant(it.value());
        }
        elemObj["id"] = getElementId(element);
        elementsArray.append(elemObj);
    }
    root["elements"] = elementsArray;
    
    // 保存连接
    QJsonArray connectionsArray;
    for (auto* conn : connections()) {
        QJsonObject connObj;
        auto map = conn->toMap();
        for (auto it = map.begin(); it != map.end(); ++it) {
            connObj[it.key()] = QJsonValue::fromVariant(it.value());
        }
        connectionsArray.append(connObj);
    }
    root["connections"] = connectionsArray;
    
    QJsonDocument doc(root);
    return doc.toJson();
}

bool LadderScene::fromJson(const QByteArray& json) {
    QJsonDocument doc = QJsonDocument::fromJson(json);
    if (doc.isNull()) return false;
    
    clearScene();
    
    QJsonObject root = doc.object();
    
    // 加载元件
    QJsonArray elementsArray = root["elements"].toArray();
    for (const auto& elemValue : elementsArray) {
        QJsonObject elemObj = elemValue.toObject();
        
        ElementType type = static_cast<ElementType>(elemObj["type"].toInt());
        LadderElement* element = nullptr;
        
        // 根据类型创建元件
        switch (type) {
            case ElementType::NormallyOpen:
                element = new NormallyOpenContact();
                break;
            case ElementType::NormallyClosed:
                element = new NormallyClosedContact();
                break;
            case ElementType::OutputCoil:
                element = new OutputCoil();
                break;
            case ElementType::SetCoil:
                element = new SetCoil();
                break;
            case ElementType::ResetCoil:
                element = new ResetCoil();
                break;
            case ElementType::LeftPowerRail:
                element = new LeftPowerRail();
                break;
            case ElementType::RightPowerRail:
                element = new RightPowerRail();
                break;
            case ElementType::Timer:
                element = new Timer();
                break;
            case ElementType::Counter:
                element = new Counter();
                break;
            default:
                continue;
        }
        
        if (element) {
            QMap<QString, QVariant> map;
            for (auto it = elemObj.begin(); it != elemObj.end(); ++it) {
                map[it.key()] = it.value().toVariant();
            }
            element->fromMap(map);
            addElement(element);
            
            // 恢复ID映射
            QString id = elemObj["id"].toString();
            m_elementMap[id] = element;
        }
    }
    
    // 加载连接（需要在所有元件加载完成后）
    QJsonArray connectionsArray = root["connections"].toArray();
    for (const auto& connValue : connectionsArray) {
        QJsonObject connObj = connValue.toObject();
        
        ConnectionLine* conn = new ConnectionLine();
        QMap<QString, QVariant> map;
        for (auto it = connObj.begin(); it != connObj.end(); ++it) {
            map[it.key()] = it.value().toVariant();
        }
        conn->fromMap(map);
        
        // 重新建立元素连接
        QString startElemId = connObj["start_element"].toString();
        QString endElemId = connObj["end_element"].toString();
        int startIdx = connObj["start_connection_index"].toInt();
        int endIdx = connObj["end_connection_index"].toInt();
        
        if (!startElemId.isEmpty()) {
            conn->setStartElement(getElementById(startElemId), startIdx);
        }
        if (!endElemId.isEmpty()) {
            conn->setEndElement(getElementById(endElemId), endIdx);
        }
        conn->updateConnection();
        
        addConnection(conn);
    }
    
    return true;
}

void LadderScene::clearScene() {
    clear();
    m_elementMap.clear();
    m_nextElementId = 1;
    m_undoStack->clear();
}

void LadderScene::setGridEnabled(bool enabled) {
    m_gridEnabled = enabled;
    invalidate();
}

void LadderScene::setGridSize(int size) {
    m_gridSize = size;
    invalidate();
}

QPointF LadderScene::snapToGrid(const QPointF& point) const {
    if (!m_gridEnabled) return point;
    
    qreal x = qRound(point.x() / m_gridSize) * m_gridSize;
    qreal y = qRound(point.y() / m_gridSize) * m_gridSize;
    return QPointF(x, y);
}

void LadderScene::setConnectionMode(bool enabled) {
    m_connectionMode = enabled;
    if (!enabled) {
        cancelConnection();
    }
}

void LadderScene::startConnection(LadderElement* element, int connectionIndex) {
    m_isConnecting = true;
    m_startElement = element;
    m_startConnectionIndex = connectionIndex;
    
    auto points = element->connectionPoints();
    if (connectionIndex >= 0 && connectionIndex < points.size()) {
        QPointF startPoint = element->mapToScene(points[connectionIndex].position);
        
        m_tempConnection = new ConnectionLine();
        m_tempConnection->setStartPoint(startPoint);
        m_tempConnection->setEndPoint(startPoint);
        m_tempConnection->setStartElement(element, connectionIndex);
        addItem(m_tempConnection);
    }
}

void LadderScene::endConnection(LadderElement* element, int connectionIndex) {
    if (!m_isConnecting || !m_startElement) return;
    
    // 检查是否可以连接
    auto startPoints = m_startElement->connectionPoints();
    auto endPoints = element->connectionPoints();
    
    if (m_startConnectionIndex >= 0 && m_startConnectionIndex < startPoints.size() &&
        connectionIndex >= 0 && connectionIndex < endPoints.size()) {
        
        if (m_startElement->canConnect(startPoints[m_startConnectionIndex], 
                                       endPoints[connectionIndex])) {
            completeConnection(element, connectionIndex);
        }
    }
    
    cancelConnection();
}

void LadderScene::cancelConnection() {
    m_isConnecting = false;
    m_startElement = nullptr;
    m_startConnectionIndex = -1;
    
    if (m_tempConnection) {
        removeItem(m_tempConnection);
        delete m_tempConnection;
        m_tempConnection = nullptr;
    }
}

void LadderScene::drawBackground(QPainter* painter, const QRectF& rect) {
    QGraphicsScene::drawBackground(painter, rect);
    
    if (m_gridEnabled) {
        drawGrid(painter, rect);
    }
}

void LadderScene::drawGrid(QPainter* painter, const QRectF& rect) {
    painter->setPen(QPen(QColor(200, 200, 200), 0.5));
    
    qreal left = qFloor(rect.left() / m_gridSize) * m_gridSize;
    qreal top = qFloor(rect.top() / m_gridSize) * m_gridSize;
    
    for (qreal x = left; x < rect.right(); x += m_gridSize) {
        painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));
    }
    
    for (qreal y = top; y < rect.bottom(); y += m_gridSize) {
        painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));
    }
}

void LadderScene::updateTemporaryConnection(const QPointF& point) {
    if (m_tempConnection) {
        m_tempConnection->setEndPoint(snapToGrid(point));
    }
}

void LadderScene::completeConnection(LadderElement* element, int connectionIndex) {
    ConnectionLine* conn = new ConnectionLine();
    conn->setStartElement(m_startElement, m_startConnectionIndex);
    conn->setEndElement(element, connectionIndex);
    conn->updateConnection();
    addConnection(conn);
}

void LadderScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (m_connectionMode && event->button() == Qt::LeftButton) {
        // 检查是否点击了连接点
        QPointF scenePos = event->scenePos();
        
        for (auto* item : items(scenePos)) {
            if (auto* element = dynamic_cast<LadderElement*>(item)) {
                auto points = element->connectionPoints();
                for (int i = 0; i < points.size(); ++i) {
                    QPointF connPos = element->mapToScene(points[i].position);
                    if (QLineF(scenePos, connPos).length() < 10) {
                        if (!m_isConnecting) {
                            startConnection(element, i);
                        } else {
                            endConnection(element, i);
                        }
                        return;
                    }
                }
            }
        }
    }
    
    QGraphicsScene::mousePressEvent(event);
}

void LadderScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (m_isConnecting) {
        updateTemporaryConnection(event->scenePos());
    }
    
    QGraphicsScene::mouseMoveEvent(event);
}

void LadderScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsScene::mouseReleaseEvent(event);
}

void LadderScene::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Delete) {
        // 删除选中的项
        for (auto* item : selectedItems()) {
            if (auto* element = dynamic_cast<LadderElement*>(item)) {
                m_undoStack->push(new RemoveElementCommand(this, element));
            } else if (auto* conn = dynamic_cast<ConnectionLine*>(item)) {
                removeConnection(conn);
                delete conn;
            }
        }
    } else if (event->key() == Qt::Key_Escape) {
        if (m_isConnecting) {
            cancelConnection();
        }
    }
    
    QGraphicsScene::keyPressEvent(event);
}

// LadderView 实现

LadderView::LadderView(QWidget* parent)
    : QGraphicsView(parent) {
    setupViewport();
}

void LadderView::setScene(LadderScene* scene) {
    m_ladderScene = scene;
    QGraphicsView::setScene(scene);
}

LadderScene* LadderView::ladderScene() const {
    return m_ladderScene;
}

void LadderView::setupViewport() {
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setDragMode(QGraphicsView::RubberBandDrag);
}

void LadderView::wheelEvent(QWheelEvent* event) {
    // 缩放
    qreal scaleFactor = 1.15;
    if (event->angleDelta().y() < 0) {
        scaleFactor = 1.0 / scaleFactor;
    }
    
    scale(scaleFactor, scaleFactor);
}

void LadderView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton) {
        m_panning = true;
        m_panStart = event->pos();
        setCursor(Qt::ClosedHandCursor);
        return;
    }
    
    QGraphicsView::mousePressEvent(event);
}

void LadderView::mouseMoveEvent(QMouseEvent* event) {
    if (m_panning) {
        QScrollBar* hBar = horizontalScrollBar();
        QScrollBar* vBar = verticalScrollBar();
        QPoint delta = event->pos() - m_panStart;
        
        hBar->setValue(hBar->value() - delta.x());
        vBar->setValue(vBar->value() - delta.y());
        
        m_panStart = event->pos();
        return;
    }
    
    QGraphicsView::mouseMoveEvent(event);
}

void LadderView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton) {
        m_panning = false;
        unsetCursor();
        return;
    }
    
    QGraphicsView::mouseReleaseEvent(event);
}

void LadderView::keyPressEvent(QKeyEvent* event) {
    QGraphicsView::keyPressEvent(event);
}

} // namespace LadderDiagram
