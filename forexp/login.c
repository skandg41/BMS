#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
struct login{
    int uname;
    char pass[30];
};
int login( int fd, int uname,char * pass){
    int re;
    struct login log;
      
    printf("Login Credentials\t");
        printf("%d\t",uname);
        printf("%s\n",pass);
    
    if (fd == -1 ) {
        perror ("open");
        exit(EXIT_FAILURE);
    }
    while(1){ printf("\nreading\n");
        re = read (fd,&log,sizeof(log));
        if(re == -1) { perror("read"); exit(EXIT_FAILURE);}
        if(re == 0 ) break;
        //printf("\n%d\t",re);
        //printf("\n Stored uname %d \t pass %s\n", log.uname,log.pass);
        if(re && log.uname == uname){
            int c = strcmp(log.pass , pass);
            if ( c == 0 ) return 1;
        }
    }
    return 0;
}
void main(){
    int s = 0,uid;
    char pass[30];
    struct login log;
    int fd = open("./login",O_CREAT | O_APPEND |O_RDWR | O_EXCL);

    for(int i=0;i<3 && fd >0 ;i++)
    {
        printf("Login Registration\t");
        scanf("%d",&log.uname);
        scanf("%s",log.pass);
        printf("s");
        write(fd,&log,sizeof(log));
        
    }
    close (fd);
    fd = open("./login",O_RDONLY);
    //while(!s) {
        scanf("%d",&uid);
        scanf("%s",pass);
        s = login(fd,uid,pass);
        if (s == 0 ) printf("\nTry Again\n");
        else printf("\n****************Login Success********\n");
    //}
    printf("random");
    close(fd);
    exit(EXIT_SUCCESS);
}