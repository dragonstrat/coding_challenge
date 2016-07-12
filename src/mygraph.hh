/*******************************
 * class to represent graph
 * by Ganghee Jang
 *******************************
 */

#ifndef __MY_GRAPH__
#define __MY_GRAPH__

#include <string>
#include <unordered_map>
#include <utility>
#include <cstdint>
#include <list>
#include <iostream>
#include "myutilities.hh"

class myGraph{
  private:
  	struct myEdge{
	  uint64_t time;
	  std::string node;
  	};

	std::unordered_map<std::string, std::list<myEdge> > nodeMap;
    std::unordered_map<std::string, std::list<myEdge> >::iterator updateIterator;

  public:
    myGraph(){
	  // reserve initial size for the hashmap
	  nodeMap.reserve(100);
    }

	~myGraph(){
	}

    // -1 is returned when search is failed
    // return the degree of node when successful search
    int nodeDegree(std::string& nodeName) {
      std::unordered_map<std::string, std::list<myEdge> >::iterator node = nodeMap.find(nodeName);

	  if ( node == nodeMap.end() )
        return -1;
      else
	  	return node->second.size();
    }

    // below addNode() operation is unsafe. (no check for the existing one.
    // must be used with care - check is required before use.
	void addNode(std::string& nodeName, std::string& otherEnd, uint64_t created_time) {
	  std::list<myEdge> tempList;
	  nodeMap.insert(std::make_pair(nodeName, tempList));

	  myEdge tempEdge;
	  tempEdge.node = otherEnd;
	  tempEdge.time = created_time;
	  nodeMap[nodeName].push_back(tempEdge);
	}

    // add link, and then return pair of node degree (old, new)
	std::pair<int, int> addLink(std::string& nodeName, std::string& otherEnd, uint64_t created_time) {
	  std::unordered_map<std::string, std::list<myEdge> >::iterator nodeIterator;
	  std::list<myEdge>::iterator edgeIterator;

      int oldsize;

      nodeIterator = nodeMap.find(nodeName);
	  if (nodeIterator == nodeMap.end() ) { // when node is not found -> add node
	    addNode( nodeName, otherEnd, created_time);
	    return std::make_pair(0, 1);
      } else { // when node is found --> add link!
        oldsize = nodeIterator->second.size();
        assert( oldsize > 0); // node has size of 0 must have been deleted!

		for (edgeIterator = nodeIterator->second.begin(); edgeIterator != nodeIterator->second.end(); ++edgeIterator) {
		  if (edgeIterator->node == otherEnd) break;
		}

		if (edgeIterator == nodeIterator->second.end()) { // edge is not found
		  myEdge tempEdge;
	      tempEdge.node = otherEnd;
	      tempEdge.time = created_time;
		  nodeIterator->second.push_back(tempEdge);
		  return std::make_pair(oldsize, nodeIterator->second.size());
		} else { // edge is found. so update time.
		  edgeIterator->time = created_time;
		  return std::make_pair(oldsize, nodeIterator->second.size());
		}
	  }
	}

	// update links in the node considering window.
	// return degree of node
    void reset_Iter(){
      updateIterator = nodeMap.begin();
	}

    // return -1, -1 when all the nodes are visited for update.
    // return value is pair <old cound, new cound>
	std::pair<int, int> updateNode(uint64_t currentTime){
	  if (updateIterator == nodeMap.end()) return std::make_pair(-1, -1);

	  int old_size = updateIterator->second.size();
	  assert(old_size > 0);

      std::list<myEdge>::iterator iter = updateIterator->second.begin();
	  while (iter != updateIterator->second.end()) {
	  	if (iter->time <= (currentTime - WINDOW_SIZE)) { //edge to be removed
	  	  updateIterator->second.erase(iter);
	  	} else { // OK edge.. check next edge.
	  	  iter++;
	  	}
	  }

	  int new_size = updateIterator->second.size();
	  if (new_size == 0) {
	  	nodeMap.erase(updateIterator++);
	  } else {
	    updateIterator++; //update for the next iteration
	  }
	  return std::make_pair(old_size, new_size);
	}

    // return value is pair <old cound, new cound>
	std::pair<int, int> deleteEdge(std::string& nodeName, std::string& otherEnd, uint64_t current_time){
	  std::unordered_map<std::string, std::list<myEdge> >::iterator nodeIterator;
	  std::list<myEdge>::iterator edgeIterator;

	  nodeIterator = nodeMap.find(nodeName);
	  assert(nodeIterator != nodeMap.end());

	  int old_size = nodeIterator->second.size();
	  assert(old_size > 0);

      edgeIterator = nodeIterator->second.begin();

	  while (edgeIterator != nodeIterator->second.end()) {
	  	if (edgeIterator->node == otherEnd) break;
		edgeIterator++;
	  }

      // consider cases for the multiple request on the same time stamp
	  if (edgeIterator != nodeIterator->second.end()) {
	    if ( edgeIterator->time == current_time) {
	      nodeIterator->second.erase(edgeIterator);
	    }
	  }

	  int new_size = nodeIterator->second.size();
	  if (new_size == 0) {
	  	nodeMap.erase(nodeIterator);
	  }
	  return std::make_pair(old_size, new_size);
	}

    // for test purpose. only works up to 10 nodes.
	void printNode() {
	  std::unordered_map<std::string, std::list<myEdge> >::iterator nodeIterator;
	  std::list<myEdge>::iterator edgeIterator;

	  nodeIterator = nodeMap.begin();
	  while (nodeIterator != nodeMap.end()) {
	  	std::cout << '\n' << nodeIterator->first << "degree: " << nodeIterator->second.size() 
			<< "  edges in the following:" << '\n';
		edgeIterator = nodeIterator->second.begin();
	    while (edgeIterator != nodeIterator->second.end()) {
	  	  std::cout << "( " << edgeIterator->node << ": " << edgeIterator->time << " )   ";
		  edgeIterator++;
	    }
		nodeIterator++;
	  }
    }
};

#endif
