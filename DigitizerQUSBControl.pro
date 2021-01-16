TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -c
QMAKE_CFLAGS += -g

QMAKE_CXXFLAGS += -O
QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -fPIC
QMAKE_CXXFLAGS += -pthread
QMAKE_CXXFLAGS += -c

#QMAKE_LFLAGS += C:\Windows\System32\QuickUSB.dll -lws2_32

SOURCES += src/BBmenu.c         \
           src/QuickUSB_menu.c  \
           src/HPTDC_menu.c     \
           src/main.c

HEADERS += inc/BBmenu.h         \
           inc/BBmenu_private.h \
           inc/BBmenuDefs.h     \
           inc/QuickUSB_menu.h  \
           inc/HPTDC_menu.h     \
           inc/main.h           \
           inc/parameters.h
