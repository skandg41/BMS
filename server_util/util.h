
#include "lib.h"

long int gen_random(){
    time_t t;
    srand(time(&t));
   return rand();
}

long int gen_random_Account(){
   srandom(time(0));
   long int low = 999999999, high = 9999999999;
   return ( rand() %  ( high- low )) + low;
}

struct req_res_packet AddUser(struct account_details CIF){
    struct req_res_packet res;
    int fd = open("db",O_WRONLY|O_APPEND);
    CIF.Account_number = gen_random_Account();
    struct flock lock;
    lock.l_pid = getpid();
    lock.l_len = sizeof(res.user);
    lock.l_whence = SEEK_END;
    lock.l_start = sizeof(res.user);
    lock.l_type = F_WRLCK;
    fcntl(fd,F_SETLKW,&lock);
    if(!write(fd,&CIF,sizeof(CIF))) 
        printf("New User %ld created",CIF.Account_number);
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    
    res.status = 1;
    res.user = CIF;
    return res;
}

struct req_res_packet Update_info(struct req_res_packet req){
    struct flock lock;
    struct req_res_packet res;
    int seqno = req.uno;
    int fd = open("db",O_RDWR);
    lock.l_pid = getpid();
    lock.l_len = sizeof(req.user);
    lock.l_whence = SEEK_SET;
    lock.l_start = sizeof(req.user) * seqno;
    lock.l_type = F_WRLCK;
    fcntl(fd,F_SETLKW,&lock);
    lseek(fd, (sizeof(req.user)*seqno), SEEK_SET);
    int wr = write(fd,&req.user,sizeof(req.user));
    if (wr == sizeof(req.user)){
        res.status = 1;
        res.user = req.user;
        printf("\nUpdation on account %ld Successful\n",res.user.Account_number);
    }
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETFL,&lock);
    printf("\nOutside CS\n");
    return res;
}


struct req_res_packet login_serv(struct req_res_packet req){           // Login Task
    int rd,i=0;
    struct req_res_packet res;
    struct account_details acc;
    req.op_code = login;

    int fd = open("db",O_RDONLY);
    struct flock lock;
    lock.l_pid = getpid();
    lock.l_len = 0;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    
    
    while(1){
        lock.l_type = F_RDLCK;
        fcntl(fd,F_SETLKW,&lock);
        rd = read(fd,&acc,sizeof(acc));
        lock.l_type = F_UNLCK;
        fcntl(fd,F_SETLKW,&lock);
        if (rd == 0){
            res.status = -1;
            return res;
        }
        else if(acc.user.user_id == req.user.user.user_id){
            int cmp = strncmp(acc.user.password,req.user.user.password,req.status);
            if ( cmp == 0 ) {
                if(acc.user.u_type == Admin) {
                    res.status = 0;
                } 
                else {
                    res.status = 1;  
                }
                res.uno = i;
                res.user = acc;
                return res; 
            }
        }
        i++;
    } 
 }

struct req_res_packet Get_seq_no(struct req_res_packet req){
    struct req_res_packet res;
    struct account_details acc;
    int rd, i = 0 , fd = open("db",O_RDONLY);
    struct flock lock;
    lock.l_pid = getpid();
    lock.l_len = sizeof(acc);
    lock.l_whence = SEEK_CUR;
    lock.l_start = 0;
    
    while(1){
        lock.l_type = F_RDLCK;
        fcntl(fd,F_SETLKW,&lock);
        rd = read(fd,&acc,sizeof(acc));
        lock.l_type = F_UNLCK;
        fcntl(fd,F_SETLKW,&lock);
        if (rd == 0){
            res.status = -1;
            return res;
        }
        else if(acc.Account_number == req.user.Account_number || acc.user.user_id == req.user.user.user_id)
        {
            res.uno = i;
            res.status = 1;
            return res; 
        }
        i++;
    }

}

struct req_res_packet ret_c_data(struct req_res_packet req){
    struct req_res_packet res;
    struct account_details acc;
    int fd = open("db",O_RDONLY);
    lseek(fd,(req.uno*sizeof(req.user)),SEEK_SET);
    struct flock lock;
    lock.l_pid = getpid();
    lock.l_len = sizeof(acc);
    lock.l_whence = SEEK_CUR;
    lock.l_start = 0;
    lock.l_type = F_RDLCK;
    fcntl(fd,F_SETLKW,&lock);
    int rd = read(fd,&acc,sizeof(acc));
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);

    if(rd > 0){
        res.user =acc;
        res.status = 1;
    }
    else res.status = 0;
    return res;
}

struct req_res_packet Del_account(struct req_res_packet req){
    struct req_res_packet res;
    struct account_details acc;
    int rd, i = 0 , fd = open("db",O_RDONLY);
    struct flock lock;
    lock.l_pid = getpid();
    lock.l_len = sizeof(acc);
    lock.l_whence = SEEK_CUR;
    lock.l_start = 0;
    
    while(1){
        lock.l_type = F_RDLCK;
        fcntl(fd,F_SETLKW,&lock);
        rd = read(fd,&acc,sizeof(acc));
        lock.l_type = F_UNLCK;
        fcntl(fd,F_UNLCK,&lock);
        if (rd == 0){
            res.status = -1;
            return res;
        }

        else if(acc.Account_number == req.user.Account_number || acc.user.user_id == req.user.user.user_id)
        {
            req.uno = i;
            req.user.status = -1;
            Update_info(req);             // Changes all other fileds to Garbage Value
            res.uno = i;
            res.status = 1;
            return res; 
        }
        i++;
    }   
}

struct req_res_packet CreditAccount(struct req_res_packet req){
    struct req_res_packet res;
    struct account_details acc;
    int fd = open("db",O_RDWR);
    lseek(fd,(req.uno*sizeof(req.user)),SEEK_SET);
    struct flock lock;
    lock.l_pid = getpid();
    lock.l_len = sizeof(acc);
    lock.l_whence = SEEK_CUR;
    lock.l_start = 0;
    lock.l_type = F_RDLCK;
    int st = fcntl(fd,F_SETLKW,&lock);
    int rd = read(fd,&acc,sizeof(acc));
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    acc.balance = (float)((float)acc.balance + req.status);
    lseek(fd,(req.uno*sizeof(req.user)),SEEK_SET);
    lock.l_type = F_WRLCK;
    int sl = fcntl(fd,F_SETLKW,&lock);
    int wr = write(fd,&acc,sizeof(acc));
    lock.l_type = F_UNLCK;
    int sul = fcntl(fd,F_SETLKW,&lock);

    if(wr >0 && sl==0 && sul ==0 ){
        res.user =acc;
        res.status = 1;
    }
    else res.status = 0;
    return res;
}

void res_read(struct req_res_packet res){
    printf("\n\nOpcode %d\n\tStatus %ld\n\tUser no %d\n\tuser Priviage %d",res.op_code,res.status,res.uno,res.user_pri);
}

struct req_res_packet DebitAccount(struct req_res_packet req){
    
    struct req_res_packet res;
    struct account_details acc;
    int fd = open("db",O_RDWR);
    lseek(fd,(req.uno*sizeof(acc)),SEEK_SET);
    struct flock lock;
    lock.l_pid = getpid();
    lock.l_len = sizeof(acc);
    lock.l_whence = SEEK_CUR;
    lock.l_start = 0;
    lock.l_type = F_RDLCK;

    int st = fcntl(fd,F_SETLKW,&lock);
    int rd = read(fd,&acc,sizeof(acc));
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    
    printf("\nWithdraw request amount %ld",req.status);
    printf("\nAvailable Balance %.2f",acc.balance);
    
    if( acc.balance >= req.status ){    
        acc.balance = ((float)acc.balance - (float) req.status);
        lseek(fd,(req.uno*sizeof(req.user)),SEEK_SET);
        
        lock.l_type = F_WRLCK;
        fcntl(fd,F_SETLKW,&lock);
        
        int wr = write(fd,&acc,sizeof(acc));
        lock.l_type = F_UNLCK;
        fcntl(fd,F_SETLKW,&lock);

        printf("\nUpdated Balance %.2f",acc.balance);
        printf("\nWrite %d",wr);
        
        if(wr > 0 ){
            res.user = acc;
            res.status = 1;
        }
        
        else res.status = 0;
    }
    else res.status = -1;
    res.user = acc;
    res.op_code = req.op_code;
    res.uno = req.uno;
    res.user_pri = req.user_pri;
    return res;
}
