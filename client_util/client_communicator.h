

struct req_res_packet Execute(struct req_res_packet req){
    struct sockaddr_in serv;
    int sd;
    char buf[100];
    sd = socket(AF_INET,SOCK_STREAM,0);
    if(sd == -1) errExit("socket");
    serv.sin_family= AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(8065);

    if (connect(sd,(void *)(&serv), sizeof(serv)) == -1 ) ;
    write(sd,&req,sizeof(req));
    struct req_res_packet res;
    read(sd,&res,sizeof(res));
    //printf("Message from server : %s\n",buf);
    return res;
 }