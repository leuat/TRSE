#!/bin/bash
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
  echo "All frameworks found (and a bit more):"
  find /usr/local/opt/qt/ | grep "libdbus.*dylib"
  find /usr/local/Cellar/qt/  | grep "libdbus.*dylib"
  echo "In homebrew Qt:"
  ls /usr/local/Cellar/qt/6.0.3/share/qt/
  echo "In local Qt:"
  ls /usr/local/opt/qt/
  macdeployqt=$(find /usr/local/Cellar/qt | grep "/macdeployqt$")
  $macdeployqt .
  git clone https://github.com/arl/macdeployqtfix.git
  qtdir=$(ls /usr/local/Cellar/qt/ | head -n 1)
  python macdeployqtfix/macdeployqtfix.py . "${qtdir}" || echo "macdeployqtfix failed"
  # macdeployqt should copy all the needed frameworks, but it fails to do so
  qt_frameworks_dir=$(find /usr/local/Cellar/qt/ -name Frameworks | head -n 1)
  echo "All frameworks found:"
  ls ${qt_frameworks_dir}
  cp -r ${qt_frameworks_dir}/QtDBus.framework "./Contents/Frameworks/" || echo "cp QtDBus.framework failed"
  echo "All plugins found:"
  find /usr/local/Cellar/qt | grep "/plugins/.*\.dylib"
  # macdeployqt should also copy all the needed plugins, but it fails to do so
  find /usr/local/Cellar/qt | grep "/plugins/.*\.dylib" | while read plugin_file
  do
    plugin=$(echo "$plugin_file" | sed 's,.*/plugins/,,')
    mkdir -p $(dirname "./Contents/PlugIns/${plugin}")
    echo "Copying '${plugin_file}' to '../Contents/PlugIns/${plugin}'"
    cp "${plugin_file}" "./Contents/PlugIns/${plugin}"
  done
else
  rm *.ini
  ~/Qt/6.0.1/clang_64/bin/macdeployqt .
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
