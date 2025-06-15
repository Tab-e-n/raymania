mkdir "export"

mkdir "export/win"
make "raymania.exe"
cp "raymania.exe" "export/win/raymania.exe"
cp "GAME_README.txt" "export/win/README.txt"
cp -r "sounds" "export/win/"
cp -r "tracks" "export/win/"
7z a "export/raymania_windows" "export/win"
