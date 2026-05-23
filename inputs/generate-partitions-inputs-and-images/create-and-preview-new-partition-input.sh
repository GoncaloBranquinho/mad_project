inputFilesPath="../input-files/"
inputFilesImagesPath="../input-files-images/"

printf "Generating new partition (input file and image) ...\n"
read -p "Type file name (no extension please, i.e, .txt, .tex etc): " fileName

inputFilePath=$inputFilesPath$fileName
inputFilesImagePath=$inputFilesImagesPath$fileName

gcc InstancesGenerator.c -o InstancesGenerator
./InstancesGenerator $inputFilePath $inputFilesImagePath.tex
pdflatex -output-directory=$inputFilesImagesPath $inputFilesImagePath.tex

rm InstancesGenerator
rm $inputFilesImagePath.aux
rm $inputFilesImagePath.log
rm $inputFilesImagePath.tex

read -p "Type 'y' to preview generated partition: " inputAnswer

if [ "$inputAnswer" == "y" ]; then 
    open -a Preview $inputFilesImagePath.pdf 
fi
