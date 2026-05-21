printf "Compiling greedy4.cpp:\n"
g++ -O3 -std=c++23 "greedy4.cpp" "../utils/Greedy4ExclusiveUtils.cpp" "../utils/SharedByAllGreedysUtils.cpp" "../utils/SharedByAllUtils.cpp" -o "greedy4"
./"greedy4" "../../PartsRectangulares/$1"
rm "greedy4"