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
#include <sys/wait.h>

using namespace std;


int main (int argc, char *argv[]) {
	int opt;
	int p = 1;
	double t = 0.0;
	int e = 1;
	
	string filename = "";
	bool has_time = false; 
	int buffer_capacity = MAX_MESSAGE;
	bool use_new_channel = false; 

	while ((opt = getopt(argc, argv, "p:t:e:f:m:c")) != -1) {

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
			case 'm':
				buffer_capacity = atoi(optarg);
				break; 
			case 'c':
				use_new_channel = true;
				break; 
		}


	// Task 4.1: running the server with a child process
	}
	// Parent connecting to server
	pid_t pid = fork();

	if(pid == 0) {
		// child starting the server itself
		string capacity_arg = to_string(buffer_capacity);
		
		char* args[] = {(char*)"./server", (char*)"-m", (char*)capacity_arg.c_str(), nullptr};
		execvp(args[0], args); 

		perror("execvp failed");
		_exit(1);
	}

	else if(pid<0) {
		perror("Fork Failed");
		return 1; 
	}
	
	FIFORequestChannel chan("control", FIFORequestChannel::CLIENT_SIDE);

	FIFORequestChannel* data_chan = nullptr;
	FIFORequestChannel* work_chan = &chan;

	if(use_new_channel) {
		MESSAGE_TYPE request = NEWCHANNEL_MSG;
		chan.cwrite(&request, sizeof(request));

		
		char name[30];
		chan.cread(name, sizeof(name));

		data_chan = new FIFORequestChannel(name, FIFORequestChannel::CLIENT_SIDE);
		work_chan = data_chan;
	}
	
	if (!filename.empty()) {
		filemsg request(0,0);

		int size_message = sizeof(request) + filename.size() + 1;
		char* message = new char[size_message];

		memcpy(message, &request, sizeof(request));
		strcpy(message + sizeof(request), filename.c_str());

		work_chan->cwrite(message, size_message);

		__int64_t size_file;
		work_chan->cread(&size_file, sizeof(size_file));
		cout << "File size: " << size_file << " bytes\n";
		mkdir("received", 0777);
		ofstream output("received/" + filename, ios::binary);

		vector<char> buffer(buffer_capacity); 

		for(__int64_t offset = 0; offset < size_file;) {
			int length;
			if (size_file - offset < buffer_capacity) {
				length = static_cast<int>(size_file - offset);
			} else {
				length = buffer_capacity;
			}

			filemsg chunk(offset, length);
			memcpy(message, &chunk, sizeof(chunk));
			work_chan->cwrite(message, size_message);

			int bytes_read = 0;
			while (bytes_read < length) {
				int n = work_chan->cread(buffer.data() + bytes_read, length - bytes_read);
				if (n <= 0) {
					cerr << "Failed to receive file data" << endl;
					break;
				}
				bytes_read += n;
			}
			if (bytes_read != length) {
				break;
			}
			output.write(buffer.data(), length);
			offset += length;
		}

		delete[] message;


	}

	else if(has_time) {
		datamsg request(p, t, e);
		work_chan->cwrite(&request, sizeof(request));

		double reply; 
		work_chan->cread(&reply, sizeof(reply));
		cout << "For person " << p << ", at time " << t << ", the value of ecg " << e << " is " << reply << endl;
	} else { 
		mkdir("received", 0777);
		ofstream csvWriter("received/x1.csv");

		for(int i = 0; i < 1000; i++) { 
			double seconds = i * 0.004; 

			datamsg first(p, seconds, 1);
			work_chan->cwrite(&first, sizeof(first));
			double ecg1;
			work_chan->cread(&ecg1, sizeof(ecg1));
			
			datamsg second(p, seconds, 2);
			work_chan->cwrite(&second, sizeof(second));
			double ecg2;
			work_chan->cread(&ecg2, sizeof(ecg2));
			
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
    work_chan->cwrite(&m, sizeof(m));

	if(data_chan != nullptr){
		delete data_chan;
		chan.cwrite(&m, sizeof(m));
	}
	waitpid(pid, nullptr, 0);

}
