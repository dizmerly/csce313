#include "common.h"
#include "channel.h"
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;

class Account {
public:
    int id;
    double balance;
    bool active;
    Account() : id(-1), balance(0.0), active(false) {}  // Used only for array initialization
    Account(int _id) : id(_id), balance(0.0), active(true) {}
};

#define MAX_ACCOUNTS 100  // Or whatever reasonable limit we want to set

int main(int argc, char* argv[]) {
    // Default max accounts if not specified
    int max_accounts = 100;
    
    // Parse command line arguments
    for(int i = 1; i < argc; i++) {
        string arg = argv[i];
        if(arg == "-m" && i + 1 < argc) {
            max_accounts = atoi(argv[++i]) + 1;
        }
    }
    
    RequestChannel channel("finance", RequestChannel::SERVER_SIDE);
    Account* accounts = new Account[max_accounts];
    
    while (true) {
        Request r = channel.receive_request(0);

        if (r.type == QUIT) {
            delete[] accounts;
            exit(0);
        }

        Response resp;
        resp.success = true;

        if (r.user_id < 0 || r.user_id >= max_accounts) {
            resp.success = false;
            resp.message = "Invalid account ID";
            channel.send_response(resp);
            continue;
        }

        // Create account if it doesn't exist
        if (!accounts[r.user_id].active) {
            accounts[r.user_id] = Account(r.user_id);
        }

        Account& acc = accounts[r.user_id];
        
        if (r.type == DEPOSIT) {
            acc.balance += r.amount;
            resp.balance = acc.balance;
            resp.message = "Deposit successful";
        } 
        else if (r.type == WITHDRAW) {
            if (acc.balance >= r.amount) {
                acc.balance -= r.amount;
                resp.balance = acc.balance;
                resp.message = "Withdrawal successful";
            } else {
                resp.success = false;
                resp.message = "Insufficient funds";
            }
        }
        else if (r.type == BALANCE) {
                resp.balance = acc.balance;
                resp.message = "View balance successful";
        }
        else {
            resp.success = false;
            resp.message = "Unknown RequestType";
        }

        channel.send_response(resp);
    }
    return 0;
}