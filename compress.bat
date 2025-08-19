del "theme.zip"
del "theme.vsix"
powershell -Command "Compress-Archive -Path 'theme\*' -DestinationPath 'theme.zip' -Force"
ren "theme.zip" "theme.vsix"
echo Compressed...
