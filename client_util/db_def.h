#define SB 1
#define Curr 2
#define Loan 3

// Status bits
#define Active 1
#define Deleted -1
#define LoggedIN 0

// User Types
#define Admin 0
#define Single 1
#define Joint 2
// User privilage
#define Normal_user
// Login Opcode
#define login 0
// IM
#define get_data -1
// Transection Type opcode for user
#define Create 1
#define Update 2
#define Get_details 3
#define Bal_enquiry 4
#define Deposite 5
#define Withdraw 6
#define Pass_change 7
#define Logout 8

// Admin Utility opcode for Admin
#define Add_user 1
#define Modify_user 2
#define Delete_user 3
#define search_user 4
#define Admin_Logout 5
#define Find_acc 6
// Logout 8

// DB

struct client_details{
    int u_type;                                               // Normal or Admin
    char first_holder_name[50],second_holder_name[50];
    long int user_id;
    char password[15];
    char address[30];
    char city[10];
    char state[15];
    char country[10];
    char zip[6];
    long int mobile_no;
    char email[15];
};

struct transaction{
    char dtime[24];
    int amount;
    double balance;
};

struct account_details {
    int Acc_type;
    long int Account_number;
    float balance;
    float lien_amount;
    struct client_details user;
    struct transaction ledger;
    char status;                                              // i for inactive a for active
};


struct req_res_packet {
    int op_code;
    int uno ;     
    struct account_details user;
    long int status;
    int user_pri;
};