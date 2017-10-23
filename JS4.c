#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NAMELENGTH 32
#define MAXFILESEC 64

extern int dinit();
extern int rsector(int t,int s,unsigned char *b);
extern int wsector(int t,int s,unsigned char *b);

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
    
    struct LEmptyNode *next;
    struct LEmptyNode *prev;
};

struct LEmptyList {
    int numEmptyNodes;
    struct LEmptyNode *head;
    struct LEmptyNode *tail;
};

void finit(){ //inits file system
    //read the disk meta data in

    //initialize the empty space list with a
    // node that covers all of tracks 1-127
    struct LEmptyList emptySpaceList;
    struct LEmptyNode godEmptyNode;

    godEmptyNode.startTrack=1;
    godEmptyNode.startSector=0;
    godEmptyNode.endTrack=127;
    godEmptyNode.endSector=4095;
    godEmptyNode.next = NULL;
    godEmptyNode.prev = NULL;

    emptySpaceList.head = &godEmptyNode;
    emptySpaceList.tail = emptySpaceList.head;

    printf("Size of __file %d\n", sizeof(struct __file));
    printf("Size of __Lfile %d\n", sizeof(struct __Lfile));
}

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

}

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
}

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
}

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
            *ret = 1
            return tmp;
        }
    }

    if(strcmp(tmp->file->meta_file.file_name,fname) == 0){
        *ret = 1
        return tmp;
    }

    *ret = -2;
    return NULL;
}

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
}

int writeFileMetatoDisk(){

}




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