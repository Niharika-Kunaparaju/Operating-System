#ifndef EVENT_H
#define EVENT_H

#include <iostream>

using namespace std;

class Event{
	public:
		enum operation_type {
			add,
			finish,
			issue
		};
		
	private:
		unsigned int timestamp;
		unsigned int tracknumber;
		static unsigned int event_id;
		operation_type operation;
		unsigned int req_id;
		unsigned int arr_time;
		
	public:
		Event(unsigned int arr_time,unsigned int req_id, unsigned int timestamp, unsigned int tracknumber, operation_type operation){
			this->arr_time = arr_time;
			this->req_id = req_id;
			this->timestamp = timestamp;
			this->tracknumber = tracknumber;
			event_id++;
			//cout << "eventID in constructor " << event_id << endl;
			this->operation = operation;
		}
	
	unsigned int getarrtime(){
		return this->arr_time;
	}
		
	unsigned int gettimestamp(){
		return this->timestamp;
	}
	
	void settimestamp(unsigned int t){
		this->timestamp = t;
	}
	unsigned int gettrack(){
		return this->tracknumber;
	}
	
	static unsigned int geteventid(){
		//cout << "event ID in getter " << event_id << endl;
		return event_id;
	}
	
	operation_type getoperation(){
		return this->operation;
	}
	
	unsigned int getreqid(){
		return this->req_id;
	}	
	
};

unsigned int Event:: event_id=0;
#endif