#include "lib.h"
#include "db_def.h"

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

void main(){
struct account_details CIF;
//    CIF.Account_number = getAccount_Number();
    CIF.status = 1;
    while(1)
    {
        printf("Enter type of Account \t 1 for Saving \n\t\t 2 for Current \n\t\t 3 for Loan\t");
        scanf("%d",&CIF.Acc_type);
        printf("\nEnter Holding Type : \t 1 for Single Holder \n\t\t 2 for Joint Holding\n");
        scanf("%d",&CIF.user.u_type);
        if( (CIF.Acc_type == SB || CIF.Acc_type == Curr || CIF.Acc_type == Loan)  && ( CIF.user.u_type == Single || CIF.user.u_type == Joint ) ) {
    
            break;
        }
        printf("\nwhil\n");
    }
    
    if (CIF.user.u_type == Single){
        printf("\nEnter Account Holder's Name :");
        scanf("%s",CIF.user.first_holder_name);
    }
    else {
        printf("\nEnter First Holder's Name :");
        scanf("%s",CIF.user.first_holder_name);
        printf("\nEnter Second Holder's Name :");
        scanf("%s",CIF.user.second_holder_name);
    }
    printf("\nEnter Address \t");
    scanf("%s", CIF.user.address);
    printf("\nEnter City\t");
    scanf("%s",CIF.user.city);
    printf("\nEnter State\t");
    scanf("%s",CIF.user.state);
    printf("\nEnter ZIP code\t");
    scanf("%s",CIF.user.zip);
    printf("\nEnter your 10 digits Account number\t");
    scanf("%ld",&CIF.user.mobile_no);
    printf("\nEnter email\t");
    scanf("%s",CIF.user.email);
        CIF.user.user_id = gen_random();
    sprintf(CIF.user.password,"%ld",gen_random());
    
    struct req_res_packet req,res;
    req.op_code = Add_user;
    res.user = CIF;
    pAccountDetails(res.user);
    }