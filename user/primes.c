#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int rd){
    int n;
    read(rd, &n, 4);
    printf("prime %d\n", n);
    int created = 0;
    int p[2];
    int num;
    while(read(rd, &num, 4) != 0){
        if(created == 0){
            pipe(p);
            created = 1;
            int pid = fork();
            if(pid == 0){
                close(p[1]);
                prime(p[0]);
                return;
            }else{
                close(p[0]);
            }
        }
        if(num % n != 0){
            write(p[1], &num, 4);
        }
    }
    close(rd);
    close(p[1]);
    wait(0);
}

int
main(int argc, char *argv[]){
    int p[2];
    pipe(p);

    int pid = fork();
    if(pid != 0){
        // first
        close(p[0]);
        for(int i = 2; i <= 35; i++){
            write(p[1], &i, 4);
        }
        close(p[1]);
        wait(0);
    }else{
        close(p[1]);
        prime(p[0]);
        close(p[0]);
    }
    exit(0);
}
