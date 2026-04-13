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
    free(q);

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
    FILE* fp;

     if(filename == NULL){
        return 0;
    }

    fp = fopen(filename, "rb"); //googled this, here we are reading from the file
    if(fp == NULL){

        return 0;
    }

uint32_t magic = MAGIC;
uint32_t magicCheck;
uint32_t version = VERSION;
uint32_t versionCheck;
uint32_t nodeCount;


    if(fread(&magicCheck, sizeof(uint32_t), 1, fp) != 1){
        fclose(fp);
        return 0;
    }

    if(magicCheck != magic){
            fclose(fp); //magic number is not matching
            return 0;
    }

    
    if(fread(&versionCheck, sizeof(uint32_t), 1, fp) != 1){
        fclose(fp);
        return 0;
    }
    
    if(versionCheck != version){
        fclose(fp);
        return 0;
    }

    if(fread(&nodeCount, sizeof(uint32_t), 1, fp) != 1){ //node count now has the number of nodes
        fclose(fp);
        return 0;
    }



    Node** nodeArr = malloc(sizeof(Node*) * nodeCount);
    if(nodeArr == NULL){
        fclose(fp);
        return 0;
    }
    int32_t* yesIdArr = malloc(sizeof(int32_t) * nodeCount);
    if(yesIdArr == NULL){
        free(nodeArr);
        fclose(fp);
        return(0);
    }
    int32_t* noIdArr = malloc(sizeof(int32_t) * nodeCount);
        if(noIdArr == NULL){
        free(nodeArr);
        free(yesIdArr);
        fclose(fp);
        return(0);
    }

    for(uint32_t i = 0; i < nodeCount; i++){//this is the first loop, looking to just get the information and reading it, 
        //on second loop we must link the children, same logic as before, checking if the yesID of the current one equald the id of the
        //current node we are looking at
       uint8_t isQuestion;
       uint32_t textLen;
       int32_t yesId = -1;
       int32_t noId = -1;

       if(fread(&isQuestion, sizeof(uint8_t), 1, fp)!= 1){
        fclose(fp);
        free(nodeArr);
        free(yesIdArr);
        free(noIdArr);
        return 0;
       }

       if(fread(&textLen, sizeof(uint32_t), 1, fp)!= 1){
        fclose(fp);
        return 0;
       }

       char* text = malloc((textLen + 1) * sizeof(char));
       if(text == NULL){
        fclose(fp);
        free(nodeArr);
        free(yesIdArr);
        free(noIdArr);
        return 0;
       }

       if(fread(text, sizeof(char), textLen, fp) != textLen){
        fclose(fp);
        free(nodeArr);
        free(yesIdArr);
        free(noIdArr);
        free(text);
        return 0;
       }
       text[textLen] = 0;

       if(fread(&yesId, sizeof(int32_t), 1, fp) != 1){
        fclose(fp);
        free(text);
        free(nodeArr);
        free(yesIdArr);
        free(noIdArr);
        return 0;
       }

       if(fread(&noId, sizeof(int32_t), 1, fp) != 1){
        fclose(fp);
        free(text);
        free(nodeArr);
        free(yesIdArr);
        free(noIdArr);
        return 0;
       }


       if(isQuestion){ //if its a 1, we create a question node
        nodeArr[i] = create_question_node(text);
       }
       else{
        nodeArr[i] = create_solution_node(text);
       }
       free(text); //the creation of the node mallocs its own text
       yesIdArr[i] = yesId;
       noIdArr[i] = noId;



    }
//      *     uint8  isQuestion
//  *     uint32 textLen          (bytes, no null terminator in file)
//  *     char[] text             (exactly textLen bytes)
//  *     int32  yesId            (-1 if NULL)
//  *     int32  noId             (-1 if NULL)
//  *

    for(uint32_t i =0; i < nodeCount; i++){ //we do this in the 2nd loop because we dont knowif that node has been created yet in the firs tloop
        int indexYesID = yesIdArr[i];
        int indexNoId = noIdArr[i];

        if(indexYesID != -1){
        nodeArr[i]->yes = nodeArr[indexYesID];
        }
        else{
            nodeArr[i]->yes = NULL;
        }
        if(indexNoId != -1){
        nodeArr[i]->no = nodeArr[indexNoId];
        }
        else{
            nodeArr[i]->no = NULL;
        }

    }
    free_tree(g_root); //free the old tree
    g_root = nodeArr[0]; //set the g_root to the node of the new tree
    free(nodeArr);
    free(yesIdArr);
    free(noIdArr);
    fclose(fp);

    return 1;

}
