#pragma once

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QSet>
#include <QtCore/QVector>
#include <memory>

namespace LadderDiagram {

// 梯形图节点类型
enum class LadderNodeType {
    Input,      // 输入触点
    Output,     // 输出线圈
    Junction,   // 连接节点（并联分支点）
    PowerRail   // 电源轨
};

// 梯形图逻辑节点
struct LadderNode {
    int id;                              // 节点ID
    LadderNodeType type;                 // 节点类型
    QString elementType;                 // 元件类型（NormallyOpen, OutputCoil等）
    QString name;                        // 变量名
    QString address;                     // 地址
    QVariant properties;                 // 额外属性
    
    // 连接关系
    QList<int> inputs;                   // 输入节点ID列表
    QList<int> outputs;                  // 输出节点ID列表
    
    // 逻辑分析用
    bool visited = false;
    int depth = 0;                       // 拓扑深度
    bool isParallelBranch = false;       // 是否为并联支路
    int parallelGroupId = -1;            // 并联组ID
};

// 梯形图网络（一个完整的逻辑行）
struct LadderNetwork {
    int id;                              // 网络编号
    QString title;                       // 网络标题/注释
    QList<QMap<QString, QVariant>> elements;      // 该网络中的所有元件
    QList<QMap<QString, QVariant>> connections;   // 连接关系
    
    // 逻辑分析后的节点图
    QMap<int, LadderNode> nodes;
    int leftRailId = -1;
    int rightRailId = -1;
};

// 并联支路组
struct ParallelBranch {
    int groupId;                         // 组ID
    int startNodeId;                     // 起始节点（分叉点）
    int endNodeId;                       // 结束节点（汇合点）
    QList<QList<int>> branches;          // 各支路的节点ID列表
    QString stExpression;                // 生成的ST表达式
};

// ST代码生成器 - 将梯形图转换为IEC 61131-3标准ST语言
class STCodeGenerator {
public:
    STCodeGenerator();
    
    // 设置程序信息
    void setProgramName(const QString& name);
    void setProgramDescription(const QString& desc);
    
    // 添加网络
    void addNetwork(const LadderNetwork& network);
    
    // 从JSON数据加载梯形图
    void loadFromJson(const QString& jsonData);
    void loadFromJsonFile(const QString& filePath);
    
    // 生成ST代码
    QString generateSTCode() const;
    
    // 生成完整的POU（程序组织单元）
    QString generatePOU() const;
    
    // 保存到文件
    bool saveToFile(const QString& filePath) const;
    
private:
    QString m_programName;
    QString m_programDescription;
    QList<LadderNetwork> m_networks;
    
    // ===== 逻辑分析核心算法 =====
    
    // 构建节点图
    void buildNodeGraph(LadderNetwork& network) const;
    
    // 拓扑排序
    QList<int> topologicalSort(const QMap<int, LadderNode>& nodes) const;
    
    // 识别并联支路
    QList<ParallelBranch> identifyParallelBranches(LadderNetwork& network) const;
    
    // 查找所有路径
    void findAllPaths(int startId, int endId, 
                     const QMap<int, LadderNode>& nodes,
                     QList<int>& currentPath,
                     QList<QList<int>>& allPaths,
                     QSet<int>& visited) const;
    
    // 分析节点逻辑（处理串联和并联）
    QString analyzeNodeLogic(int nodeId, 
                            const LadderNetwork& network,
                            const QList<ParallelBranch>& parallelBranches,
                            QSet<int>& processedNodes) const;
    
    // 生成并联支路的ST表达式
    QString generateParallelExpression(const ParallelBranch& branch,
                                      const LadderNetwork& network) const;
    
    // 生成串联路径的ST表达式
    QString generateSeriesExpression(const QList<int>& nodeIds,
                                    const LadderNetwork& network) const;
    
    // ===== 代码生成 =====
    
    // 变量声明生成
    QString generateVariableDeclarations() const;
    
    // 单个网络转换为ST代码
    QString networkToST(const LadderNetwork& network) const;
    
    // 分析网络逻辑结构（新版本）
    QString analyzeNetworkLogicV2(LadderNetwork& network) const;
    
    // 生成布尔表达式
    QString generateBooleanExpression(const QMap<QString, QVariant>& element) const;
    
    // 元件类型到ST操作符的映射
    QString getElementOperator(const QString& elementType) const;
    
    // 生成定时器ST代码
    QString generateTimerCode(const QMap<QString, QVariant>& element) const;
    
    // 生成计数器ST代码
    QString generateCounterCode(const QMap<QString, QVariant>& element) const;
    
    // 生成功能块ST代码
    QString generateFunctionBlockCode(const QMap<QString, QVariant>& element) const;
    
    // 生成逻辑运算ST代码
    QString generateLogicCode(const QMap<QString, QVariant>& element) const;
    
    // 格式化代码
    QString formatCode(const QString& code) const;
    
    // 缩进处理
    QString indent(int level) const;
};

} // namespace LadderDiagram
