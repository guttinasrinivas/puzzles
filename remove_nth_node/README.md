# TL;DR
LeetCode problem number 19.

# Introduction
Remove the `n`th node from the end of a singly linked list and return its head.

# Design Choices
The simplest design choice is to scan the list and count the number of nodes. Then calculate the node of interest, walk to it again and delete it.

A more efficient choice is to use two iterators: one to scan the list till the end, while the other maintaining a gap of exactly the `n` we are interested in. When the sanning iterator reaches the end of the list, the tracking iterator points to the node we are interested in. Removing, relink the nodes and celebrate.

While in-place deletion of the node is efficient, deleting last node is painful. Thus tracker always tracks `n - 1`th node.

Corner cases such as first node, last node are easily handled. A linked list with single entry or no entries left out for now.

# Safety
While the code is not re-entrant, most of the work is in scanning the list.

# Real World Use Case Scenario
Deleting blocks from disk, empty block tracking etc. In ideal cases they use a self-balancing tree like RBT or at least a doubly-linked list. But legacy applications or resource constrained systems might still use similar singly linked list techniques. Thus it is still useful pattern.

OTOH, corner cases are pretty much the same across these block tracking applications, for example.

# Notes

Without using any coding assistants, AI or AI tooling, it took `1:06` hours in `vim`. Did not refer to documentation.
