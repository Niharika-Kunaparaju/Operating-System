#ifndef Scheduler_H
#define Scheduler_H

#include<vector>
#include "Event.cpp"
#include <climits>

using namespace std;

class Scheduler{
	protected:
		vector<Event*> ready_q;
	public:
		virtual void putevent(Event*) = 0;
		virtual Event* getevent(unsigned int) = 0;
};


class FIFO : public Scheduler{
	public:	
		
		FIFO(){
		}
		
		void putevent(Event* event){
			ready_q.push_back(event);
		}
		
		Event* getevent(unsigned int current_track){
			Event* event = ready_q.front();
			ready_q.erase(ready_q.begin());
			return event;
		}
};

class SSTF : public Scheduler{
	
	public:	
		
		SSTF(){
		}
		
		void putevent(Event* event){
			ready_q.push_back(event);
		}
		
		Event* getevent(unsigned int current_track){
			
			unsigned int shortest_seek = INT_MAX, track_diff, eve = 0;
			
			for (int i=0;i<ready_q.size();i++){
			
				track_diff = abs(ready_q[i]->gettrack() - current_track);		
				
				if(track_diff < shortest_seek){
					shortest_seek = track_diff;
					eve = i;
				}
				
			}		
			
			Event* event = ready_q.at(eve);
			ready_q.erase(ready_q.begin()+eve);
			return event;
		}
};


class CSCAN : public Scheduler{
	
	
	public:	
		
		CSCAN(){
		}
		
		
		void putevent(Event* event){	
			
			unsigned int insert_loc=ready_q.size();
			
			for(int a=0; a<ready_q.size();a++){
				if(event->gettrack() < ready_q[a]->gettrack()){
					insert_loc = a; 
					break;
				}
			}				
			ready_q.insert(ready_q.begin()+insert_loc, event);	

		}
		
		Event* getevent(unsigned int current_track){
			
			unsigned int loc = 0;
			for (int j=0;j<ready_q.size();j++){
				if(current_track > ready_q[j]->gettrack())	{
				continue;
				}
				loc = j;
				break;
			}
			
			Event* event = ready_q.at(loc);
			ready_q.erase(ready_q.begin()+loc);
			return event;
			
		}
};

class SCAN : public Scheduler{
	private:
		bool positive;
	
	public:	
		
		SCAN(){
			positive = true;
		}
		
		void putevent(Event* event){	
			
			unsigned int insert_loc=ready_q.size();
			
			for(int a=0; a<ready_q.size();a++){
				if(event->gettrack() < ready_q[a]->gettrack()){
					insert_loc = a; 
					break;
				}
			}				
			ready_q.insert(ready_q.begin()+insert_loc, event);	

		}
		
		Event* getevent(unsigned int current_track){
			
			Event *left_min = NULL, *right_min = NULL, *ioreturn = NULL;
			int erase = -1;
			int left_mindist = INT_MAX, right_mindist = INT_MAX, left_loc = -1, right_loc = -1;
			unsigned int shortest_seek;
			bool next = false;
			for(int i=0; i < ready_q.size(); i++){
				shortest_seek = abs(ready_q[i]->gettrack() - current_track);
				if(shortest_seek == 0){
					next = true;
					erase = i;
					ioreturn = ready_q[i];
					break;
				}
				if(ready_q[i]->gettrack() < current_track){
					if(shortest_seek < left_mindist){
						left_min = ready_q[i];
						left_loc = i;
						left_mindist = shortest_seek;
					}
				}else{
				if(shortest_seek < right_mindist){
					right_min = ready_q[i];
					right_loc = i;
					right_mindist = shortest_seek;
				}
			}
		}
		if(!next){
			if(positive && right_min == NULL){
				ioreturn = left_min;
				erase = left_loc;

				positive = !positive;
			}else if(positive && right_min != NULL){
				ioreturn = right_min;
				erase = right_loc;
			}else if(!positive && left_min == NULL){
				ioreturn = right_min;
				erase = right_loc;

				positive = !positive;
			}else{
				ioreturn = left_min;
				erase = left_loc;
			}
		}

		ready_q.erase(ready_q.begin() + erase);

		return ioreturn;		
	}		
		
};

class FSCAN : public Scheduler{
	
	private:
		vector<Event*>* ready_q;
		vector<Event*>* buffer_q;

		bool positive;

	public:
		FSCAN(){
			positive = true;
			buffer_q = new vector<Event*>();
			ready_q = new vector<Event*>();
		}

		void putevent(Event* event){	
			
			int loc = buffer_q->size();
			for(int i = 0; i < buffer_q->size(); i++){
				if(event->gettrack() < buffer_q->at(i)->gettrack()){
					loc = i;
					break;
				}
			}
			buffer_q->insert(buffer_q->begin() + loc, event);

		}
		
		
		Event* getevent(unsigned int current_track){

			if(ready_q->size() == 0 && buffer_q->size() == 0){
			return NULL;
			}
		  
			if(ready_q->size() == 0){
				vector<Event*>* temp = ready_q;
				ready_q = buffer_q;
				buffer_q = temp;
				positive = true;
			}
			Event *minToLeft = NULL, *minToRight = NULL, *ioToReturn = NULL;
			int locToErase = -1;
			int minLeftDistance = INT_MAX, minRightDistance = INT_MAX, leftLoc = -1, rightLoc = -1;
			unsigned int track_distance;
			bool skip = false;
			for(int i=0; i < ready_q->size(); i++){
				track_distance = abs(ready_q->at(i)->gettrack() - current_track);
				if(track_distance == 0){
					skip = true;
					locToErase = i;
					ioToReturn = ready_q->at(i);
					break;
				}

				if(ready_q->at(i)->gettrack() < current_track){
					if(track_distance < minLeftDistance){
						minToLeft = ready_q->at(i);
						leftLoc = i;
						minLeftDistance = track_distance;
					}
				}else{
					if(track_distance < minRightDistance){
						minToRight = ready_q->at(i);
						rightLoc = i;
						minRightDistance = track_distance;
					}
				}
			}

			if(!skip){
				if(positive && minToRight == NULL){
					ioToReturn = minToLeft;
					locToErase = leftLoc;

					positive = !positive;
				}else if(positive && minToRight != NULL){
					ioToReturn = minToRight;
					locToErase = rightLoc;
				}else if(!positive && minToLeft == NULL){
					ioToReturn = minToRight;
					locToErase = rightLoc;

					positive = !positive;
				}else{
					ioToReturn = minToLeft;
					locToErase = leftLoc;
				}
			}

			ready_q->erase(ready_q->begin() + locToErase);

			return ioToReturn;		
		}		
		
};


#endif