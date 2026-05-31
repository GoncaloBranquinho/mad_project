read -p "Type Greedy algorithm to run (1, 2, 3, 3v2, 4): Greedy" greedyVersion
Greedy="Greedy"$greedyVersion

if [ ! -f "$Greedy.cpp" ]; then
    printf "$Greedy.cpp file doesn't exist\n"
else
    inputFilesPath="../../inputs/input-files/"
    read -p "Type input file name (from $inputFilesPath): " inputFileName
    fullPathToInputFile=$inputFilesPath$inputFileName

    if [ ! -f "$fullPathToInputFile" ]; then
        printf "$inputFilesPath file doesn't exist\n"
    else
        inputFilesImagesPath="../../inputs/input-files-images/"
        read -p "Type 'y' to preview input partition image (any other input otherwise): " inputAnswer
        fullPathToInputFileImage=$inputFilesImagesPath$inputFileName.pdf

        if [ "$inputAnswer" == "y" ]; then
            if [ -f "$fullPathToInputFileImage" ]; then
                open -a Preview $fullPathToInputFileImage 
            else
                printf "$inputFileName.pdf image file doesn't exist\n"
            fi
        fi

        
        if [ ! -f "$Greedy" ]; then 
            printf "Compiling $Greedy.cpp, please wait... "

            if [ "$greedyVersion" -eq "2" ]; then
                g++ -O3 -std=c++23 "$Greedy.cpp" -o $Greedy
            else
                g++ -O3 -std=c++23 "$Greedy.cpp" "../utils/Greedy"$greedyVersion"NecessaryUtils.cpp" "../utils/SharedByAllGreedysUtils.cpp" "../utils/SharedByAllUtils.cpp" -o $Greedy
            fi
        fi

        printf "\n"
        ./"$Greedy" $fullPathToInputFile
        rm $Greedy
    fi
fi
