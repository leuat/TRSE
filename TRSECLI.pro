#-------------------------------------------------
#
# Project created by QtCreator 2018-02-03T17:17:06
#
#-------------------------------------------------

QT += core qml
VER = $$QT_MAJOR_VERSION
QT -= gui

CONFIG += c++14 console

CONFIG+=warn_off

TARGET = trse
TEMPLATE = app

DEFINES -= QT_DEPRECATED_WARNINGS

DEFINES += USE_LUA

DEFINES +=USE_OMP
DEFINES +=CLI_VERSION

INCLUDEPATH +=$$PWD/libs/lua/include

DEPENDPATH += $$PWD/../Libs
win32:RC_ICONS += trse.ico
ICON = trse.icns

ARCH = $$QMAKE_HOST.arch


macx{
    #LIBS += -openmp
#    ICON = trse.icns
    QMAKE_CXXFLAGS_RELEASE += -Ofast
    LIBS += -L$$PWD/libs -Ofast
    LIBS += -ldl
#    QMAKE_LFLAGS += -F /Library/Frameworks
#    LIBS += -framework SDL2

    LIBS += -L/usr/local/lib /usr/local/lib/libomp.dylib -lomp
    DEFINES -=USE_OMP
    contains(DEFINES, USE_OMP) {
      QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp -I/usr/local/include
    }

    contains(ARCH, arm64): {
      message("Arme meg!")
      QMAKE_APPLE_DEVICE_ARCHS=arm64
      LIBS += -L$$PWD/libs/lua/ -lluamac_arm
      CONFIG += arm64

    }
    contains(ARCH, x86_64) |contains(ARCH, amd64):  {
        LIBS += -L$$PWD/libs/lua/ -lluamac
   }
   LIBS += -L$$PWD/libs/lua/ -lluamac
   INCLUDEPATH += /usr/local/include/
   INCLUDEPATH += /opt/homebrew/include/

#   LIBS +=  -L/Users/leuat/code/sdl-x86/Versions/A -lSDL2


#    QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp -lomp -I/opt/homebrew/include/
#   QMAKE_LFLAGS += -lomp
#     LIBS += -L /opt/homebrew/lib
}

win32-g++ {
    QMAKE_CXXFLAGS += -fopenmp
    LIBS += -fopenmp
    QMAKE_CXXFLAGS_RELEASE += -Ofast
    LIBS += $$PWD/libs/lua/liblua.a
    LIBS += -lopengl32

}
win32-msvc*{
    QMAKE_CXXFLAGS += -openmp
    LIBS += -openmp
    QMAKE_CXXFLAGS_RELEASE += -Ofast -O2
    LIBS += -L$$PWD/libs/lua/ -llua
    LIBS += -lopengl32
}

linux*{
    QMAKE_CXXFLAGS += -fopenmp
#    QMAKE_CXXFLAGS +=  -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare -Wno-comment -Wno-parentheses -Wno-delete-non-virtual-dtor -Wno-missing-noreturn
    LIBS += -fopenmp
    QMAKE_CXXFLAGS_RELEASE +=  -Ofast
    LIBS += -L$$PWD/libs/lua/ -llua -ldl
}




SOURCES += maincli.cpp \
    source/Compiler/assembler/abstractmethods.cpp \
    source/Compiler/assembler/asmx86.cpp \
    source/Compiler/assembler/asmz80.cpp \
    source/Compiler/assembler/astdispatcherx86.cpp \
    source/Compiler/assembler/dispatcherz80.cpp \
    source/Compiler/assembler/factorymethods.cpp \
    source/Compiler/assembler/methods68000atari.cpp \
    source/Compiler/assembler/methods6800amiga.cpp \
    source/Compiler/assembler/methodsz80.cpp \
    source/Compiler/assembler/mos6502/methods6502c64.cpp \
    source/Compiler/assembler/mos6502/methods6502ok64.cpp \
    source/Compiler/assembler/mos6502/methods6502vic20.cpp \
    source/Compiler/assembler/methodsx86.cpp \
    source/Compiler/ast/nodecase.cpp \
    source/Compiler/ast/nodecontrolstatement.cpp \
    source/Compiler/ast/noderepeatuntil.cpp \
    source/Compiler/compilers/compiler6502.cpp \
    source/Compiler/compilers/compilergbz80.cpp \
    source/Compiler/compilers/compilerm68k.cpp \
    source/Compiler/compilers/compilerx86.cpp \
    source/Compiler/compilers/compilerz80.cpp \
    source/Compiler/compilers/factorycompiler.cpp \
    source/Compiler/optimiser/postoptimiser.cpp \
    source/Compiler/optimiser/postoptimizerm68k.cpp \
    source/Compiler/optimiser/postoptimizerx86.cpp \
    source/Compiler/optimiser/postoptimizerz80.cpp \
    source/Compiler/systems/systemamstradcpc.cpp \
    source/Compiler/systems/systemappleii.cpp \
    source/Compiler/systems/systematari2600.cpp \
    source/Compiler/systems/systematari520st.cpp \
    source/Compiler/systems/systematari800.cpp \
    source/Compiler/systems/systembbcm.cpp \
    source/Compiler/systems/systemcoleco.cpp \
    source/Compiler/systems/systemgameboy.cpp \
    source/Compiler/systems/systemmsx.cpp \
    source/Compiler/systems/systemok64.cpp \
    source/Compiler/systems/systemplus4.cpp \
    source/Compiler/systems/systemspectrum.cpp \
    source/Compiler/systems/systemtiki100.cpp \
    source/Compiler/systems/systemx86.cpp \
    source/Compiler/systems/systemz80.cpp \
    source/LeLib/bbc/asmexception.cpp \
    source/LeLib/bbc/discimage.cpp \
    source/LeLib/bbc/globaldata.cpp \
    source/LeLib/bbc/stringutils.cpp \
    source/LeLib/limage/lcolor.cpp \
    source/LeLib/limage/limageamstradcpc.cpp \
    source/LeLib/limage/limageamstradgeneric.cpp \
    source/LeLib/limage/limageatari520st.cpp \
    source/LeLib/limage/limagebbc.cpp \
    source/LeLib/limage/limagefooter.cpp \
    source/LeLib/limage/limagegamboy.cpp \
    source/LeLib/limage/limagehybridcharset.cpp \
    source/LeLib/limage/limagelevelgameboy.cpp \
    source/LeLib/limage/limagelevelnes.cpp \
    source/LeLib/limage/limagemetablocksprites.cpp \
    source/LeLib/limage/limagemetachunk.cpp \
    source/LeLib/limage/limagenes.cpp \
    source/LeLib/limage/limageok64.cpp \
    source/LeLib/limage/limagevga.cpp \
    source/LeLib/limage/limagex16.cpp \
    source/LeLib/limage/lpen.cpp \
    source/LeLib/limage/pixelchar.cpp \
    source/LeLib/limage/ssim.cpp \
#    source/LeLib/miniaudio.c \
#    source/LeLib/miniaudio_engine.c \
    source/LeLib/ttrfile.cpp \
    source/LeLib/util/fc8/FC8Compression.cpp \
    source/LeLib/util/lz4/lz4.c \
    source/LeLib/util/lz4/lz4hc.c \
    source/LeLib/util/spline2d.cpp \
    source/LeLib/util/tikidisk.cpp \
    source/LeLib/util/tool.cpp \
    source/LeLib/util/utilclasses.cpp \
    source/Compiler/data_pmm.cpp \
    source/Compiler/errorhandler.cpp \
    source/Compiler/compilers/compiler.cpp \
    source/Compiler/lexer.cpp \
    source/Compiler/parser.cpp \
    source/Compiler/pvar.cpp \
    source/Compiler/symboltable.cpp \
    source/Compiler/syntax.cpp \
    source/Compiler/token.cpp \
    source/Compiler/assembler/asmpascal.cpp \
    source/Compiler/assembler/assembler.cpp \
    source/Compiler/assembler/mos6502/mos6502.cpp \
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
    source/Compiler/misc/machinecodeanalyzer.cpp \
    source/OrgAsm/orgasm.cpp \
    source/Compiler/Opcodes/opcodes6502.cpp \
    source/OrgAsm/orgasmlexer.cpp \
    source/LeLib/limage/limagevic20.cpp \
    source/LeLib/limage/limagesprites2.cpp \
    source/LeLib/limage/limagecontainer.cpp \
    source/Compiler/assembler/abstractastdispatcher.cpp \
    source/Compiler/assembler/mos6502/astdispatcher6502.cpp \
    source/Compiler/assembler/mos6502/methods6502.cpp \
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
    source/LeLib/limage/bitmapfont.cpp \
    source/Compiler/sourcebuilder.cpp \
    source/Compiler/projectbuilder.cpp \
    trc.cpp


HEADERS  += source/Compiler/assembler/abstractmethods.h \
    source/Compiler/assembler/asmx86.h \
    source/Compiler/assembler/asmz80.h \
    source/Compiler/assembler/astdispatcherx86.h \
    source/Compiler/assembler/dispatcherz80.h \
    source/Compiler/assembler/factorymethods.h \
    source/Compiler/assembler/methods68000atari.h \
    source/Compiler/assembler/methods6800amiga.h \
    source/Compiler/assembler/methodsz80.h \
    source/Compiler/assembler/mos6502/methods6502c64.h \
    source/Compiler/assembler/mos6502/methods6502ok64.h \
    source/Compiler/assembler/mos6502/methods6502vic20.h \
    source/Compiler/assembler/methodsx86.h \
    source/Compiler/ast/nodecase.h \
    source/Compiler/ast/nodecontrolstatement.h \
    source/Compiler/ast/noderepeatuntil.h \
    source/Compiler/compilers/compiler6502.h \
    source/Compiler/compilers/compilergbz80.h \
    source/Compiler/compilers/compilerm68k.h \
    source/Compiler/compilers/compilerx86.h \
    source/Compiler/compilers/compilerz80.h \
    source/Compiler/compilers/factorycompiler.h \
    source/Compiler/optimiser/postoptimiser.h \
    source/Compiler/optimiser/postoptimizerm68k.h \
    source/Compiler/optimiser/postoptimizerx86.h \
    source/Compiler/optimiser/postoptimizerz80.h \
    source/Compiler/systems/systemamstradcpc.h \
    source/Compiler/systems/systemappleii.h \
    source/Compiler/systems/systematari2600.h \
    source/Compiler/systems/systematari520st.h \
    source/Compiler/systems/systematari800.h \
    source/Compiler/systems/systembbcm.h \
    source/Compiler/systems/systemcoleco.h \
    source/Compiler/systems/systemgameboy.h \
    source/Compiler/systems/systemmsx.h \
    source/Compiler/systems/systemok64.h \
    source/Compiler/systems/systemplus4.h \
    source/Compiler/systems/systemspectrum.h \
    source/Compiler/systems/systemtiki100.h \
    source/Compiler/systems/systemx86.h \
    source/Compiler/systems/systemz80.h \
    source/LeLib/bbc/asmexception.h \
    source/LeLib/bbc/discimage.h \
    source/LeLib/bbc/globaldata.h \
    source/LeLib/bbc/stringutils.h \
    source/LeLib/limage/lcolor.h \
    source/LeLib/limage/limageamstradcpc.h \
    source/LeLib/limage/limageamstradgeneric.h \
    source/LeLib/limage/limageatari520st.h \
    source/LeLib/limage/limagebbc.h \
    source/LeLib/limage/limagefooter.h \
    source/LeLib/limage/limagegamboy.h \
    source/LeLib/limage/limagehybridcharset.h \
    source/LeLib/limage/limagelevelgameboy.h \
    source/LeLib/limage/limagelevelnes.h \
    source/LeLib/limage/limagemetablocksprites.h \
    source/LeLib/limage/limagemetachunk.h \
    source/LeLib/limage/limagenes.h \
    source/LeLib/limage/limageok64.h \
    source/LeLib/limage/limagevga.h \
    source/LeLib/limage/limagex16.h \
    source/LeLib/limage/lpen.h \
    source/LeLib/limage/pixelchar.h \
    source/LeLib/limage/ssim.h \
    source/LeLib/ttrfile.h \
    source/LeLib/util/fc8/FC8Compression.h \
    source/LeLib/util/lz4/lz4.h \
    source/LeLib/util/lz4/lz4hc.h \
    source/LeLib/util/spline2d.h \
    source/LeLib/util/tikidisk.h \
    source/LeLib/util/tool.h \
    source/LeLib/util/utilclasses.h \
#    source/LeLib/miniaudio_engine.h \
    source/Compiler/data_pmm.h \
    source/Compiler/errorhandler.h \
    source/Compiler/compilers/compiler.h \
    source/Compiler/lexer.h \
    source/Compiler/parser.h \
    source/Compiler/pvar.h \
    source/Compiler/symboltable.h \
    source/Compiler/syntax.h \
    source/Compiler/token.h \
    source/Compiler/assembler/asmpascal.h \
    source/Compiler/assembler/assembler.h \
    source/Compiler/assembler/mos6502/mos6502.h \
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
    source/Compiler/misc/machinecodeanalyzer.h \
    source/OrgAsm/orgasm.h \
    source/Compiler/Opcodes/opcodes6502.h \
    source/OrgAsm/orgasmlexer.h \
    source/LeLib/limage/limagevic20.h \
    source/LeLib/limage/limagesprites2.h \
    source/LeLib/limage/limagecontainer.h \
    source/Compiler/assembler/abstractastdispatcher.h \
    source/Compiler/assembler/mos6502/astdispatcher6502.h \
    source/Compiler/assembler/mos6502/methods6502.h \
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
    source/LeLib/limage/bitmapfont.h \
    source/Compiler/sourcebuilder.h \
    source/Compiler/projectbuilder.h \
    trc.h


RESOURCES += \
    resources.qrc




#LELIB INCLUDES

DISTFILES += \
    resources/bin/rasm.exe \
    resources/bin/rasm_osx \
    resources/code/amiga/init_p61_player.s \
    resources/code/amiga/p61-play.i \
    resources/code/atari520st/end.s \
    resources/code/atari520st/init.s \
    resources/code/c64_keyboard_input.asm \
    resources/code/gameboy/gbt_player.asm \
    resources/code/gameboy/gbt_player_bank1.asm \
    resources/code/vbm/vic20_vbm.asm \
    resources/code/x86/init_cga_scanlines.asm \
    resources/code/x86/init_playnote.asm \
    resources/images/AMIGA.png \
    resources/images/ATAR800.png \
    resources/images/ATARI520ST.png \
    resources/images/OK64.jpg \
    resources/images/OK64.png \
    resources/images/PLUS4.png \
    resources/images/amiga_intro.png \
    resources/images/tutorials/floskel.png \
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
    resources/text/help/c/aux_color_and_volume.rtf \
    resources/text/help/c/hpen_register.rtf \
    resources/text/help/c/hscroll_register.rtf \
    resources/text/help/c/joy1.rtf \
    resources/text/help/c/joy1pressed.rtf \
    resources/text/help/c/joy2.rtf \
    resources/text/help/c/joy2pressed.rtf \
    resources/text/help/c/joy_down.rtf \
    resources/text/help/c/joy_fire.rtf \
    resources/text/help/c/joy_left.rtf \
    resources/text/help/c/joy_right.rtf \
    resources/text/help/c/joy_up.rtf \
    resources/text/help/c/pal_or_ntsc.rtf \
    resources/text/help/c/screen_bg_color.rtf \
    resources/text/help/c/screen_char_loc.rtf \
    resources/text/help/c/screen_col_loc.rtf \
    resources/text/help/c/screen_text_height.rtf \
    resources/text/help/c/screen_text_width.rtf \
    resources/text/help/c/sound1_register.rtf \
    resources/text/help/c/sound2_register.rtf \
    resources/text/help/c/sound3_register.rtf \
    resources/text/help/c/soundfx_register.rtf \
    resources/text/help/c/vic_data_loc.rtf \
    resources/text/help/c/vic_rasterline.rtf \
    resources/text/help/c/vpen_register.rtf \
    resources/text/help/c/vscroll_register.rtf \
    resources/text/help/c/xpaddle_register.rtf \
    resources/text/help/c/ypaddle_register.rtf \
    resources/text/help/m/abs.rtf \
    resources/text/help/m/addresstable.rtf \
    resources/text/help/m/atan2.rtf \
    resources/text/help/m/bcdadd.rtf \
    resources/text/help/m/bcdcompare.rtf \
    resources/text/help/m/bcdisequal.rtf \
    resources/text/help/m/bcdprint.rtf \
    resources/text/help/m/bcdsub.rtf \
    resources/text/help/m/blockmemcpy.rtf \
    resources/text/help/m/call.rtf \
    resources/text/help/m/clearbitmap.rtf \
    resources/text/help/m/clearscreen.rtf \
    resources/text/help/m/clearsound.rtf \
    resources/text/help/m/closeirq.rtf \
    resources/text/help/m/closeirqwedge.rtf \
    resources/text/help/m/copybytesshift.rtf \
    resources/text/help/m/copycharsetfromrom.rtf \
    resources/text/help/m/copyfullscreen.rtf \
    resources/text/help/m/copyhalfscreen.rtf \
    resources/text/help/m/copyimagecolordata.rtf \
    resources/text/help/m/createaddresstable.rtf \
    resources/text/help/m/createinteger.rtf \
    resources/text/help/m/createpointer.rtf \
    resources/text/help/m/dec.rtf \
    resources/text/help/m/decrunch.rtf \
    resources/text/help/m/decrunchfromindex.rtf \
    resources/text/help/m/disableciainterrupts.rtf \
    resources/text/help/m/enableallram.rtf \
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
    resources/text/help/m/isoverlapping.rtf \
    resources/text/help/m/isoverlappingwh.rtf \
    resources/text/help/m/jammer.rtf \
    resources/text/help/m/joystick.rtf \
    resources/text/help/m/keypressed.rtf \
    resources/text/help/m/krillload.rtf \
    resources/text/help/m/krillloadcompressed.rtf \
    resources/text/help/m/leftbitshift.rtf \
    resources/text/help/m/lo.rtf \
    resources/text/help/m/loop.rtf \
    resources/text/help/m/memcpy.rtf \
    resources/text/help/m/mod.rtf \
    resources/text/help/m/moveto.rtf \
    resources/text/help/m/nmiirq.rtf \
    resources/text/help/m/nop.rtf \
    resources/text/help/m/peek.rtf \
    resources/text/help/m/playsound.rtf \
    resources/text/help/m/poke.rtf \
    resources/text/help/m/ppupoint.rtf \
    resources/text/help/m/preventirq.rtf \
    resources/text/help/m/printdecimal.rtf \
    resources/text/help/m/printnumber.rtf \
    resources/text/help/m/printstring.rtf \
    resources/text/help/m/rand.rtf \
    resources/text/help/m/random.rtf \
    resources/text/help/m/random4.rtf \
    resources/text/help/m/rasterirq.rtf \
    resources/text/help/m/readjoy1.rtf \
    resources/text/help/m/readjoy2.rtf \
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
    resources/text/help/m/tile.rtf \
    resources/text/help/m/togglebit.rtf \
    resources/text/help/m/topointer.rtf \
    resources/text/help/m/vbmsetdisplaymode.rtf \
    resources/text/help/m/wait.rtf \
    resources/text/help/m/waitforraster.rtf \
    resources/text/help/m/waitnoraster.rtf \
    resources/images/PET.png \
    resources/code/amiga/end.s \
    resources/code/amiga/init.s \
    resources/text/help/p/bitwise_operations.rtf \
    resources/text/help/p/c64.rtf \
    resources/text/help/p/constants.rtf \
    resources/text/help/p/forward_references.rtf \
    resources/text/help/p/getting_started.rtf \
    resources/text/help/p/interrupts.rtf \
    resources/text/help/p/pascal.rtf \
    resources/text/help/p/screen.rtf \
    resources/text/help/p/variables.rtf \
    resources/text/help/p/vbm_memory.rtf \
    resources/text/help/p/vbm_vic_bitmap_mode.rtf \
    resources/text/help/p/vic20.rtf \
    resources/text/help/r/and.rtf \
    resources/text/help/r/array.rtf \
    resources/text/help/r/asm.rtf \
    resources/text/help/r/at.rtf \
    resources/text/help/r/begin.rtf \
    resources/text/help/r/byte.rtf \
    resources/text/help/r/chipmem.rtf \
    resources/text/help/r/cstring.rtf \
    resources/text/help/r/define.rtf \
    resources/text/help/r/do.rtf \
    resources/text/help/r/else.rtf \
    resources/text/help/r/end.rtf \
    resources/text/help/r/endblock.rtf \
    resources/text/help/r/endif.rtf \
    resources/text/help/r/error.rtf \
    resources/text/help/r/for.rtf \
    resources/text/help/r/global.rtf \
    resources/text/help/r/if.rtf \
    resources/text/help/r/ifdef.rtf \
    resources/text/help/r/ifndef.rtf \
    resources/text/help/r/ignoremethod.rtf \
    resources/text/help/r/incbin.rtf \
    resources/text/help/r/include.rtf \
    resources/text/help/r/incnsf.rtf \
    resources/text/help/r/incsid.rtf \
    resources/text/help/r/integer.rtf \
    resources/text/help/r/interrupt.rtf \
    resources/text/help/r/invert.rtf \
    resources/text/help/r/long.rtf \
    resources/text/help/r/of.rtf \
    resources/text/help/r/offpage.rtf \
    resources/text/help/r/onpage.rtf \
    resources/text/help/r/or.rtf \
    resources/text/help/r/pointer.rtf \
    resources/text/help/r/procedure.rtf \
    resources/text/help/r/program.rtf \
    resources/text/help/r/requirefile.rtf \
    resources/text/help/r/startassembler.rtf \
    resources/text/help/r/startblock.rtf \
    resources/text/help/r/step.rtf \
    resources/text/help/r/string.rtf \
    resources/text/help/r/then.rtf \
    resources/text/help/r/to.rtf \
    resources/text/help/r/unroll.rtf \
    resources/text/help/r/use.rtf \
    resources/text/help/r/userdata.rtf \
    resources/text/help/r/var.rtf \
    resources/text/help/r/vbmexportcolor.rtf \
    resources/text/help/r/vicmemoryconfig.rtf \
    resources/text/help/r/wedge.rtf \
    resources/text/help/r/while.rtf \
    resources/text/help/m/drawtextbox.rtf \
    resources/text/help/m/drawcolortextbox.rtf \
    resources/text/tutorials.txt \
    resources/text/help/m/inccrange.rtf \
    resources/text/help/m/decrange.rtf
