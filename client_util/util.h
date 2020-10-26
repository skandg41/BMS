//#include "db_def.h"

void pAccountDetails(struct account_details ledger ){
    printf("\t\t Account holder name : %s\n", ledger.user.first_holder_name);
    if(ledger.user.u_type == Joint ) printf("\t\t Account holder name : %s\n", ledger.user.second_holder_name);
    printf("\n Account Number : %ld \n Available Balance : %.2f \n Lien Amount : %.2f\n Address : %s, %s, %s, %s,%s \n",ledger.Account_number,ledger.balance,ledger.lien_amount, ledger.user.address,ledger.user.city,ledger.user.state,ledger.user.country,ledger.user.zip);
    printf("Mobile : %ld\n email : %s", ledger.user.mobile_no,ledger.user.email);
    printf("\nUser id : %ld \nPassword : %s",ledger.user.user_id,ledger.user.password);
}

long int gen_random(){
    time_t t;
   srand(time(&t));
   return rand();
}


int feed_Details(){                                               // Sets up a new account
    struct account_details CIF;
//    CIF.Account_number = getAccount_Number();
    CIF.status = 1;
    while(1)
    {
        printf("Enter type of Account \t 1 for Saving \n\t\t 2 for Current \n\t\t 3 for Loan\t");
        scanf("%d",&CIF.Acc_type);
        fflush(stdin);
        printf("\nEnter Holding Type : \t 1 for Single Holder \n\t\t 2 for Joint Holding\n");
        scanf("%d",&CIF.user.u_type);
        fflush(stdin);
        if( (CIF.Acc_type == SB || CIF.Acc_type == Curr || CIF.Acc_type == Loan)  && ( CIF.user.u_type == Single || CIF.user.u_type == Joint ) ) {
            break;
        }
    }
    CIF.balance = 0;
    CIF.lien_amount = 0;
    if (CIF.user.u_type == Single){
        printf("\nEnter Account Holder's Name :");
        scanf("%s",CIF.user.first_holder_name);
        fflush(stdin);
    }
    else {
        printf("\nEnter First Holder's Name :");
        scanf("%s",CIF.user.first_holder_name);
        fflush(stdin);

        printf("\nEnter Second Holder's Name :");
        scanf("%s",CIF.user.second_holder_name);
        fflush(stdin);
    }
    printf("\nEnter Address \t");
    scanf("%s", CIF.user.address);
    fflush(stdin);

    printf("\nEnter City\t");
    scanf("%s",CIF.user.city);
    fflush(stdin);

    printf("\nEnter State\t");
    scanf("%s",CIF.user.state);
    fflush(stdin);

    printf("\nEnter ZIP code\t");
    scanf("%s",CIF.user.zip);
    fflush(stdin);

    printf("\nEnter your 10 digits Mobile number\t");
    scanf("%ld",&CIF.user.mobile_no);
    fflush(stdin);

    printf("\nEnter email\t");
    scanf("%s",CIF.user.email);
    fflush(stdin);
    CIF.user.user_id = gen_random();
    sprintf(CIF.user.password,"%ld",gen_random());
    
    struct req_res_packet req,res;
    req.op_code = Add_user;
    req.user = CIF;
    res = Execute(req);
    if(res.status ) pAccountDetails(res.user);
    return res.status;
}

int update_info(struct account_details user, int seq){                    //Update Account details like mob no, email, uid, pass of user 
    char flag;
    
    printf("\nCurrent mobile number in database : %ld\t", user.user.mobile_no);
    printf("\nEnter 'y' if want to update else press any key\t");
    scanf(" %c",&flag);

    if(flag =='y' || flag =='Y'){
        printf("\nEnter New Mobile number\t");
        scanf("%ld",&user.user.mobile_no);
        
    }

    printf("\nCurrent email in database : %s\t", user.user.email);
    printf("\nEnter 'y' if want to update else press any key\t");
    scanf(" %c",&flag);
    if(flag =='y' || flag =='Y'){
        printf("\nEnter New Email \t");
        scanf("%s",user.user.email);
    }
    printf("\nCurrent user id in database : %ld\t", user.user.user_id);
    printf("\nEnter 'y' if want to update else press any key\t");
    scanf(" %c",&flag);
    if(flag =='y' || flag =='Y'){
        printf("\nEnter New user id \t");
        scanf("%ld",&user.user.user_id);
    }
    printf("\nCurrent password in database : %s\t", user.user.password);
    printf("\nEnter 'y' if want to update else press any key\t");
    scanf(" %c",&flag);
    if(flag =='y' || flag =='Y'){
        printf("\nEnter New password\t");
        scanf("%s",user.user.password);
    }

    struct req_res_packet req,res;
    req.op_code = Update;
    printf("\nSend opcode %d",req.op_code);
    req.user = user;
    req.uno = seq;
    res = Execute(req);

    return res.status;                           // Return 1 for Success 0 for Failure
}

 

struct req_res_packet dologin(int uid , char pass[15], int plngth){                   // Login Task
    struct req_res_packet req;
    req.op_code = login;
    req.user.user.user_id = uid;
    strcpy(req.user.user.password,pass);
    req.status = plngth;                     // Length of entered password
    return Execute(req);
 }

struct account_details getdata(int seqno){                     // To get account struct from seq no
    struct account_details req_user;
    struct req_res_packet req;
    req.op_code = get_data;
    req.uno = seqno;
    return Execute(req).user;
}

int getseqno(long int acc){
    struct req_res_packet req,res;
    req.op_code = Find_acc;
    req.user.Account_number = acc;
    req.user.user.user_id = acc;
    res = Execute(req);
    if(res.status = 1) return res.uno;
    else printf("\nUser not found\n");
    return -1;
}

struct req_res_packet deposite(int seq,int amount){
    struct req_res_packet req,res;
    req.uno = seq;
    req.status = amount;
    req.user_pri = 1;
    req.op_code = Deposite;
    res = Execute(req);
    return res;
}

void res_read(struct req_res_packet res){
    printf("\n\nOpcode %d\n\tStatus %ld\n\tUnser no %d\n\tuser Priviage %d",res.op_code,res.status,res.uno,res.user_pri);
}

struct req_res_packet Withdraw_u(int seq,int amount){
    struct req_res_packet req,res;
    req.uno = seq;
    req.status = amount;
    req.user_pri = 1;
    

    req.op_code = Withdraw;
    res = Execute(req);
//    printf("\nBal %ld\n",res.status);
    return res;
}

long int ChangePass(struct account_details account, int seqno){
    struct req_res_packet req,res;
    req.op_code = Pass_change;
    req.user = account;
    req.user_pri = 1;
    req.uno = seqno;
    res = Execute(req);
    return res.status;
}

int logout(struct req_res_packet detail){
    struct req_res_packet req,res;
    req.op_code = Logout;
    req.user_pri = detail.status;
    req.user = detail.user;
    req.uno = detail.uno;
   // printf("Logout at uno %d",req.uno);
    res = Execute(req);
    return res.status;
}
/*
struct req_res_packet (int seq,int amount){
    struct req_res_packet req,res;
    req.uno = seq;
    req.status = amount;
    req.user_pri = 1;
    req.op_code = Withdraw;

    res = Execute(req);
    //res_read(res);
    return res;
}*/