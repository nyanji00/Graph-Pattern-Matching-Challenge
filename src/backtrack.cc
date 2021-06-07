/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <limits>
#include <algorithm>
#include <queue>

using namespace std;

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

vector<Vertex> M;
vector<Vertex> M_search;

vector<vector<Vertex>> parents;

vector<Vertex> findParents(Vertex u);

void initializeParents(Vertex root, size_t numVertice, const Graph &query);
void tracking(const Graph &query, const CandidateSet &cs);
vector<Vertex> findNext(const Graph &query, Vertex r);
Vertex calculateCm(Vertex u, vector<Vertex> up, const CandidateSet &cs);
Vertex findRoot(const Graph &query, const CandidateSet &cs);
Vertex firstCandidate(const Graph &query, const CandidateSet &cs, Vertex u);

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  
  size_t numVertice = query.GetNumVertices();
  cout << "t " << numVertice << "\n";

  /* initialize M */
  vector<Vertex> temp(numVertice, -1);
  M = temp;

  /* find root */
  Vertex root = findRoot(query, cs);

  /* initialize Parents */
  initializeParents(root, numVertice, query);

  /* vector<vector<Vertex>>::iterator iter;
	int i=0;
  for(iter = parents.begin(); iter != parents.end(); iter++) {
	  vector<Vertex> idx = *iter;
		cout << i << " ";
		vector<Vertex>::iterator initer;
		for(initer = idx.begin(); initer != idx.end(); initer++) {
			cout << *initer << " ";
		}
		cout << endl;
		i++;
  } */

	
  
  // M_search.insert(upper_bound(M_search.begin(), M_search.end(), 161), 161);
  // cout << firstCandidate(query, cs, 3) << endl;
}


void tracking(const Graph &query, const CandidateSet &cs) {

}

Vertex firstCandidate(const Graph &query, const CandidateSet &cs, Vertex u) {
  size_t candidateSize = cs.GetCandidateSize(u);

  for(size_t i=0; i<candidateSize; i++) {
    Vertex v = cs.GetCandidate(u, i);
    if(!binary_search(M_search.begin(), M_search.end(), v)) {
      return v;
    }
  }

  return (Vertex)(-1);
}

void initializeParents(Vertex root, size_t numVertice, const Graph &query) {
  parents.resize(numVertice);
  vector<Vertex> childs;
  queue<Vertex> q;
  
  vector<bool> visited(numVertice, false);

  size_t start_offset = query.GetNeighborStartOffset(root);
  size_t end_offset = query.GetNeighborEndOffset(root);
  for(size_t i = start_offset; i < end_offset; i++) {
		childs.push_back(query.GetNeighbor(i));
  }

  visited[root] = true;
  vector<Vertex>::iterator iter;
  for(iter = childs.begin(); iter != childs.end(); iter++) {
    parents[*iter].push_back(root);
		q.push(*iter);
  }
  
  while(!q.empty()) {
    Vertex next = q.front();
    q.pop();

    if(!visited[next]) {
      childs.clear();
      
      visited[next] = true;
      size_t start_offset = query.GetNeighborStartOffset(next);
      size_t end_offset = query.GetNeighborEndOffset(next);

      for(size_t i=start_offset; i < end_offset; i++) {
        Vertex child = query.GetNeighbor(i);
        if (!visited[child]) childs.push_back(child);
      }

      for(iter = childs.begin(); iter != childs.end(); iter++) {
        parents[*iter].push_back(next);
        q.push(*iter);
      }
    }
  }
}

vector<Vertex> findParents(Vertex u) {return parents[u];}
vector<Vertex> findNext(const Graph &query) {
  
}

vector<Vertex> calculateCm(Vertex u, vector<Vertex> up_set, const Graph &data, const Graph &query, const CandidateSet &cs) {
	
	// complete parent vertex set
	vector<Vertex> vp_set;

	vector<Vertex>::iterator iter;
	for(iter = up_set.begin(); iter != up_set.end(); iter++) {
		vp_set[*iter] = M[*iter];
	}
	
	// get possible candidates of u
	size_t size_cs_u = cs.GetCandidateSize(u);
	size_t size_cm_u = 0;
	vector<Vertex> cs_u(size_cs_u);
	vector<Vertex> cm_u;

	for(size_t i = 0; i < size_cs_u; i++) {

	  cs_u[i] = cs.GetCandidate(u, i);

	  for(iter = vp_set.begin(); iter != vp_set.end(); iter++) {

		if( !data.IsNeighbor(cs_u[i], *iter )) {
			continue;
		}

		cm_u[i] = cs_u[i];
	  }
	}

	return cm_u;
}

Vertex findRoot(const Graph &query, const CandidateSet &cs) {
  size_t numVertice = query.GetNumVertices();
  Vertex root = (Vertex)numVertice;
  double minNum = numeric_limits<double>::max();

  for(size_t i=0; i<numVertice; i++) {
    size_t candidateSize = cs.GetCandidateSize(i);
    size_t degree = query.GetDegree(i);
    double d = (double)candidateSize / (double)degree;
    if(d < minNum) {
      root = (Vertex)i;
      minNum = d;
    }
  }

  return root;
}