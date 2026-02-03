#include <QApplication>
#include "ui/RibbonMainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setOrganizationName("LadderDiagram");
    app.setApplicationName("梯形图编辑器");
    app.setApplicationVersion("1.0.0");
    
    // 创建并显示主窗口
    LadderDiagram::RibbonMainWindow window;
    window.show();
    
    return app.exec();
}
