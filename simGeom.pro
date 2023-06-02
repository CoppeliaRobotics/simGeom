TARGET = simGeom
TEMPLATE = lib
DEFINES -= UNICODE
CONFIG += shared plugin
CONFIG -= core
CONFIG -= gui

# not yet supported for double-precision on CoppeliaSim side
DEFINES += SIM_MATH_DOUBLE # for double-precision

*-msvc* {
    QMAKE_CFLAGS += -O2
    QMAKE_CFLAGS += -fp:precise
    QMAKE_CXXFLAGS += -O2
    QMAKE_CXXFLAGS += -fp:precise
    QMAKE_CFLAGS_WARN_ON = -W3
    QMAKE_CXXFLAGS_WARN_ON = -W3
}

*-g++* {
    QMAKE_CFLAGS += -O3
    QMAKE_CXXFLAGS += -O3
    QMAKE_CFLAGS_WARN_ON = -Wall
    QMAKE_CXXFLAGS_WARN_ON = -Wall
    QMAKE_CXXFLAGS += -fvisibility=hidden
    QMAKE_CFLAGS += -Wno-float-equal
    QMAKE_CXXFLAGS += -Wno-float-equal
}

clang* {
    QMAKE_CFLAGS += -O3
    QMAKE_CXXFLAGS += -O3
    QMAKE_CFLAGS_WARN_ON = -Wall
    QMAKE_CXXFLAGS_WARN_ON = -Wall
    QMAKE_CFLAGS += -Wno-float-equal
    QMAKE_CXXFLAGS += -Wno-float-equal
}

win32 {
    DEFINES += WIN_SIM
}

macx {
    DEFINES += MAC_SIM
    INCLUDEPATH += "/usr/local/include"
}

unix:!macx {
    DEFINES += LIN_SIM
}

INCLUDEPATH += "../include"
INCLUDEPATH += "../coppeliaGeometricRoutines"

HEADERS += simGeom.h \
    ../include/simLib/simLib.h \
    ../include/simLib/scriptFunctionData.h \
    ../include/simLib/scriptFunctionDataItem.h \
    ../include/simMath/mathFuncs.h \
    ../include/simMath/3Vector.h \
    ../include/simMath/4Vector.h \
    ../include/simMath/7Vector.h \
    ../include/simMath/3X3Matrix.h \
    ../include/simMath/4X4Matrix.h \
    ../include/simMath/mXnMatrix.h \
    ../coppeliaGeometricRoutines/geom.h \
    ../coppeliaGeometricRoutines/conf.h \
    ../coppeliaGeometricRoutines/calcUtils.h \
    ../coppeliaGeometricRoutines/obbNode.h \
    ../coppeliaGeometricRoutines/obbStruct.h \
    ../coppeliaGeometricRoutines/ocNode.h \
    ../coppeliaGeometricRoutines/ocStruct.h \
    ../coppeliaGeometricRoutines/pcNode.h \
    ../coppeliaGeometricRoutines/pcStruct.h \
    ../coppeliaGeometricRoutines/kdNode.h \

SOURCES += simGeom.cpp \
    ../include/simLib/simLib.cpp \
    ../include/simLib/scriptFunctionData.cpp \
    ../include/simLib/scriptFunctionDataItem.cpp \
    ../include/simMath/mathFuncs.cpp \
    ../include/simMath/3Vector.cpp \
    ../include/simMath/4Vector.cpp \
    ../include/simMath/7Vector.cpp \
    ../include/simMath/3X3Matrix.cpp \
    ../include/simMath/4X4Matrix.cpp \
    ../include/simMath/mXnMatrix.cpp \
    ../coppeliaGeometricRoutines/geom.cpp \
    ../coppeliaGeometricRoutines/calcUtils.cpp \
    ../coppeliaGeometricRoutines/obbNode.cpp \
    ../coppeliaGeometricRoutines/obbStruct.cpp \
    ../coppeliaGeometricRoutines/ocNode.cpp \
    ../coppeliaGeometricRoutines/ocStruct.cpp \
    ../coppeliaGeometricRoutines/pcNode.cpp \
    ../coppeliaGeometricRoutines/pcStruct.cpp \
    ../coppeliaGeometricRoutines/kdNode.cpp \

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
