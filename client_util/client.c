#include "lib.h"
#include "db_def.h"
#include "client_communicator.h"
#include "util.h"


int main(){
    struct req_res_packet req,res;
    struct req_res_packet user_identity;
    long int id; char pass[15]; int plength;
    printf("\n\t\t########## Welcome To Your Bank ###################\n");
while(1){    
    printf("\n\t\t################ Login Here #######################\n");
    printf("\t\tUser id :\t");
    scanf("%ld",&id);
    printf("\n\t\tPassword : \t");
    plength = scanf("%s",pass);
    
    user_identity = dologin (id,pass,plength) ;

    if( user_identity.status >= 0) break;
    else printf("\n\t\t * Invalid Login \n\t\t * Please contact Admin incase you Forgot password\n"); 
    }
    printf("\n\t\t *Login Successful*\nWelcome %s\n",user_identity.user.user.first_holder_name);
    long int acc;
    int st;
    printf("User Type : %d", user_identity.user.user.u_type);
    if( user_identity.user.user.u_type == Admin){
        int admin_choice,seqno;
adm_op:    printf("\n\t\t Welcome %s to Your Bank Management Portal \n", user_identity.user.user.first_holder_name);
        struct account_details operating_acc;  
        printf("\n\n You can Choose one of Below options to perform Operations\n\t Add_user 1 \n\t Modify_user 2 \n\tDelete_user 3 \n\tsearch_user 4 \n\tAdmin_Logout 5\t");
        scanf("%d",&admin_choice);
    
        switch(admin_choice){
            
            case Add_user:
                st = feed_Details();
                if(st == 1 ) printf("\n\t*** Thank You for opening an Account ***\n");
                else printf("\nSorry Something Went Wrong please try again\n");
                goto adm_op;
                break;
            case Modify_user:
                 
                printf("\nEnter user account number or user id\t");
                scanf("%ld",&acc);
                int seq = getseqno(acc);
                if(seq < 0 ) {
                    printf("/n/t/tInvalid Account number/n");
                    goto adm_op;
                }
                operating_acc = getdata(seq);
                printf("data found");
                update_info(operating_acc,seq);
                goto adm_op;
                break;
            
            case Delete_user:
                req.op_code = Delete_user;
                printf("\nEnter user account number or user id\t");
                scanf("%ld",&acc);
                req.user.Account_number = acc;
                req.user.user.user_id = acc;
                req.user_pri = Admin;
                res = Execute(req);
                if(res.status) printf("\nSuccess");
                goto adm_op;
                break;

            case search_user:
                printf("\nEnter user account number or user id\t");
                scanf("%ld",&acc);
                seqno = getseqno(acc);
                if(seqno < 0 ) {
                    printf("User inactive or Does not exist\n");
                    }
                else {
                    printf("\nUser Found\n");
                    struct account_details account;
                    account = getdata(seqno);
                    pAccountDetails(account);
                }
                goto adm_op;
                break;
            case Admin_Logout:
                printf("\nLoging out...\n");
                req.op_code = Admin_Logout;
                return 0;
                break;
        }
    }
    else if(user_identity.user.user.u_type == Single || user_identity.user.user.u_type == Joint ) {                            // Status 1 if user
        int user_choice, seqno;

    usr_op:    printf("\n\t\t Welcome %s to Your Bank Management Portal \n", user_identity.user.user.first_holder_name);
        if(user_identity.user.Acc_type == Joint) printf("\t\t Welcome %s to Your Bank Management Portal \n", user_identity.user.user.second_holder_name);
        
        seqno = user_identity.uno;
        
        if ( user_identity.user.status < 0 || user_identity.uno < 1 ) {
            printf("\n User either Inactive or does not exists\n");
            return 0;
        }            
        
        struct account_details account;
        account = user_identity.user;
        long int amount;
        printf("\n\n You can Choose one of Below options to perform Operations\n\t Create new Account 1 \n\t Update your Account 2 \n\tGet Acc Details 3 \n\tBalance Enquiry 4 \n\tDeposite 5\n\tWithdraw 6\n\tChange Password 7\n\tLogout 8\t");
        scanf("%d",&user_choice);
        
        switch(user_choice){       
            case Create : 
                st = feed_Details();
                if(st == 1 ) printf("\n\t*** Thank You for opening a new Account ***\n");
                else printf("\nSorry Something Went Wrong please try again\n");
                goto usr_op;
                break;
            case Update:
                update_info(account,seqno);
                goto usr_op;
                break;
            case Get_details:
                pAccountDetails(account);
                goto usr_op;
                break;
            case Bal_enquiry:
                printf("Available Balance : %.2f",account.balance);
                goto usr_op;
                break;
            case Deposite:
                printf("\n\tEnter Amount u wish to deposite in your account \t");
                scanf("%ld",&amount);
                res = deposite(seqno, amount);
                printf("Status %ld",res.status);
                if(res.status == 1) {
                    account = res.user;
                    printf("\n\t Rupees %d \n\tdeposited in Account number %ld \n\tupdated balance is %.2f\n",amount,account.Account_number,account.balance);
                }
                else
                {
                     printf("\n\tTransection failed please try again later\n");
                }
                goto usr_op;
                break;
            case Withdraw:
                printf("\n\tEnter Amount u wish to Withdraw from your account \t");
                scanf("%ld",&amount);
                res = Withdraw_u(seqno, amount);
                printf("Responce received %ld",res.status);
                if(res.status == 1) {
                    account = res.user;
                    printf("\n\t Rupees %d \n\tdeposited in Account number %ld \n\tupdated balance is %.2f\n",amount,account.Account_number,account.balance);
                }
                else if(res.status == 0)
                {
                     printf("\n\tTransection failed please try again later\n");
                }
                else printf("\n\tInsufficient balance in your account\n");
                goto usr_op;
                break;
            
            case Pass_change:

                break;

            case Logout:
                printf("\n..Logging out... Please visit us soon %s\n",account.user.first_holder_name);
                return 0;
                break;
            //default :
              //  goto usr_op;
        }
    }
    else
    {
        printf("\nInvalid login\n");
        return 0;
    }
}

