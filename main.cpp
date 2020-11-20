#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>


using namespace std;

struct Trace{
	string eventID;
	string timestamp;
	string desc;
	string eventletter;
	string resource;
	string cost;

	Trace(){
		eventID = "";
		timestamp = "";
		desc = "";
		eventletter = "";
		resource = "";
		cost = ""; 
	}

};

bool inputData(string filename, vector<vector<Trace>>& events){
	ifstream infile(filename);
	
	cout << "Reading " << filename << endl;
	
	if (!infile.good())
	{
		cout << "**Error: unable to open '" << filename << "'..." << endl;
		return false;
	}

	string line; 
	getline(infile, line);  // input and discard first row --- header row
	int count = 0;
	string oldid = "";
	vector<Trace> traces;
	while(getline(infile, line)){
		count++; 
		// if(count > 2909){
		// 	events.push_back(traces);
		// 	traces.clear();
		// 	return true;
		// }
		//cout << count << endl;
		stringstream s(line);
		string caseid, eventid, timestamp;

		getline(s, caseid, ',');    // first value => caseID
		getline(s, eventid, ',');   // second value => activityID
		getline(s, timestamp, ','); // third value => timestamp

		Trace t; 
		t.eventID = caseid;
		t.eventletter = eventid;
		t.timestamp = timestamp;

		if(count == 1){
			oldid = caseid;
		}

		if(oldid == caseid){
			traces.push_back(t);
		}
		else{
			oldid = caseid;
			events.push_back(traces);
			//cout << "new id" << endl;
			traces.clear();
			traces.push_back(t);
		}
	}
	events.push_back(traces);
    traces.clear();
	return true; //We have data 
}

void eventstats(vector<vector<Trace>>& events){
  ofstream eventstats;
  eventstats.open("eventstats.txt");
  for(int k = 0; k < events.size(); k++){
  	eventstats << "Event: " << events[k][0].eventID << endl;
  	for(int i = 0; i < events[k].size(); i++){
	  	eventstats << " ID: " << events[k][i].eventletter << endl;
	  	eventstats << " Time Stamp: " << events[k][i].timestamp << endl;
  	}
  }
  eventstats.close(); 
}


void eventsname(vector<vector<Trace>>& events){
  ofstream eventnames;
  eventnames.open("eventnames.txt");
  vector<string> uniqueevents;

  for(int i = 0; i < events.size(); i++){
	  for(int k = 0; k < events[i].size(); k++){
		  if(uniqueevents.size() == 0){
			  uniqueevents.push_back(events[i][k].eventletter);
		  }
		  if(count(uniqueevents.begin(), uniqueevents.end(), events[i][k].eventletter) == 0){
			  uniqueevents.push_back(events[i][k].eventletter);
		  }
	  }

  }

  for(int o = 0; o < uniqueevents.size(); o++){
	  eventnames << uniqueevents[o] << endl;
  }
  
  eventnames.close(); 
}

void presuf(vector<vector<Trace>>& events){
  ofstream prefixsuffix;
  prefixsuffix.open("prefixsuffix.txt");
  
  for(int i = 0; i<events.size(); i++){
	  prefixsuffix << "Event: " << events[i][0].eventID << endl;
	  for(int k = 0; k < events[i].size() - 1; k++){
		  prefixsuffix << (k + 1) << " Prefix: " << events[i][k].eventletter << endl;
		  prefixsuffix << (k + 1) << " Suffix: ";
		  for(int q = k+1; q < events[i].size(); q++){
			   prefixsuffix << events[i][q].eventletter << " ";
		  }
		  prefixsuffix << endl << endl;
	  }
  }
  prefixsuffix.close(); 
}


void variants(vector<vector<Trace>>& events){
	map<string, int> variants;
	ofstream vartxt;
	vartxt.open("variants.txt");
	for(int i = 0; i < events.size(); i++){
		string var = "";
		for(int k = 0; k < events[i].size(); k++){
			var = var + events[i][k].eventletter;
		}

		if(variants.count(var) == 0){
			variants.emplace(var, 1);
		}
		else{
			variants[var] = variants[var] + 1;
		}
		
	}
	for(auto key: variants){
		vartxt << "Variant: " << key.first << ", Frequency: " << key.second << endl;
	}
}



int main() {
  string filename = "eventlog.csv";
  vector<vector<Trace>> events;
  

  bool success = inputData(filename, events);

  if (!success) //If file was invalid 
  {
		cout << "No data, file not found?" << endl;
		return 0;
  }  	
  
  int count = 0;
  for(vector<Trace> trace: events){
	  //cout<<"runs events" << endl;
	  count++;
  }
  //Output number of events 
  cout << "Number of events: " << count << endl;
  int traceTime; 

  //Output every trace with all the events, and the time stamps 
  //eventstats(events);
  //eventsname(events);
  //presuf(events);
  variants(events);
}
