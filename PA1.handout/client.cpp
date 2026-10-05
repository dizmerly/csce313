/*
	Original author of the starter code
    Tanzir Ahmed
    Department of Computer Science & Engineering
    Texas A&M University
    Date: 2/8/20
	
	Please include your Name, UIN, and the date below
	Name: Daniel Izmerly
	UIN: 437005378
	Date: 09/30/2026
*/
#include "common.h"
#include "FIFORequestChannel.h"
#include <fstream>

using namespace std;


int main (int argc, char *argv[]) {
	int opt;
	int p = 1;
	double t = 0.0;
	int e = 1;
	
	string filename = "";
	bool has_time = false; 
	while ((opt = getopt(argc, argv, "p:t:e:f:")) != -1) {

		switch (opt) {
			case 'p':
				p = atoi (optarg);
				break;
			case 't':
				t = atof (optarg);
				has_time = true; 
				break;
			case 'e':
				e = atoi (optarg);
				break;
			case 'f':
				filename = optarg;
				break;
		}


	// Task 4.1: running the server with a child process
	}
	// Parent connecting to server
	pid_t pid = fork();

	if(pid == 0) {
		// child starting the server itself
		char *args[] = {(char * ) "./server", NULL};
		execvp(args[0], args); 

		perror("execvp failed");
		_exit(1);
	}

	else if(pid<0) {
		perror("Fork Failed");
		return 1; 
	}
	
	FIFORequestChannel chan("control", FIFORequestChannel::CLIENT_SIDE);
	

	if(has_time) {
		datamsg request(p, t, e);
		chan.cwrite(&request, sizeof(request));

		double reply; 
		chan.cread(&reply, sizeof(reply));
		cout << "For person " << p << ", at time " << t << ", the value of ecg " << e << " is " << reply << endl;
	} else { 

		ofstream csvWriter("x1.csv");

		for(int i = 0; i < 1000; i++) { 
			double seconds = i * 0.004; 

			datamsg first(p, seconds, 1);
			chan.cwrite(&first, sizeof(first));
			double ecg1;
			chan.cread(&ecg1, sizeof(ecg1));
			
			datamsg second(p, seconds, 2);
			chan.cwrite(&second, sizeof(second));
			double ecg2;
			chan.cread(&ecg2, sizeof(ecg2));
			
			csvWriter << seconds << "," << ecg1 << "," << ecg2 << "\n";
		}
	}


	// example data point request
    // char buf[MAX_MESSAGE]; // 256
    // datamsg x(p, t, e);
	
	// memcpy(buf, &x, sizeof(datamsg));
	// chan.cwrite(buf, sizeof(datamsg)); // question
	// double reply;
	// chan.cread(&reply, sizeof(double)); //answer
	// cout << "For person " << p << ", at time " << t << ", the value of ecg " << e << " is " << reply << endl;
	
    // sending a non-sense message, you need to change this
	// filemsg fm(0, 0);
	// string fname = "teslkansdlkjflasjdf.dat";
	
	// int len = sizeof(filemsg) + (fname.size() + 1);
	// char* buf2 = new char[len];
	// memcpy(buf2, &fm, sizeof(filemsg));
	// strcpy(buf2 + sizeof(filemsg), fname.c_str());
	// chan.cwrite(buf2, len);  // I want the file length;

	// delete[] buf2;
	
	// closing the channel    
    MESSAGE_TYPE m = QUIT_MSG;
    chan.cwrite(&m, sizeof(MESSAGE_TYPE));
	waitpid(pid, nullptr, 0);
}
