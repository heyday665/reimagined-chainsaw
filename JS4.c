#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

extern int dinit();
extern int rsector(int t,int s,unsigned char *b);
extern int wsector(int t,int s,unsigned char *b);

struct __meta_file{
    char file_name[32];
    unsigned int file_size_byte;
    time_t file_create_time;
    time_t last_read_time;
    time_t last_write_time;
};

struct __open_file_info{
    char RWO;
    int fd;
    int s_sector;
    int s_track;
};

struct __file{
    struct __meta_file  meta_file;
    struct __open_file_info open_file_info;
};

struct __Lfile{
    struct __file *file;
    struct __Lfile *prev;
    struct __Lfile *next;
};

int finit(){
    //read the disk meta data in
    printf("Size of __file %d\n",sizeof(struct __file));
    printf("Size of __Lfile %d\n",sizeof(struct __Lfile));
}

int dopen(char *fname, char *mode){
    if(strlen(fname) > 128){
        return -1;
    }

    if(mode[0] == 'w'){//

    }else if(mode[0] == 'r'){

    }else{
        return -1;
    }

}

struct __file* __getFile(char *fname){
    
}

struct __Lfile *start = NULL;

struct __file* __addFile(){//adds file to linked list of files
    struct __Lfile *tmp = malloc(sizeof(struct __Lfile));
    tmp->file->meta_file.file_create_time = time(NULL);
    tmp->file->meta_file.last_read_time = time(NULL);
    tmp->file->meta_file.last_write_time = time(NULL);
    tmp->prev = NULL;
    tmp->next = NULL;

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
}

int __delFile(char *fname){
    struct __Lfile *tmp;   
    
    for(tmp = start;tmp->next;tmp = tmp->next){
        if(strcmp(tmp->file->meta_file.file_name,fname) == 0){
            tmp->prev->next = tmp->next;
            if(tmp->next){
                tmp->next->prev = tmp->prev;
            }
            free(tmp);
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
    }

    return 0;
    
}


//open a file for reading or writing
//if opened for writing and it does not exist, create it
//returns a file descriptor to reference the file


//int dclose(int fd)
//close a file == file descriptor is no longer valid and may be reused


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