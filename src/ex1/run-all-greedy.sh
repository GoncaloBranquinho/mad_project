g++ -O3 -std=c++23 "greedy1.cpp" "../utils/utils.cpp" -o "greedy1"
printf "Running greedy1.cpp:"
./"greedy1" "../../PartsRectangulares/$1"
rm "greedy1"

g++ -O3 -std=c++23 "greedy3.cpp" "../utils/utils.cpp" -o "greedy3"
printf "Running greedy3.cpp:"
./"greedy3" "../../PartsRectangulares/$1"
rm "greedy3"

g++ -O3 -std=c++23 "greedy3v2.cpp" "../utils/utils.cpp" -o "greedy3v2"
printf "Running greedy3v2.cpp:"
./"greedy3v2" "../../PartsRectangulares/$1"
rm "greedy3v2"
