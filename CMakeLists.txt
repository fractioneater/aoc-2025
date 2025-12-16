cmake_minimum_required(VERSION 4.1)
project(aoc_2025)

set(CMAKE_CXX_STANDARD 23)

include_directories(h)

add_executable(aoc_2025
	file.h
	memoizer.cpp
	day1.cpp
	day2.cpp
	day3.cpp
	day4.cpp
	day5.cpp
	day6.cpp
	day7.cpp
	day8.cpp
	day9.cpp
	day10.cpp
	day11.cpp
	day12.cpp
	main.cpp
)
