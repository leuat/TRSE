#!/bin/bash
RELEASE=./Release

if ! [ -d $RELEASE ]; then
    mkdir $RELEASE
fi

#if ! [ -h $RELEASE/themes ]; then
#    ln -s -r -t $RELEASE/ ./Publish/source/themes
#fi

if ! [ -h ./themes ]; then
    ln -s ./Publish/source/themes ./
fi

if ! [ -h $RELEASE/tutorials ]; then
    ln -s -r -t $RELEASE/ ./Publish/tutorials
fi

if ! [ -h $RELEASE/units ]; then
    ln -s -r -t $RELEASE/ ./units
fi

if ! [ -h $RELEASE/project_templates ]; then
    ln -s -r -t $RELEASE/ ./Publish/project_templates
fi

