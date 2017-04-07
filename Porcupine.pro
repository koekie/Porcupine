#-------------------------------------------------
#
# Project created by QtCreator 2014-01-08T10:23:07
#
#-------------------------------------------------


ROOT                                    = $$_PRO_FILE_PWD_

QT                                     += widgets core gui xmlpatterns xml printsupport

CONFIG                                 += static

macx:{
    QMAKE_MAC_SDK                       = macosx10.12
    QMAKE_MACOSX_DEPLOYMENT_TARGET      = 10.12
    QMAKE_CXXFLAGS                     += -stdlib=libc++
    QMAKE_CXXFLAGS                     += -std=c++11

    DEFINES                            += DARWIN
}

QMAKE_CXXFLAGS                         += -std=gnu++0x

TARGET                                  = bin/Porcupine

TEMPLATE                                = app

INCLUDEPATH                             = include/ \
                                          include/Settings \
                                          include/NodeEditor \
                                          include/NodeTreeEditor \
                                          include/CodeEditor \
                                          include/CodeInterpreter

OBJECTS_DIR                             = ./obj/

MOC_DIR                                 = ./moc/

SOURCES                                += $$ROOT/main.cpp \
                                          $$ROOT/src/MainWindow.cpp \
                                          $$ROOT/src/Settings/Argument.cpp \
                                          $$ROOT/src/Settings/DataType.cpp \
                                          $$ROOT/src/Settings/NodeLibrary.cpp \
                                          $$ROOT/src/Settings/NodeSetting.cpp \
                                          $$ROOT/src/Settings/Preferences.cpp \
                                          $$ROOT/src/NodeEditor/Link.cpp \
                                          $$ROOT/src/NodeEditor/Node.cpp \
                                          $$ROOT/src/NodeEditor/NodeAntenna.cpp \
                                          $$ROOT/src/NodeEditor/NodeEditor.cpp \
                                          $$ROOT/src/NodeEditor/Port.cpp \
                                          $$ROOT/src/NodeEditor/PortPair.cpp \
                                          $$ROOT/src/NodeTreeEditor/NodeTreeEditor.cpp \
                                          $$ROOT/src/NodeTreeEditor/NodeTreeItem.cpp \
                                          $$ROOT/src/CodeEditor/BashHighlighter.cpp \
                                          $$ROOT/src/CodeEditor/CodeEditor.cpp \
                                          $$ROOT/src/CodeEditor/MatlabHighlighter.cpp \
                                          $$ROOT/src/CodeEditor/PythonHighlighter.cpp \
                                          $$ROOT/src/CodeEditor/SyntaxHighlighter.cpp \
                                          $$ROOT/src/CodeEditor/TvmGenerator.cpp \
                                          $$ROOT/src/CodeEditor/PythonGenerator.cpp \
                                          $$ROOT/src/CodeEditor/FastrGenerator.cpp \
                                          $$ROOT/src/CodeEditor/AfniGenerator.cpp \
                                          $$ROOT/src/CodeEditor/FslGenerator.cpp \
                                          $$ROOT/src/CodeEditor/CodeGenerator.cpp \
                                          $$ROOT/src/NodeEditor/SelectionBox.cpp \
                                          $$ROOT/src/CodeInterpreter/CodeInterpreter.cpp \
                                          $$ROOT/src/CodeEditor/SpmGenerator.cpp \
                                          $$ROOT/src/CodeEditor/FieldtripGenerator.cpp

HEADERS                                += $$ROOT/include/MainWindow.hpp \
                                          $$ROOT/include/Settings/Argument.hpp \
                                          $$ROOT/include/Settings/DataType.hpp \
                                          $$ROOT/include/Settings/NodeLibrary.hpp \
                                          $$ROOT/include/Settings/NodeSetting.hpp \
                                          $$ROOT/include/Settings/Preferences.hpp \
                                          $$ROOT/include/Settings/Singleton.hpp \
                                          $$ROOT/include/NodeEditor/Link.hpp \
                                          $$ROOT/include/NodeEditor/Node.hpp \
                                          $$ROOT/include/NodeEditor/NodeAntenna.hpp \
                                          $$ROOT/include/NodeEditor/NodeEditor.hpp \
                                          $$ROOT/include/NodeEditor/Port.hpp \
                                          $$ROOT/include/NodeEditor/PortPair.hpp \
                                          $$ROOT/include/NodeTreeEditor/NodeTreeItem.hpp \
                                          $$ROOT/include/NodeTreeEditor/NodeTreeEditor.hpp \
                                          $$ROOT/include/CodeEditor/BashHighlighter.hpp \
                                          $$ROOT/include/CodeEditor/CodeEditor.hpp \
                                          $$ROOT/include/CodeEditor/SyntaxHighlighter.hpp \
                                          $$ROOT/include/CodeEditor/MatlabHighlighter.hpp \
                                          $$ROOT/include/CodeEditor/PythonHighlighter.hpp \
                                          $$ROOT/include/CodeEditor/TvmGenerator.hpp \
                                          $$ROOT/include/CodeEditor/PythonGenerator.hpp \
                                          $$ROOT/include/CodeEditor/FastrGenerator.hpp \
                                          $$ROOT/include/CodeEditor/AfniGenerator.hpp \
                                          $$ROOT/include/CodeEditor/FslGenerator.hpp \
                                          $$ROOT/include/CodeEditor/CodeGenerator.hpp \
                                          $$ROOT/include/NodeEditor/SelectionBox.hpp \
                                          $$ROOT/include/CodeInterpreter/CodeInterpreter.hpp \
                                          $$ROOT/include/CodeEditor/SpmGenerator.hpp \
                                          $$ROOT/include/CodeEditor/FieldtripGenerator.hpp

RESOURCES                               = $$ROOT/Resources/resources.qrc

win32:RC_ICONS                         += $$ROOT/Resources/Icon/porcupine.ico
#linux-g++:
#linux-g++-64:
