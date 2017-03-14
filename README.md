# Search_Algorithms_EightPuzzleProblem
We implement various search algorithms to solve the Eight Puzzle Problem. For this, we use the following heuristics:
1. ha(n) = 0, i.e. leading to breadth-first search

2. hb(n) = number of misplaced tiles (excluding the blank tile)

3. hc(n) = sum of the distances of the tiles from their goal positions (excluding the blank tile)

For simulation, we generate a set of 100 random initial boards and test the search algorithms, running all the heuristics on each of the 100 problems. We also print relevant statistics such as the number of nodes expanded and the running times for each heuristic and algorithm.

