#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lab4.h"

/* ----------------------------------------------------------------
 * ds.c  --  all data structures for the Tech Support Diagnosis Tool
 *
 * Implement every function marked TODO.  The only functions in this
 * entire lab permitted to use recursion are free_tree and count_nodes.
 * Everything else must be iterative.
 * ---------------------------------------------------------------- */


/* ====== Tree nodes ============================================== */

/* TODO 1 */
Node *create_question_node(const char *question) {
    Node* questionNode = malloc(sizeof(Node));
    questionNode->isQuestion = 1;
    questionNode->text = strdup(question);
    questionNode->yes = NULL;
    questionNode->no = NULL;
    return questionNode;
}

/* TODO 2 */
Node *create_solution_node(const char *solution) {
    Node* solutionNode = malloc(sizeof(Node));
    solutionNode->isQuestion = 0;
    solutionNode->text = strdup(solution);
    solutionNode->yes = NULL;
    solutionNode->no = NULL;
    return solutionNode;
}

/* TODO 3  (recursion allowed) */
void free_tree(Node *node) {
    if(node == NULL){
        return;
    }
    free_tree(node->yes);
    free_tree(node->no);
    free(node->text);
    free(node);

}

/* TODO 4  (recursion allowed) */
int count_nodes(Node *root) {
    if(root == NULL){
        return 0;
    }

    return 1 + count_nodes(root->yes) + count_nodes(root->no);
}


/* ====== FrameStack  (dynamic array, iterative traversal) ======== */
//Dynamic array that doubles in capacity when full. After this, the stack unit tests should pass.
/* TODO 5 */
void fs_init(FrameStack *s) {

    s->capacity = 1;
    s->size = 0;
    Frame* dummy = malloc(s->capacity * sizeof(Frame));
    if(dummy == NULL){
        return;
    }
    s->frames = dummy;
}

/* TODO 6 */
void fs_push(FrameStack *s, Node *node, int answeredYes) {
    if(s->capacity <= s->size){
        s->capacity = s->capacity * 2;
        Frame* dummy = realloc(s->frames, sizeof(Frame) * s->capacity);
        if(dummy == NULL){
            return;
        }
        else{
            s->frames = dummy;
        }
    }



    s->frames[s->size].node = node; 
    s->frames[s->size].answeredYes = answeredYes;
    s->size = s->size + 1;

}

/* TODO 7 */
//if we get errors for empty, come back
Frame fs_pop(FrameStack *s) {
    Frame dummy = s->frames[s->size - 1]; 
    s->size = s->size - 1;

    return dummy;
    
}

/* TODO 8 */
int fs_empty(FrameStack *s) {
    if(s->size == 0){
        return 1;
    }
    else{
        return 0;
    }
}

/* TODO 9 */
void fs_free(FrameStack *s) {
    free(s->frames);
    s->size = 0;
    s->capacity = 0;
    s->frames = NULL;
}


/* ====== EditStack  (dynamic array, undo/redo) =================== */

/* TODO 10 */
// Structurally identical to FrameStack but stores Edit structs. The pattern is the same.

void es_init(EditStack *s) {
    s->capacity = 1;
    s->size = 0;
    Edit* dummy = malloc(s->capacity * sizeof(Edit));
    if(dummy == NULL){
        return;
    }
    s->edits = dummy;
}

/* TODO 11 */
void es_push(EditStack *s, Edit e) {
     if(s->capacity <= s->size){
        s->capacity = s->capacity * 2;
        Edit* dummy = realloc(s->edits, sizeof(Edit) * s->capacity);
        if(dummy == NULL){
            return;
        }
        else{
            s->edits = dummy;
        }
    }



    s->edits[s->size] = e;
    s->size = s->size + 1;

}

/* TODO 12 */

Edit es_pop(EditStack *s) {
    Edit dummy = s->edits[s->size -1];
    s->size = s->size -1;
    return dummy;
}

/* TODO 13 */
int es_empty(EditStack *s) {
   if(s->size == 0){
    return 1;
   }
   return 0;
}

/* TODO 14 */
void es_clear(EditStack *s) {
    s->size = 0;
}

/* provided -- do not modify */
void es_free(EditStack *s) {
    free(s->edits);
    s->edits    = NULL;
    s->size     = 0;
    s->capacity = 0;
}

void free_edit_stack(EditStack *s) { es_free(s); }


/* ====== Queue  (linked list, BFS) ============================== */
//Linked-list queue. Pay close attention to what must happen when the 
//last element is dequeued — this is the most common source of crashes in this section.
/* TODO 15 */
void q_init(Queue *q) {
    q->size = 0;
    q->front = NULL;
    q->rear = NULL;
}

/* TODO 16 */
void q_enqueue(Queue *q, Node *node, int id) {
    QueueNode* dummy= malloc(sizeof(QueueNode));
    if(dummy == NULL){
        return;
    }
    dummy->treeNode = node;
    dummy->id = id;
    dummy->next = NULL;
    if(q->front == NULL){
        q->front = dummy;
        q->rear = dummy;
        q->size = q->size + 1;
        return;
    }
    q->rear->next = dummy;
    q->rear = dummy;
    q->size = q->size + 1;
}

/* TODO 17 */
int q_dequeue(Queue *q, Node **node, int *id) {
    if(q->size == 0){
        return 0;
    }

    QueueNode* dummy= q->front;
    *node = (dummy->treeNode);
    *id = (dummy->id);
    if(q->size == 1){
        q->front = NULL;
        q->rear = NULL;
        q->size = q->size - 1;
        free(dummy);
        return 1;
    }
    q->size = q->size - 1;

    q->front = dummy->next;
    

    free(dummy);
    return 1;
    
}

/* TODO 18 */
int q_empty(Queue *q) {
    if(q->size == 0){
        return 1;
    }
    else{
        return 0;
    }
}

/* TODO 19 */
void q_free(Queue *q) {
    QueueNode* curr = q->front;
    while(curr != NULL){
        QueueNode* temp = curr->next;
        free(curr);
        curr = temp;
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}


/* ====== Hash table  (separate chaining) ======================== */

/* TODO 20
 * Convert a string to a canonical key:
 *   letters  -> lowercase
 *   spaces   -> underscore
 *   anything else -> drop
 * Caller owns the returned string and must free() it.
 */
char *canonicalize(const char *s) {
    if (s == NULL) return strdup("");

    uint32_t len = strlen(s);
    char* mine = calloc(len + 1, sizeof(char));
    int m = 0;

    if(mine == NULL){
        return NULL;
    }

    for(uint32_t i = 0; i < len; i++){
        if(isalpha(s[i])){
            mine[m] = tolower(s[i]);
            m++;
        }
        else if(s[i] == ' '){
            mine[m] = '_';
            m++;
        }
        
    }

    return mine;

}

/* TODO 21  (djb2: hash = hash*33 + c, seed 5381) */
unsigned h_hash(const char *s) {
    unsigned hash = 5381; // taken from google

    uint32_t length = strlen(s);

    for(uint32_t i = 0; i < length; i++){
        hash = (hash*33) + s[i];
    }

    return hash;
}

/* TODO 22 */
void h_init(Hash *h, int nbuckets) {
    if (h == NULL) return;

    h->nbuckets = nbuckets;
    h->size = 0;
    
    Entry** dummy = calloc(nbuckets, sizeof(Entry*)); 
    if(dummy == NULL){
        h->nbuckets = 0;
        return;
    }
    h->buckets = dummy;

}

/* TODO 23 */
int h_put(Hash *h, const char *key, int solutionId) {
    unsigned hashNum = hash(key);

    unsigned index = hashNum % h->nbuckets;
    uint32_t i = 0;

    if(h->buckets[index] == NULL){
        Entry* dummy = malloc(sizeof(Entry));
        if(dummy == NULL){
            return 0;
        }
        dummy->key = key;
        dummy->next =NULL;

        dummy->vals.capacity = 1;
        dummy->vals.count = 1;
        int* temp = malloc(dummy->vals.capacity * sizeof(int));
        if(temp == NULL){
            return 0;
        }
    
        temp[0] = solutionId;
        dummy->vals.ids = temp;

        dummy->next = NULL;
        return 1;
    }
    else{
        Entry* current = h->buckets[index];
        Entry* prev = h->buckets[index];
        while(current != NULL){
            prev = current;
            
            if(strcmp(current->key,key) == 0){
                if(current->vals.count >= current->vals.capacity){
                    current->vals.capacity*=2;
                    int* tempCap = realloc(current->vals.ids, sizeof(int) * current->vals.capacity);
                    if(tempCap == NULL){
                        return 0;
                    }

                    current->vals.count = current->vals.count + 1;
                    current->vals.ids = tempCap;
                    current->vals.ids[current->vals.count - 1] = solutionId; //setting the value at the index = to the solutionid
                    return 1;
                }
            }
            
            current = current->next;
            
        }

    

    }


}

/* TODO 24 */
int h_contains(const Hash *h, const char *key, int solutionId) {
    return 0;
}

/* TODO 25 */
int *h_get_ids(const Hash *h, const char *key, int *outCount) {
    *outCount = 0;
    return NULL;
}

/* TODO 26 */
void h_free(Hash *h) {
}
