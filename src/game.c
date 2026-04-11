#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "lab4.h"

extern Node      *g_root;
extern EditStack  g_undo;
extern EditStack  g_redo;
extern Hash       g_index;

/* ----------------------------------------------------------------
 * TODO 31  run_diagnosis
 *
 * Walk the decision tree iteratively (no recursion) using a
 * FrameStack.  At each question node ask the user yes/no and push
 * the appropriate child.  At each solution leaf display the fix and
 * ask whether it solved the problem.
 *
 * If the fix did not help, enter the learning phase:
 *   - Ask the user what would actually fix the problem.
 *   - Ask for a yes/no question that distinguishes their problem
 *     from the solution just shown.
 *   - Ask which answer applies to their problem.
 *   - Create a new question node and a new solution node, wire them
 *     correctly, graft them into the tree, record an Edit for
 *     undo/redo, and index the new question with canonicalize/h_put.
 *
 * Edge case: if parent is NULL the root itself must be replaced.

 

 * ---------------------------------------------------------------- */
void run_diagnosis(void) {
    clear();
    attron(COLOR_PAIR(5) | A_BOLD);
    mvprintw(0, 0, "%-80s", " Tech Support Diagnosis");
    attroff(COLOR_PAIR(5) | A_BOLD);


    mvprintw(2, 2, "I'll help diagnose your tech problem.");
    mvprintw(3, 2, "Answer each question with y or n.");
    mvprintw(4, 2, "Press any key to start...");
    refresh();
    getch();

    FrameStack stack;
    fs_init(&stack); 
    int questionCount = 0;
    int row = 4;
    if(g_root == NULL) //if the root is empty, we must replace the root instead of adding a leaf
    {

    }
    fs_push(&stack, g_root, -1); 
    /* TODO: implement */
    while(!fs_empty(&stack)){
        Frame f = fs_pop(&stack);
        Node* current = f.node;

        if(current->isQuestion == 1){ //if its a question ask the user yes/no and push the appropriate child.
            questionCount++;
            mvprintw(row++, 2, "Y/N");
            refresh();
            char text = getch();
            if(text == 'Y'){
                fs_push(&stack, current->yes, 1);
            }
            else{
                 fs_push(&stack, current->no, 0);
            }
        }
        else{ //each solution leaf display the fix and ask whether it solved the problem.
            mvprintw(row++, 2, "Fix: %s", current->text);
            refresh();
            mvprintw(row++, 2, "Did this fix your problem? Y/N");
            refresh();
            char text = getch();

            if(text == 'Y'){ //we did fix it

            }else{
            char solution[256]; //taken from AI
            char userQuestion[256];
            mvprintw(row++, 2, "What would fix your problem?");
            refresh();

            echo(); //taken from AI
            getnstr(solution, 255);//taken from AI
            noecho();

            mvprintw(row++, 2, "What is a Y/N question that would distinguish your problem from the one above?");
            refresh();
            getnstr(userQuestion, 255);//taken from AI
            noecho();

                //now i need to create a new question node and a corresponding solution node

            }

        }
    }
    fs_free(&stack);
}

/* ----------------------------------------------------------------
 * TODO 32  undo_last_edit
 * Return 1 on success, 0 if the undo stack is empty.
 * ---------------------------------------------------------------- */
int undo_last_edit(void) {
    return 0;
}

/* ----------------------------------------------------------------
 * TODO 33  redo_last_edit
 * Return 1 on success, 0 if the redo stack is empty.
 * ---------------------------------------------------------------- */
int redo_last_edit(void) {
    return 0;
}
