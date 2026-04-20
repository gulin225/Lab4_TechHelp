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
The time complexity of my 'find_shortest_path' function is O(N). This is because there is a loop for while the stack is not empty, which has a time complexity of O(N), since at a worst case it would visit N nodes. Then, depending on the height of the tree H, I index through the pathNodeArray to set the sol1path and sol2path array's parent indices. This happens twice and has a time complexity of O(H), where if the tree is balanced H = log(N), and if its not H = N. Next, I compare the two arrays starting from the last index, to find the last matching index, which gets the LCA.  Next, we go through the pathNode Array and print out the shared nodes, which aslo has a time complexity of O(H), because the amount of shared nodes depends on the height of the tree. Overall, the time complexity would be O(N + H + H + H), which would simplify to O(N). This is because if the tree were skewed, H = N, and then it would be O(2N) which equals O(N). If the tree were balanced, H = log(N), O = (N * log(N)), which simplifies to
O(N), since the N term would dominate. The space complexity would be O(N) because we allocate space for every node in the tree. Even though we do this multiple times, it would simplify to O(N).
---

## Section 3 — Bugs (two required)

### 3.A — [Segmentation Fault]

*Symptom:* My code crashed while I was looking for a LCA, stating that there was a segmentation fault. 

*Cause:* In my LCA code during the DFS, I was setting parent_index inside of the (index != 0) loop, which caused the root, at index 0, to not get a parent_index. Then, when I tried accessing the parent_index of the root, I got a segmentation fault.

*Fix:* Move the line that sets the parent_index of the pathNodeArr outside of the while(index !=0) loop, so that the root parent_index is also set. 

*Rule that would have prevented it:* Make sure that all of the data inside of a struct is set and not garbage data. 

---

### 3.B — [Memory Leaks]

*Symptom:* Valgrind showed that I was leaking 12 bytes of memory.

*Cause:* Inside of my create_question_node and create_solution_node functions, I was duplicating the string and putting it into the node. However, inside of my run_diagnosis, I was also using strdup() in order to pass the string into the function. This led to 2 allocations, and I never freed the second one.

*Fix:* Remove the second strdup, and just directly pass the string into the function.

*Rule that would have prevented it:* Do not allocate memory where you do not need it, and when you allocate memory, it should be in a place where you can free it. In my case, I was unable to free the strdup of the string I put in the function, so I just got rid of it and directly passed it into the function. 

---

## Section 4 — Knowledge Base Reflection (~100 words)

1. How many nodes does your submitted `techsupport.dat` contain? 25 nodes

2. What categories of problems did you teach the program? Give one example question/solution pair for each category. I taught the program problems concerning network devices, the computer itself, and peripheral devices. For network devices, I asked "Is the problem with a network device?" and then if "YES" I asked "Are the lights on your router blinking orange or red?" and for "NO" I said "Check the fiber line connected to the router for physical damage." For peripheral devices, I asked "Is a peripheral device not working?" and for "YES" I asked "Can the device be plugged in elsewhere?" and for "NO" I asked "Is your hard drive at 100% capacity". 

3. Look at the tree with `[V]`.  Are the questions you taught it good distinguishing questions — do they split the remaining candidates roughly in half?  Name one question you would improve and describe what you would replace it with. I believe the questions I asked were broad enough to cut candidates in half. One question I would improve is "Is your hard drive at 100% capacity?" This is because before I should have asked "Is the problem with a computer device?"

4. Describe one `[F]ind Path` result.  What were the two solutions, what was the shared path, and did the output match your expectation? I looked for "Test the device on another port/computer" and "Delete large files and empty your recycling bin" The output did match my expectation, showing that the correct shared path "Is the problem with a network device?" and then "Is a peripheral device not working." Then it shows that the divergence point is at "Is a p

---

## Section 5 — Reflection (3–5 sentences)

Answer at least two:

- What was the hardest part and why?
- What did the iterative diagnosis loop teach you about recursion?
- What would you do differently if starting over?
- Was there a moment something clicked? What was it?

The hardest part for me was definitely finding the LCA. This was because I was struggling to visualize the tree, and understand why I needed two different loops. I also just thought the function was extremely complex in and of itself. There was definitely a moment where something clicked for the run_diagnosis function. At first I was struggling on how to run through it iteratively, but then I realized that if we keep track of the parent and the branch, we can easily build the tree based on those two pieces of information. 

---

## Section 6 — Time Log

| Date | Hours | What you worked on |
|------|-------|--------------------|
| 3/31 | 3   | | DS.C functions
| 4/1  |  2  | | DS.C functions and persist.c
| 4/4 |    4|  | persist.c and utils.c
| 4/9 |    5|  | utils.c 
| 4/11 |   5  | |game.c
| 4/12 | 4  | | game.c and WRITEUP
| 4/13|   2  | | WRITEUP
| | | |
| | | |
| | | |

**Total hours:** 25
