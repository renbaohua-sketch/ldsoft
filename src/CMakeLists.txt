set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 收集所有源文件
set(CORE_SOURCES
    core/LadderElement.cpp
    core/LadderElement.h
)

set(ELEMENTS_SOURCES
    elements/ContactElements.cpp
    elements/ContactElements.h
    elements/ConnectionLine.cpp
    elements/ConnectionLine.h
    elements/FunctionBlockElements.cpp
    elements/FunctionBlockElements.h
    elements/LogicElements.cpp
    elements/LogicElements.h
    elements/MathElements.cpp
    elements/MathElements.h
    elements/ControlElements.cpp
    elements/ControlElements.h
)

set(CODEGEN_SOURCES
    codegen/STCodeGenerator.cpp
    codegen/STCodeGenerator.h
)

set(UI_SOURCES
    ui/LadderScene.cpp
    ui/LadderScene.h
    ui/RibbonMainWindow.cpp
    ui/RibbonMainWindow.h
    ui/PropertyEditor.cpp
    ui/PropertyEditor.h
    ui/ThemeManager.h
    ui/ThemeManager.cpp
)

set(ALL_SOURCES
    ${CORE_SOURCES}
    ${ELEMENTS_SOURCES}
    ${CODEGEN_SOURCES}
    ${UI_SOURCES}
    main.cpp
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# 链接Qt库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# 设置目标属性
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# 安装目标
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)
