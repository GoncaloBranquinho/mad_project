g++ -O3 -std=c++23 "$1.cpp" -o "$1"
time ./"$1" "../../PartsRectangulares/$2" ${3:+$3}
rm "$1"