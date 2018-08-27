#-------------------------------------------------
#
# Project created by QtCreator 2018-02-03T17:17:06
#
#-------------------------------------------------

QT       += core gui
QT += widgets


TARGET = trse
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make yopur code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


#QMAKE_CXXFLAGS += -fopenmp -std=c++11

macx {
  QMAKE_CXXFLAGS += -openmp
  #LIBS += -openmp
  ICON = trse.icns

}



win32-g++ {
  QMAKE_CXXFLAGS += -fopenmp
  LIBS += -fopenmp
}
win32-msvc*{
  QMAKE_CXXFLAGS += -openmp
  LIBS += -openmp
}

linux-g*{
  QMAKE_CXXFLAGS += -fopenmp
  LIBS += -fopenmp
}

win32:RC_ICONS += trse.ico


SOURCES += main.cpp\
        mainwindow.cpp \
    imageworker.cpp \
    source/toolbox.cpp \
    source/toolboxitem.cpp \
    source/workerthread.cpp \
    imageeditor.cpp \
    dialognewimage.cpp \
    dialogimport.cpp \
    source/PmmEdit/highlighter.cpp\
    source/PmmEdit/codeeditor.cpp\
# PMM
    source/trsedocuments/trsedocument.cpp \
    source/trsedocuments/formraseditor.cpp \
    source/trsedocuments/formimageeditor.cpp \
    source/dialogtrsesettings.cpp \
    source/dialogmessage.cpp \
    source/messages.cpp \
    source/dialogabout.cpp \
    source/dialoghelp.cpp \
    source/dialogexport.cpp \
    source/dialogmemoryanalyze.cpp \
    source/Compiler/data_pmm.cpp \
    source/Compiler/errorhandler.cpp \
    source/Compiler/interpreter.cpp \
    source/Compiler/lexer.cpp \
    source/Compiler/parser.cpp \
    source/Compiler/pvar.cpp \
    source/Compiler/symboltable.cpp \
    source/Compiler/syntax.cpp \
    source/Compiler/token.cpp \
    source/Compiler/assembler/asmpascal.cpp \
    source/Compiler/assembler/assembler.cpp \
    source/Compiler/assembler/mos6502.cpp \
    source/Compiler/ast/ast.cpp \
    source/Compiler/ast/node.cpp \
    source/Compiler/ast/nodeasm.cpp \
    source/Compiler/ast/nodeassign.cpp \
    source/Compiler/ast/nodebinaryclause.cpp \
    source/Compiler/ast/nodebinop.cpp \
    source/Compiler/ast/nodeblock.cpp \
    source/Compiler/ast/nodebuiltinmethod.cpp \
    source/Compiler/ast/nodecompound.cpp \
    source/Compiler/ast/nodeconditional.cpp \
    source/Compiler/ast/nodeforloop.cpp \
    source/Compiler/ast/nodenumber.cpp \
    source/Compiler/ast/nodeprocedure.cpp \
    source/Compiler/ast/nodeproceduredecl.cpp \
    source/Compiler/ast/nodeprogram.cpp \
    source/Compiler/ast/nodestring.cpp \
    source/Compiler/ast/nodeunaryop.cpp \
    source/Compiler/ast/nodevar.cpp \
    source/Compiler/ast/nodevararray.cpp \
    source/Compiler/ast/nodevardecl.cpp \
    source/Compiler/ast/nodevartype.cpp \
    source/Compiler/ast/nodewhileloop.cpp \
    source/Compiler/misc/sidfile.cpp \
    source/LeLib/data.cpp \
    source/LeLib/limage/c64fullscreenchar.cpp \
    source/LeLib/limage/charsetimage.cpp \
    source/LeLib/limage/imageleveleditor.cpp \
    source/LeLib/limage/lcolorlist.cpp \
    source/LeLib/limage/limage.cpp \
    source/LeLib/limage/limagecharsetregular.cpp \
    source/LeLib/limage/limageeffects.cpp \
    source/LeLib/limage/limagefactory.cpp \
    source/LeLib/limage/limageio.cpp \
    source/LeLib/limage/limageqimage.cpp \
    source/LeLib/limage/limagesprites.cpp \
    source/LeLib/limage/multicolorimage.cpp \
    source/LeLib/limage/standardcolorimage.cpp \
    source/LeLib/util/cinifile.cpp \
    source/LeLib/util/lgraph.cpp \
    source/LeLib/util/lmessage.cpp \
    source/LeLib/util/spline.cpp \
    source/LeLib/util/util.cpp \
    source/LeLib/util/SimplexNoise.cpp \
    source/LeLib/limage/movieconverter.cpp


HEADERS  += mainwindow.h \
    imageworker.h \
    source/toolbox.h \
    source/toolboxitem.h \
    source/workerthread.h \
    imageeditor.h \
    dialognewimage.h \
    dialogimport.h \
    source/PmmEdit/highlighter.h \
# PMM
    source/PmmEdit/codeeditor.h \
    source/trsedocuments/formraseditor.h \
    source/trsedocuments/trsedocument.h \
    source/trsedocuments/formimageeditor.h \
    source/dialogtrsesettings.h \
    source/dialogmessage.h \
    source/messages.h \
    source/dialogabout.h \
    source/dialoghelp.h \
    source/dialogexport.h \
    source/dialogmemoryanalyze.h \
    source/Compiler/data_pmm.h \
    source/Compiler/errorhandler.h \
    source/Compiler/interpreter.h \
    source/Compiler/lexer.h \
    source/Compiler/parser.h \
    source/Compiler/pvar.h \
    source/Compiler/symboltable.h \
    source/Compiler/syntax.h \
    source/Compiler/token.h \
    source/Compiler/assembler/asmpascal.h \
    source/Compiler/assembler/assembler.h \
    source/Compiler/assembler/mos6502.h \
    source/Compiler/ast/ast.h \
    source/Compiler/ast/node.h \
    source/Compiler/ast/nodeasm.h \
    source/Compiler/ast/nodeassign.h \
    source/Compiler/ast/nodebinaryclause.h \
    source/Compiler/ast/nodebinop.h \
    source/Compiler/ast/nodeblock.h \
    source/Compiler/ast/nodebuiltinmethod.h \
    source/Compiler/ast/nodecompound.h \
    source/Compiler/ast/nodeconditional.h \
    source/Compiler/ast/nodeforloop.h \
    source/Compiler/ast/nodenumber.h \
    source/Compiler/ast/nodeprocedure.h \
    source/Compiler/ast/nodeproceduredecl.h \
    source/Compiler/ast/nodeprogram.h \
    source/Compiler/ast/nodestring.h \
    source/Compiler/ast/nodeunaryop.h \
    source/Compiler/ast/nodevar.h \
    source/Compiler/ast/nodevararray.h \
    source/Compiler/ast/nodevardecl.h \
    source/Compiler/ast/nodevartype.h \
    source/Compiler/ast/nodewhileloop.h \
    source/Compiler/misc/sidfile.h \
    source/LeLib/data.h \
    source/LeLib/limage/c64fullscreenchar.h \
    source/LeLib/limage/charsetimage.h \
    source/LeLib/limage/imageleveleditor.h \
    source/LeLib/limage/lcolorlist.h \
    source/LeLib/limage/limage.h \
    source/LeLib/limage/limagecharsetregular.h \
    source/LeLib/limage/limageeffects.h \
    source/LeLib/limage/limagefactory.h \
    source/LeLib/limage/limageio.h \
    source/LeLib/limage/limageqimage.h \
    source/LeLib/limage/limagesprites.h \
    source/LeLib/limage/multicolorimage.h \
    source/LeLib/limage/standardcolorimage.h \
    source/LeLib/util/cinifile.h \
    source/LeLib/util/counter.h \
    source/LeLib/util/lgraph.h \
    source/LeLib/util/lmessage.h \
    source/LeLib/util/random.h \
    source/LeLib/util/spline.h \
    source/LeLib/util/util.h \
    source/LeLib/util/SimplexNoise.h \
    source/LeLib/limage/movieconverter.h

FORMS    += mainwindow.ui \
    dialognewimage.ui \
    dialogimport.ui \
    source/trsedocuments/formraseditor.ui \
    source/trsedocuments/formimageeditor.ui \
    source/dialogtrsesettings.ui \
    source/dialogmessage.ui \
    source/dialogabout.ui \
    source/dialoghelp.ui \
    source/dialogexport.ui \
    source/dialogmemoryanalyze.ui

RESOURCES += \
    resources.qrc


#LELIB INCLUDES

DISTFILES += \
    resources/text/about.txt \
    resources/text/Documentation.txt
