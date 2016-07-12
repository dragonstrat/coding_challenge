/*****************************************
 * class to keep the venmo transactions
 * by Ganghee Jang
 *****************************************
 */

#ifndef __EDGE_REPOSITORY__
#define __EDGE_REPOSITORY__

#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <queue>
#include "myutilities.hh"

struct venmoTransaction{
  uint64_t created_time;
  std::string actor;
  std::string target;
};

class edgeRepository{
  private:

	uint64_t current_time;

    class myComparison{
      public:
        bool operator() (const venmoTransaction& lhs, const venmoTransaction&rhs) const
        {
          return (lhs.created_time > rhs.created_time);
        }
    };

	std::priority_queue<venmoTransaction, std::vector<venmoTransaction>, myComparison> minHeap;

  public:
  	edgeRepository(){
	  current_time = 0;
  	}

    // return true when the given time is new and updated successfully
    // return false when the given time is not new and update is not performed
  	bool set_current_time(uint64_t new_time) {
	  if (new_time > current_time) {
	    current_time = new_time;
		return true;
	  } else {
	    return false;
	  }
  	}

	uint64_t get_repository_time() {
	  return current_time;
	}

    void addTransaction(venmoTransaction& newTransaction) {
	  minHeap.push(newTransaction);
    }

    // test function whether we can get transaction or not from repository for the update
    // unsafe. must use isEmpty() and update current_time before use.
    bool isStale(){
      uint64_t window_border = current_time - WINDOW_SIZE;
      if (minHeap.top().created_time <= window_border)
	  	return true;
	  else
	  	return false;
    }

    // this function blindly return and remove min node
    venmoTransaction getTransaction() {
      venmoTransaction tempTransaction;
	  tempTransaction.actor = minHeap.top().actor;
	  tempTransaction.target = minHeap.top().target;
	  tempTransaction.created_time = minHeap.top().created_time;

	  minHeap.pop();
	  return tempTransaction;
    }

    bool isEmpty(){
	  if (minHeap.size() == 0)
	  	return true;
	  else
	    return false;
    }

	void printEdges() {
	  while (!isEmpty()) {
	  	std::cout << "actor = " << minHeap.top().actor << 
			"\ttarget = "  << minHeap.top().target << 
			"\tcreated_time = " << minHeap.top().created_time << '\n';
		minHeap.pop();
	  }
	}
};


#endif
