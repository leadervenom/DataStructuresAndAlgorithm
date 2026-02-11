#Data Structure and Algorithm
# Queue-Based Game Matchmaking System (C++)

A semester-end project for **SCSE2103-01 Data Structures & Algorithm** at **Universiti Teknologi Malaysia (UTM)**.

This project simulates a **fair matchmaking system** using core DSA concepts (especially **Queue**) by considering:
- **Rank / MMR**
- **Player behaviour score**

Players are routed into different pools (Good vs Troll) and matched within the same pool to keep games balanced.

---

## Features

- Queue-based waiting system (**FIFO**)
- Fair matching using **MMR/rank** + **behaviour score**
- Separate pools:
  - **Good Pool** (well-behaved players)
  - **Troll Pool** (poor behaviour)
- Finds closest candidates using **linear search**
- Orders candidates by MMR using **insertion sort**
- Prints the final match results after forming teams

---

## Tech Stack

- Language: **C++**
- Concepts: **Queue (FIFO)**, **Linear Search**, **Insertion Sort**, basic file/data handling

---

## Project Structure (example)

> Adjust these
.
├── src/
│ ├── main.cpp
│ ├── MatchmakingSystem.cpp
│ └── MatchmakingSystem.h
├── data/
│ ├── players.txt
│ └── heroes.txt
└── README.md
---

## How It Works (High Level)

1. Load player and hero data from the databases.
2. Add players to the matchmaking queue.
3. Route players into **Good Pool** or **Troll Pool** based on behaviour score.
4. Match players within the same pool by closest **MMR** (while respecting waiting time).
5. Output the match results and remove matched players from the queue.

---

## How To Run

### Compile
g++ -std=c++17 -O2 -o matchmaking src/main.cpp

### Run
./matchmaking


> If your code uses multiple `.cpp` files, compile them together, e.g.
g++ -std=c++17 -O2 -o matchmaking src/*.cpp

---

## Demo Data

This project includes a simple custom:
- **Player database** (player details + rank/MMR + behaviour score)
- **Hero database** (available heroes)

Update the values in `data/players.txt` and `data/heroes.txt` to test different matchmaking cases.

---

## Acknowledgement

Big thanks to **Dr Ruhaidah** for guiding and supporting me throughout the project.

---

## License

This project is for academic and learning purposes. Feel free to explore and extend it.

