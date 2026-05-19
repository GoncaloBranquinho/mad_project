g++ -O3 -std=c++23 "$1.cpp" "utils.cpp" -o "$1"
time ./"$1" "../../PartsRectangulares/$2"
rm "$1"