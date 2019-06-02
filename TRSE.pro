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
DEFINES -= QT_DEPRECATED_WARNINGS

DEFINES += USE_LUA

# You can also make yopur code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


#QMAKE_CXXFLAGS += -fopenmp -std=c++11


INCLUDEPATH +=$$PWD/libs/lua/include

#if USE_LUA
#LIBS
DEPENDPATH += $$PWD/../Libs

macx {
    QMAKE_CXXFLAGS += -openmp
    #LIBS += -openmp
    ICON = trse.icns
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



SOURCES += main.cpp\
        mainwindow.cpp \
    imageworker.cpp \
    source/dialogexport3d.cpp \
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
    source/qlabellimage.cpp \
    source/trsedocuments/formpaw.cpp \
    source/dialogimagehelp.cpp \
    source/Compiler/misc/machinecodeanalyzer.cpp \
    source/dialogprojectsettings.cpp \
    source/OrgAsm/orgasm.cpp \
    source/Compiler/Opcodes/opcodes6502.cpp \
    source/OrgAsm/orgasmlexer.cpp \
    source/dialogdonate.cpp \
    source/dialogfindfile.cpp \
    source/LeLib/limage/limagevic20.cpp \
    source/LeLib/limage/limagesprites2.cpp \
    source/LeLib/limage/limagecontainer.cpp \
    source/Compiler/assembler/abstractastdispatcher.cpp \
    source/Compiler/assembler/astdispather6502.cpp \
    source/Compiler/assembler/methods6502.cpp \
    source/dialogeffects.cpp \
    source/effects/abstractdemoeffect.cpp \
    source/effects/demoeffecttwister.cpp \
    source/effects/demoeffectraytracer.cpp \
    source/Raytracer/raytracer.cpp \
    source/Raytracer/rayobject.cpp \
    source/Raytracer/camera.cpp \
    source/Raytracer/raytracerglobals.cpp \
    source/Raytracer/material.cpp \
    source/effects/demoeffectcharsettwister.cpp \
    source/trsedocuments/formfjong.cpp \
    source/PmmEdit/fjongeditor.cpp \
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
    source/dialogcolors.cpp \
    source/LeLib/limage/bitmapfont.cpp \
    source/Raytracer/particles.cpp


HEADERS  += mainwindow.h \
    imageworker.h \
    source/dialogexport3d.h \
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
    source/qlabellimage.h \
    source/trsedocuments/formpaw.h \
    source/dialogimagehelp.h \
    source/Compiler/misc/machinecodeanalyzer.h \
    source/dialogprojectsettings.h \
    source/OrgAsm/orgasm.h \
    source/Compiler/Opcodes/opcodes6502.h \
    source/OrgAsm/orgasmlexer.h \
    source/dialogdonate.h \
    source/dialogfindfile.h \
    source/LeLib/limage/limagevic20.h \
    source/LeLib/limage/limagesprites2.h \
    source/LeLib/limage/limagecontainer.h \
    source/Compiler/assembler/abstractastdispatcher.h \
    source/Compiler/assembler/astdispather6502.h \
    source/Compiler/assembler/methods6502.h \
    source/dialogeffects.h \
    source/effects/abstractdemoeffect.h \
    source/effects/demoeffecttwister.h \
    source/effects/demoeffectraytracer.h \
    source/Raytracer/raytracer.h \
    source/Raytracer/rayobject.h \
    source/Raytracer/camera.h \
    source/Raytracer/raytracerglobals.h \
    source/Raytracer/material.h \
    source/effects/demoeffectcharsettwister.h \
    source/trsedocuments/formfjong.h \
    source/PmmEdit/fjongeditor.h \
    source/LeLib/luascript.h \
    source/LeLib/objloader/objLoader.h \
    source/LeLib/objloader/obj_parser.h \
    source/LeLib/objloader/string_extra.h \
    source/LeLib/objloader/list.h \
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
    source/dialogcolors.h \
    source/LeLib/limage/bitmapfont.h \
    source/Raytracer/particles.h

FORMS    += mainwindow.ui \
    dialognewimage.ui \
    dialogimport.ui \
    source/dialogexport3d.ui \
    source/trsedocuments/formraseditor.ui \
    source/trsedocuments/formimageeditor.ui \
    source/dialogtrsesettings.ui \
    source/dialogmessage.ui \
    source/dialogabout.ui \
    source/dialoghelp.ui \
    source/dialogexport.ui \
    source/dialogmemoryanalyze.ui \
    source/trsedocuments/formpaw.ui \
    source/dialogimagehelp.ui \
    source/dialogprojectsettings.ui \
    source/dialogdonate.ui \
    source/dialogfindfile.ui \
    source/dialogeffects.ui \
    source/trsedocuments/formfjong.ui \
    source/dialogcolors.ui

RESOURCES += \
    resources.qrc




#LELIB INCLUDES

DISTFILES += \
    resources/text/about.txt \
    resources/text/Documentation.txt \
    resources/text/builtinmethods.txt \
    resources/text/builtinmethods.txt \
    resources/text/builtinmethods.txt \
    resources/images/NES.png \
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
    resources/text/help/m/abs.rtf \
    resources/text/help/m/atan2.rtf \
    resources/text/help/m/blockmemcpy.rtf \
    resources/text/help/m/call.rtf \
    resources/text/help/m/clearbitmap.rtf \
    resources/text/help/m/clearscreen.rtf \
    resources/text/help/m/clearsound.rtf \
    resources/text/help/m/closeirq.rtf \
    resources/text/help/m/copycharsetfromrom.rtf \
    resources/text/help/m/copyfullscreen.rtf \
    resources/text/help/m/copyhalfscreen.rtf \
    resources/text/help/m/copyimagecolordata.rtf \
    resources/text/help/m/dec.rtf \
    resources/text/help/m/decrunch.rtf \
    resources/text/help/m/decrunchfromindex.rtf \
    resources/text/help/m/disableciainterrupts.rtf \
    resources/text/help/m/enableallram.rtf \
    resources/text/help/m/enableieq.rtf \
    resources/text/help/m/enableirq.rtf \
    resources/text/help/m/enablerasterirq.rtf \
    resources/text/help/m/fill.rtf \
    resources/text/help/m/fld.rtf \
    resources/text/help/m/getbit.rtf \
    resources/text/help/m/hi.rtf \
    resources/text/help/m/hideborderx.rtf \
    resources/text/help/m/hidebordery.rtf \
    resources/text/help/m/inc.rtf \
    resources/text/help/m/inczp.rtf \
    resources/text/help/m/jammer.rtf \
    resources/text/help/m/joystick.rtf \
    resources/text/help/m/keypressed.rtf \
    resources/text/help/m/krillload.rtf \
    resources/text/help/m/krillloadcompressed.rtf \
    resources/text/help/m/leftbitshift.rtf \
    resources/text/help/m/lo.rtf \
    resources/text/help/m/loop.rtf \
    resources/text/help/m/memcpy.rtf \
    resources/text/help/m/moveto.rtf \
    resources/text/help/m/nmiirq.rtf \
    resources/text/help/m/nop.rtf \
    resources/text/help/m/peek.rtf \
    resources/text/help/m/playsound.rtf \
    resources/text/help/m/poke.rtf \
    resources/text/help/m/preventirq.rtf \
    resources/text/help/m/printdecimal.rtf \
    resources/text/help/m/printnumber.rtf \
    resources/text/help/m/printstring.rtf \
    resources/text/help/m/rand.rtf \
    resources/text/help/m/rasterirq.rtf \
    resources/text/help/m/return.rtf \
    resources/text/help/m/returninterrupt.rtf \
    resources/text/help/m/rightbitshift.rtf \
    resources/text/help/m/screenoff.rtf \
    resources/text/help/m/screenon.rtf \
    resources/text/help/m/scrollx.rtf \
    resources/text/help/m/scrolly.rtf \
    resources/text/help/m/setbank.rtf \
    resources/text/help/m/setbitmapmode.rtf \
    resources/text/help/m/setcharsetlocation.rtf \
    resources/text/help/m/setfrequency.rtf \
    resources/text/help/m/setmemoryconfig.rtf \
    resources/text/help/m/setmulticolormode.rtf \
    resources/text/help/m/setscreenlocation.rtf \
    resources/text/help/m/setspriteloc.rtf \
    resources/text/help/m/setspritepos.rtf \
    resources/text/help/m/settextmode.rtf \
    resources/text/help/m/spritepos.rtf \
    resources/text/help/m/sqrt.rtf \
    resources/text/help/m/startirq.rtf \
    resources/text/help/m/startrasterchain.rtf \
    resources/text/help/m/swap.rtf \
    resources/text/help/m/togglebit.rtf \
    resources/text/help/m/wait.rtf \
    resources/text/help/m/waitforraster.rtf \
    resources/text/help/m/waitnoraster.rtf \
    resources/images/PET.png \
    resources/code/amiga/end.s \
    resources/code/amiga/init.s
