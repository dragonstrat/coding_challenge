/*******************************
 * main.cc
 * by Ganghee Jang
 *******************************
 */

#include <iostream>
#include <fstream>
#include <iomanip>

#include "myutilities.hh"
#include "mygraph.hh"
#include "edgeRepository.hh"
#include "degreeStatistics.hh"


using namespace std;

int main (int argc, char* argv[]) {

  // terminate if proper number of arguments are not given.
  if ( argc != 3 ) {
    cout<<"usage: "<< argv[0] <<" <input file name> <output file name>\n";
	return -1;
  }

  string line;
  string actor, target, created_time;
  string actorString("actor");
  string targetString("target");
  string created_timeString("created_time");

  myGraph venmoGraph;
  edgeRepository venmoTransactionBuffer;
  edgeDegreeStatistics nodeDegreeCounters;
  uint64_t time_in_sec, time_venmoBuffer;
  std::pair<int, int> linkUpdateResult;

  std::ofstream medianOutputFile(argv[2]);
  //medianOutputFile.open ("./venmo_output/output.txt", std::ofstream::out | std::ofstream::trunc);

  if (!medianOutputFile.is_open()) {
  	std::cout << "Error opening output.txt file!\n";
	return -1;
  }

  ifstream venmoDataFile (argv[1]);
  if (venmoDataFile.is_open()) {
    while ( getline(venmoDataFile,line) ) {
      created_time = findField(created_timeString, line);
	  actor = findField(actorString, line);
	  target = findField(targetString, line);
	  //time_in_sec = timeToSec(created_time);
	  time_in_sec = timeToSec_v2(created_time);

	  // prepare data to insert
	  venmoTransaction tempTransaction;
	  tempTransaction.actor = actor;
	  tempTransaction.target = target;
	  tempTransaction.created_time = time_in_sec;

	  // check the most recent time
	  time_venmoBuffer = venmoTransactionBuffer.get_repository_time();

      // time update and add link/node from a new transaction
	  if (time_venmoBuffer < time_in_sec) { // new time. add edge to the repository
	    venmoTransactionBuffer.set_current_time(time_in_sec);
		venmoTransactionBuffer.addTransaction(tempTransaction);

		linkUpdateResult = venmoGraph.addLink( actor, target, time_in_sec);
		nodeDegreeCounters.updateNodeCountsOldNNew(linkUpdateResult.first, linkUpdateResult.second);
		linkUpdateResult = venmoGraph.addLink( target, actor, time_in_sec);
		nodeDegreeCounters.updateNodeCountsOldNNew(linkUpdateResult.first, linkUpdateResult.second);

		// now, it is time to update edges - exclude old edges outside of the new window
		venmoTransaction edgeToRemove;
		while(venmoTransactionBuffer.isStale()){
		  edgeToRemove = venmoTransactionBuffer.getTransaction();
		  linkUpdateResult = venmoGraph.deleteEdge( edgeToRemove.actor, edgeToRemove.target, edgeToRemove.created_time);
		  nodeDegreeCounters.updateNodeCountsOldNNew(linkUpdateResult.first, linkUpdateResult.second);
		  linkUpdateResult = venmoGraph.deleteEdge( edgeToRemove.target, edgeToRemove.actor, edgeToRemove.created_time);
		  nodeDegreeCounters.updateNodeCountsOldNNew(linkUpdateResult.first, linkUpdateResult.second);
		}

	  } else {
	     if ((time_venmoBuffer - WINDOW_SIZE) >= time_in_sec) { // drop transaction
	     } else { // take transaction without time update
	       venmoTransactionBuffer.addTransaction(tempTransaction);

		   linkUpdateResult = venmoGraph.addLink( actor, target, time_in_sec);
		   nodeDegreeCounters.updateNodeCountsOldNNew(linkUpdateResult.first, linkUpdateResult.second);
		   linkUpdateResult = venmoGraph.addLink( target, actor, time_in_sec);
		   nodeDegreeCounters.updateNodeCountsOldNNew(linkUpdateResult.first, linkUpdateResult.second);
	     }
	  }

      float new_median = nodeDegreeCounters.getMedian();
	  medianOutputFile << fixed << setprecision(2) << new_median << '\n';
	  
      // following 4 lines are for debug XXXXX
      //cout << "actor = " << actor << "\ttarget = "  << target << "\tcreated_time = " << created_time << '\n';
	  //cout << "converted time in sec = " << time_in_sec<< "\n\n";
	  //cout << "new median = " << new_median << '\n';
	  //nodeDegreeCounters.printEdgeStatistics();
	  
    }

    // following for lines are for debug XXXXX
    //std::cout << "\n\n<Edges in the minHeap>";
    //venmoTransactionBuffer.printEdges();
	//std::cout << "\n\n<Graph>\n";
	//venmoGraph.printNode();
	
    venmoDataFile.close();
  }
  else cout << "Unable to open file"; 

  medianOutputFile.close();

  return 0;
}
