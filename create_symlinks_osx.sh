rm Release$1/trse.app/themes
rm Release$1/trse.app/tutorials
rm Release$1/trse.app/units
rm Release$1/trse.app/project_templates
rm Debug$1/trse.app/themes
rm Debug$1/trse.app/tutorials
rm Debug$1/trse.app/units
rm Debug$1/trse.app/project_templates

ln -s  ../../Publish/source/themes Release$1/trse.app/themes
ln -s  ../../Publish/tutorials Release$1/trse.app/tutorials
ln -s  ../../units Release$1/trse.app/units
ln -s  ../../project_templates Release$1/trse.app/project_templates

ln -s  ../../Publish/source/themes Debug$1/trse.app/themes
ln -s  ../../Publish/tutorials Debug$1/trse.app/tutorials
ln -s  ../../units Debug/trse.app$1/units
ln -s  ../../project_templates Debug$1/trse.app/project_templates

