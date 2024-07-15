#!/bin/bash
projectFolder=LinkedInParser
currentFolder=$(pwd | rev | cut -d / -f1 | rev)
if [ "$currentFolder" != "$projectFolder" ]; then
    echo Error: couldn\'t find project folder
    echo Hint: call from $projectFolder
    exit 1
fi

cmake --build build
errorCode=$?
if [[ $errorCode -ne 0 ]]; then
    exit $errorCode
fi
echo Build successfully compeleted!