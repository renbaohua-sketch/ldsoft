#include "PropertyEditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QPushButton>

namespace LadderDiagram {

PropertyEditor::PropertyEditor(QWidget* parent)
    : QWidget(parent) {
    setupUI();
    setupConnections();
}

void PropertyEditor::setupUI() {
    setWindowTitle(tr("属性"));
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // 名称
    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel(tr("名称:")));
    m_nameEdit = new QLineEdit();
    nameLayout->addWidget(m_nameEdit);
    mainLayout->addLayout(nameLayout);
    
    // 地址
    QHBoxLayout* addressLayout = new QHBoxLayout();
    addressLayout->addWidget(new QLabel(tr("地址:")));
    m_addressEdit = new QLineEdit();
    addressLayout->addWidget(m_addressEdit);
    mainLayout->addLayout(addressLayout);
    
    // 注释
    mainLayout->addWidget(new QLabel(tr("注释:")));
    m_commentEdit = new QTextEdit();
    m_commentEdit->setMaximumHeight(60);
    mainLayout->addWidget(m_commentEdit);
    
    // 属性表格
    mainLayout->addWidget(new QLabel(tr("属性:")));
    m_propertyTable = new QTableWidget();
    m_propertyTable->setColumnCount(2);
    m_propertyTable->setHorizontalHeaderLabels({tr("名称"), tr("值")});
    m_propertyTable->horizontalHeader()->setStretchLastSection(true);
    m_propertyTable->verticalHeader()->setVisible(false);
    mainLayout->addWidget(m_propertyTable);
    
    mainLayout->addStretch();
}

void PropertyEditor::setupConnections() {
    connect(m_nameEdit, &QLineEdit::textChanged, this, &PropertyEditor::onNameChanged);
    connect(m_addressEdit, &QLineEdit::textChanged, this, &PropertyEditor::onAddressChanged);
    connect(m_commentEdit, &QTextEdit::textChanged, this, &PropertyEditor::onCommentChanged);
    connect(m_propertyTable, &QTableWidget::cellChanged, this, &PropertyEditor::onPropertyChanged);
}

void PropertyEditor::setElement(LadderElement* element) {
    m_currentElement = element;
    refresh();
}

void PropertyEditor::clear() {
    m_currentElement = nullptr;
    refresh();
}

void PropertyEditor::refresh() {
    m_updating = true;
    
    if (m_currentElement) {
        m_nameEdit->setText(m_currentElement->name());
        m_nameEdit->setEnabled(true);
        
        m_addressEdit->setText(m_currentElement->address());
        m_addressEdit->setEnabled(true);
        
        m_commentEdit->setPlainText(m_currentElement->comment());
        m_commentEdit->setEnabled(true);
        
        // 更新属性表格
        auto properties = m_currentElement->properties();
        m_propertyTable->setRowCount(properties.size());
        
        int row = 0;
        for (auto it = properties.begin(); it != properties.end(); ++it, ++row) {
            m_propertyTable->setItem(row, 0, new QTableWidgetItem(it.key()));
            m_propertyTable->setItem(row, 1, new QTableWidgetItem(it.value().toString()));
        }
        m_propertyTable->setEnabled(true);
    } else {
        m_nameEdit->clear();
        m_nameEdit->setEnabled(false);
        
        m_addressEdit->clear();
        m_addressEdit->setEnabled(false);
        
        m_commentEdit->clear();
        m_commentEdit->setEnabled(false);
        
        m_propertyTable->setRowCount(0);
        m_propertyTable->setEnabled(false);
    }
    
    m_updating = false;
}

void PropertyEditor::onNameChanged(const QString& text) {
    if (!m_updating && m_currentElement) {
        m_currentElement->setName(text);
    }
}

void PropertyEditor::onAddressChanged(const QString& text) {
    if (!m_updating && m_currentElement) {
        m_currentElement->setAddress(text);
    }
}

void PropertyEditor::onCommentChanged() {
    if (!m_updating && m_currentElement) {
        m_currentElement->setComment(m_commentEdit->toPlainText());
    }
}

void PropertyEditor::onPropertyChanged() {
    if (!m_updating && m_currentElement) {
        updateElementFromUI();
    }
}

void PropertyEditor::updateElementFromUI() {
    for (int row = 0; row < m_propertyTable->rowCount(); ++row) {
        QTableWidgetItem* keyItem = m_propertyTable->item(row, 0);
        QTableWidgetItem* valueItem = m_propertyTable->item(row, 1);
        
        if (keyItem && valueItem) {
            QString key = keyItem->text();
            QString value = valueItem->text();
            m_currentElement->setProperty(key, value);
        }
    }
}

} // namespace LadderDiagram
