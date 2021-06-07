/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <limits>
#include <algorithm>
#include <queue>
#include <utility>

using namespace std;

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

vector<Vertex> M;
vector<Vertex> M_search;

vector<vector<Vertex>> parents;
vector<vector<Vertex>> childs;

vector<vector<Vertex>> Cm;
struct Cm_pair_compare{
	bool operator()(pair<size_t, Vertex> a, pair<size_t, Vertex> b){
		return a.first > b.first;
	}
};
priority_queue<pair<size_t, Vertex>, 
							 vector<pair<size_t, Vertex>>,
							 Cm_pair_compare
							 > Cm_queue;

void printVV(vector<vector<Vertex>> VV);
void printV(vector<Vertex> V);
void calculateChildsCm(const Graph &data, const CandidateSet &cs, Vertex u);
void calculateCm(const Graph &data, const CandidateSet &cs, Vertex u);
vector<Vertex> findParents(Vertex u);
void initializeParents(Vertex root, size_t numVertice, const Graph &query);
Vertex findRoot(const Graph &query, const CandidateSet &cs);
Vertex firstCandidate(Vertex u);

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  
  size_t numVertice = query.GetNumVertices();
  cout << "t " << numVertice << "\n";

  /* initialize M */
  M = vector<Vertex>(numVertice, -1);

  /* initialize Cm */
  Cm = vector<vector<Vertex>>(numVertice, vector<Vertex>(0));

  /* initialize parents and childs */
  parents.resize(numVertice);
  childs.resize(numVertice);

  /* find root */
  Vertex root = findRoot(query, cs);
  calculateCm(data, cs, root);

  /* Parents and Childs */
  initializeParents(root, numVertice, query);
  printVV(parents);
  printVV(childs);

  Vertex v1 = firstCandidate(root);
  M[root] = v1;
  M_search.insert(upper_bound(M_search.begin(), M_search.end(), v1), v1);

  calculateChildsCm(data, cs, root);
	Cm_queue.pop();

  while(!Cm_queue.empty()) {
    printVV(Cm);

		Vertex next = Cm_queue.top().second;
		cout << "pop! " << next << endl;
		Cm_queue.pop();

		Vertex v = firstCandidate(next);
		M[next] = v;
		M_search.insert(upper_bound(M_search.begin(), M_search.end(), v), v);
		calculateChildsCm(data, cs, next);
	}

	vector<Vertex>::iterator iter;
	for(size_t i =0; i < M.size(); i++) {
		cout << "a " << i << " " << M[i] << endl;
	}

}

void printVV(vector<vector<Vertex>> VV) {
  int i = 0;

  vector<vector<Vertex>>::iterator iter;
  for(iter=VV.begin(); iter!=VV.end(); iter++) {
    cout << i << " ";
    printV(VV[i]);
    i++;
  }
}

void printV(vector<Vertex> V) {
  vector<Vertex>::iterator iter;
  for(iter=V.begin(); iter!=V.end(); iter++) {
    cout << *iter << " ";
  }
  cout << endl;
}

void calculateChildsCm(const Graph &data, const CandidateSet &cs, Vertex u) {
  vector<Vertex>::iterator iter;
  for(iter=childs[u].begin(); iter!=childs[u].end(); iter++) {
    calculateCm(data, cs, *iter);
  }
}

void calculateCm(const Graph &data, const CandidateSet &cs, Vertex u) {
  vector<Vertex> parentsList;

  /* Cm can only be calculated when u's all parents are in M. */
  vector<Vertex>::iterator iter;
  for(iter=parents[u].begin(); iter!=parents[u].end(); iter++) {
    if(M[*iter] == -1) return;
    else parentsList.push_back(M[*iter]);
  }

  /* For one v, must be connected to all parents in M */
  for(size_t i=0; i<cs.GetCandidateSize(u); i++) {
    Vertex v = cs.GetCandidate(u, i);
    bool isCm = true;

    for(iter=parentsList.begin(); iter!=parentsList.end(); iter++) {
      if(!data.IsNeighbor(*iter, v)) {
        isCm = false;
        break;
      }
    }

    if(isCm) Cm[u].push_back(v);
  }

	pair<size_t, Vertex> p = make_pair(Cm[u].size(), u);
  Cm_queue.push(p);
}

vector<Vertex> findParents(Vertex u) {return parents[u];}

void initializeParents(Vertex root, size_t numVertice, const Graph &query) {
  queue<Vertex> q;
  
  vector<bool> visited(numVertice, false);

  size_t start_offset = query.GetNeighborStartOffset(root);
  size_t end_offset = query.GetNeighborEndOffset(root);
  for(size_t i = start_offset; i < end_offset; i++) {
		childs[root].push_back(query.GetNeighbor(i));
  }

  visited[root] = true;
  vector<Vertex>::iterator iter;
  for(iter = childs[root].begin(); iter != childs[root].end(); iter++) {
    parents[*iter].push_back(root);
		q.push(*iter);
  }
  
  while(!q.empty()) {
    Vertex next = q.front();
    q.pop();

    if(!visited[next]) {
      visited[next] = true;
      size_t start_offset = query.GetNeighborStartOffset(next);
      size_t end_offset = query.GetNeighborEndOffset(next);

      for(size_t i=start_offset; i < end_offset; i++) {
        Vertex child = query.GetNeighbor(i);
        if (!visited[child]) childs[next].push_back(child);
      }

      for(iter = childs[next].begin(); iter != childs[next].end(); iter++) {
        parents[*iter].push_back(next);
        q.push(*iter);
      }
    }
  }
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

Vertex firstCandidate(Vertex u) {
  vector<Vertex>::iterator iter;
  for(iter=Cm[u].begin(); iter!=Cm[u].end(); iter++) {
    if(!binary_search(M_search.begin(), M_search.end(), *iter)) {
      return *iter;
    }
  }

  return (Vertex)(-1);
}