#include "STCodeGenerator.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

namespace LadderDiagram {

STCodeGenerator::STCodeGenerator()
    : m_programName("MainProgram")
    , m_programDescription("Generated from Ladder Diagram")
{
}

void STCodeGenerator::setProgramName(const QString& name) {
    m_programName = name;
}

void STCodeGenerator::setProgramDescription(const QString& desc) {
    m_programDescription = desc;
}

void STCodeGenerator::addNetwork(const LadderNetwork& network) {
    m_networks.append(network);
}

QString STCodeGenerator::generateSTCode() const {
    QString code;
    
    // 生成文件头注释
    code += "(*\n";
    code += " * Program: " + m_programName + "\n";
    code += " * Description: " + m_programDescription + "\n";
    code += " * Generated from Ladder Diagram\n";
    code += " * Date: " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "\n";
    code += " *)\n\n";
    
    // 生成变量声明
    code += generateVariableDeclarations();
    code += "\n";
    
    // 生成每个网络的代码
    for (const LadderNetwork& network : m_networks) {
        code += networkToST(network);
        code += "\n";
    }
    
    return code;
}

QString STCodeGenerator::generatePOU() const {
    QString pou;
    
    // POU头部
    pou += "PROGRAM " + m_programName + "\n";
    pou += "\n";
    
    // 变量声明区
    pou += "VAR\n";
    pou += generateVariableDeclarations();
    pou += "END_VAR\n";
    pou += "\n";
    
    // 代码区
    pou += generateSTCode();
    
    // POU尾部
    pou += "\n";
    pou += "END_PROGRAM\n";
    
    return pou;
}

bool STCodeGenerator::saveToFile(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << generatePOU();
    file.close();
    
    return true;
}

QString STCodeGenerator::generateVariableDeclarations() const {
    QString vars;
    
    // 简单起见，生成一些示例变量
    vars += "    (* Input Variables *)\n";
    vars += "    StartButton : BOOL;\n";
    vars += "    StopButton : BOOL;\n";
    vars += "\n";
    vars += "    (* Output Variables *)\n";
    vars += "    Motor : BOOL;\n";
    vars += "\n";
    vars += "    (* Internal Variables *)\n";
    vars += "    TempVar : BOOL;\n";
    
    return vars;
}

QString STCodeGenerator::networkToST(const LadderNetwork& network) const {
    QString code;
    
    // 网络注释
    code += "(* Network " + QString::number(network.id);
    if (!network.title.isEmpty()) {
        code += " - " + network.title;
    }
    code += " *)\n";
    
    // 简化的逻辑生成
    code += "(* Simplified logic for network " + QString::number(network.id) + " *)\n";
    code += "TempVar := StartButton AND NOT StopButton;\n";
    code += "Motor := TempVar;\n";
    
    return code;
}

} // namespace LadderDiagram
