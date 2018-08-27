rm -rf trse
mkdir trse
cp -r ../../release/trse.app trse/
cp -r  ../source/* trse/trse.app/	
cp -r ../tutorials/RasLib trse/RasLib/
cp -r ../tutorials/Tutorials trse/Tutorials
cp -r ../tutorials/TutorialGame_RogueBurgerOne trse/TutorialGame_RogueBurgerOne
cd trse/trse.app
 ~/Qt/5.11.0/clang_64/bin/macdeployqt .
 rm *.ini
 cd ../../
 find trse -name '*.prg' -delete
 find trse -name '*.asm' -delete

