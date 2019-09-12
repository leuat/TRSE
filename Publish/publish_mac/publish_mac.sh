rm -rf trse
mkdir trse
cp -r ../../release/trse.app trse/
<<<<<<< HEAD
cp -r  ../source/* trse/trse.app/
Mkdir trse/tutorials/	
cp -r ../tutorials/RasLib trse/tutorials/RasLib/
cp -r ../tutorials/VIC20 trse/tutorials/VIC20
cp -r ../tutorials/C64 trse/tutorials/C64
cp -r ../tutorials/C128 trse/tutorials/C128
cp -r ../tutorials/NES trse/tutorials/NES
cp -r ../tutorials/Fjong trse/tutorials/Fjong
cp -r ../tutorials/PET trse/tutorials/Amiga
cp -r ../tutorials/Amiga trse/tutorials/Amiga
=======
cp -r  ../source/* trse/trse.app/	
cp -r ../tutorials trse/
>>>>>>> ca36e1390518ba11031c9120d2545e0d45a22455
cd trse/trse.app
 ~/Qt/5.12.3/clang_64/bin/macdeployqt .
 rm *.ini
 cd ../../
 find trse -name '*.prg' -delete
<<<<<<< HEAD
 find trse/tutorials/ -name '*.asm' -delete
=======
 find trse -name '*.asm' -delete
 find trse -name '*.sym' -delete
>>>>>>> ca36e1390518ba11031c9120d2545e0d45a22455
tar -cvf trse_osx.tar trse/
gzip trse_osx.tar
scp trse_osx.tar.gz leuat@www.irio.co.uk:www.irio.co.uk/trse/latest_snapshots/

