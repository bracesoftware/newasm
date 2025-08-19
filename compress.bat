del "extension.zip"
del "extension.vsix"
powershell -Command "Compress-Archive -Path 'theme\*' -DestinationPath 'theme.zip' -Force"
ren "theme.zip" "theme.vsix"
echo Compressed...
