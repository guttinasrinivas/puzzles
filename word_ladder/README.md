# TL;DR
LeetCode problem number 127.

# Introduction
Find words in a dictionary such that each successive word in the ladder differs from previous by only one letter, eventually landing on the target word.


# Design Choices

The simplest design choice is to brute force through the dictionary.


A better design approach is to find only the words matching the length of start/target words and scan through them.


The best design approach may be to find the hash of the words stored and scan through the hashes to reduce the string comparison overhead.


# Realworld Use Case Scenario

Data deduplication has the heavy string matching function, for example.


# Discussion


# Final Solution
