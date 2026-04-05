#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "lab4.h"

extern Node *g_root;

/* ----------------------------------------------------------------
 * TODO 29  check_integrity
 *
 * Use BFS to verify:
 *   - Every question node has both yes and no children (non-NULL).
 *   - Every solution node has both children NULL.
 * Return 1 if valid, 0 if any violation is found.
 * ---------------------------------------------------------------- */
int check_integrity(void) {
    if(g_root == NULL){
        return 1; //there is no parent node, error
    }

    Queue* q = malloc(sizeof(Queue));
    if(q == NULL){
        return 0;
    }
    q_init(q);
    int idNum = 0;
    q_enqueue(q, g_root,idNum);

    while(!(q_empty(q))){
        Node* getNode;
        int getId;
        q_dequeue(q, &getNode, &getId);
        if(getNode->isQuestion){
            if(getNode->yes == NULL){
                q_free(q);
                free(q);
                return 0; //violation
            }
            if(getNode->no == NULL){
                q_free(q);
                free(q);
                return 0; //violation
            }
        if(getNode->yes != NULL){
            idNum++;
            q_enqueue(q, getNode->yes, idNum);
        }
        if(getNode->no != NULL){
            idNum++;
            q_enqueue(q, getNode->no, idNum);
        }
        }
        else{
            if(getNode->yes != NULL){
                q_free(q);
                free(q);
                return 0; //violation 
            }
            if(getNode->no != NULL){
                q_free(q);
                free(q);
                return 0; //violation
            }

        }

      

    }
    //BFS, queue root, while(queue not empty) dequee(), processnode , enqueue left, enqueue right 
    q_free(q);
    return 1;

}

/* ----------------------------------------------------------------
 * TODO 30  find_shortest_path
 *
 * Given the exact text of two solution leaves, display the
 * questions that distinguish them.  Use BFS with a parent-tracking
 * PathNode array to find both leaves, build ancestor arrays for
 * each, find the Lowest Common Ancestor (LCA), then print:
 *   - The shared path of questions both solutions pass through.
 *   - The divergence question (LCA) and which branch leads where.
 *
 * Display results with mvprintw.  Print an error if either
 * solution is not found.  Free all allocations before returning.
 //Can use DFS
 * ---------------------------------------------------------------- */
void find_shortest_path(const char *sol1, const char *sol2) {
    typedef struct PathNode{
        Node* current;
        int32_t parentIndex; 
        int32_t branch; // root = -1 1 = yes 0 = no
    }PathNode;


    if (g_root == NULL) {
        mvprintw(10, 2, "Error: knowledge base is empty.");
        refresh();
        return;
    }

    int nodeCount = count_nodes(g_root);
    if(nodeCount <= 0){
        mvprintw(10, 2, "Error: knowledge base is empty.");
        refresh();
        return;
    }

    PathNode* pathNodeArr = calloc(nodeCount, sizeof(PathNode));
    if(pathNodeArr == NULL){
        mvprintw(10, 2, "Error: Memory Allocation Failure");
        refresh();
        return;
    }
    FrameStack* fs = malloc(sizeof(FrameStack));
    fs_init(fs);
    int sol1Idx = -1; //given from AI
    int sol2Idx = -1;// given from AI

    fs_push(fs,g_root,-1);
    PathNode root;
    root.current = g_root;
    root.parentIndex = -1;
    root.branch = -1;
    int nextIndex = 0;

    pathNodeArr[0] = root;
    while(!(fs_empty)){
        Frame f = fs_pop;
        Node* current = f.node;
    }

    mvprintw(10, 2, "find_shortest_path not yet implemented.");
    refresh();
}
