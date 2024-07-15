projectFolder=LinkedInParser
currentFolder=$(pwd | rev | cut -d / -f1 | rev)
if [ "$currentFolder" != "$projectFolder" ]; then
    echo Error: couldn\'t find project folder
    echo Hint: call from $projectFolder
    exit 1
fi
scriptsFolder=LinuxScripts

source $scriptsFolder/generate.sh
source $scriptsFolder/buildAndRun.sh