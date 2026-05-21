gcc InstancesGenerator.c -o InstancesGenerator && ./InstancesGenerator $1.txt $2.tex
pdflatex $2.tex
open -a Preview $2.pdf
rm InstancesGenerator
rm $2.aux
rm $2.log
rm $2.tex