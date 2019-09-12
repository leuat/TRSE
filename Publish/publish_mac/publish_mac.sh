rm -rf trse
mkdir trse
cp -r ../../release/trse.app trse/
cp -r  ../source/* trse/trse.app/
cp -r  ../source/* trse/trse.app/	
cp -r ../tutorials trse/trse.app/
cd trse/trse.app
 ~/Qt/5.12.3/clang_64/bin/macdeployqt .
 rm *.ini
 cd ../../
 find trse -name '*.prg' -delete
 find trse -name '*.asm' -delete
 find trse -name '*.sym' -delete
tar -cvf trse_osx.tar trse/trse.app
gzip trse_osx.tar
scp trse_osx.tar.gz leuat@www.irio.co.uk:www.irio.co.uk/trse/latest_snapshots/

