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
    free(q);
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
    clear();
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
    int index = 0;


    //in this while loop, i created a pathnode arr that has all of the pathNodes inside of it, and it 
    //looks at whether or not you took a yes or no branch, and it has the parent index
    while(!(fs_empty(fs))){
        int32_t parentIndex = -1;
        int32_t branch = -1;
        Frame f = fs_pop(fs);
        Node* current = f.node;

        pathNodeArr[index].current = current;
        if(index != 0){ //its not the root, we check to see if we took yes or no
            for(int i =0; i < index; i++){
                if(pathNodeArr[i].current->yes == current){
                    parentIndex = i;
                    branch = 1;
                }
                else if(pathNodeArr[i].current->no == current){
                    parentIndex = i;
                    branch = 0;
                }
            }


            pathNodeArr[index].parentIndex = parentIndex;
            pathNodeArr[index].branch = branch;
           //add branach to struct here
        }

        if(strcmp(current->text, sol1) == 0){
                sol1Idx = index;
        }

        if(strcmp(current->text, sol2) == 0){
                sol2Idx = index;
        }

        index++;
        if(current->isQuestion){
        fs_push(fs, current->yes, 1);
        fs_push(fs, current->no, 0);
        }
    }

    if(sol1Idx == -1 || sol2Idx == -1){
        free(pathNodeArr);
        fs_free(fs);
        free(fs);

        mvprintw(10, 2, "Error: Solution not Found");
        refresh();
        return;
    }

    //in the second loop, we need to find their LCA, so exactly where the 2 solutions diverge
    int* path1 = calloc(nodeCount, sizeof(int));
    if(path1 == NULL){
        free(pathNodeArr);
        fs_free(fs);
        free(fs);
        mvprintw(10, 2, "Error: Memory Allocation Failure");
        refresh();
        return;
    }

    int* path2 = calloc(nodeCount, sizeof(int));
    if(path2 == NULL){
        free(path1);
        free(pathNodeArr);
        fs_free(fs);
        free(fs);
        mvprintw(10, 2, "Error: Memory Allocation Failure");
        refresh();
        return;
    }

    index = sol1Idx;
    int i =0;
    while(index != -1){ //go until you reach the parent of the root
        path1[i] = index;
        index = pathNodeArr[index].parentIndex;
        i++;
    }
    int length1 = i;

    index = sol2Idx;
    i = 0;
    while(index != -1){
        path2[i] = index;
        index = pathNodeArr[index].parentIndex;
        i++;
    }
    int length2 = i;

    int p1_ptr = length1 - 1; //taken from AI
    int p2_ptr = length2 - 1;// taken from AI
    int row = 10; //taken from AI

    while((p1_ptr >= 0) && (p2_ptr >= 0) && path1[p1_ptr] == path2[p2_ptr]){
        row = row + 1;
        mvprintw((row), 2, "Question %d: %s", row - 10, pathNodeArr[path1[p1_ptr]].current->text);
        refresh();
        p1_ptr--;
        p2_ptr--;
    }//continue going until they don't match, the place where they stop matching is the parent question
  //  row++;
 //   mvprintw((row + 1), 2, "LCA Question: %s", row - 10, pathNodeArr[path1[p1_ptr + 1]].current->text);

    char *text1, *text2;
    if(pathNodeArr[path1[p1_ptr]].branch == 1){
       text1 = "yes";
    }
    else{
        text1 = "no";
    }
    if(pathNodeArr[path2[p2_ptr]].branch == 1){
       text2 = "yes";
    }
    else{
        text2 = "no";
    }
    row++;
    mvprintw(row, 2, "For %s, the path taken is: %s", sol1, text1);
    refresh();
    row++;
    mvprintw((row), 2, "For %s, the path taken is: %s", sol2, text2);
    refresh();


    free(path1);
    free(path2);
    free(pathNodeArr);
    fs_free(fs);
    free(fs);

}
