inputFilesPath="../../inputs/input-files/"
read -p "Type input file name (from $inputFilesPath): " inputFileName
fullPathToInputFile=$inputFilesPath$inputFileName

if [ ! -f "$fullPathToInputFile" ]; then
    printf "$inputFilesPath file doesn't exist\n"
else
    ./build/main $fullPathToInputFile
fi