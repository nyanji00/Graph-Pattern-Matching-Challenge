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
vector<vector<Vertex>> childs;

vector<vector<Vertex>> Cm;

void calculateCm(const Graph &data, const CandidateSet &cs, Vertex u);
vector<Vertex> findParents(Vertex u);
void initializeParents(Vertex root, size_t numVertice, const Graph &query);
Vertex findRoot(const Graph &query, const CandidateSet &cs);
Vertex firstCandidate(const Graph &query, const CandidateSet &cs, Vertex u);

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

  /* Parents and Childs */
  initializeParents(root, numVertice, query);

  /*
  vector<vector<Vertex>>::iterator iter;

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
  }
  */

  M[16] = 211;
  calculateCm(data, cs, 37);

  vector<Vertex>::iterator iter;
  for(iter = Cm[37].begin(); iter != Cm[37].end(); iter++) {
    cout << *iter << " ";
  }
  cout << endl;
  
  // M_search.insert(upper_bound(M_search.begin(), M_search.end(), 161), 161);
}

void calculateCm(const Graph &data, const CandidateSet &cs, Vertex u) {
  vector<Vertex> parentsList;

  /* Cm can only be calculated when u's all parents are in M. */
  vector<Vertex>::iterator iter;
  for(iter=parents[u].begin(); iter!=parents[u].end(); iter++) {
    if(M[*iter] == -1) return;
    else parentsList.push_back(M[*iter]);
  }

  /*  */
  for(int i=0; i<cs.GetCandidateSize(u); i++) {
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