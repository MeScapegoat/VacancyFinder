#!/bin/bash
projectFolder=LinkedInParser
currentFolder=$(pwd | rev | cut -d / -f1 | rev)
if [ "$currentFolder" != "$projectFolder" ]; then
    echo Error: couldn\'t find project folder
    echo Hint: call from $projectFolder
    exit 1
fi

scriptsFolder=LinuxScripts
source $scriptsFolder/build.sh
errorCode=$?
if [[ $errorCode -ne 0 ]]; then
    exit $errorCode
fi
build/LinkedInParser