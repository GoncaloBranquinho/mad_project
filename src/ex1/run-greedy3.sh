printf "Compiling greedy3.cpp:\n"
g++ -O3 -std=c++23 "greedy3.cpp" "../utils/Greedy3ExclusiveUtils.cpp" "../utils/SharedByAllGreedysUtils.cpp" "../utils/SharedByAllUtils.cpp" -o "greedy3"
./"greedy3" "../../PartsRectangulares/$1"
rm "greedy3"