# Advent of Code 2023 - 🎄

![](https://img.shields.io/badge/stars%20⭐-50-yellow) ![](https://img.shields.io/badge/days%20completed-25-red)

### Intro

This repository is my solution in `c++20` to the 2023 Advent of Code event. The solutions to the parts listed below in the task list section was completed in `Python3.11` either because it was too cumbersome to debug in C++ or because I was not proficient enough in algorithms to implement them. I will be looking to update and implement the solutions in C++ over the coming weeks.

### Task List

- [x] Python to C++
  - [x] Day 17: part 1 and part 2.
  - [x] Day 19: part 2.
  - [x] Day 20: part 1 and part 2.
  - [x] Day 24: part 2.
  - [x] Day 25: part 1.
- [ ] Include comments on top of solutions to denote problem.
- [x] Include `input.txt` used.
- [x] Include `Makefile` to clean and build everything.

### Experience

This year, 2023, was my first time participating in the Advent of Code event. I was largely motivated by two of the most influential and brilliant upperclassmen I know from Imperial College - [@Jpnock](https://github.com/Jpnock) and [@simon-staal](https://github.com/simon-staal) - I have them to thank for this exciting adventure.

During my attempts of the challenges, it was not always obvious what was wrong with my solution, so I had to use other people's programs and results to compare against to debug. In this process, I frequently visited [@jonathanpaulson](https://github.com/jonathanpaulson) and [@hyper-neutrino](https://github.com/hyper-neutrino)'s YouTube channels and repositories as references. They are great at explaining their thought process and demonstrating algorithms how algorithms work. I would recommend anyone who is stuck to give their channels a go.

Lastly, I originally anticipated the Advent of Code puzzles to be only simple coding challenges, like LeetCode problems, so I chose C++ as the language to use and practice. It turns out that the puzzles cover a wide range of topics, including not only algorithms but also linear algebra and number theory. In this case, using Python would be a better idea than using C++ as it allows for easier integration of a wide range of libraries to solve the problem, since it has a mature package manager (albeit at the cost of not being able to tweak and optimize underlying code). Needless to say, I will select Python as my preferred language for future Advent of Code events.

### Results

| Day   | Part 1 | Part 2 |
| :---: | :----- | :----- |
| 1     | >24h   | >24h   |
| 2     | >24h   | >24h   |
| 3     | >24h   | >24h   |
| 4     | 13h26m | 13h51m |
| 5     | 12h27m | >24h   |
| 6     | 11h54m | 12h03m |
| 7     | 10h36m | 11h26m |
| 8     | 09h25m | 10h00m |
| 9     | 06h32m | 06h51m |
| 10    | 07h57m | >24h   |
| 11    | >24h   | >24h   |
| 12    | >24h   | >24h   |
| 13    | >24h   | >24h   |
| 14    | 09h48m | 17h40m |
| 15    | 00h07m | 00h28m |
| 16    | 04h01m | 04h16m |
| 17    | >24h   | >24h   |
| 18    | 18h27m | >24h   |
| 19    | 06h04m | >24h   |
| 20    | >24h   | >24h   |
| 21    | 04h42m | >24h   |
| 22    | 17h25m | 18h08m |
| 23    | 01h44m | 05h03m |
| 24    | 05h05m | 08h12m |
| 25    | 06h29m | >24h   |

### File Structure

```txt
root/
├─ .github/workflow/actions.yml     -> YAML file that updates the badges on README.md
├─ day_<N>/                         -> <N> = day number
│  ├─ input.txt                     -> User-specific input
│  └─ part_<M>.cpp                  -> Solution in C++; <M> = part number
├─ Makefile                         -> Makefile to build binaries and clean binaries
├─ README.md                        -> Main docs
└─ .gitignore
```

### Makefile Usage

1. `make build`: Build all `*.cpp` files found in all day subdirectories into binaries with `g++`.
2. `make clean`: Remove all `main`, `part_1`, or `part_2` binaries in day subdirectories.
