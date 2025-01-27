output="triray.out"

files=""
files=$files" triray.c"
files=$files" rmlib.c"
files=$files" asset.c"

gcc -o $output $files -lraylib
