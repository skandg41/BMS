#include "lib.h"
#include "db_def.h"
#include "util.h"

static void *Handle_client(int * arg ){
    struct req_res_packet req,res;
    //int *nsd = (int * ) arg;
    printf("\nThread 1\n");
    if ( read(*arg, &req , sizeof(res) ) == -1 ) errExit("read");
    printf("Message from Client opcode : %d\n",req.op_code);
    if(req.op_code == get_data){
        res = ret_c_data(req);
        write(*arg,&res,sizeof(res));
        printf("\nresponded %ld",res.status); 
    }
    if(req.op_code == login) {
        res = login_serv(req);
        write(*arg,&res,sizeof(res));
        printf("\nresponded %ld",res.status);
    }
    if(req.op_code == Add_user ){
        res = AddUser(req.user);
        write(*arg,&res,sizeof(res));
        printf("\nResponded %ld\n",res.status);
    }
    if(req.op_code == Update ){
        res = Update_info(req);
        write(*arg,&res,sizeof(res));
        printf("\nResponded %ld\n",res.status);
    }
    if(req.op_code == Delete_user && req.user_pri == Admin){
        res = Del_account(req);
        write(*arg,&res,sizeof(res));
        printf("\nResponded %ld\n",res.status);
    }

    if(req.op_code == Find_acc ){
        res = Get_seq_no(req);
        write(*arg,&res,sizeof(res));
        printf("\nResponded %ld\n",res.status);
    }

    if(req.op_code == Deposite && req.user_pri == 1){
        res = CreditAccount(req);
        write(*arg,&res,sizeof(res));
        printf("\nResponded %ld\n",res.status);
    }

    if(req.op_code == Withdraw && req.user_pri == 1){
        struct req_res_packet res = DebitAccount(req);
        write(*arg,&res,sizeof(res));
        printf("\nResponded %ld\n",res.status);
    }

    if(req.op_code == Pass_change && req.user_pri == 1){
        struct req_res_packet res = Update_info(req);
        write(*arg,&res,sizeof(res));
        printf("\nResponded %ld\n",res.status);
    }
  /*if( close(*arg) == -1) errExit("close"); 
    exit(EXIT_SUCCESS);*/
}

int main(int argc, char * argv[]){
    struct sockaddr_in serv, cli;
    int sd,sz,nsd;
    int port = 8065;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == -1) errExit("socket");
    serv.sin_family = AF_INET ;
    serv.sin_addr.s_addr = htonl( INADDR_ANY);
    serv.sin_port = htons(port);

    if( bind(sd, (void *) &serv, sizeof(serv)) == -1) errExit("bind");

    if(listen(sd,100) == -1 ) errExit("listen");
    
    printf("Initializing Server \n Listening to port %d \n Server max limit 100",port);
    
    sz = sizeof(cli);
    int i = 0 ;

    while(1){
        nsd = accept(sd, (void * ) &cli, &sz );
        if (nsd == -1 ) errExit("accept");
        pthread_t thr;
        if(pthread_create(&thr, NULL, (void *) Handle_client,(void *) &nsd) != 0 ) errExit("pthread");
    }
    close(sd);
    return 0;
}



