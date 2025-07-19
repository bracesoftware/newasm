del "extension.zip"
del "extension.vsix"
powershell -Command "Compress-Archive -Path 'extension\*' -DestinationPath 'extension.zip' -Force"
ren "extension.zip" "extension.vsix"
pause