DIR=../input-files-images
IN=../input-files/$1.txt
OUT=$DIR/$1

gcc InstancesGenerator.c -o InstancesGenerator
./InstancesGenerator $IN $OUT.tex
pdflatex -output-directory=$DIR $OUT.tex
open -a Preview $OUT.pdf

rm InstancesGenerator
rm $OUT.aux
rm $OUT.log
rm $OUT.tex