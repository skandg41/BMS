# BMS
**Banking Management System**
Basic Functionality defined like admin and client
admin can Create, modify, search, delete accounts
Client can Add a account, Modify his own Account, Deposit and withdraw his own account, and change password of his account

Details :
    Admin can   Add_user 1
                Modify_user 2
                Delete_user 3
                search_user 4
                Admin_Logout 5
                Find_acc 6
                Logout 7

    User can    Create 1
                Update 2
                Get_details 3
                Bal_enquiry 4
                Deposite 5
                Withdraw 6
                Pass_change 7
                Logout 8

A user can login using one device at a time if logout fails or some error occurs then need to contact admin for status reset otherwise will not be able to login again

For updation, Deposit and Withdrawal operations at user end there is some dependency user has to logout and login again to get the updated status everyting is consistent at server side.
