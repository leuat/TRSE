cmake_minimum_required(VERSION 3.16)
project(trse VERSION 1.0 LANGUAGES C CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Gui OpenGL Widgets Qml OpenGLWidgets)
#find_package(Qt6 OPTIONAL_COMPONENTS OpenGLWidgets)

qt_add_executable(trse WIN32 MACOSX_BUNDLE
    source/Compiler/assembler/asm6502.cpp source/Compiler/assembler/asm6502.h
    source/Compiler/assembler/asm68000.cpp source/Compiler/assembler/asm68000.h
    source/Compiler/assembler/asm6809.cpp source/Compiler/assembler/asm6809.h
    source/Compiler/assembler/asmTripe.cpp source/Compiler/assembler/asmTripe.h
    source/Compiler/assembler/asmarm.cpp source/Compiler/assembler/asmarm.h
    source/Compiler/assembler/asmchip8.cpp source/Compiler/assembler/asmchip8.h
    source/Compiler/assembler/asmjdh8.cpp source/Compiler/assembler/asmjdh8.h
    source/Compiler/assembler/asmpdp11.cpp source/Compiler/assembler/asmpdp11.h
    source/Compiler/assembler/asmx86.cpp source/Compiler/assembler/asmx86.h
    source/Compiler/assembler/asmz80.cpp source/Compiler/assembler/asmz80.h
    source/Compiler/assembler/assembler.cpp source/Compiler/assembler/assembler.h
    source/Compiler/ast/ast.cpp source/Compiler/ast/ast.h
    source/Compiler/ast/node.cpp source/Compiler/ast/node.h
    source/Compiler/ast/nodeasm.cpp source/Compiler/ast/nodeasm.h
    source/Compiler/ast/nodeassign.cpp source/Compiler/ast/nodeassign.h
    source/Compiler/ast/nodebinaryclause.cpp source/Compiler/ast/nodebinaryclause.h
    source/Compiler/ast/nodebinop.cpp source/Compiler/ast/nodebinop.h
    source/Compiler/ast/nodeblock.cpp source/Compiler/ast/nodeblock.h
    source/Compiler/ast/nodebuiltinmethod.cpp source/Compiler/ast/nodebuiltinmethod.h
    source/Compiler/ast/nodecase.cpp source/Compiler/ast/nodecase.h
    source/Compiler/ast/nodecast.cpp source/Compiler/ast/nodecast.h
    source/Compiler/ast/nodecompound.cpp source/Compiler/ast/nodecompound.h
    source/Compiler/ast/nodeconditional.cpp source/Compiler/ast/nodeconditional.h
    source/Compiler/ast/nodecontrolstatement.cpp source/Compiler/ast/nodecontrolstatement.h
    source/Compiler/ast/nodefactory.cpp source/Compiler/ast/nodefactory.h
    source/Compiler/ast/nodeforloop.cpp source/Compiler/ast/nodeforloop.h
    source/Compiler/ast/nodenumber.cpp source/Compiler/ast/nodenumber.h
    source/Compiler/ast/nodeprocedure.cpp source/Compiler/ast/nodeprocedure.h
    source/Compiler/ast/nodeproceduredecl.cpp source/Compiler/ast/nodeproceduredecl.h
    source/Compiler/ast/nodeprogram.cpp source/Compiler/ast/nodeprogram.h
    source/Compiler/ast/noderepeatuntil.cpp source/Compiler/ast/noderepeatuntil.h
    source/Compiler/ast/nodestring.cpp source/Compiler/ast/nodestring.h
    source/Compiler/ast/nodeunaryop.cpp source/Compiler/ast/nodeunaryop.h
    source/Compiler/ast/nodevar.cpp source/Compiler/ast/nodevar.h
    source/Compiler/ast/nodevardecl.cpp source/Compiler/ast/nodevardecl.h
    source/Compiler/ast/nodevartype.cpp source/Compiler/ast/nodevartype.h
    source/Compiler/codegen/abstractcodegen.cpp source/Compiler/codegen/abstractcodegen.h
    source/Compiler/codegen/codegen_6502.cpp source/Compiler/codegen/codegen_6502.h
    source/Compiler/codegen/codegen_6809.cpp source/Compiler/codegen/codegen_6809.h
    source/Compiler/codegen/codegen_arm.cpp source/Compiler/codegen/codegen_arm.h
    source/Compiler/codegen/codegen_chip8.cpp source/Compiler/codegen/codegen_chip8.h
    source/Compiler/codegen/codegen_jdh8.cpp source/Compiler/codegen/codegen_jdh8.h
    source/Compiler/codegen/codegen_m68k.cpp source/Compiler/codegen/codegen_m68k.h
    source/Compiler/codegen/codegen_pdp11.cpp source/Compiler/codegen/codegen_pdp11.h
    source/Compiler/codegen/codegen_s1c88.cpp source/Compiler/codegen/codegen_s1c88.h
    source/Compiler/codegen/codegen_tripe.cpp source/Compiler/codegen/codegen_tripe.h
    source/Compiler/codegen/codegen_x86.cpp source/Compiler/codegen/codegen_x86.h
    source/Compiler/codegen/codegen_z80.cpp source/Compiler/codegen/codegen_z80.h
    source/Compiler/codegen/methods/abstractmethods.cpp source/Compiler/codegen/methods/abstractmethods.h
    source/Compiler/codegen/methods/factorymethods.cpp source/Compiler/codegen/methods/factorymethods.h
    source/Compiler/codegen/methods/methods6502.cpp source/Compiler/codegen/methods/methods6502.h
    source/Compiler/codegen/methods/methods6502c64.cpp source/Compiler/codegen/methods/methods6502c64.h
    source/Compiler/codegen/methods/methods6502ok64.cpp source/Compiler/codegen/methods/methods6502ok64.h
    source/Compiler/codegen/methods/methods6502vic20.cpp source/Compiler/codegen/methods/methods6502vic20.h
    source/Compiler/codegen/methods/methods68000.cpp source/Compiler/codegen/methods/methods68000.h
    source/Compiler/codegen/methods/methods68000atari.cpp source/Compiler/codegen/methods/methods68000atari.h
    source/Compiler/codegen/methods/methods6800amiga.cpp source/Compiler/codegen/methods/methods6800amiga.h
    source/Compiler/codegen/methods/methods6809.cpp source/Compiler/codegen/methods/methods6809.h
    source/Compiler/codegen/methods/methodsx86.cpp source/Compiler/codegen/methods/methodsx86.h
    source/Compiler/codegen/methods/methodsz80.cpp source/Compiler/codegen/methods/methodsz80.h
    source/Compiler/codegen/registerstack.cpp source/Compiler/codegen/registerstack.h
    source/Compiler/compilers/compiler.cpp source/Compiler/compilers/compiler.h
    source/Compiler/compilers/compiler6502.cpp source/Compiler/compilers/compiler6502.h
    source/Compiler/compilers/compiler65c816.cpp source/Compiler/compilers/compiler65c816.h
    source/Compiler/compilers/compiler6809.cpp source/Compiler/compilers/compiler6809.h
    source/Compiler/compilers/compilerarm.cpp source/Compiler/compilers/compilerarm.h
    source/Compiler/compilers/compilerchip8.cpp source/Compiler/compilers/compilerchip8.h
    source/Compiler/compilers/compilergbz80.cpp source/Compiler/compilers/compilergbz80.h
    source/Compiler/compilers/compilerjdh8.cpp source/Compiler/compilers/compilerjdh8.h
    source/Compiler/compilers/compilerm68k.cpp source/Compiler/compilers/compilerm68k.h
    source/Compiler/compilers/compilerpdp11.cpp source/Compiler/compilers/compilerpdp11.h
    source/Compiler/compilers/compilers1c88.cpp source/Compiler/compilers/compilers1c88.h
    source/Compiler/compilers/compilerx86.cpp source/Compiler/compilers/compilerx86.h
    source/Compiler/compilers/compilerz80.cpp source/Compiler/compilers/compilerz80.h
    source/Compiler/compilers/factorycompiler.cpp source/Compiler/compilers/factorycompiler.h
    source/Compiler/errorhandler.cpp source/Compiler/errorhandler.h
    source/Compiler/lexer.cpp source/Compiler/lexer.h
    source/Compiler/misc/machinecodeanalyzer.cpp source/Compiler/misc/machinecodeanalyzer.h
    source/Compiler/misc/sidfile.cpp source/Compiler/misc/sidfile.h
    source/Compiler/opcodes/opcodes6502.cpp source/Compiler/opcodes/opcodes6502.h
    source/Compiler/optimiser/postoptimiser.cpp source/Compiler/optimiser/postoptimiser.h
    source/Compiler/optimiser/postoptimiser6502.cpp source/Compiler/optimiser/postoptimiser6502.h
    source/Compiler/optimiser/postoptimizer6809.cpp source/Compiler/optimiser/postoptimizer6809.h
    source/Compiler/optimiser/postoptimizerm68k.cpp source/Compiler/optimiser/postoptimizerm68k.h
    source/Compiler/optimiser/postoptimizerx86.cpp source/Compiler/optimiser/postoptimizerx86.h
    source/Compiler/optimiser/postoptimizerz80.cpp source/Compiler/optimiser/postoptimizerz80.h
    source/Compiler/parser.cpp source/Compiler/parser.h
    source/Compiler/projectbuilder.cpp source/Compiler/projectbuilder.h
    source/Compiler/pvar.cpp source/Compiler/pvar.h
    source/Compiler/sourcebuilder.cpp source/Compiler/sourcebuilder.h
    source/Compiler/symboltable.cpp source/Compiler/symboltable.h
    source/Compiler/syntax.cpp source/Compiler/syntax.h
    source/Compiler/systems/abstractsystem.cpp source/Compiler/systems/abstractsystem.h
    source/Compiler/systems/factorysystem.cpp source/Compiler/systems/factorysystem.h
    source/Compiler/systems/system65c816.cpp source/Compiler/systems/system65c816.h
    source/Compiler/systems/system6809.cpp source/Compiler/systems/system6809.h
    source/Compiler/systems/systemacorn.cpp source/Compiler/systems/systemacorn.h
    source/Compiler/systems/systemagon.cpp source/Compiler/systems/systemagon.h
    source/Compiler/systems/systemamiga.cpp source/Compiler/systems/systemamiga.h
    source/Compiler/systems/systemamstradcpc.cpp source/Compiler/systems/systemamstradcpc.h
    source/Compiler/systems/systemappleii.cpp source/Compiler/systems/systemappleii.h
    source/Compiler/systems/systemarm.cpp source/Compiler/systems/systemarm.h
    source/Compiler/systems/systematari2600.cpp source/Compiler/systems/systematari2600.h
    source/Compiler/systems/systematari520st.cpp source/Compiler/systems/systematari520st.h
    source/Compiler/systems/systematari800.cpp source/Compiler/systems/systematari800.h
    source/Compiler/systems/systembbcm.cpp source/Compiler/systems/systembbcm.h
    source/Compiler/systems/systembk0010.cpp source/Compiler/systems/systembk0010.h
    source/Compiler/systems/systemc128.cpp source/Compiler/systems/systemc128.h
    source/Compiler/systems/systemc64.cpp source/Compiler/systems/systemc64.h
    source/Compiler/systems/systemchip8.cpp source/Compiler/systems/systemchip8.h
    source/Compiler/systems/systemcoleco.cpp source/Compiler/systems/systemcoleco.h
    source/Compiler/systems/systemcustom.cpp source/Compiler/systems/systemcustom.h
    source/Compiler/systems/systemdragon.cpp source/Compiler/systems/systemdragon.h
    source/Compiler/systems/systemfoenix.cpp source/Compiler/systems/systemfoenix.h
    source/Compiler/systems/systemgameboy.cpp source/Compiler/systems/systemgameboy.h
    source/Compiler/systems/systemjdh8.cpp source/Compiler/systems/systemjdh8.h
    source/Compiler/systems/systemm1arm.cpp source/Compiler/systems/systemm1arm.h
    source/Compiler/systems/systemm6800.cpp source/Compiler/systems/systemm6800.h
    source/Compiler/systems/systemmega65.cpp source/Compiler/systems/systemmega65.h
    source/Compiler/systems/systemmos6502.cpp source/Compiler/systems/systemmos6502.h
    source/Compiler/systems/systemmsx.cpp source/Compiler/systems/systemmsx.h
    source/Compiler/systems/systemnes.cpp source/Compiler/systems/systemnes.h
    source/Compiler/systems/systemok64.cpp source/Compiler/systems/systemok64.h
    source/Compiler/systems/systemoric.cpp source/Compiler/systems/systemoric.h
    source/Compiler/systems/systempcw.cpp source/Compiler/systems/systempcw.h
    source/Compiler/systems/systempdp11.cpp source/Compiler/systems/systempdp11.h
    source/Compiler/systems/systempet.cpp source/Compiler/systems/systempet.h
    source/Compiler/systems/systemplus4.cpp source/Compiler/systems/systemplus4.h
    source/Compiler/systems/systempokemonmini.cpp source/Compiler/systems/systempokemonmini.h
    source/Compiler/systems/systemprimo.cpp source/Compiler/systems/systemprimo.h
    source/Compiler/systems/systemschip.h
    source/Compiler/systems/systemsnes.cpp source/Compiler/systems/systemsnes.h
    source/Compiler/systems/systemspectrum.cpp source/Compiler/systems/systemspectrum.h
    source/Compiler/systems/systemthomson.cpp source/Compiler/systems/systemthomson.h
    source/Compiler/systems/systemtiki100.cpp source/Compiler/systems/systemtiki100.h
    source/Compiler/systems/systemtim.cpp source/Compiler/systems/systemtim.h
    source/Compiler/systems/systemtrs80.cpp source/Compiler/systems/systemtrs80.h
    source/Compiler/systems/systemtrs80coco.cpp source/Compiler/systems/systemtrs80coco.h
    source/Compiler/systems/systemtvc.cpp source/Compiler/systems/systemtvc.h
    source/Compiler/systems/systemvectrex.cpp source/Compiler/systems/systemvectrex.h
    source/Compiler/systems/systemvic20.cpp source/Compiler/systems/systemvic20.h
    source/Compiler/systems/systemvz200.cpp source/Compiler/systems/systemvz200.h
    source/Compiler/systems/systemwonderswan.cpp source/Compiler/systems/systemwonderswan.h
    source/Compiler/systems/systemx16.cpp source/Compiler/systems/systemx16.h
    source/Compiler/systems/systemx86.cpp source/Compiler/systems/systemx86.h
    source/Compiler/systems/systemz80.cpp source/Compiler/systems/systemz80.h
    source/Compiler/token.cpp source/Compiler/token.h
    source/ImageEditor/abstractimageeditor.cpp source/ImageEditor/abstractimageeditor.h
    source/ImageEditor/customqtablewidget.cpp source/ImageEditor/customqtablewidget.h
    source/ImageEditor/glwidget.cpp source/ImageEditor/glwidget.h
    source/ImageEditor/hexview.cpp source/ImageEditor/hexview.h
    source/ImageEditor/qlabellimage.cpp source/ImageEditor/qlabellimage.h
    source/LeLib/bbc/asmexception.cpp source/LeLib/bbc/asmexception.h
    source/LeLib/bbc/discimage.cpp source/LeLib/bbc/discimage.h
    source/LeLib/bbc/globaldata.cpp source/LeLib/bbc/globaldata.h
    source/LeLib/bbc/stringutils.cpp source/LeLib/bbc/stringutils.h
    source/LeLib/data.cpp source/LeLib/data.h
    source/LeLib/lglshowcases.cpp source/LeLib/lglshowcases.h
    source/LeLib/lglslideshow.cpp source/LeLib/lglslideshow.h
    source/LeLib/limage/bitmapfont.cpp source/LeLib/limage/bitmapfont.h
    source/LeLib/limage/c64fullscreenchar.cpp source/LeLib/limage/c64fullscreenchar.h
    source/LeLib/limage/charsetimage.cpp source/LeLib/limage/charsetimage.h
    source/LeLib/limage/compression.cpp source/LeLib/limage/compression.h
    source/LeLib/limage/imageleveleditor.cpp source/LeLib/limage/imageleveleditor.h
    source/LeLib/limage/lcolor.cpp source/LeLib/limage/lcolor.h
    source/LeLib/limage/lcolorlist.cpp source/LeLib/limage/lcolorlist.h
    source/LeLib/limage/limage.cpp source/LeLib/limage/limage.h
    source/LeLib/limage/limageagon.cpp source/LeLib/limage/limageagon.h
    source/LeLib/limage/limageamiga4.cpp source/LeLib/limage/limageamiga4.h
    source/LeLib/limage/limageamstradcpc.cpp source/LeLib/limage/limageamstradcpc.h
    source/LeLib/limage/limageamstradgeneric.cpp source/LeLib/limage/limageamstradgeneric.h
    source/LeLib/limage/limageamstradsprites.cpp source/LeLib/limage/limageamstradsprites.h
    source/LeLib/limage/limageatari520st.cpp source/LeLib/limage/limageatari520st.h
    source/LeLib/limage/limagebbc.cpp source/LeLib/limage/limagebbc.h
    source/LeLib/limage/limagecga.cpp source/LeLib/limage/limagecga.h
    source/LeLib/limage/limagecga160x100.cpp source/LeLib/limage/limagecga160x100.h
    source/LeLib/limage/limagecga_hires.cpp source/LeLib/limage/limagecga_hires.h
    source/LeLib/limage/limagecharsetfixedcolor.cpp source/LeLib/limage/limagecharsetfixedcolor.h
    source/LeLib/limage/limagecharsetregular.cpp source/LeLib/limage/limagecharsetregular.h
    source/LeLib/limage/limagecoco3.cpp source/LeLib/limage/limagecoco3.h
    source/LeLib/limage/limagecontainer.cpp source/LeLib/limage/limagecontainer.h
    source/LeLib/limage/limagecustomc64multicolor.cpp source/LeLib/limage/limagecustomc64multicolor.h
    source/LeLib/limage/limageeffects.cpp source/LeLib/limage/limageeffects.h
    source/LeLib/limage/limagefactory.cpp source/LeLib/limage/limagefactory.h
    source/LeLib/limage/limagefooter.cpp source/LeLib/limage/limagefooter.h
    source/LeLib/limage/limagegamboy.cpp source/LeLib/limage/limagegamboy.h
    source/LeLib/limage/limagegeneric.cpp source/LeLib/limage/limagegeneric.h
    source/LeLib/limage/limagegenericsprites.cpp source/LeLib/limage/limagegenericsprites.h
    source/LeLib/limage/limagehybridcharset.cpp source/LeLib/limage/limagehybridcharset.h
    source/LeLib/limage/limageio.cpp source/LeLib/limage/limageio.h
    source/LeLib/limage/limagejdh8.cpp source/LeLib/limage/limagejdh8.h
    source/LeLib/limage/limagelevelgameboy.cpp source/LeLib/limage/limagelevelgameboy.h
    source/LeLib/limage/limagelevelgeneric.cpp source/LeLib/limage/limagelevelgeneric.h
    source/LeLib/limage/limagelevelnes.cpp source/LeLib/limage/limagelevelnes.h
    source/LeLib/limage/limagelevelsnes.cpp source/LeLib/limage/limagelevelsnes.h
    source/LeLib/limage/limagemetablocksprites.cpp source/LeLib/limage/limagemetablocksprites.h
    source/LeLib/limage/limagemetachunk.cpp source/LeLib/limage/limagemetachunk.h
    source/LeLib/limage/limagenes.cpp source/LeLib/limage/limagenes.h
    source/LeLib/limage/limageok64.cpp source/LeLib/limage/limageok64.h
    source/LeLib/limage/limageoric.cpp source/LeLib/limage/limageoric.h
    source/LeLib/limage/limageprimo.cpp source/LeLib/limage/limageprimo.h
    source/LeLib/limage/limageqimage.cpp source/LeLib/limage/limageqimage.h
    source/LeLib/limage/limagesnes.cpp source/LeLib/limage/limagesnes.h
    source/LeLib/limage/limagesnesgeneric.cpp source/LeLib/limage/limagesnesgeneric.h
    source/LeLib/limage/limagespectrum.cpp source/LeLib/limage/limagespectrum.h
    source/LeLib/limage/limagesprites.cpp source/LeLib/limage/limagesprites.h
    source/LeLib/limage/limagesprites2.cpp source/LeLib/limage/limagesprites2.h
    source/LeLib/limage/limagethomson.cpp source/LeLib/limage/limagethomson.h
    source/LeLib/limage/limagetim.cpp source/LeLib/limage/limagetim.h
    source/LeLib/limage/limagetimgen.cpp source/LeLib/limage/limagetimgen.h
    source/LeLib/limage/limagetvc.cpp source/LeLib/limage/limagetvc.h
    source/LeLib/limage/limagevga.cpp source/LeLib/limage/limagevga.h
    source/LeLib/limage/limagevic20.cpp source/LeLib/limage/limagevic20.h
    source/LeLib/limage/limagevz200.cpp source/LeLib/limage/limagevz200.h
    source/LeLib/limage/limagex16.cpp source/LeLib/limage/limagex16.h
    source/LeLib/limage/lpen.cpp source/LeLib/limage/lpen.h
    source/LeLib/limage/movieconverter.cpp source/LeLib/limage/movieconverter.h
    source/LeLib/limage/multicolorimage.cpp source/LeLib/limage/multicolorimage.h
    source/LeLib/limage/pixelchar.cpp source/LeLib/limage/pixelchar.h
    source/LeLib/limage/ssim.cpp source/LeLib/limage/ssim.h
    source/LeLib/limage/standardcolorimage.cpp source/LeLib/limage/standardcolorimage.h
    source/LeLib/luascript.cpp source/LeLib/luascript.h
    source/LeLib/miniaudio.h
    source/LeLib/objloader/objloader.cpp source/LeLib/objloader/objloader.h
    source/LeLib/ttrfile.cpp source/LeLib/ttrfile.h
    source/LeLib/util/SimplexNoise.cpp source/LeLib/util/SimplexNoise.h
    source/LeLib/util/cc1541.cpp source/LeLib/util/cc1541.h
    source/LeLib/util/cinifile.cpp source/LeLib/util/cinifile.h
    source/LeLib/util/counter.h
    source/LeLib/util/cpmtools/config.h
    source/LeLib/util/cpmtools/cpmdir.h
    source/LeLib/util/cpmtools/cpmfs.c source/LeLib/util/cpmtools/cpmfs.h
    source/LeLib/util/cpmtools/cpmtools.c source/LeLib/util/cpmtools/cpmtools.h
    source/LeLib/util/cpmtools/device.c source/LeLib/util/cpmtools/device.h
    source/LeLib/util/cpmtools/getopt_.h
    source/LeLib/util/dirartd64.cpp source/LeLib/util/dirartd64.h
    source/LeLib/util/fc8/FC8Compression.cpp source/LeLib/util/fc8/FC8Compression.h
    source/LeLib/util/lgraph.cpp source/LeLib/util/lgraph.h
    source/LeLib/util/lmessage.cpp source/LeLib/util/lmessage.h
    #source/LeLib/util/lz4/lz4.c source/LeLib/util/lz4/lz4.h
    source/LeLib/util/lz4/lz4hc.c source/LeLib/util/lz4/lz4hc.h
    source/LeLib/util/random.h
    source/LeLib/util/spline.cpp source/LeLib/util/spline.h
    source/LeLib/util/spline2d.cpp source/LeLib/util/spline2d.h
    source/LeLib/util/tikidisk.cpp source/LeLib/util/tikidisk.h
    source/LeLib/util/tool.cpp source/LeLib/util/tool.h
    source/LeLib/util/util.cpp source/LeLib/util/util.h
    source/LeLib/util/utilclasses.cpp source/LeLib/util/utilclasses.h
    source/LeLib/util/zx0/compress.c
    source/LeLib/util/zx0/memory.c
    source/LeLib/util/zx0/optimize.c
    source/LeLib/util/zx0/zx0.c source/LeLib/util/zx0/zx0.h
    source/OrgAsm/morgasm.cpp source/OrgAsm/morgasm.h
    source/OrgAsm/orgasm.cpp source/OrgAsm/orgasm.h
    source/OrgAsm/orgasm68k.cpp
    source/OrgAsm/zorgasm.cpp source/OrgAsm/zorgasm.h
    source/PmmEdit/asmhighlighter.cpp source/PmmEdit/asmhighlighter.h
    source/PmmEdit/codeeditor.cpp source/PmmEdit/codeeditor.h
    source/PmmEdit/fjongeditor.cpp source/PmmEdit/fjongeditor.h
    source/PmmEdit/fjonghighlighter.cpp source/PmmEdit/fjonghighlighter.h
    source/PmmEdit/highlighter.cpp source/PmmEdit/highlighter.h
    source/PmmEdit/trsehighlighter.cpp source/PmmEdit/trsehighlighter.h
    source/Raytracer/camera.cpp source/Raytracer/camera.h
    source/Raytracer/material.cpp source/Raytracer/material.h
    source/Raytracer/particles.cpp source/Raytracer/particles.h
    source/Raytracer/rayobject.cpp source/Raytracer/rayobject.h
    source/Raytracer/raytracer.cpp source/Raytracer/raytracer.h
    source/Raytracer/raytracerglobals.cpp source/Raytracer/raytracerglobals.h
    source/chip8emu/c8asm.c source/chip8emu/c8asm.h
    source/chip8emu/chip8emu.cpp source/chip8emu/chip8emu.h
    source/chip8emu/dialogchip8.cpp source/chip8emu/dialogchip8.h source/chip8emu/dialogchip8.ui
    source/dialogs/dialogabout.cpp source/dialogs/dialogabout.h source/dialogs/dialogabout.ui
    source/dialogs/dialogcolors.cpp source/dialogs/dialogcolors.h source/dialogs/dialogcolors.ui
    source/dialogs/dialogcolorselect.cpp source/dialogs/dialogcolorselect.h source/dialogs/dialogcolorselect.ui
    source/dialogs/dialogdonate.cpp source/dialogs/dialogdonate.h source/dialogs/dialogdonate.ui
    source/dialogs/dialogeffects.cpp source/dialogs/dialogeffects.h source/dialogs/dialogeffects.ui
    source/dialogs/dialogexport.cpp source/dialogs/dialogexport.h source/dialogs/dialogexport.ui
    source/dialogs/dialogexport3d.cpp source/dialogs/dialogexport3d.h source/dialogs/dialogexport3d.ui
    source/dialogs/dialogfindfile.cpp source/dialogs/dialogfindfile.h source/dialogs/dialogfindfile.ui
    source/dialogs/dialoghelp.cpp source/dialogs/dialoghelp.h source/dialogs/dialoghelp.ui
    source/dialogs/dialogimagehelp.cpp source/dialogs/dialogimagehelp.h source/dialogs/dialogimagehelp.ui
    source/dialogs/dialogimport.cpp source/dialogs/dialogimport.h source/dialogs/dialogimport.ui
    source/dialogs/dialoginfo.cpp source/dialogs/dialoginfo.h source/dialogs/dialoginfo.ui
    source/dialogs/dialogmemoryanalyze.cpp source/dialogs/dialogmemoryanalyze.h source/dialogs/dialogmemoryanalyze.ui
    source/dialogs/dialogmessage.cpp source/dialogs/dialogmessage.h source/dialogs/dialogmessage.ui
    source/dialogs/dialognew8bplproject.cpp source/dialogs/dialognew8bplproject.h source/dialogs/dialognew8bplproject.ui
    source/dialogs/dialognewimage.cpp source/dialogs/dialognewimage.h source/dialogs/dialognewimage.ui
    source/dialogs/dialognewproject.cpp source/dialogs/dialognewproject.h source/dialogs/dialognewproject.ui
    source/dialogs/dialognewtrt.cpp source/dialogs/dialognewtrt.h source/dialogs/dialognewtrt.ui
    source/dialogs/dialogprojectbuilder.cpp source/dialogs/dialogprojectbuilder.h source/dialogs/dialogprojectbuilder.ui
    source/dialogs/dialogprojectsettings.cpp source/dialogs/dialogprojectsettings.h source/dialogs/dialogprojectsettings.ui
    source/dialogs/dialogselectcharset.cpp source/dialogs/dialogselectcharset.h source/dialogs/dialogselectcharset.ui
    source/dialogs/dialogselectroom.cpp source/dialogs/dialogselectroom.h source/dialogs/dialogselectroom.ui
    source/dialogs/dialogsimplelineedit.cpp source/dialogs/dialogsimplelineedit.h source/dialogs/dialogsimplelineedit.ui
    source/dialogs/dialogsizeanalyser.cpp source/dialogs/dialogsizeanalyser.h source/dialogs/dialogsizeanalyser.ui
    source/dialogs/dialogsplash.cpp source/dialogs/dialogsplash.h source/dialogs/dialogsplash.ui
    source/dialogs/dialogtrsesettings.cpp source/dialogs/dialogtrsesettings.h source/dialogs/dialogtrsesettings.ui
    source/dialogs/formhelp.cpp source/dialogs/formhelp.h source/dialogs/formhelp.ui
    source/dialogs/formtutorialitem.cpp source/dialogs/formtutorialitem.h source/dialogs/formtutorialitem.ui
    source/effects/abstractdemoeffect.cpp source/effects/abstractdemoeffect.h
    source/effects/demoeffectcharsettwister.cpp source/effects/demoeffectcharsettwister.h
    source/effects/demoeffectraytracer.cpp source/effects/demoeffectraytracer.h
    source/effects/demoeffecttwister.cpp source/effects/demoeffecttwister.h
    source/main.cpp
    source/mainwindow.cpp source/mainwindow.h source/mainwindow.ui
    source/misc/cli.cpp source/misc/cli.h
    source/misc/imageeditor.cpp source/misc/imageeditor.h
    source/misc/imageworker.cpp source/misc/imageworker.h
    source/misc/messages.cpp source/misc/messages.h
    source/misc/toolbox.cpp source/misc/toolbox.h
    source/misc/toolboxitem.cpp source/misc/toolboxitem.h
    source/misc/tutorials.cpp source/misc/tutorials.h
    source/misc/workerthread.cpp source/misc/workerthread.h
    source/mynth/ma_context.cpp source/mynth/ma_context.h
    source/mynth/mynth_osc.h
    source/mynth/mynthesizer.cpp source/mynth/mynthesizer.h
    source/trsedocuments/dialogcustomwarning.cpp source/trsedocuments/dialogcustomwarning.h source/trsedocuments/dialogcustomwarning.ui
    source/trsedocuments/formfjong.cpp source/trsedocuments/formfjong.h source/trsedocuments/formfjong.ui
    source/trsedocuments/formhexedit.cpp source/trsedocuments/formhexedit.h source/trsedocuments/formhexedit.ui
    source/trsedocuments/formimageeditor.cpp source/trsedocuments/formimageeditor.h source/trsedocuments/formimageeditor.ui
    source/trsedocuments/formpaw.cpp source/trsedocuments/formpaw.h source/trsedocuments/formpaw.ui
    source/trsedocuments/formraseditor.cpp source/trsedocuments/formraseditor.h source/trsedocuments/formraseditor.ui
    source/trsedocuments/formrtf.cpp source/trsedocuments/formrtf.h source/trsedocuments/formrtf.ui
    source/trsedocuments/formttredit.cpp source/trsedocuments/formttredit.h source/trsedocuments/formttredit.ui
    source/trsedocuments/helpdocumentbuilder.cpp source/trsedocuments/helpdocumentbuilder.h
    source/trsedocuments/trsedocument.cpp source/trsedocuments/trsedocument.h
    source/trsedocuments/ttrview.cpp source/trsedocuments/ttrview.h
    source/trsetracker/dialogtrseinstrumenteditor.cpp source/trsetracker/dialogtrseinstrumenteditor.h source/trsetracker/dialogtrseinstrumenteditor.ui
    source/trsetracker/ttrplayer.cpp source/trsetracker/ttrplayer.h
    source/trsetracker/widgetpattern.cpp source/trsetracker/widgetpattern.h source/trsetracker/widgetpattern.ui
)
qt_add_qml_module(trse
    URI trse
    VERSION ${PROJECT_VERSION}
)

# Avoid collision between executable name and QML module directory (QTBUG-98568).
if(NOT WIN32 AND NOT APPLE)
    set_property(TARGET trse PROPERTY OUTPUT_NAME trse_app)
endif()

target_include_directories(trse PRIVATE
    libs/lua/include
)

target_compile_definitions(trse PRIVATE
    QT_DEPRECATED_WARNINGS
    USE_LUA
#    USE_OMP
)

target_link_libraries(trse PRIVATE
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    Qt::Qml
    Qt::Widgets
    Qt::OpenGLWidgets
)


# Resources:
set(resources_resource_files
    "resources/bin/apple2/ac.jar"
    "resources/bin/apple2/disk.do"
    "resources/bin/apple2/stripped.do"
    "resources/bin/bootsect.bin"
    "resources/bin/columntab.bin"
    "resources/bin/crt_header.bin"
    "resources/bin/gbt-player/gbt_player.o"
    "resources/bin/gbt-player/gbt_player_bank1.o"
    "resources/bin/krill_18/install_PAL_NTSC_1000-c64.prg"
    "resources/bin/krill_18/install_PAL_NTSC_2000-c64.prg"
    "resources/bin/krill_18/install_PAL_NTSC_3000-c64.prg"
    "resources/bin/krill_18/install_PAL_NTSC_4000-c64.prg"
    "resources/bin/krill_18/install_PAL_NTSC_5000-c64.prg"
    "resources/bin/krill_18/install_PAL_NTSC_6000-c64.prg"
    "resources/bin/krill_18/install_PAL_NTSC_7000-c64.prg"
    "resources/bin/krill_18/install_PAL_NTSC_8000-c64.prg"
    "resources/bin/krill_18/install_PAL_NTSC_9000-c64.prg"
    "resources/bin/krill_18/install_PAL_NTSC_A000-c64.prg"
    "resources/bin/krill_18/install_PAL_NTSC_B000-c64.prg"
    "resources/bin/krill_18/loader_PAL_NTSC_0200-c64.prg"
    "resources/bin/krill_18/loader_PAL_NTSC_1000-c64.prg"
    "resources/bin/krill_18/loader_PAL_NTSC_2000-c64.prg"
    "resources/bin/krill_18/loader_PAL_NTSC_3000-c64.prg"
    "resources/bin/krill_18/loader_PAL_NTSC_4000-c64.prg"
    "resources/bin/krill_18/loader_PAL_NTSC_5000-c64.prg"
    "resources/bin/krill_18/loader_PAL_NTSC_6000-c64.prg"
    "resources/bin/krill_18/loader_PAL_NTSC_7000-c64.prg"
    "resources/bin/krill_18/loader_PAL_NTSC_8000-c64.prg"
    "resources/bin/krill_18/loader_PAL_NTSC_9000-c64.prg"
    "resources/bin/krill_18/loader_PAL_NTSC_A000-c64.prg"
    "resources/bin/krill_18/loader_PAL_NTSC_B000-c64.prg"
    "resources/bin/krill_19/install_PAL_NTSC_1000-c64.prg"
    "resources/bin/krill_19/install_PAL_NTSC_2000-c64.prg"
    "resources/bin/krill_19/install_PAL_NTSC_3000-c64.prg"
    "resources/bin/krill_19/install_PAL_NTSC_4000-c64.prg"
    "resources/bin/krill_19/install_PAL_NTSC_5000-c64.prg"
    "resources/bin/krill_19/install_PAL_NTSC_6000-c64.prg"
    "resources/bin/krill_19/install_PAL_NTSC_7000-c64.prg"
    "resources/bin/krill_19/install_PAL_NTSC_8000-c64.prg"
    "resources/bin/krill_19/install_PAL_NTSC_9000-c64.prg"
    "resources/bin/krill_19/install_PAL_NTSC_A000-c64.prg"
    "resources/bin/krill_19/install_PAL_NTSC_B000-c64.prg"
    "resources/bin/krill_19/loader_PAL_NTSC_0200-c64.prg"
    "resources/bin/krill_19/loader_PAL_NTSC_1000-c64.prg"
    "resources/bin/krill_19/loader_PAL_NTSC_2000-c64.prg"
    "resources/bin/krill_19/loader_PAL_NTSC_3000-c64.prg"
    "resources/bin/krill_19/loader_PAL_NTSC_4000-c64.prg"
    "resources/bin/krill_19/loader_PAL_NTSC_5000-c64.prg"
    "resources/bin/krill_19/loader_PAL_NTSC_6000-c64.prg"
    "resources/bin/krill_19/loader_PAL_NTSC_7000-c64.prg"
    "resources/bin/krill_19/loader_PAL_NTSC_8000-c64.prg"
    "resources/bin/krill_19/loader_PAL_NTSC_9000-c64.prg"
    "resources/bin/krill_19/loader_PAL_NTSC_A000-c64.prg"
    "resources/bin/krill_19/loader_PAL_NTSC_B000-c64.prg"
    "resources/bin/rasm"
    "resources/bin/rasm.exe"
    "resources/bin/rasm_osx"
    "resources/bin/tiki/disk.dsk"
    "resources/bin/tiki/tiki.rom"
    "resources/bin/tim_floppy/diskdefs"
    "resources/bin/tim_floppy/empty_9600.img"
    "resources/bin/tim_floppy/floppy/emu.com"
    "resources/bin/tim_floppy/floppy/ldr.com"
    "resources/bin/tim_floppy/floppy/mdsk.com"
    "resources/bin/tim_floppy/floppy/names.ndr"
    "resources/bin/tim_floppy/floppy/start.com"
    "resources/bin/tim_floppy/floppy/tim.fcp"
    "resources/bin/tim_floppy/floppy/tim.iop"
    "resources/bin/tim_floppy/floppy/tim.rcp"
    "resources/bin/tim_floppy/floppy/tim.z3t"
    "resources/bin/tim_floppy/floppy/valias.com"
    "resources/bin/tim_floppy/floppy/wheel.com"
    "resources/bin/tim_floppy/floppy/zcpr3.sys"
    "resources/character.rom"
    "resources/code/6502/mul8bit_signed.asm"
    "resources/code/6809/mul16.asm"
    "resources/code/6809/vectrex_header.asm"
    "resources/code/Z80/init.s"
    "resources/code/Z80/init_z180.s"
    "resources/code/Z80/memcpy.asm"
    "resources/code/amiga/end.s"
    "resources/code/amiga/end_stripped.s"
    "resources/code/amiga/init.s"
    "resources/code/amiga/init_p61_player.s"
    "resources/code/amiga/init_projectall.s"
    "resources/code/amiga/init_stripped.s"
    "resources/code/amiga/intline.s"
    "resources/code/amiga/matmul.s"
    "resources/code/amiga/matmulvec.s"
    "resources/code/amiga/matmulvec_norm_z.s"
    "resources/code/amiga/p61-play.i"
    "resources/code/amiga/poly.s"
    "resources/code/amiga/simpleproject.s"
    "resources/code/amstrad/playerakg.asm"
    "resources/code/amstrad/playerakg_soundeffects.asm"
    "resources/code/amstrad/unpack.asm"
    "resources/code/arm/init.s"
    "resources/code/atan2.asm"
    "resources/code/atari2600/end.asm"
    "resources/code/atari2600/init.asm"
    "resources/code/atari520st/end.s"
    "resources/code/atari520st/init.s"
    "resources/code/atari520st/poly.s"
    "resources/code/c64_keyboard_input.asm"
    "resources/code/coleco/footer.asm"
    "resources/code/coleco/header.asm"
    "resources/code/fjong_template.fjo"
    "resources/code/gameboy/defs.s"
    "resources/code/gameboy/gbt_player.asm"
    "resources/code/gameboy/gbt_player.inc"
    "resources/code/gameboy/gbt_player_bank1.asm"
    "resources/code/gameboy/hardware.inc"
    "resources/code/gameboy/init.s"
    "resources/code/init_decompress.asm"
    "resources/code/init_wedge_bad.ras"
    "resources/code/init_wedge_regular.ras"
    "resources/code/log2_table.asm"
    "resources/code/mega65/config.cfg"
    "resources/code/mega65/init.asm"
    "resources/code/nes_end.asm"
    "resources/code/nes_init.asm"
    "resources/code/nes_readinput.asm"
    "resources/code/pokemini/init.asm"
    "resources/code/pokemini/pm.s"
    "resources/code/snes/end.asm"
    "resources/code/snes/init.asm"
    "resources/code/snes/smc.cfg"
    "resources/code/tiki100/unpack.asm"
    "resources/code/vbm/init_vbm.asm"
    "resources/code/vbm/vbmClear.asm"
    "resources/code/vbm/vbmScreenShiftLeft.asm"
    "resources/code/vbm/vbmScreenShiftRight.asm"
    "resources/code/vbm/vbmScrollFixBottom.asm"
    "resources/code/vbm/vbmScrollFixTop.asm"
    "resources/code/vbm/vbmScrollLeft.asm"
    "resources/code/vbm/vbmScrollRight.asm"
    "resources/code/vbm/vbmSetPosition.asm"
    "resources/code/vic20_irq.asm"
    "resources/code/vic20_mod_music.asm"
    "resources/code/vic20_playsid_init.asm"
    "resources/code/videoton.asm"
    "resources/code/wonderswan/end.s"
    "resources/code/wonderswan/init.s"
    "resources/code/x86/init_cga_scanlines.asm"
    "resources/code/x86/init_drawline_cga.asm"
    "resources/code/x86/init_keyboard_irq.asm"
    "resources/code/x86/init_playnote.asm"
    "resources/fonts/c64.ttf"
    "resources/images/ACORN.png"
    "resources/images/AGON.png"
    "resources/images/AMIGA.png"
    "resources/images/AMSTRADCPC.png"
    "resources/images/APPLEII.png"
    "resources/images/ATARI2600.png"
    "resources/images/ATARI520ST.png"
    "resources/images/ATARI800.png"
    "resources/images/BBCM.png"
    "resources/images/C128.png"
    "resources/images/C64.png"
    "resources/images/CHIP8.png"
    "resources/images/CROSSPLATFORM.png"
    "resources/images/DRAGON.png"
    "resources/images/FOENIX.png"
    "resources/images/GAMEBOY.png"
    "resources/images/JDH8.png"
    "resources/images/MEGA65.png"
    "resources/images/MSX.png"
    "resources/images/NES.png"
    "resources/images/OK64.png"
    "resources/images/ORIC.png"
    "resources/images/PET.png"
    "resources/images/PLUS4.png"
    "resources/images/POKEMONMINI.png"
    "resources/images/PRIMO.png"
    "resources/images/SNES.png"
    "resources/images/SPECTRUM.png"
    "resources/images/THOMSON.png"
    "resources/images/TIKI100.png"
    "resources/images/TIM.png"
    "resources/images/TRS80.png"
    "resources/images/TRS80COCO.png"
    "resources/images/TVC.png"
    "resources/images/VECTREX.png"
    "resources/images/VIC20.png"
    "resources/images/VZ200.png"
    "resources/images/WONDERSWAN.png"
    "resources/images/X16.png"
    "resources/images/X86.png"
    "resources/images/amiga_intro.png"
    "resources/images/asm_icon.png"
    "resources/images/beep.png"
    "resources/images/billboard/b1.png"
    "resources/images/billboard/b10.png"
    "resources/images/billboard/b11.png"
    "resources/images/billboard/b12.png"
    "resources/images/billboard/b13.png"
    "resources/images/billboard/b14.png"
    "resources/images/billboard/b15.png"
    "resources/images/billboard/b16.png"
    "resources/images/billboard/b17.png"
    "resources/images/billboard/b18.png"
    "resources/images/billboard/b19.png"
    "resources/images/billboard/b2.png"
    "resources/images/billboard/b20.png"
    "resources/images/billboard/b21.png"
    "resources/images/billboard/b22.png"
    "resources/images/billboard/b23.png"
    "resources/images/billboard/b24.png"
    "resources/images/billboard/b25.png"
    "resources/images/billboard/b26.png"
    "resources/images/billboard/b27.png"
    "resources/images/billboard/b28.png"
    "resources/images/billboard/b29.png"
    "resources/images/billboard/b3.png"
    "resources/images/billboard/b30.png"
    "resources/images/billboard/b31.png"
    "resources/images/billboard/b4.png"
    "resources/images/billboard/b5.png"
    "resources/images/billboard/b6.png"
    "resources/images/billboard/b7.png"
    "resources/images/billboard/b8.png"
    "resources/images/billboard/b9.png"
    "resources/images/bin.png"
    "resources/images/brush_blur.png"
    "resources/images/brush_circle.png"
    "resources/images/brush_circle_background.png"
    "resources/images/brush_copy.png"
    "resources/images/brush_dither.png"
    "resources/images/brush_drawcircle.png"
    "resources/images/brush_fill.png"
    "resources/images/brush_invert.png"
    "resources/images/brush_line.png"
    "resources/images/brush_rect.png"
    "resources/images/brush_rotate.png"
    "resources/images/brush_shape.png"
    "resources/images/brush_spray.png"
    "resources/images/brush_wet.png"
    "resources/images/charset-trans.png"
    "resources/images/charset_icons/btn-1-FullView.png"
    "resources/images/charset_icons/btn-2-CharView.png"
    "resources/images/charset_icons/btn-3-RepeatTile.png"
    "resources/images/charset_icons/btn-4-NoRepeatTile.png"
    "resources/images/charset_icons/btn-5-Copy.png"
    "resources/images/charset_icons/btn-6-Paste.png"
    "resources/images/charset_icons/btn-7-ShiftUp.png"
    "resources/images/charset_icons/btn-8-ShiftDown.png"
    "resources/images/charset_icons/btn-9-ShiftLeft.png"
    "resources/images/charset_icons/btn-9-ShiftRightt.png"
    "resources/images/charset_icons/btn-a-mirror-horiz.png"
    "resources/images/charset_icons/btn-b-mirror-vert.png"
    "resources/images/charset_icons/btn-c-cleart.png"
    "resources/images/cmb_icon.png"
    "resources/images/colortextboxexample.png"
    "resources/images/creatures.png"
    "resources/images/data-trans.png"
    "resources/images/error.png"
    "resources/images/friday.jpg"
    "resources/images/friday.png"
    "resources/images/fx-trans.png"
    "resources/images/hammer.jpg"
    "resources/images/hammer.png"
    "resources/images/help_icon.png"
    "resources/images/image_icon.png"
    "resources/images/import-export-trans.png"
    "resources/images/levels-trans.png"
    "resources/images/morketid.png"
    "resources/images/nopgrl.png"
    "resources/images/optic1.png"
    "resources/images/paw_icon.png"
    "resources/images/petscii.png"
    "resources/images/plab.png"
    "resources/images/portraits/andy.png"
    "resources/images/portraits/andy2.png"
    "resources/images/portraits/bowler.png"
    "resources/images/portraits/bowler_small.png"
    "resources/images/portraits/brain.png"
    "resources/images/portraits/brain_small.png"
    "resources/images/portraits/dead_astronaut.png"
    "resources/images/portraits/dead_astronaut_small.png"
    "resources/images/portraits/hairguy.png"
    "resources/images/portraits/hairguy_small.png"
    "resources/images/portraits/hat.png"
    "resources/images/portraits/hat_small.png"
    "resources/images/portraits/italian.png"
    "resources/images/portraits/italian_small.png"
    "resources/images/portraits/leuat2.png"
    "resources/images/portraits/leuat_gb.png"
    "resources/images/portraits/leuat_small.png"
    "resources/images/portraits/megane.png"
    "resources/images/portraits/megane_small.png"
    "resources/images/portraits/nina_small.png"
    "resources/images/portraits/ninja.png"
    "resources/images/portraits/prettyboy.png"
    "resources/images/portraits/prettyboy_small.png"
    "resources/images/portraits/pushead.png"
    "resources/images/portraits/red_sunglasses.png"
    "resources/images/portraits/red_sunglasses_small.png"
    "resources/images/portraits/robot.png"
    "resources/images/portraits/robot_small.png"
    "resources/images/portraits/turk_grey.png"
    "resources/images/ras.png"
    "resources/images/shapes/heart_line.png"
    "resources/images/shapes/star_line.png"
    "resources/images/shapes/star_thin_filled.png"
    "resources/images/sid.png"
    "resources/images/superski2021.png"
    "resources/images/tabs-meta-trans.png"
    "resources/images/tabs-meta.png"
    "resources/images/tape.png"
    "resources/images/textboxexample.png"
    "resources/images/torus.jpg"
    "resources/images/trse_banner.png"
    "resources/images/trse_icon.png"
    "resources/images/trse_optic.png"
    "resources/images/trselogo.png"
    "resources/images/tru.png"
    "resources/images/tutorials/4kdreams.png"
    "resources/images/tutorials/SGATBG.gif"
    "resources/images/tutorials/VZPaganitzu.png"
    "resources/images/tutorials/bbc1.png"
    "resources/images/tutorials/bbc2.png"
    "resources/images/tutorials/bigpixelintro.png"
    "resources/images/tutorials/c128.png"
    "resources/images/tutorials/cat.png"
    "resources/images/tutorials/cheesy.gif"
    "resources/images/tutorials/crossplatform.png"
    "resources/images/tutorials/demoeffects.png"
    "resources/images/tutorials/dos1.png"
    "resources/images/tutorials/dos2.png"
    "resources/images/tutorials/dos3.png"
    "resources/images/tutorials/escape.png"
    "resources/images/tutorials/fig3d.png"
    "resources/images/tutorials/fjong1.png"
    "resources/images/tutorials/floppy.jpeg"
    "resources/images/tutorials/floskel.png"
    "resources/images/tutorials/grave_intentions.png"
    "resources/images/tutorials/gulch.png"
    "resources/images/tutorials/hopscotch.png"
    "resources/images/tutorials/joypaddle.png"
    "resources/images/tutorials/mega1.png"
    "resources/images/tutorials/mega2.png"
    "resources/images/tutorials/mega65_platformer.png"
    "resources/images/tutorials/mibri1.png"
    "resources/images/tutorials/mrtibbles.png"
    "resources/images/tutorials/nibbler.gif"
    "resources/images/tutorials/ok1.png"
    "resources/images/tutorials/ok2.png"
    "resources/images/tutorials/ok3.png"
    "resources/images/tutorials/olimp.png"
    "resources/images/tutorials/pbm.png"
    "resources/images/tutorials/plus4_tut.png"
    "resources/images/tutorials/ppl.png"
    "resources/images/tutorials/pumpkid.png"
    "resources/images/tutorials/rb1.png"
    "resources/images/tutorials/redwine.png"
    "resources/images/tutorials/small_game_c128.png"
    "resources/images/tutorials/soundtest.png"
    "resources/images/tutorials/spacewar.png"
    "resources/images/tutorials/sparkle.png"
    "resources/images/tutorials/timtris.png"
    "resources/images/tutorials/torus.png"
    "resources/images/tutorials/trse_game_tuts.png"
    "resources/images/tutorials/trse_vic.png"
    "resources/images/tutorials/units.png"
    "resources/images/tutorials/vbm.png"
    "resources/images/tutorials/vixmas.png"
    "resources/images/tutorials/vz200.png"
    "resources/images/tutorials/vz200_demo.png"
    "resources/images/tutorials/wormwood.png"
    "resources/images/tutorials/yogrl.png"
    "resources/images/tutorials/zx.png"
    "resources/palette/nes.pal"
    "resources/palette/nes.txt"
    "resources/shaders/screen.frag"
    "resources/shaders/screen.vert"
    "resources/shaders/screen_showcases.frag"
    "resources/shaders/screen_showcases.vert"
    "resources/shaders/screen_slider.frag"
    "resources/shaders/screen_slider.vert"
    "resources/text/Documentation.txt"
    "resources/text/about.txt"
    "resources/text/adlib_instruments.txt"
    "resources/text/diskdefs"
    "resources/text/help/c/aux_color_and_volume.rtf"
    "resources/text/help/c/hpen_register.rtf"
    "resources/text/help/c/hscroll_register.rtf"
    "resources/text/help/c/joy1.rtf"
    "resources/text/help/c/joy1pressed.rtf"
    "resources/text/help/c/joy2.rtf"
    "resources/text/help/c/joy2pressed.rtf"
    "resources/text/help/c/joy_down.rtf"
    "resources/text/help/c/joy_fire.rtf"
    "resources/text/help/c/joy_left.rtf"
    "resources/text/help/c/joy_right.rtf"
    "resources/text/help/c/joy_up.rtf"
    "resources/text/help/c/pal_or_ntsc.rtf"
    "resources/text/help/c/ppuctrl.rtf"
    "resources/text/help/c/ppumask.rtf"
    "resources/text/help/c/screen_bg_color.rtf"
    "resources/text/help/c/screen_char_loc.rtf"
    "resources/text/help/c/screen_col_loc.rtf"
    "resources/text/help/c/screen_text_height.rtf"
    "resources/text/help/c/screen_text_width.rtf"
    "resources/text/help/c/sound1_register.rtf"
    "resources/text/help/c/sound2_register.rtf"
    "resources/text/help/c/sound3_register.rtf"
    "resources/text/help/c/soundfx_register.rtf"
    "resources/text/help/c/vic_data_loc.rtf"
    "resources/text/help/c/vic_rasterline.rtf"
    "resources/text/help/c/viccr0.rtf"
    "resources/text/help/c/viccr1.rtf"
    "resources/text/help/c/viccr2.rtf"
    "resources/text/help/c/viccr3.rtf"
    "resources/text/help/c/viccr4.rtf"
    "resources/text/help/c/viccr5.rtf"
    "resources/text/help/c/viccr6.rtf"
    "resources/text/help/c/viccr7.rtf"
    "resources/text/help/c/viccr8.rtf"
    "resources/text/help/c/viccr9.rtf"
    "resources/text/help/c/viccra.rtf"
    "resources/text/help/c/viccrb.rtf"
    "resources/text/help/c/viccrc.rtf"
    "resources/text/help/c/viccrd.rtf"
    "resources/text/help/c/viccre.rtf"
    "resources/text/help/c/viccrf.rtf"
    "resources/text/help/c/vpen_register.rtf"
    "resources/text/help/c/vscroll_register.rtf"
    "resources/text/help/c/xpaddle_register.rtf"
    "resources/text/help/c/ypaddle_register.rtf"
    "resources/text/help/f/addc64linetodata.rtf"
    "resources/text/help/f/addobject.rtf"
    "resources/text/help/f/addrawcharsetdata.rtf"
    "resources/text/help/f/addscreen.rtf"
    "resources/text/help/f/clearallobjects.rtf"
    "resources/text/help/f/compressandsavehorizontaldata.rtf"
    "resources/text/help/f/compresscharset.rtf"
    "resources/text/help/f/globals.rtf"
    "resources/text/help/f/introduction_to_the_fjong_raytracer.rtf"
    "resources/text/help/f/message.rtf"
    "resources/text/help/f/output.rtf"
    "resources/text/help/f/saveimage.rtf"
    "resources/text/help/f/saverawdata.rtf"
    "resources/text/help/f/savescreenandcharset.rtf"
    "resources/text/help/f/setposition.rtf"
    "resources/text/help/f/setrotation.rtf"
    "resources/text/help/m/ablit.rtf"
    "resources/text/help/m/abs.rtf"
    "resources/text/help/m/addbreakpoint.rtf"
    "resources/text/help/m/addcoppercommand.rtf"
    "resources/text/help/m/addresstable.rtf"
    "resources/text/help/m/applycopperlist.rtf"
    "resources/text/help/m/atan2.rtf"
    "resources/text/help/m/bcdadd.rtf"
    "resources/text/help/m/bcdcompare.rtf"
    "resources/text/help/m/bcdisequal.rtf"
    "resources/text/help/m/bcdprint.rtf"
    "resources/text/help/m/bcdsub.rtf"
    "resources/text/help/m/blockmemcpy.rtf"
    "resources/text/help/m/call.rtf"
    "resources/text/help/m/clearbitmap.rtf"
    "resources/text/help/m/clearscreen.rtf"
    "resources/text/help/m/clearsound.rtf"
    "resources/text/help/m/closeirq.rtf"
    "resources/text/help/m/copybytesshift.rtf"
    "resources/text/help/m/copycharsetfromrom.rtf"
    "resources/text/help/m/copyfullscreen.rtf"
    "resources/text/help/m/copyhalfscreen.rtf"
    "resources/text/help/m/copyimagecolordata.rtf"
    "resources/text/help/m/createaddresstable.rtf"
    "resources/text/help/m/createinteger.rtf"
    "resources/text/help/m/createpointer.rtf"
    "resources/text/help/m/dec.rtf"
    "resources/text/help/m/decrange.rtf"
    "resources/text/help/m/decrunch.rtf"
    "resources/text/help/m/decrunchfromindex.rtf"
    "resources/text/help/m/definescreen.rtf"
    "resources/text/help/m/disableciainterrupts.rtf"
    "resources/text/help/m/drawcolortextbox.rtf"
    "resources/text/help/m/drawline.rtf"
    "resources/text/help/m/drawtextbox.rtf"
    "resources/text/help/m/enableallram.rtf"
    "resources/text/help/m/enableinterrupt.rtf"
    "resources/text/help/m/enableirq.rtf"
    "resources/text/help/m/enablerasterirq.rtf"
    "resources/text/help/m/endcustomcopperlist.rtf"
    "resources/text/help/m/fill.rtf"
    "resources/text/help/m/fillfast.rtf"
    "resources/text/help/m/fld.rtf"
    "resources/text/help/m/getbit.rtf"
    "resources/text/help/m/getkey.rtf"
    "resources/text/help/m/hi.rtf"
    "resources/text/help/m/hideborderx.rtf"
    "resources/text/help/m/hidebordery.rtf"
    "resources/text/help/m/inc.rtf"
    "resources/text/help/m/incrange.rtf"
    "resources/text/help/m/inczp.rtf"
    "resources/text/help/m/initcustomcopperlist.rtf"
    "resources/text/help/m/initsinetable.rtf"
    "resources/text/help/m/isoverlapping.rtf"
    "resources/text/help/m/isoverlappingwh.rtf"
    "resources/text/help/m/jammer.rtf"
    "resources/text/help/m/joystick.rtf"
    "resources/text/help/m/keypressed.rtf"
    "resources/text/help/m/krillload.rtf"
    "resources/text/help/m/krillloadcompressed.rtf"
    "resources/text/help/m/leftbitshift.rtf"
    "resources/text/help/m/lo.rtf"
    "resources/text/help/m/loadpalette.rtf"
    "resources/text/help/m/loop.rtf"
    "resources/text/help/m/matmul3x3.rtf"
    "resources/text/help/m/max.rtf"
    "resources/text/help/m/memcpy.rtf"
    "resources/text/help/m/memcpyfast.rtf"
    "resources/text/help/m/memcpyunroll.rtf"
    "resources/text/help/m/min.rtf"
    "resources/text/help/m/mod.rtf"
    "resources/text/help/m/mod16.rtf"
    "resources/text/help/m/moveto.rtf"
    "resources/text/help/m/nmiirq.rtf"
    "resources/text/help/m/nop.rtf"
    "resources/text/help/m/peek.rtf"
    "resources/text/help/m/playsound.rtf"
    "resources/text/help/m/poke.rtf"
    "resources/text/help/m/poke16.rtf"
    "resources/text/help/m/ppupoint.rtf"
    "resources/text/help/m/ppupointi.rtf"
    "resources/text/help/m/ppuread.rtf"
    "resources/text/help/m/ppusingle.rtf"
    "resources/text/help/m/ppuwrite.rtf"
    "resources/text/help/m/preventirq.rtf"
    "resources/text/help/m/printdecimal.rtf"
    "resources/text/help/m/printnumber.rtf"
    "resources/text/help/m/printstring.rtf"
    "resources/text/help/m/rand.rtf"
    "resources/text/help/m/random.rtf"
    "resources/text/help/m/rasterirq.rtf"
    "resources/text/help/m/readinput.rtf"
    "resources/text/help/m/readjoy1.rtf"
    "resources/text/help/m/readjoy2.rtf"
    "resources/text/help/m/return.rtf"
    "resources/text/help/m/returninterrupt.rtf"
    "resources/text/help/m/returnvalue.rtf"
    "resources/text/help/m/rightbitshift.rtf"
    "resources/text/help/m/screenoff.rtf"
    "resources/text/help/m/screenon.rtf"
    "resources/text/help/m/scroll.rtf"
    "resources/text/help/m/scrollx.rtf"
    "resources/text/help/m/scrolly.rtf"
    "resources/text/help/m/setbank.rtf"
    "resources/text/help/m/setbitmapmode.rtf"
    "resources/text/help/m/setcharsetandscreenlocation.rtf"
    "resources/text/help/m/setcharsetlocation.rtf"
    "resources/text/help/m/setcolormemoryaddress.rtf"
    "resources/text/help/m/setcopperlist32.rtf"
    "resources/text/help/m/setfrequency.rtf"
    "resources/text/help/m/sethi.rtf"
    "resources/text/help/m/setlo.rtf"
    "resources/text/help/m/setmemoryconfig.rtf"
    "resources/text/help/m/setmulticolormode.rtf"
    "resources/text/help/m/setnametable.rtf"
    "resources/text/help/m/setpalette.rtf"
    "resources/text/help/m/setrotationx.rtf"
    "resources/text/help/m/setscreenlocation.rtf"
    "resources/text/help/m/setspriteloc.rtf"
    "resources/text/help/m/setspritelocation.rtf"
    "resources/text/help/m/setspritepos.rtf"
    "resources/text/help/m/settextmode.rtf"
    "resources/text/help/m/spritepos.rtf"
    "resources/text/help/m/sqrt.rtf"
    "resources/text/help/m/startirq.rtf"
    "resources/text/help/m/startrasterchain.rtf"
    "resources/text/help/m/strcmp.rtf"
    "resources/text/help/m/strgetfromindex.rtf"
    "resources/text/help/m/strsplit.rtf"
    "resources/text/help/m/swap.rtf"
    "resources/text/help/m/tile.rtf"
    "resources/text/help/m/togglebackground.rtf"
    "resources/text/help/m/togglebackgroundtableaddress.rtf"
    "resources/text/help/m/togglebit.rtf"
    "resources/text/help/m/togglenmi.rtf"
    "resources/text/help/m/togglesprites.rtf"
    "resources/text/help/m/togglespritetableaddress.rtf"
    "resources/text/help/m/togglevram32inc.rtf"
    "resources/text/help/m/vbirq.rtf"
    "resources/text/help/m/vbmclear.rtf"
    "resources/text/help/m/vbmclearblock.rtf"
    "resources/text/help/m/vbmclearblot.rtf"
    "resources/text/help/m/vbmclearcolor.rtf"
    "resources/text/help/m/vbmcleardot.rtf"
    "resources/text/help/m/vbmclearsmalltext.rtf"
    "resources/text/help/m/vbmclearsprite.rtf"
    "resources/text/help/m/vbmclearsprite16.rtf"
    "resources/text/help/m/vbmclearsprite2.rtf"
    "resources/text/help/m/vbmclearsprite8.rtf"
    "resources/text/help/m/vbmclearspriteslice.rtf"
    "resources/text/help/m/vbmcleartext.rtf"
    "resources/text/help/m/vbmcleartile.rtf"
    "resources/text/help/m/vbmcleartilemap.rtf"
    "resources/text/help/m/vbmcopyfrombuffer.rtf"
    "resources/text/help/m/vbmcopytobuffer.rtf"
    "resources/text/help/m/vbmdebug.rtf"
    "resources/text/help/m/vbmdrawbcd.rtf"
    "resources/text/help/m/vbmdrawblot.rtf"
    "resources/text/help/m/vbmdrawblote.rtf"
    "resources/text/help/m/vbmdrawdot.rtf"
    "resources/text/help/m/vbmdrawdote.rtf"
    "resources/text/help/m/vbmdrawsmallbcd.rtf"
    "resources/text/help/m/vbmdrawsmallbcdoy.rtf"
    "resources/text/help/m/vbmdrawsmalltexte.rtf"
    "resources/text/help/m/vbmdrawsmalltexto.rtf"
    "resources/text/help/m/vbmdrawsprite.rtf"
    "resources/text/help/m/vbmdrawsprite16.rtf"
    "resources/text/help/m/vbmdrawsprite16e.rtf"
    "resources/text/help/m/vbmdrawsprite2.rtf"
    "resources/text/help/m/vbmdrawsprite2E.rtf"
    "resources/text/help/m/vbmdrawsprite8.rtf"
    "resources/text/help/m/vbmdrawsprite8e.rtf"
    "resources/text/help/m/vbmdrawspritee.rtf"
    "resources/text/help/m/vbmdrawspriteslice.rtf"
    "resources/text/help/m/vbmdrawspriteslicee.rtf"
    "resources/text/help/m/vbmdrawtext.rtf"
    "resources/text/help/m/vbmdrawtexte.rtf"
    "resources/text/help/m/vbmdrawtexto.rtf"
    "resources/text/help/m/vbmdrawtile.rtf"
    "resources/text/help/m/vbmdrawtilee.rtf"
    "resources/text/help/m/vbmdrawtilemap.rtf"
    "resources/text/help/m/vbmdrawtilemape.rtf"
    "resources/text/help/m/vbmdrawtilemapo.rtf"
    "resources/text/help/m/vbmdrawtileo.rtf"
    "resources/text/help/m/vbmnextcolumn.rtf"
    "resources/text/help/m/vbmresetdisplaymode.rtf"
    "resources/text/help/m/vbmscreenshiftleft.rtf"
    "resources/text/help/m/vbmscreenshiftright.rtf"
    "resources/text/help/m/vbmscrollfixbottom.rtf"
    "resources/text/help/m/vbmscrollfixtop.rtf"
    "resources/text/help/m/vbmscrollleft.rtf"
    "resources/text/help/m/vbmscrollright.rtf"
    "resources/text/help/m/vbmsetcolumn.rtf"
    "resources/text/help/m/vbmsetdisplaymode.rtf"
    "resources/text/help/m/vbmsetposition1.rtf"
    "resources/text/help/m/vbmsetposition2.rtf"
    "resources/text/help/m/vbmsetposition4.rtf"
    "resources/text/help/m/vbmspriteshiftl.rtf"
    "resources/text/help/m/vbmspriteshiftr.rtf"
    "resources/text/help/m/vbmspriteshiftsl.rtf"
    "resources/text/help/m/vbmspriteshiftsr.rtf"
    "resources/text/help/m/vbmspritestitch.rtf"
    "resources/text/help/m/vbmtestpixel.rtf"
    "resources/text/help/m/vbmtestpixel2.rtf"
    "resources/text/help/m/vbmtesttilepixel.rtf"
    "resources/text/help/m/vbmtesttilepixel2.rtf"
    "resources/text/help/m/wait.rtf"
    "resources/text/help/m/waitforraster.rtf"
    "resources/text/help/m/waitnoraster.rtf"
    "resources/text/help/m/waitverticalblank.rtf"
    "resources/text/help/p/asm_files.rtf"
    "resources/text/help/p/asm_tips.rtf"
    "resources/text/help/p/bitwise_operations.rtf"
    "resources/text/help/p/buildtable.rtf"
    "resources/text/help/p/buildtable2d.rtf"
    "resources/text/help/p/c64.rtf"
    "resources/text/help/p/constants.rtf"
    "resources/text/help/p/exomizer.rtf"
    "resources/text/help/p/fjo_files.rtf"
    "resources/text/help/p/flf_files.rtf"
    "resources/text/help/p/forward_references.rtf"
    "resources/text/help/p/getting_started.rtf"
    "resources/text/help/p/interrupts.rtf"
    "resources/text/help/p/pascal.rtf"
    "resources/text/help/p/paw_files.rtf"
    "resources/text/help/p/postoptimize.rtf"
    "resources/text/help/p/ras_files.rtf"
    "resources/text/help/p/removeunusedsymbols.rtf"
    "resources/text/help/p/rtf_files.rtf"
    "resources/text/help/p/screen.rtf"
    "resources/text/help/p/tru_files.rtf"
    "resources/text/help/p/variables.rtf"
    "resources/text/help/p/vbm_memory.rtf"
    "resources/text/help/p/vbm_vic_bitmap_mode.rtf"
    "resources/text/help/p/vic20.rtf"
    "resources/text/help/p/vic_interrupts.rtf"
    "resources/text/help/r/addemulatorparam.rtf"
    "resources/text/help/r/and.rtf"
    "resources/text/help/r/array.rtf"
    "resources/text/help/r/asm.rtf"
    "resources/text/help/r/at.rtf"
    "resources/text/help/r/begin.rtf"
    "resources/text/help/r/bin2inc.rtf"
    "resources/text/help/r/break.rtf"
    "resources/text/help/r/buildtable.rtf"
    "resources/text/help/r/byte.rtf"
    "resources/text/help/r/case.rtf"
    "resources/text/help/r/chipmem.rtf"
    "resources/text/help/r/compressed.rtf"
    "resources/text/help/r/const.rtf"
    "resources/text/help/r/continue.rtf"
    "resources/text/help/r/cpcexport0.rtf"
    "resources/text/help/r/cpcexportpal.rtf"
    "resources/text/help/r/cpcexporttile0.rtf"
    "resources/text/help/r/cstring.rtf"
    "resources/text/help/r/define.rtf"
    "resources/text/help/r/do.rtf"
    "resources/text/help/r/donotremove.rtf"
    "resources/text/help/r/else.rtf"
    "resources/text/help/r/end.rtf"
    "resources/text/help/r/endassembler.rtf"
    "resources/text/help/r/endblock.rtf"
    "resources/text/help/r/endif.rtf"
    "resources/text/help/r/endmacro.rtf"
    "resources/text/help/r/error.rtf"
    "resources/text/help/r/export.rtf"
    "resources/text/help/r/exportcompressed.rtf"
    "resources/text/help/r/exportframe.rtf"
    "resources/text/help/r/exportprg2bin.rtf"
    "resources/text/help/r/for.rtf"
    "resources/text/help/r/fori.rtf"
    "resources/text/help/r/forward.rtf"
    "resources/text/help/r/function.rtf"
    "resources/text/help/r/global.rtf"
    "resources/text/help/r/if.rtf"
    "resources/text/help/r/ifdef.rtf"
    "resources/text/help/r/ifndef.rtf"
    "resources/text/help/r/ignoremethod.rtf"
    "resources/text/help/r/incbin.rtf"
    "resources/text/help/r/include.rtf"
    "resources/text/help/r/incnsf.rtf"
    "resources/text/help/r/incsid.rtf"
    "resources/text/help/r/inline.rtf"
    "resources/text/help/r/integer.rtf"
    "resources/text/help/r/interrupt.rtf"
    "resources/text/help/r/invert.rtf"
    "resources/text/help/r/krillsloader.rtf"
    "resources/text/help/r/length.rtf"
    "resources/text/help/r/long.rtf"
    "resources/text/help/r/macro.rtf"
    "resources/text/help/r/not.rtf"
    "resources/text/help/r/of.rtf"
    "resources/text/help/r/offpage.rtf"
    "resources/text/help/r/onpage.rtf"
    "resources/text/help/r/or.rtf"
    "resources/text/help/r/pathtool.rtf"
    "resources/text/help/r/pbmexport.rtf"
    "resources/text/help/r/pointer.rtf"
    "resources/text/help/r/procedure.rtf"
    "resources/text/help/r/program.rtf"
    "resources/text/help/r/projectsettings.rtf"
    "resources/text/help/r/pure.rtf"
    "resources/text/help/r/pure_number.rtf"
    "resources/text/help/r/pure_variable.rtf"
    "resources/text/help/r/record.rtf"
    "resources/text/help/r/return.rtf"
    "resources/text/help/r/shl.rtf"
    "resources/text/help/r/shr.rtf"
    "resources/text/help/r/sizeof.rtf"
    "resources/text/help/r/startassembler.rtf"
    "resources/text/help/r/startblock.rtf"
    "resources/text/help/r/step.rtf"
    "resources/text/help/r/string.rtf"
    "resources/text/help/r/then.rtf"
    "resources/text/help/r/to.rtf"
    "resources/text/help/r/unit.rtf"
    "resources/text/help/r/unroll.rtf"
    "resources/text/help/r/use.rtf"
    "resources/text/help/r/userdata.rtf"
    "resources/text/help/r/var.rtf"
    "resources/text/help/r/vbmcompilechunk.rtf"
    "resources/text/help/r/vbmexport.rtf"
    "resources/text/help/r/vbmexportchunk.rtf"
    "resources/text/help/r/vbmexportcolor.rtf"
    "resources/text/help/r/vbmexportpixel.rtf"
    "resources/text/help/r/vicmemoryconfig.rtf"
    "resources/text/help/r/volatile.rtf"
    "resources/text/help/r/wedge.rtf"
    "resources/text/help/r/while.rtf"
    "resources/text/news.txt"
    "resources/text/opcodes.txt"
    "resources/text/opcodes_6809.txt"
    "resources/text/opcodes_GS4510.txt"
    "resources/text/opcodes_m68k.txt"
    "resources/text/opcodes_s1c88.txt"
    "resources/text/opcodes_z80.txt"
    "resources/text/project_templates.txt"
    "resources/text/syntax.txt"
    "resources/text/tutorials.txt"
)

qt_add_resources(trse "resources"
    PREFIX
        "/"
    FILES
        ${resources_resource_files}
)

if((VER EQUAL 6))
    target_link_libraries(trse PRIVATE
        Qt::OpenGLWidgets
    )
endif()

if(MACOS)
    target_link_directories(trse PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/lua/
        /opt/homebrew/opt/libomp/lib
    )

    target_link_libraries(trse PRIVATE
        # Remove flag: --framework
        # Remove flag: -AGL
        dl
        luamac_arm
        omp
    )

    target_compile_options(trse
        -I/opt/homebrew/opt/libomp/include
        -I/usr/local/include
        -O3
        -Werror=deprecated-declarations
        -Werror=return-type
        -Xpreprocessor
        -fopenmp
    )
endif()

if(win32-g++)
    set_target_properties(trse PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
    target_link_libraries(trse PRIVATE
        # Remove flag: -fopenmp
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/lua/liblua.a
        opengl32
    )

    target_compile_options(trse
        -fopenmp
    )
endif()

if(win32-msvc_x_)
    target_link_directories(trse PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/lua/
    )

    target_link_libraries(trse PRIVATE
        # Remove flag: -openmp
        lua
        opengl32
    )

    target_compile_options(trse
        -openmp
    )
endif()



if(LINUX)
    target_link_directories(trse PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/lua/
    )
    target_compile_definitions(trse PRIVATE
        USE_OMP
    )
    find_package(OpenMP)
    target_link_libraries(trse PRIVATE
        # Remove flag: -fopenmp
	OpenMP::OpenMP_CXX
        #dl
        lua
    )

    target_compile_options(trse PRIVATE 
#        -Werror
#        -Wall
        -Wno-delete-non-virtual-dtor
        -Wno-format-truncation
        -Wno-implicit-fallthrough
        -Wno-mismatched-new-delete
        -Wno-missing-field-initializers
        -Wno-sign-compare
        -Wno-stringop-overflow
        -Wno-type-limits
        -Wno-unused-but-set-variable
        -Wno-unused-function
        -Wno-unused-parameter
        -Wno-unused-result
        -Wno-unused-value
        -Wno-unused-variable
    )
endif()

if(freebsd_x_)

    target_link_directories(trse PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/lua/
    )

    target_link_libraries(trse PRIVATE
        lua_bsd
    )
endif()

install(TARGETS trse
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
