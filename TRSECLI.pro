#-------------------------------------------------
#
# Project created by QtCreator 2019-10-08T17:17:06
#
#-------------------------------------------------

QT       += core gui widgets


TARGET = clasc
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES -= QT_DEPRECATED_WARNINGS

DEFINES += USE_LUA
DEFINES += NO_GUI


# You can also make yopur code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


#QMAKE_CXXFLAGS += -fopenmp -std=c++11


INCLUDEPATH +=$$PWD/libs/lua/include

#if USE_LUA
#LIBS
DEPENDPATH += $$PWD/../Libs
ICON = trse.icns

macx {
    QMAKE_CXXFLAGS += -openmp
    #LIBS += -openmp
#    ICON = trse.icns
    QMAKE_CXXFLAGS += -Ofast
    LIBS += -ldl
    LIBS += -L$$PWD/libs/lua/ -lluamac

}

win32-g++ {
    QMAKE_CXXFLAGS += -fopenmp
    LIBS += -fopenmp
    QMAKE_CXXFLAGS += -Ofast

}
win32-msvc*{
    QMAKE_CXXFLAGS += -openmp
    LIBS += -openmp
    QMAKE_CXXFLAGS += -Ofast
    LIBS += -L$$PWD/libs/lua/ -llua

}

linux*{
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_CXXFLAGS +=  -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare -Wno-comment -Wno-parentheses -Wno-delete-non-virtual-dtor -Wno-missing-noreturn
    LIBS += -fopenmp
    QMAKE_CXXFLAGS +=  -Ofast
    LIBS += -L$$PWD/libs/lua/ -llua -ldl
#    LIBS += -ldl


}

win32:RC_ICONS += trse.ico



SOURCES += clasc.cpp \
    source/Compiler/ast/nodecase.cpp \
    source/Compiler/ast/noderepeatuntil.cpp \
    source/Compiler/systems/systemok64.cpp \
    source/Compiler/systems/systemplus4.cpp \
    source/LeLib/limage/limageok64.cpp \
    source/LeLib/limage/limagex16.cpp \
    source/Compiler/data_pmm.cpp \
    source/Compiler/errorhandler.cpp \
    source/Compiler/compiler.cpp \
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
    source/Compiler/misc/machinecodeanalyzer.cpp \
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
    source/LeLib/limage/movieconverter.cpp \
    source/LeLib/limage/limagecharsetfixedcolor.cpp \
    source/OrgAsm/orgasm.cpp \
    source/Compiler/Opcodes/opcodes6502.cpp \
    source/OrgAsm/orgasmlexer.cpp \
    source/LeLib/limage/limagevic20.cpp \
    source/LeLib/limage/limagesprites2.cpp \
    source/LeLib/limage/limagecontainer.cpp \
    source/Compiler/assembler/abstractastdispatcher.cpp \
    source/Compiler/assembler/astdispather6502.cpp \
    source/Compiler/assembler/methods6502.cpp \
    source/LeLib/luascript.cpp \
    source/LeLib/objloader/objloader.cpp \
    source/LeLib/limage/compression.cpp \
    source/LeLib/limage/limagecga.cpp \
    source/LeLib/limage/limageamiga4.cpp \
    source/Compiler/systems/abstractsystem.cpp \
    source/Compiler/systems/systemmos6502.cpp \
    source/Compiler/systems/factorysystem.cpp \
    source/Compiler/systems/systemc64.cpp \
    source/Compiler/systems/systempet.cpp \
    source/Compiler/systems/systemvic20.cpp \
    source/Compiler/systems/systemnes.cpp \
    source/Compiler/systems/systemc128.cpp \
    source/Compiler/systems/systemm6800.cpp \
    source/Compiler/systems/systemamiga.cpp \
    source/Compiler/assembler/astdispatcher68000.cpp \
    source/Compiler/assembler/AsmM68000.cpp \
    source/Compiler/assembler/methods68000.cpp \
    source/Compiler/sourcebuilder.cpp \
    source/Compiler/projectbuilder.cpp


HEADERS  +=  clasc.h \
    source/Compiler/ast/nodecase.h \
    source/Compiler/ast/noderepeatuntil.h \
    source/Compiler/systems/systemok64.h \
    source/Compiler/systems/systemplus4.h \
    source/LeLib/limage/limageok64.h \
    source/LeLib/limage/limagex16.h \
    source/Compiler/data_pmm.h \
    source/Compiler/errorhandler.h \
    source/Compiler/compiler.h \
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
    source/Compiler/misc/machinecodeanalyzer.h \
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
    source/LeLib/limage/movieconverter.h \
    source/LeLib/limage/limagecharsetfixedcolor.h \
    source/OrgAsm/orgasm.h \
    source/Compiler/Opcodes/opcodes6502.h \
    source/OrgAsm/orgasmlexer.h \
    source/LeLib/limage/limagevic20.h \
    source/LeLib/limage/limagesprites2.h \
    source/LeLib/limage/limagecontainer.h \
    source/Compiler/assembler/abstractastdispatcher.h \
    source/Compiler/assembler/astdispather6502.h \
    source/Compiler/assembler/methods6502.h \
    source/LeLib/luascript.h \
    source/LeLib/objloader/objloader.h \
    source/LeLib/limage/compression.h \
    source/LeLib/limage/limagecga.h \
    source/LeLib/limage/limageamiga4.h \
    source/Compiler/systems/abstractsystem.h \
    source/Compiler/systems/systemmos6502.h \
    source/Compiler/systems/factorysystem.h \
    source/Compiler/systems/systemc64.h \
    source/Compiler/systems/systempet.h \
    source/Compiler/systems/systemvic20.h \
    source/Compiler/systems/systemnes.h \
    source/Compiler/systems/systemc128.h \
    source/Compiler/systems/systemm6800.h \
    source/Compiler/systems/systemamiga.h \
    source/Compiler/assembler/astdispatcher68000.h \
    source/Compiler/assembler/AsmM68000.h \
    source/Compiler/assembler/methods68000.h \
    source/Compiler/sourcebuilder.h \
    source/Compiler/projectbuilder.h \


RESOURCES += \
    resources.qrc

#LELIB INCLUDES

DISTFILES += \
    resources/code/amiga/init_p61_player.s \
    resources/code/amiga/p61-play.i \
    resources/text/about.txt \
    resources/text/Documentation.txt \
    resources/text/builtinmethods.txt \
    resources/text/builtinmethods.txt \
    resources/text/builtinmethods.txt \
    resources/code/vic20_playsid_init.asm \
    resources/bin/krill/loader_f000-c64.prg \
    resources/bin/krill/loader_e000-c64.prg \
    resources/bin/krill/loader_c000-c64.prg \
    resources/bin/krill/loader_b000-c64.prg \
    resources/bin/krill/loader_a000-c64.prg \
    resources/bin/krill/loader_9000-c64.prg \
    resources/bin/krill/loader_8000-c64.prg \
    resources/bin/krill/loader_7000-c64.prg \
    resources/bin/krill/loader_6000-c64.prg \
    resources/bin/krill/loader_5000-c64.prg \
    resources/bin/krill/loader_4000-c64.prg \
    resources/bin/krill/loader_3000-c64.prg \
    resources/bin/krill/loader_2000-c64.prg \
    resources/bin/krill/loader_1000-c64.prg \
    resources/bin/krill/loader_0200-c64.prg \
    resources/bin/krill/install_b000-c64.prg \
    resources/bin/krill/install_a000-c64.prg \
    resources/bin/krill/install_9000-c64.prg \
    resources/bin/krill/install_8000-c64.prg \
    resources/bin/krill/install_7000-c64.prg \
    resources/bin/krill/install_6000-c64.prg \
    resources/bin/krill/install_5000-c64.prg \
    resources/bin/krill/install_4000-c64.prg \
    resources/bin/krill/instal_3000-c64.prg \
    resources/bin/krill/install_2000-c64.prg \
    resources/bin/krill/install_1000-c64.prg \
    resources/bin/krill/loader_0200-c64.prg \
    resources/bin/krill/install_5000-c64.prg \
    resources/bin/krill/loader_f000-c64.prg \
    resources/bin/krill/loader_e000-c64.prg \
    resources/bin/krill/loader_c000-c64.prg \
    resources/bin/krill/loader_b000-c64.prg \
    resources/bin/krill/loader_a000-c64.prg \
    resources/bin/krill/loader_9000-c64.prg \
    resources/bin/krill/loader_8000-c64.prg \
    resources/bin/krill/loader_7000-c64.prg \
    resources/bin/krill/loader_6000-c64.prg \
    resources/bin/krill/loader_5000-c64.prg \
    resources/bin/krill/loader_4000-c64.prg \
    resources/bin/krill/loader_3000-c64.prg \
    resources/bin/krill/loader_2000-c64.prg \
    resources/bin/krill/loader_1000-c64.prg \
    resources/bin/krill/install_b000-c64.prg \
    resources/bin/krill/install_a000-c64.prg \
    resources/bin/krill/install_9000-c64.prg \
    resources/bin/krill/install_8000-c64.prg \
    resources/bin/krill/install_7000-c64.prg \
    resources/bin/krill/install_6000-c64.prg \
    resources/bin/krill/install_4000-c64.prg \
    resources/bin/krill/install_2000-c64.prg \
    resources/bin/krill/install_1000-c64.prg \
    resources/bin/krill/instal_3000-c64.prg \
    resources/bin/krill/new/loader_PAL_NTSCL_0200-c64.prg \
    resources/bin/krill/new/install_PAL_NTSC_5000-c64.prg \
    resources/code/amiga/end.s \
    resources/code/amiga/init.s \
