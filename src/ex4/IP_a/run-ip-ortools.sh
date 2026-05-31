inputFilesPath="../../../inputs/input-files/"
read -p "Type input file name (from $inputFilesPath): " inputFileName
fullPathToInputFile=$inputFilesPath$inputFileName

if [ ! -f "$fullPathToInputFile" ]; then
    printf "$inputFilesPath file doesn't exist\n"
else

    inputFilesImagesPath="../../../inputs/input-files-images/"
    read -p "Type 'y' to preview input partition image (any other input otherwise): " inputAnswer
    fullPathToInputFileImage=$inputFilesImagesPath$inputFileName.pdf

    if [ "$inputAnswer" == "y" ]; then
        if [ -f "$fullPathToInputFileImage" ]; then
            open -a Preview $fullPathToInputFileImage 
        else
            printf "$inputFileName.pdf image file doesn't exist\n"
        fi
    fi

    ./build/main $fullPathToInputFile
fi


