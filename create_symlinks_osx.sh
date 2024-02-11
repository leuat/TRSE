rm Release$1/trse.app/themes
rm Release$1/trse.app/tutorials
rm Release$1/trse.app/units
rm Release$1/trse.app/project_templates
rm Debug$1/trse.app/themes
rm Debug$1/trse.app/tutorials
rm Debug$1/trse.app/units
rm Debug$1/trse.app/project_templates

ln -sf  ../../Publish/source/themes Release$1/trse.app/themes
ln -sf  ../../Publish/tutorials Release$1/trse.app/tutorials
ln -sf  ../../units Release$1/trse.app/units
ln -sf  ../../Publish/project_templates Release$1/trse.app/project_templates

ln -sf  ../../Publish/source/themes Debug$1/trse.app/themes
ln -sf  ../../Publish/tutorials Debug$1/trse.app/tutorials
ln -sf  ../../units Debug$1/trse.app/units
ln -sf  ../../Publish/project_templates Debug$1/trse.app/project_templates

