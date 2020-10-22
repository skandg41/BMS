#include "lib.h"
#include "db_def.h"

void pAccountDetails(struct account_details ledger ){
    printf("\t\t Account holder name : %s\n", ledger.user.first_holder_name);
    if(ledger.user.u_type == Joint ) printf("\t\t Account holder name : %s\n", ledger.user.second_holder_name);
    printf("\n Account Number : %ld \n Available Balance : %.2f \n Lien Amount : %.2f\n Address : %s, %s, %s, %s,%s \n",ledger.Account_number,ledger.balance,ledger.lien_amount, ledger.user.address,ledger.user.city,ledger.user.state,ledger.user.country,ledger.user.zip);
    printf("Mobile : %ld\n email : %s", ledger.user.mobile_no,ledger.user.email);
    printf("\n\tUser id : %ld\n\t Password : %s",ledger.user.user_id,ledger.user.password);
    printf("\n\tUser Type : %d\n",ledger.user.u_type);
    printf("\n\tUser Status : %d\n",ledger.status);
    
}

void main(){
    struct account_details acc;
    int fd = open("db",O_RDONLY);
    int i = 0;
    while( read(fd,&acc,sizeof(acc))) 
    {
        printf("\n*****At seq %d\n",i++); 
        pAccountDetails(acc);
    }
    
}