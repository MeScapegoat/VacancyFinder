#!/bin/bash
projectFolder=LinkedInParser
currentFolder=$(pwd | rev | cut -d / -f1 | rev)
if [ "$currentFolder" != "$projectFolder" ]; then
    echo Error: couldn\'t find project folder
    echo Hint: call from $projectFolder
    exit 1
fi

rm -rv build
cmake -G Ninja -S . -B build
errorCode=$?
if [[ $errorCode -ne 0 ]]; then
    echo Error: Generating files failed.
    exit $errorCode
fi
echo Files generated successfully
