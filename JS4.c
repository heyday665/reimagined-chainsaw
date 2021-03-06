#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NAMELENGTH 32
#define MAXFILESEC 64

//DYLAN READ HERE! vvv
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * Suggest using:                                                          *
 * 11000001 10010111 11110010 10010101 01101010 11011101 00011101 11010000 *
 * 00101100 10001100 01000001 10000010 00001111 00011000 01100101 00101011 *
 * 01100010 01010101 00101001 01000111 01010110 01011000 01011011 11111010 *
 * 00001101 00111000 01000110 01111111 00000000 10110010 00111000 01000011 *
 * as the first 32 bytes of our T0,S0 to nearly guarantee disk is new      *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//Perhaps we could do 64 bytes of data to guarantee?

// Memory layout for array to disk
     /* * * * * * * * *\
      * name size     *
      * name size     *
      * name size     *
      * name size     *
      * file_name[0]  *
      * ........      *
      * file_name[n]  *
      * num ST        *
      * num ST        *
      * num ST        *
      * num ST        *
      * s[0]          *
      * s[0]          *
      * s[0]          *
      * s[0]          *
      * t[0]          *
      * t[0]          *
      * t[0]          *
      * t[0]          *
      * ...           *
      * ...           *
      * ...           *
      * ...           *
      * ...           *
      * ...           *
      * ...           *
      * ...           *
      * s[n]          *
      * s[n]          *
      * s[n]          *
      * s[n]          *
      * t[n]          *
      * t[n]          *
      * t[n]          *
      * t[n]          *
     \* * * * * * * * */

extern int dinit();
extern int rsector(int t,int s,unsigned char *b);
extern int wsector(int t,int s,unsigned char *b);
struct LEmptyList emptySpaceList;

struct __SectorTrack{
    unsigned int sector;
    unsigned int track;
};

struct __LSectorTrack{
    struct __SectorTrack *ST;
    struct __LSectorTrack *next;
    struct __LSectorTrack *prev;
};

struct __meta_file{
    char *file_name;
    unsigned int file_size_byte;
    time_t file_create_time;
    time_t last_read_time;
    time_t last_write_time;
};

struct __open_file_info{
    char RW;//if its for read write
    int ST; //how many blocks we take up
    int fd; //its identifier
    struct __LSectorTrack *start; //the start of the list of blocks we take up
};

struct __file{ //holds meta data and info about openfile
    struct __meta_file  meta_file;
    struct __open_file_info open_file_info;
};

struct __Lfile{ //linked list for files
    struct __file *file;
    struct __Lfile *next;
    struct __Lfile *prev;
};

struct LEmptyNode {
    int startTrack;
    int startSector;
    int endTrack;
    int endSector;
    int size;
    
    struct LEmptyNode *next;
    struct LEmptyNode *prev;
};

struct LEmptyList {
    int numEmptyNodes;
    struct LEmptyNode *head;
    struct LEmptyNode *tail;
};

//Need to store each file into a char* to save to disk
/*
unsigned char* metaDataToArray(){
    //int numFiles = 0;
    //unsigned char* retArray;
    //retArray = malloc(sizeof());

    //We don't have the time to implement saving the FS to disk,
    //as such, it's not here.

    //since most of our data is in ints, we need to use a combination
    //of four chars and left shifting to get our ints into the right
    //format for saving to disk.
};
*/

// Gets distance from _s (start) to _t (target)
// Return is number of sectors
int getDistance(int _s_S, int _s_T, int _t_S, int _t_T){
    int distance = 0;
    if (_s_T == _t_T) {
        distance = _t_S - _s_S;
    } else {
        distance = 4096 * (_t_T - _s_T);
        distance += _t_S - _s_S;
    }
    return distance;
};

/*
void reCombobulateEmptySpace() {
    struct LEmptyNode* curr = emptySpaceList.head;
    while (curr->next) {
        int currEndSector = curr->endSector;
        int currEndTrack = curr->endTrack;
        curr = curr->next;
        int nextStartSector = curr->startSector;
        int nextStartTrack = curr->startTrack;
    }
    return;
};
*/

void retSectorTrack(struct __LSectorTrack* LST) {
    // returns the block to empty space
    int startSector = LST->ST->sector;
    int startTrack = LST->ST->track;
    int endSector = -1;
    int endTrack = -1;
    
    struct __LSectorTrack* delItr = LST;
    struct __LSectorTrack* itr = LST;
    struct LEmptyNode* newEmptyNode = malloc(sizeof(struct LEmptyNode));
    struct LEmptyNode* currNode = emptySpaceList.head;

    newEmptyNode->startSector = startSector;
    newEmptyNode->startTrack = startTrack;

    while (1) {
        if (itr->next) {
            delItr = itr;
            itr = itr->next;
            free(delItr);
        }
        else {
            endSector = itr->ST->sector;
            endTrack = itr->ST->track;
            free(itr);
            break;
        }
    }

    newEmptyNode->endSector = endSector;
    newEmptyNode->endTrack = endTrack;

    //while (currNode->next) {
    //    if (currNode->startTrack < endTrack) {
    //        currNode = currNode->next;
    //    }
    //}

    //reCombobulateEmptySpace();
    free(LST);
};

struct __LSectorTrack* getEmptySpace(int numBlocks) {
    // Get a block of empty space of size numBlocks
    int isDone = 0;
    struct __LSectorTrack* returnLST;
    struct __LSectorTrack* pointerMemes;
    while (1) {
        int nodesToCheck = emptySpaceList.numEmptyNodes;
        struct LEmptyNode* currNode = emptySpaceList.head;
        
        //Once found a viable block
        if (currNode->size >= numBlocks){
            pointerMemes = returnLST;
            returnLST = malloc(sizeof(struct __LSectorTrack));
            struct __LSectorTrack* jim = malloc(sizeof(struct __LSectorTrack));
            struct __SectorTrack* jimBob = malloc(sizeof(struct __SectorTrack));
            for (int i = 0; i<(numBlocks); i++) {
                if (!i) {
                    pointerMemes->ST->sector = currNode->startSector;
                    pointerMemes->ST->track = currNode->startTrack;
                }
                else {
                    jim->ST = jimBob;
                    jimBob->sector = currNode->startSector+i;
                    jimBob->track = currNode->startTrack;
                }
                if (jimBob->sector >= 4096){
                    jimBob->sector = jimBob->sector - 4096;
                    jimBob->track = jimBob->track + 1;
                }
                pointerMemes->next = jim;
                pointerMemes = pointerMemes->next;
                if (i == numBlocks){
                    pointerMemes->next = NULL;
                }
            }
            isDone = 1;
        }
        if (isDone) break;

        if (currNode->next) {
            currNode = currNode->next;
        }
        else {
            printf("NO SPACE!\n");
            break;
        }
    }
    return returnLST;
};

void finit(){ //inits file system
    //read the disk meta data in

    //initialize the empty space list with a
    // node that covers all of tracks 1-127
    struct LEmptyNode godEmptyNode;

    godEmptyNode.startTrack=1;
    godEmptyNode.startSector=0;
    godEmptyNode.endTrack=127;
    godEmptyNode.endSector=4095;
    godEmptyNode.size = 1 + getDistance(godEmptyNode.startSector, godEmptyNode.startTrack, godEmptyNode.endSector, godEmptyNode.endTrack);
    godEmptyNode.next = NULL;
    godEmptyNode.prev = NULL;

    emptySpaceList.head = &godEmptyNode;
    emptySpaceList.tail = emptySpaceList.head;

    printf("Size of __file %d\n", sizeof(struct __file));
    printf("Size of __Lfile %d\n", sizeof(struct __Lfile));
};

int dopen(char *fname, char *mode){

    int fle;
    struct __Lfile *tmp = __isfile(fname,&fle);

    if(fle == -1){
        printf("File name to long\n");
        return -1;
    }

    if(mode[0] == 'w' && fle == 1){//file exists no need to create it
        
    }else if (mode[0] == 'w' && fle == -2){//create file
        struct __file *tmp =  __addFile(fname);
        //set mode and give fd
    }else if(mode[0] == 'r' && fle == 1){//file exists and its good to read

    }else{
        return -1;
    }

};

struct __file* __getFile(char *fname){
    if(strlen(fname) > 32){
        return -1;
    }
}

struct __Lfile *start = NULL;
int __NumFiles = 0;

struct __file* __addFile(char *fname){//adds file to linked list of files
    struct __Lfile *tmp = malloc(sizeof(struct __Lfile));
    tmp->file->meta_file.file_create_time = time(NULL);
    tmp->file->meta_file.last_read_time = time(NULL);
    tmp->file->meta_file.last_write_time = time(NULL);
    tmp->prev = NULL;
    tmp->next = NULL;
    tmp->file->meta_file.file_name = strdup(fname);

    if(start == NULL){
        start = tmp;
    }else{
        struct __Lfile *tmp2 = NULL;
        for(tmp2 = start;tmp2->next;tmp2 = tmp2->next){
            ///seeeyah folllss
        }
        
        tmp2->next = tmp;
        tmp->prev = tmp2;
    }

    __NumFiles++;
    return tmp;
};

int highestFD;
struct FDLIST *returnedfdList;

struct FDLIST{
    int fd;
    struct FDLIST *next;
    struct FDLIST *prev;
};


int __nextFD(){

}

int dclose(int fd){
    if(fd > highestFD){
        return -1;
    }
};

struct __Lfile* __isfile(char *fname,int *ret){//-1 fname to long, -2 file does not exist,1 file exists
    if(strlen(fname) > NAMELENGTH){
        *ret = -1;
        return NULL;
    }

    if(start == NULL){
        *ret = -2;
        return NULL;
    }

    struct __Lfile *tmp;
    for(tmp = start;tmp->next;tmp = tmp->next){
        if(strcmp(tmp->file->meta_file.file_name,fname) == 0){
            *ret = 1;
            return tmp;
        }
    }

    if(strcmp(tmp->file->meta_file.file_name,fname) == 0){
        *ret = 1;
        return tmp;
    }

    *ret = -2;
    return NULL;
};

int dunlink(char *fname){
    
    struct __Lfile *tmp;  
    int ret;
    
    tmp = __isfile(fname,&ret);

    if(tmp != NULL){
        if(tmp == start){
            if(tmp->next){
                start = next;
            }else{
                start = NULL;
            }
        }else{
            if(tmp->next){
                tmp->prev->next = tmp->next;
                if(__freeFile(tmp) > 0){
                    return 1;
                }else{
                    return 0;
                }
            }else{
                tmp->prev->next = NULL;

            }
        }
    }else{
        return 0;
    }
    

    int __freeFile(struct __Lfile *file,struct __Lfile *prev,struct __Lfile*next){//returns <= 1 on error or 1 on success
        if(prev == NULL && next == NULL){
            START == NULL;
        }else if(prev == NULL){
            start = file->next;
        }else if(next == NULL){
            prev->next = NULL;
        }else{
            prev->next = next;
            next->prev = prev;
        }
        free(file->file->meta_file->filename);//name the only thing in meta that needs to be freed
        free(file->file->meta_file);//we can free meta file now

        num_fields--;
    }

    /*for(tmp = start;tmp->next;tmp = tmp->next){
        if(strcmp(tmp->file->meta_file.file_name,fname) == 0){
            tmp->prev->next = tmp->next;
            if(tmp->next){
                tmp->next->prev = tmp->prev;
            }
            free(tmp);
            __NumFiles--;
            return 1;
        }
    }

    if(strcmp(tmp->file->meta_file.file_name,fname) == 0){
        tmp->prev->next = tmp->next;
        if(tmp->next){
            tmp->next->prev = tmp->prev;
        }
        free(tmp);
        return 1;
    }*/

    
    return 0;
};

int writeFileMetatoDisk(){

};




//open a file for reading or writing
//if opened for writing and it does not exist, create it
//returns a file descriptor to reference the file


//int dclose(int fd)
//close a file == file descriptor is no longer

//int dread(int fd, char *buf)
//read the next sector-size chunk of bytes from the file and store it in buf
//all i/o always happens in sector-size chunks
//return sector-size on success
//return -1 on failure,  fd not valid or file opened for writing


//int dwrite(fd, buf)
//write the next sector-size chunk of bytes as found in buf
//all i/o always happens in sector-size chunks
//return sector-size on success
//return -1 on failure,  fd not valid or file opened for reading


//int dunlink(char *fname)
//delete the file
//return 1 on success, 0 on failure

int main(int argc,char** argv){
    finit();
    //dinit();
}