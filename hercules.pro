QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hercules
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
        fm_login.cpp \
        fm_nuc_alvo.cpp \
        fm_nuc_alvo_pesquisar.cpp \
        fm_nuc_operacao.cpp \
        fm_nuc_operacao_pesquisar.cpp \
        global_database.cpp \
        global_variaveis.cpp \
        local_variaveis_alvo.cpp \
        local_variaveis_operacao.cpp \
        main.cpp \
        fm_main.cpp

HEADERS += \
        fm_login.h \
        fm_main.h \
        fm_nuc_alvo.h \
        fm_nuc_alvo_pesquisar.h \
        fm_nuc_operacao.h \
        fm_nuc_operacao_pesquisar.h \
        global_database.h \
        global_variaveis.h \
        local_variaveis_alvo.h \
        local_variaveis_operacao.h

FORMS += \
        fm_login.ui \
        fm_main.ui \
        fm_nuc_alvo.ui \
        fm_nuc_alvo_pesquisar.ui \
        fm_nuc_operacao.ui \
        fm_nuc_operacao_pesquisar.ui


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    recursos.qrc
