printf "Compiling greedy3v2.cpp:\n"
g++ -O3 -std=c++23 "greedy3v2.cpp" "../utils/Greedy3v2ExclusiveUtils.cpp" "../utils/SharedByAllGreedysUtils.cpp" "../utils/SharedByAllUtils.cpp" -o "greedy3v2"
./"greedy3v2" "../../PartsRectangulares/$1"
rm "greedy3v2"