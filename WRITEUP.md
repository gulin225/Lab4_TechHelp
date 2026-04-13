# ECE 312 Lab 4 Write-Up: Tech Support Diagnosis Tool

**Name:** Gulin Gurbuz
**EID:** gzg84
**Date:** 4/13/2026

---

## What This Document Is

1–2 pages of honest reflection on the decisions you made and the problems you hit. Not a summary of the lab spec.

Full credit requires:
- Two specific design choices with a stated reason and a named alternative
- Four Big-O analyses with reasoning shown
- Two concrete bugs (symptom → cause → fix → rule)
- Reflection on the knowledge base you grew through sessions
- A note on `find_shortest_path`

Vague entries ("I had a leak and fixed it") earn no credit.

---

## Section 1 — Design Choices (two required, ~100 words each)

For each: what did you choose, what was the alternative, and why?

Candidate topics:
- Array-backed stack vs. linked-list stack
- Two-pass design in `load_tree` — why not link during the read phase?
- Dynamic `PathNode` array in `find_shortest_path` vs. fixed-size stack array
- Ownership model for nodes in undo/redo — why not free on undo?
- Iterative diagnosis loop — what state did you have to track explicitly?

### 1.A — [Component]

*What I chose:*  I chose to do a DFS for find_shortest_path. 

*What I considered instead:* I considered doing a BFS instead. 

*Why:* Ultimately, I chose a DFS search, because my other methods were using DFS, so it felt more familiar to use that. Additionally, it allowed me to easily find the common ancestor by searching for both solution indexes, and putting them into an array.Then, I could easily print out divergence point by indexing backwards thorugh the array, until they have a common ancestor.

---

### 1.B — [Component]

*What I chose:* I chose to do a dynamic path node array.

*What I considered instead:* I considered doing a fixed-size stack array instead. 

*Why:* I chose to do a dynamic path node array, because I am more familiar with using dynamic variables, than using stack variables. Additionally, if I have too many nodes, the fixed-size stack array could potentially cause a stack overflow error. Also, since its a fixed size, it could lead to it the stack not having enough space, while dynamically allocated can be changed to fit my needs.

---

## Section 2 — Complexity Analysis (all four required)

Show the reasoning, not just the answer.

### 2.1 — Amortized cost of a single FrameStack push
A single FrameStack push has an amortized cost of O(1). When the size is less than the capacity, the only thing it needs to do is add the element to the stack. When the size is greater than or equal to the capacity, it is an O(N) operation. However, since the resizing only happens when size is greater than or equal to the capacity, you resize less and less as time goes on. When you have pushed N elements, you would push N elements and then resizing would happen for N-1 elements.So you would simplify to 2N/N = 2 which then simplifies to O(1), since constants do not matter.

### 2.2 — Hash table average-case lookup
The average time complexity for hash table case lookup is O(1). Finding the index has O(1) complexity because it only depends on the length of the string, and that number is a constant so it simplifies to O(1). Then, in our lab we have 31 buckets, found in the main.c file. Since I only have 25 nodes, and 31 buckets the average number of nodes per bucket would be 25/31. Then in order to find the time complexity it would be O(1 + 25/31) which simplifies to O(1).


### 2.3 — Diagnosis traversal (best, worst, average)
The best case for Diagnosis travel would be O(1), which would happen when there is only one question node and two solution leaves. The worst case would be traveling to every single node, which would happen if you only had a no or yes associated with each question. This would have a time complexity of O(N), since you reach every node. The average case (assuming a balanced tree) would be O(log(N)), because a binary tree works by splitting the answers in half each time, which is effectively log(N). 

### 2.4 — `find_shortest_path` time and space
The time complexty of my 'find_shortest_path' function is O(N). This is because there is a loop for while the stack is not empty, which has a time complexity of O(N), since at a worst case it would visit N nodes. Then, depending on the height of the tree H, we index through the pathNodeArray to set the sol1path and sol2path array's parent indices. This has a time complexity of O(H), where if the tree is balanced H = log(N), and if its not H = N. Next, we compare the two arrays starting from the last index, to find the last matching index, which gets the LCA. 
---

## Section 3 — Bugs (two required)

### 3.A — [Short name]

*Symptom:*

*Cause:* [quote the wrong line or describe the wrong logic]

*Fix:*

*Rule that would have prevented it:*

---

### 3.B — [Short name]

*Symptom:*

*Cause:*

*Fix:*

*Rule that would have prevented it:*

---

## Section 4 — Knowledge Base Reflection (~100 words)

1. How many nodes does your submitted `techsupport.dat` contain?

2. What categories of problems did you teach the program? Give one example question/solution pair for each category.

3. Look at the tree with `[V]`.  Are the questions you taught it good distinguishing questions — do they split the remaining candidates roughly in half?  Name one question you would improve and describe what you would replace it with.

4. Describe one `[F]ind Path` result.  What were the two solutions, what was the shared path, and did the output match your expectation?

---

## Section 5 — Reflection (3–5 sentences)

Answer at least two:

- What was the hardest part and why?
- What did the iterative diagnosis loop teach you about recursion?
- What would you do differently if starting over?
- Was there a moment something clicked? What was it?

---

## Section 6 — Time Log

| Date | Hours | What you worked on |
|------|-------|--------------------|
| | | |
| | | |
| | | |
| | | |
| | | |
| | | |
| | | |
| | | |
| | | |
| | | |

**Total hours:** ___
