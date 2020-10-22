#include "lib.h"
#include "db_def.h"


void main()
{

    int fd = open("./db", O_RDWR);
    if(fd == -1 ) errExit("open");

    struct account_details admin;
    admin.status = 1;
    strcpy(admin.user.first_holder_name,"Skand");
    admin.user.u_type = Admin;
    admin.user.user_id = 1;
    strcpy(admin.user.password,"1");
    int w = write(fd,&admin,sizeof(admin));
    printf("\nSize of struct is %ld",sizeof(admin));
    if ( w > 0 ) printf("\n Admin Created SuccessFully \n");

}