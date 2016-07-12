/*****************************************
 * class to process median of edge degrees.
 * by Ganghee Jang
 *****************************************
 */

#ifndef __DEGREE_STATISTICS__
#define __DEGREE_STATISTICS__

#include <iostream>
#include <vector>

class edgeDegreeStatistics{
  private:
  	struct nodeCount{
	  int num;
	  int nextDegree;
	  int previousDegree;
  	};

	std::vector<nodeCount> edgeStatistics;
	int total_nodes;
  public:
  	edgeDegreeStatistics() {
	  total_nodes = 0;
	  int init_size = 100;
	  edgeStatistics.resize(init_size);
  	}

  	int nodeCountsForEdgeN(int N) {
	  return edgeStatistics[N].num;
  	}

    void updateNodeCountsOnEdgeDegree(int degreeNumber, int newCount){
      assert(degreeNumber != 0);
	  assert(newCount > 0);

	  if (degreeNumber >= edgeStatistics.size()) { // need to increase array size
	    // the following line is for the debug purpose - must be removed XXXXXXXXX
	    std::cout << "\nsize of edgeDegreeStatics object is increased!\n";

		int old_size = edgeStatistics.size();
		if (old_size*2>degreeNumber)
		  edgeStatistics.resize(old_size*2);
		else
		  edgeStatistics.resize(degreeNumber + 1);

		edgeStatistics[degreeNumber].num = newCount;
		total_nodes = total_nodes + newCount;
	  } else {
	    if (edgeStatistics[degreeNumber].num == 0){ // when new node
	      edgeStatistics[degreeNumber].num = newCount;
		  total_nodes = total_nodes + newCount;
	    } else { // already existing node
	      int old_count = edgeStatistics[degreeNumber].num;
		  edgeStatistics[degreeNumber].num = newCount;
		  total_nodes = total_nodes - old_count + newCount;
	    }
	  }
    }

    // oldEdge == 0 when new node
    void updateNodeCountsOldNNew(int oldEdge, int newEdge) {
//	  assert(newEdge != 0);

	  if ((oldEdge >= edgeStatistics.size())||(newEdge >= edgeStatistics.size())) {
	  	// the following line is for the debug purpose - must be removed XXXXXXXXX
	    std::cout << "\nsize of edgeDegreeStatics object is increased!\n";

		int old_size = edgeStatistics.size();
		int bigger;
		if (oldEdge < newEdge) bigger = newEdge;
		else
		  bigger = oldEdge;
		
		if (old_size*2>bigger)
		  edgeStatistics.resize(old_size*2);
		else
		  edgeStatistics.resize(bigger + 1);
	  }
	  
	  if (oldEdge == 0) {
	  	assert(newEdge != 0);
	  	edgeStatistics[newEdge].num++;
		total_nodes++;
	  } else if (newEdge == 0) {
	    assert(oldEdge !=0);
		assert(edgeStatistics[oldEdge].num>0);
		edgeStatistics[oldEdge].num--;
		total_nodes--;
	  } else{
	    assert(edgeStatistics[oldEdge].num>0);
	    edgeStatistics[oldEdge].num--;
		edgeStatistics[newEdge].num++;
	  }
    }

  	float getMedian() {
	  int beforMedian = -1;
	  int pointerMedian = -1;
	  int afterMedian = -1;

	  int halfNodes;
	  if (total_nodes%2 == 0) halfNodes = total_nodes/2;
	  else
	  	halfNodes = total_nodes/2 + 1;
	  
	  int sum = 0;

      if (total_nodes == 0) return 0;
	  
	  for (int i=0; i<edgeStatistics.size(); i++) {
	  	if (edgeStatistics[i].num > 0) {
		  sum = sum + edgeStatistics[i].num;
		  if (sum < halfNodes) {
		  	beforMedian = i;
		  } else if (sum == halfNodes) {
		    if (total_nodes%2 == 0) {
			  pointerMedian = i;
		    } else {
		      pointerMedian = i;
			  break;
		    }
		  } else {
		    if (pointerMedian == -1) {
			  pointerMedian = i;
			  break;
		    } else {
		      afterMedian = i;
			  break;
		    }
		  }
	  	}
	  }

	  if (afterMedian == -1) return (float)pointerMedian;
	  else {
	    return (float)(pointerMedian + afterMedian)/2;
	  }
  	}

    // the following is only for the debug purpose. only print until edge counts until 10.
	void printEdgeStatistics(){
	  std::cout << "total nodes: " << total_nodes << '\n';
	  for(int i=0; i < 10; i++){
	  	std::cout << edgeStatistics[i].num << "   ";
	  }
	  std::cout << '\n';
	}
};

#endif
