#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char* readline() {
    char* buf = malloc(100);
    char* p = buf;
    while(read(0, p, 1) != 0){
        if(*p == '\n' || *p == '\0'){
            *p = '\0';
            return buf;
        }
        p++;
    }
    if(p != buf) return buf;
    free(buf);
    return 0;
}

int
main(int argc, char *argv[]){
    if(argc < 2) {
        printf("Usage: xargs [command]\n");
        exit(-1);
    }
    char* l;
    argv++;
    char* nargv[16];
    char** pna = nargv;
    char** pa = argv;
    while(*pa != 0){
        *pna = *pa;
        pna++;
        pa++;
    }
    while((l = readline()) != 0){
        //printf("%s\n", l);
        char* p = l;
        char* buf = malloc(36);
        char* bh = buf;
        int nargc = argc - 1;
        while(*p != 0){
            if(*p == ' ' && buf != bh){
                *bh = 0;
                nargv[nargc] = buf;
                buf = malloc(36);
                bh = buf;
                nargc++;
            }else{
                *bh = *p;
                bh++;
            }
            p++;
        }
        if(buf != bh){
            nargv[nargc] = buf;
            nargc++;
        }
        nargv[nargc] = 0;
        free(l);
        int pid = fork();
        if(pid == 0){
            // printf("%s %s\n", nargv[0], nargv[1]);
            exec(nargv[0], nargv);
        }else{
            wait(0);
        }
    }
    exit(0);
}
