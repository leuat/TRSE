rm -rf trse
mkdir trse
cp -r ../../release/trse.app trse/
cp -r  ../source/* trse/trse.app/	
cp -r ../tutorials/RasLib trse/RasLib/
cp -r ../tutorials/VIC20 trse/VIC20
cp -r ../tutorials/C64 trse/C64
cp -r ../tutorials/C128 trse/C128
cp -r ../tutorials/NES trse/NES
cp -r ../tutorials/Fjong trse/Fjong
cp -r ../tutorials/PET trse/Amiga
cp -r ../tutorials/Amiga trse/Amiga
cd trse/trse.app
 ~/Qt/5.11.0/clang_64/bin/macdeployqt .
 rm *.ini
 cd ../../
 find trse -name '*.prg' -delete
 find trse/C64/ -name '*.asm' -delete
 find trse/Vic20/ -name '*.asm' -delete
 find trse/C128/ -name '*.asm' -delete
 find trse/NES/ -name '*.asm' -delete
 find trse/Fjong/ -name '*.asm' -delete
 find trse/PET/ -name '*.asm' -delete
 find trse/Amiga/ -name '*.asm' -delete
tar -cvf trse_osx.tar trse/
gzip trse_osx.tar
scp trse_osx.tar.gz leuat@www.irio.co.uk:www/trse/latest_snapshots/

