inputFilesPath="../../../inputs/input-files/"
read -p "Type input file name (from $inputFilesPath): " inputFileName
fullPathToInputFile=$inputFilesPath$inputFileName

if [ ! -f "$fullPathToInputFile" ]; then
    printf "$inputFilesPath file doesn't exist\n"
else
    g++ -O3 -std=c++23 "AC3.cpp" -o "AC3"
    ./AC3 $fullPathToInputFile
fi

rm AC3