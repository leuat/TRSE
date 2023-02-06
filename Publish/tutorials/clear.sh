find . -name "*.asm" -type f -delete
find . -name "*.prg" -type f -delete
find . -name "*.sym" -type f -delete
find . -name "*.gb" -type f -delete
find . -name "*.o" -type f -delete
find . -name "*.dup" -type f -delete
find . -name "*.nes" -type f -delete
find . -name "*.com" -type f -delete

#find . -name "*.trse" -type f  -exec sed -i /project_path/d {} +
#find . -name "*.trse" -type f  -exec sed -i /open_files/d {} +
#find . -name "*.trse" -type f  -exec sed -i /current_file/d {} +

find . -name "*.trse" -type f  -exec sed -i '' -e /project_path/d {} +
find . -name "*.trse" -type f  -exec sed -i '' -e /open_files/d {} +
find . -name "*.trse" -type f  -exec sed -i '' -e /current_file/d {} +

find . -name .backup -exec rm -rf {} \;

echo "open_files =, Hello.ras" >> C64/Tutorials/C64_Tutorials.trse
echo "open_files =, RogueBurgerOne.ras" >> C64/TutorialGame_RogueBurgerOne/RogueBurgerOne.trse
echo "open_files =, demo_part2_empty.ras" >> C64/DemoMaker/demomaker.trse
echo "open_files =, example5_single_sprite.ras, example5_single_sprite.fjo" >> C64/DemoEffects_raytracer/DemoEffects_raytracer.trse
echo "open_files =, main.ras" >> C64/Disk_loader_project/disk_loader_project.trse
echo "open_files =, part3.ras" >> C64/TutorialGame_Introduction/Introductiongame.trse
echo "open_files =, intro.ras" >> C64/4kDreams/4kDreams.trse
echo "open_files =, main.ras" >> C64/16kb_cartridge_project/crt_project.trse
echo "open_files =, main.ras" >> C64/disk_demo_project/disk_demo_project.trse

echo "open_files =, easy/01_hello_world.ras" >> MEGA65/Tutorials/Mega65_Tutorials.trsee
echo "open_files =, example05_single_sprite.ras" >> MEGA65/Raytracer/DemoEffects_raytracer.trse

echo "open_files =, easy/01_hello_world.ras" >> VIC20/tutorials/vic20_tutorials.trse
echo "open_files =, demo.ras" >> VIC20/PurplePlanetYo/vicdemo.trse
echo "open_files =, nibbler.ras" >> VIC20/VicNibbler/Nibbler.trse
echo "open_files =, pumpkid.ras" >> VIC20/PumpKid/Pumpkid.trse
echo "open_files =, game.ras" >> VIC20/cheesy/echo.trse
echo "open_files =, main.ras" >> VIC20/escape/escape.trse

echo "open_files =, Hello.ras" >> PLUS4/Tutorials/PLUS4_Tutorials.trse
echo "open_files =, example1.ras" >> OK64/Tutorials/OK64.trse
echo "open_files =, kos.ras" >> OK64/KOS/kos.trse

echo "open_files =, graveintentions.ras" >> CrossPlat/GraveIntentions/GraveIntentions.trse
echo "open_files =, small_game.ras" >> C128/small_game/small_game.trse

echo "open_files =, 01_hello_world.ras" >> PET/examples/examples.trse
echo "open_files =, main.ras" >> OK64/OkComputer/ok_computer.trse
echo "open_files =, tutorial1.ras" >> GAMEBOY/tutorials/gameboy_tutorials.trse

echo "open_files =, player.ras" >> C64/MusicPlayer/dualsidplayer.trse

echo "open_files =, tutorial1.ras" >> ATARI2600/tutorials/atari2600.trse
echo "open_files =, tutorial1.ras" >> TIKI100/tutorials/tiki100.trse
echo "open_files =, tutorial1.ras" >> SPECTRUM/tutorials/spectrum.trse
echo "open_files =, tut1.ras" >> ATARI520ST/tutorials/atari520st_tutorials.trse
echo "open_files =, floskel.ras" >> C64/Floskel/floskel.trse
echo "open_files =, demo.ras" >> GAMEBOY/yo-grl/yo-grl.trse

echo "open_files =, tutorial1.ras" >> AMIGA/tutorials/tutorials.trse
echo "open_files =, tut1.ras" >> AMSTRADCPC/tutorials/amstrad_tutorials.trse
echo "open_files =, main.ras" >> AMSTRADCPC/Morketid/morketid.trse
echo "open_files =, tutorial1_helloworld.ras" >> ATARI800/Tutorials/Tutorials.trse

echo "open_files =, tutorial1.ras" >> VZ200/tutorials/main_vz200_tutorials.trse
echo "open_files =, 01_tiles.ras" >> SNES/tutorials/snes_tutorials.trse

echo "open_files =, millipede.ras" >> VZ200/millipede/millipede.trse

echo "open_files =, main.ras" >> GAMEBOY/mrtibbles/mrtibbles.trse

echo "open_files =, info.rtf" >> C64/demoeffects/c64_demoeffects.trse
echo "open_files =, tutorial1_putpixel.ras" >> TRS80COCO/tutorials/trs80coco_tutorials.trse
