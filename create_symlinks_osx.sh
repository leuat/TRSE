rm Release/trse.app/themes
rm Release/trse.app/tutorials
rm Release/trse.app/units
rm Release/trse.app/project_templates
rm Debug/trse.app/themes
rm Debug/trse.app/tutorials
rm Debug/trse.app/units
rm Debug/trse.app/project_templates

ln -s  ../../Publish/source/themes Release/trse.app/themes
ln -s  ../../Publish/tutorials Release/trse.app/tutorials
ln -s  ../../units Release/trse.app/units
ln -s  ../../project_templates Release/trse.app/project_templates

ln -s  ../../Publish/source/themes Debug/trse.app/themes
ln -s  ../../Publish/tutorials Debug/trse.app/tutorials
ln -s  ../../units Debug/trse.app/units
ln -s  ../../project_templates Debug/trse.app/project_templates

