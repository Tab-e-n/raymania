@echo off
mkdir "export"

rm -rf "export/raymania_win"

mkdir "export/raymania_win"
make raymania.exe
cp "raymania.exe" "export/raymania_win/raymania.exe"
cp "GAME_README.txt" "export/raymania_win/README.txt"
cp -r "sounds" "export/raymania_win/"
cp -r "tracks" "export/raymania_win/"
cd "export"
echo Zip up folder 'raymania_win' in export/
pause