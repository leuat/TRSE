rm -rf trse
mkdir trse
cp -r ../../release/trse.app trse/
cp -r  ../source/* trse/trse.app/	
cp -r ../tutorials/RasLib trse/RasLib/
cp -r ../tutorials/Tutorials trse/Tutorials
cp -r ../tutorials/TutorialGame_RogueBurgerOne trse/TutorialGame_RogueBurgerOne
cp -r ../tutorials/TutorialGame_Introduction trse/TutorialGame_Introduction
cp -r ../tutorials/Vic20 trse/Vic20
cd trse/trse.app
 ~/Qt/5.11.0/clang_64/bin/macdeployqt .
 rm *.ini
 cd ../../
 find trse -name '*.prg' -delete
 find trse/Tutorials/ -name '*.asm' -delete
 find trse/TutorialGame_RogueBurgerOne/ -name '*.asm' -delete
 find trse/TutorialGame_Introduction/ -name '*.asm' -delete
 find trse/Vic20/ -name '*.asm' -delete
tar -cvf trse_osx.tar trse/
gzip trse_osx.tar
scp trse_osx.tar.gz leuat@www.irio.co.uk:www/trse/latest_snapshots/

