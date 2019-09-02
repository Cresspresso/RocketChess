#include "HistoryEventLog.h"

void HistoryEvents::randomEvent(){

	//get a random string from the vector<string>historicalEvents
	//display the string

}

//add to the vector of historical events
void HistoryEvents::addToHistoricalEvents(){

	string ChernobylDisaterFact1 = "The Chernobyl disaster was a nuclear accident that occurred at the No. 4 nuclear reactor in the Chernobyl Nuclear Power Plant in the north of the Ukrainian SSR.";
	string antiNuclearNZ = "New Zealand’s anti-nuclear stance led to the severing of overt military ties with the United States in 1985.";

	historicalEvents.push_back(ChernobylDisaterFact1);
	historicalEvents.push_back(antiNuclearNZ);



}
