#include "signals.h"
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctime>
#include <algorithm>
#include <sstream>

using namespace std;

namespace SignalHandling {

    // TODO: Declare atomic flags for shutdown_requested (boolean), timeout_occurred (boolean), and child_exited (integer)
    // Initialize atomic flags here
    std::atomic<bool> shutdown_requested = false;
    std::atomic<bool> timeout_occurred = false;
    std::atomic<int> child_exited = 0;
    
    // Server process registry
    std::vector<ServerProcess> server_processes;
    
    void setup_handlers() {

        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sigemptyset(&sa.sa_mask);

        // TODO: Set up signal handlers using sigaction
        // - Set up SIGINT handler with no flags
        sa.sa_handler = sigint_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0; 
        sigaction(SIGINT, &sa, nullptr);
        // - Set up SIGALRM handler with no flags
        sa.sa_handler = sigalrm_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGALRM, &sa, nullptr);
        // - Set up SIGCHLD handler with SA_RESTART flags
        sa.sa_handler = sigchld_handler; 
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sigaction(SIGCHLD, &sa, nullptr);
        
        log_signal_event("Signal handlers initialized");
    }

    // TODO: Declare functions for signal handlers (sigint_handler, sigalrm_handler, sigchld_handler)
    
    void sigint_handler(int sig) {

        // TODO: Implement SIGINT handler
        // - Set shutdown flag during first SIGINT
        // - Log the signal event using pre-defined function log_signal_event()
        // - Handle second SIGINT for forced exit
        // - Print messages for the user on terminal using signal-safe functions
        // - Strictly use the following messages for logging :
        // - - For 1st SIGINT - "SIGINT received - initiating graceful shutdown"
        // - - For 2nd SIGINT - "Second SIGINT received - forcing exit"
        
        if (!shutdown_requested) {
            // First SIGINT - Graceful termination
            shutdown_requested = true;
            log_signal_event("SIGINT received - initiating graceful shutdown");
            printf("SIGINT received - initiating graceful shutdown\n");
        } else {
            // Second SIGINT - force exit
            log_signal_event("Second SIGINT received - forcing exit");
            printf("Second SIGINT received - forcing exit\n");
        }
    }
    
    void sigalrm_handler(int sig) {

        // TODO: Implement SIGALRM handler
        // - Set timeout flag
        // - Print message for the user on terminal using signal-safe functions
        // - Log the signal event using pre-defined function log_signal_event()
        // - Strictly use the following message for logging :
        // - - "SIGALRM received - operation timed out"
    }
    
    void sigchld_handler(int sig) {
        int status;
        pid_t pid;
        
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            child_exited++;
            
            // Update server registry
            for (auto& server : server_processes) {
                // TODO: Update server registry when a child process terminates
                // - Find the server in server_processes vector using its PID
                // - Update its status to inactive
                // - Log the event using pre-defined function log_signal_event()
                // - Strictly use the following message for logging :
                // - - "Child process terminated: <server-name> (PID: <pid>)"
            }
        }
    }

    // TODO: Declare functions for signal operations (block_signals, unblock_signals, wait_with_timeout)
    
    void block_signals() {

        // TODO: Implement signal blocking
        // - Create and populate signal set
        // - Block SIGINT signal
        // - Log the signal event using pre-defined function log_signal_event()
        // - Strictly use the following message for logging :
        // - - "Signals blocked for critical section"
    }
    
    void unblock_signals() {

        // TODO: Implement signal unblocking
        // - Create and populate signal set
        // - Unblock SIGINT signal
        // - Log the signal event using pre-defined function log_signal_event()
        // - Strictly use the following message for logging :
        // - - "Signals unblocked"
    }
    
    bool wait_with_timeout(int seconds) {
        
        // TODO: Implement timeout mechanism
        timeout_occurred = false;
        // - Set alarm

        return true;
    }
    
    void cancel_timeout() {
        // TODO: Implement timeout cancellation
        // - Cancel any pending alarm
    }
    
    void register_server(pid_t pid, const std::string& name) {
        server_processes.push_back({pid, name, true});
        
        std::stringstream ss;
        ss << "Registered server: " << name << " (PID: " << pid << ")";
        log_signal_event(ss.str());
    }
    
    bool is_server_active(const std::string& name) {
        auto it = std::find_if(server_processes.begin(), server_processes.end(),
                              [&name](const ServerProcess& p) { return p.name == name; });
        
        return (it != server_processes.end()) && it->active;
    }
    
    void print_server_status() {
        std::cout << "\n=== Server Status ===\n";
        for (const auto& server : server_processes) {
            std::cout << server.name << " (PID: " << server.pid << "): "
                      << (server.active ? "ACTIVE" : "TERMINATED") << std::endl;
        }
        std::cout << "====================\n";
    }
    
    void log_signal_event(const std::string& message) {
        // Get timestamp
        time_t now = time(NULL);
        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        
        // Format log entry
        std::string log_entry = std::string(timestamp) + " - " + message + "\n";
        
        // Write to log file (signal-safe)
        int fd = open("signals.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd >= 0) {
            write(fd, log_entry.c_str(), log_entry.size());
            close(fd);
        }
    }
}