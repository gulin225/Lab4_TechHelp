#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "lab4.h"

extern Node *g_root;

#define MAGIC   0x54454348u   /* "TECH" */
#define VERSION 1u

typedef struct { Node *node; int id; } NodeMapping;

/* ----------------------------------------------------------------
 * TODO 27  save_tree
 *
 * Serialize the entire tree to a binary file using BFS order.
 *
 * File format:
 *   Header:  uint32 magic | uint32 version | uint32 nodeCount
 *   Per node (BFS order):
 *     uint8  isQuestion
 *     uint32 textLen          (bytes, no null terminator in file)
 *     char[] text             (exactly textLen bytes)
 *     int32  yesId            (-1 if NULL)
 *     int32  noId             (-1 if NULL)
 *
 * Return 1 on success, 0 on failure.
 * ---------------------------------------------------------------- */
int save_tree(const char *filename) {
   
    FILE* fp;
    NodeMapping* map;
    int idNum = 0;

    uint32_t magic = MAGIC;
    uint32_t version = VERSION;
    uint32_t nodeCount;

    if(filename == NULL){
        return 0;
    }

    fp = fopen(filename, "wb"); //googled this
    if(fp == NULL){
        fclose(fp);
        return 0;
    }



    nodeCount = (uint32_t)count_nodes(g_root);
    if(nodeCount == 0){
        fclose(fp);
        return 1;
    }

    if(fwrite(&magic, sizeof(uint32_t), 1, fp) != 1) { //googled this
        fclose(fp);
        return 0;
    }

    if(fwrite(&version, sizeof(uint32_t), 1, fp) != 1) { //googled this
        fclose(fp);
        return 0;
    }

    if(fwrite(&nodeCount, sizeof(uint32_t), 1, fp) != 1) { //googled this
        fclose(fp);
        return 0;
    }

    Queue *q = malloc(sizeof(Queue));
    if(q == NULL){
        fclose(fp);
        return 0;
    }
    map = malloc(sizeof(NodeMapping) * nodeCount);
    if(map == NULL){
        fclose(fp);
        return 0;
    }
    q_init(q);
    q_enqueue(q, g_root,idNum);
    

    while(!(q_empty(q))){
        Node* current;
        int passId;

        if(!(q_dequeue(q, &current, &passId))){
            free(q);
            free(map);
            fclose(fp);
            return 0;
        }
        map[passId].node = current;
        map[passId].id= passId;
      
        if(current->yes != NULL){
            idNum++;
            q_enqueue(q, current->yes, idNum);
        }
        if(current->no != NULL){
            idNum++;
            q_enqueue(q, current->no, idNum);
        }

        

    }

    for(uint32_t i = 0; i <nodeCount; i++){
        Node* current = map[i].node;
        uint8_t isQuestion = current->isQuestion;
        uint32_t textLen = strlen(current->text);
        char* text = current->text;
        int32_t yesId = -1;
        int32_t noId = -1;

        if(current->yes != NULL){
            for(uint32_t j = 0; j < nodeCount; j++){
                if(current->yes == map[j].node){
                    yesId = map[j].id;
                }
            }
        }
        if(current->no != NULL){
            for(uint32_t j = 0; j < nodeCount; j++){
                if(current->no == map[j].node){
                    noId = map[j].id;
                }
            }
        }

        if(fwrite(&(isQuestion), sizeof(uint8_t), 1, fp) != 1) { //googled this
            
            fclose(fp);
            return 0;
        }
        if(fwrite(&textLen, sizeof(uint32_t), 1, fp) != 1) { //googled this
            fclose(fp);
            return 0;
        }

        if(fwrite(text, sizeof(char), textLen, fp) != textLen) { //googled this
            fclose(fp);
            return 0;
        }

        if(fwrite(&yesId, sizeof(int32_t), 1, fp) != 1){
            fclose(fp);
            return 0;
        }
        if(fwrite(&noId, sizeof(int32_t), 1, fp) != 1){
            fclose(fp);
            return 0;
        }



   
    }


    q_free(q);

    free(map);
    fclose(fp);
    return 1;
}

/* ----------------------------------------------------------------
 * TODO 28  load_tree
 *
 * Read a file written by save_tree and reconstruct the tree.
 * Validate the magic number.  Read all nodes into a flat array
 * first, then link children in a second pass.
 * Free any existing g_root before installing the new one.
 * Return 1 on success, 0 on any error (free partial allocations).
 * ---------------------------------------------------------------- */
int load_tree(const char *filename) {
    return 0;
}
