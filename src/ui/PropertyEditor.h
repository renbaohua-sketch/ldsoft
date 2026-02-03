#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QComboBox>
#include "../core/LadderElement.h"

namespace LadderDiagram {

// 属性编辑器
class PropertyEditor : public QWidget {
    Q_OBJECT

public:
    explicit PropertyEditor(QWidget* parent = nullptr);
    
    // 设置当前编辑的元件
    void setElement(LadderElement* element);
    LadderElement* currentElement() const { return m_currentElement; }
    
    // 清除选择
    void clear();

public slots:
    // 更新属性显示
    void refresh();

private slots:
    void onNameChanged(const QString& text);
    void onAddressChanged(const QString& text);
    void onCommentChanged();
    void onPropertyChanged();

private:
    void setupUI();
    void setupConnections();
    void updateElementFromUI();
    
    LadderElement* m_currentElement = nullptr;
    
    // UI控件
    QLineEdit* m_nameEdit = nullptr;
    QLineEdit* m_addressEdit = nullptr;
    QTextEdit* m_commentEdit = nullptr;
    QTableWidget* m_propertyTable = nullptr;
    
    bool m_updating = false;
};

} // namespace LadderDiagram
