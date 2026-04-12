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
    Node* parent = NULL;
    int branch = -1; //1 = yes, 0 = no, 

    int row = 4;
    if(g_root == NULL) //if the root is empty, we must replace the root instead of adding a leaf
    {
        char* solution = get_input(row++, 2, "Knowledge Base Empty. What is a solution that would fix your problem?");
        g_root = create_solution_node(strdup(solution));

        Edit edit;
        edit.type = EDIT_INSERT_SPLIT;
        edit.parent = NULL; //we can just set it to parent cuz the parent is set inside of the qeustion node
        edit.wasYesChild = -1;
        edit.oldLeaf = NULL;
        edit.newQuestion = g_root; //taken from AI
        edit.newLeaf = NULL;

        es_push(&g_undo, edit);
        es_clear(&g_redo); //taken from AI
        return;
    }
    fs_push(&stack, g_root, -1); 
    /* TODO: implement */
    while(!fs_empty(&stack)){
        Frame f = fs_pop(&stack);
        Node* current = f.node;

        if(current->isQuestion == 1){ //if its a question ask the user yes/no and push the appropriate child.
            parent = current;
            mvprintw(row++, 2, "%s", current->text);
            mvprintw(row++, 2, "Y/N");
            refresh();
            char text = getch();
            if(text == 'Y' || text == 'y'){
                branch = 1;
                fs_push(&stack, current->yes, 1);
            }
            else{
                branch = 0;
                 fs_push(&stack, current->no, 0);
            }
        }
        else{ //each solution leaf display the fix and ask whether it solved the problem.
            mvprintw(row++, 2, "Fix: %s", current->text);
            refresh();
            mvprintw(row++, 2, "Did this fix your problem? Y/N");
            refresh();
            char text = getch();

            if(text == 'Y' || text == 'y'){ //we did fix it
            mvprintw(row++, 2, "Glad we could help! Press any key to return to the menu.");
            refresh();
            getch();
            }
            else{
        
            char* solution = get_input(row++, 2, "What would actually fix it? ");
            Node* solutionNode = create_solution_node(strdup(solution));


            char* userQuestion = get_input(row++, 2, "What is a Y/N question that would distinguish your problem from the one above?");
            Node* questionNode = create_question_node(strdup(userQuestion));

            mvprintw(row++, 2, "Is the solution to your problem Y/N");
            refresh();
            text = getch();

        //now i need to create a new question node and a corresponding solution node, parent of the question node will be the
        //parent of the current ndoe, and then the current node will be a child of the new question node
            if(text == 'Y' || text == 'y'){
                questionNode->yes = solutionNode;
                questionNode->no = current;
            }
            else{
                questionNode->yes = current;
                questionNode->no = solutionNode;
            }

            if(parent == NULL){
                
                g_root = questionNode;
            }
            else{
                    if(branch == 1){
                        parent->yes = questionNode;
                    }
                    else{
                        parent->no = questionNode;
                    }
                }

                Edit edit;
                edit.type = EDIT_INSERT_SPLIT;
                edit.parent = parent; //we can just set it to parent cuz the parent is set inside of the qeustion node
                edit.wasYesChild = branch;
                edit.oldLeaf = current;
                edit.newQuestion = questionNode;
                edit.newLeaf = solutionNode;

                es_push(&g_undo, edit);
                es_clear(&g_redo); //taken from AI


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
