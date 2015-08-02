#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include<queue>
#include "Scheduler.cpp"
#include<iomanip>

using namespace std;

ifstream myFile;
Scheduler *algo;
unsigned int total_time = 0;
unsigned int tot_movement = 0;
unsigned int turnaround = 0;
double avg_turnaround = 0.0;
unsigned int waittime = 0;
double avg_waittime = 0.0;
unsigned int max_waittime = 0;
bool verbose;


struct event_compare{
	bool operator()(Event* a, Event* b)const{
		if(a->gettimestamp() > b->gettimestamp()){
			return true;
		}	
		
		else if(a->gettimestamp() == b->gettimestamp()){
			if(a->getoperation() > b->getoperation()) {
				return true;
			}
		}
		
		else if((a->gettimestamp() == b->gettimestamp()) && (a->geteventid() > b->geteventid())){
			return true;
		}		
	}
};

void summary(){
	printf("SUM: %d %d %.2lf %.2lf %d\n",
		total_time,
		tot_movement,
		avg_turnaround,
		avg_waittime,
		max_waittime);	
}




int main(int argc, char* argv[]) {
	if(argc > 4){
		cout << "Number of arguments are greater than 4" << endl;
		exit(99);
	}
	
	int sflag = 0;
	char svalue;
	int c;
	bool verbose;
	
	while ((c = getopt (argc, argv, "vs:")) != -1){
		switch (c){
			
			case 'v':
				verbose = 1;
				break;
			
			case 's':
				svalue = optarg[0];
				sflag = 1;
				break;			
			
			default:
				break;
		}
	}
	
	myFile.open(argv[optind]);
	
	switch(svalue){
		case 'i':
			algo = new FIFO();
			break;
			
		case 'j':
			algo = new SSTF();
			break;
		
		case 's':
			algo = new SCAN();
			break;
			
		case 'c':
			algo = new CSCAN();
			break;
			
		case 'f':
			algo = new FSCAN();
			break;
	}
	
	string line;
	int id = 0;
	double count = 0;
	
	priority_queue<Event*, vector<Event*>, event_compare> event_q;
	
	while (getline(myFile, line)){
		istringstream iss(line);
		if (line[0] == '#'){
			continue;
		}
		int timeStep=0;
		int track = 0;
		iss >> timeStep >> track; 

		Event *event = new Event(timeStep,id,timeStep,track,Event::add);
		event_q.push(event);
		id++;
		count++;
	}
	
	vector<unsigned int> at(count,0);
	vector<unsigned int> it(count,0);
	vector<unsigned int> ft(count,0);
	
	if(verbose){
		cout << "TRACE" << endl;
	}
	
	unsigned int finish_time;
	unsigned int eid = 0;
	unsigned int next_available = event_q.top()->gettimestamp();
	unsigned int current_position = 0;
	unsigned int prev_position = 0;
	unsigned int diff = 0;
	
	while(!event_q.empty()){
		Event *event_now = event_q.top();
		event_q.pop();
		
		Event::operation_type op = event_now->getoperation();
		//cout << "Event ID " << event_now->geteventid() << endl;
		unsigned int timestamp = event_now->gettimestamp();
		
		
		if(op == Event::add) {
			algo->putevent(event_now);
			if(next_available<timestamp){
				next_available = timestamp;
			}
			//cout << "request id is" << event_now->getreqid() << endl;
			at[event_now->getreqid()] = event_now->gettimestamp();	
			Event *event_new = new Event(event_now->getarrtime(),event_now->getreqid(),next_available,event_now->gettrack(),Event::issue);
			event_q.push(event_new);
			
			if(verbose){
				cout << event_now->gettimestamp() << ":" << setfill(' ') << setw(6) << event_now->getreqid() << " add " << event_now->gettrack() << endl;	
			}
			
		}
		
		if(op == Event::issue){
			if(event_now->gettimestamp() < next_available) {
				//Event *event_new = new Event(event_now->getarrtime(),event_now->getreqid(),next_available,event_now->gettrack(),Event::issue);
				event_now->settimestamp(next_available);
				event_q.push(event_now);
				continue;
			}
			event_now = algo->getevent(current_position);
			prev_position = current_position;
			current_position = event_now->gettrack();
			it[event_now->getreqid()] = next_available;
			if(verbose){
			cout << next_available << ":" << setfill(' ') << setw(6) << event_now->getreqid() << " issue " << current_position << " " << prev_position << endl;
			}
			
			
			waittime = waittime + next_available - event_now->getarrtime();
			
			if(max_waittime<(next_available - event_now->getarrtime())){
				max_waittime = next_available - event_now->getarrtime();
			}
			
			diff = abs(current_position - prev_position);
			next_available = next_available + diff;
			tot_movement = tot_movement + diff;
			
			Event *event_new = new Event(event_now->getarrtime(),event_now->getreqid(),next_available,event_now->gettrack(),Event::finish);
			event_q.push(event_new);
		}
		
		if(op == Event::finish){
			ft[event_now->getreqid()] = event_now->gettimestamp();
			if(verbose){
				cout << event_now->gettimestamp() << ":" << setfill(' ') << setw(6) << event_now->getreqid() << " finish " << event_now->gettimestamp() - event_now->getarrtime()<< endl;
			}	
			
			turnaround = turnaround + event_now->gettimestamp() - event_now->getarrtime();
		}		
		total_time = timestamp;
		avg_turnaround = turnaround/count;
		avg_waittime = waittime/count;
	}
	
	if(verbose){
		cout << "IOREQS INFO" << endl;
		for (int i=0;i<at.size();i++){
			cout << setfill(' ') << setw(5) << i << ":" << setfill(' ') << setw(6) << at[i] << setfill(' ') << setw(6) << it[i] << setfill(' ') << setw(6) << ft[i] << endl;
		}
	}
	summary();
}