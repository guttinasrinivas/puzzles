# TL;DR
Implement LeetCode Problem 79 titled Word Search.

# Introduction
Find words from a dictionary in a grid such that each successive character is in horizontally or vertically adjacent cells without reusing any cell.


# Design Choices
The simplest and of course, the chosen design choice is to use the graph theory and DFS. Walk through the dictionary and look up each word in the grid.

The time-complexity is `O(M * n)`, where `M` is the number of possible grid patterns in the given grid, and `n` is the number of words in the dictionary.

The `M` is typically small for smaller grids.

The space complexity is `O(1)`.

PoC, not designed for scalability.

# Assumptions
 * Only ASCII, English characters supported. Unicode multi-language is not supported yet.
 * Square grid `G = g x g`.
 * One word per line.
 * Case sensitive search and look up.
 * The grid is fixed to `4 x 4` characters for now.
   * But code is flexible enough to accommodate any reasonable `g x g` characters.

# Realworld Use Case Scenario
Node discovery, network traversal, path verification etc.

# Discussion


# Final Solution
