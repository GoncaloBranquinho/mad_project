printf "Compiling greedy1.cpp:\n"
g++ -O3 -std=c++23 "greedy1.cpp" "../utils/SharedByAllGreedysUtils.cpp" "../utils/SharedByAllUtils.cpp" -o "greedy1"
./"greedy1" "../../PartsRectangulares/$1"
rm "greedy1"
