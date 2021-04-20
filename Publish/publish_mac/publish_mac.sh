rm -rf trse
mkdir trse
if [ "$1" = "nightly" ]
then
  cp -r ../../trse.app trse/
else
  cp -r ../../Release/trse.app trse/
fi

cp -r Contents trse/trse.app

# Copy themes, tutorials, units and templates
cp -r  ../source/* trse/trse.app/
cp -r ../tutorials trse/trse.app/
cp -r ../../units trse/trse.app/
cp -r ../project_templates trse/trse.app/

cd trse/trse.app
if [ "$1" = "nightly" ]
then
  macdeployqt=$(find /usr/local/Cellar/qt | grep "/macdeployqt$")
  # Correct plugin dir is /usr/local/Cellar/qt/6.0.3/share/qt/plugins/
  $macdeployqt . -verbose=3
else
  rm *.ini
  ~/Qt/5.15.0/clang_64/bin/macdeployqt .
fi
cd ../../

if [ "$1" = "nightly" ]
then
  echo "Skipping OK64 for now"
else
  cp -r ~/code/OK64/Release/OK64.app  trse/
  # No cleanup needed on nightly, git repo should be clean at checkout
  find trse -name '*.prg' -delete
  find trse -name '*.asm' -delete
  find trse -name '*.sym' -delete
fi

# TODO(ColinPitrat): Deliver OK64 too?
if [ "$1" = "nightly" ]
then
  tar -cvzf trse_osx.tar.gz trse/trse.app
else
  tar -cvzf trse_osx.tar.gz trse/trse.app trse/OK64.app
  scp trse_osx.tar.gz leuat@www.irio.co.uk:www.irio.co.uk/trse/latest_snapshots/
fi
