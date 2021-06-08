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

vector<vector<Vertex>> parents;
vector<vector<Vertex>> childs;

int cnt = 0;

struct Cm_pair_compare{
	bool operator()(pair<size_t, Vertex> a, pair<size_t, Vertex> b){
		return a.first > b.first;
	}
};

void printVV(vector<vector<Vertex>> VV);
void printV(vector<Vertex> V);
void doInitTrace(Vertex root, const Graph &data, const CandidateSet &cs);
void doTrace(Vertex v, Vertex u, vector<Vertex> M, vector<Vertex> M_search, 
            priority_queue<pair<size_t, Vertex>, 
            vector<pair<size_t, Vertex>>,
            Cm_pair_compare
            > Cm_queue, vector<vector<Vertex>> Cm, const Graph &data, const CandidateSet &cs);
pair<vector<vector<Vertex>>, priority_queue<pair<size_t, Vertex>, 
vector<pair<size_t, Vertex>>, Cm_pair_compare>> calculateChildsCm(vector<Vertex> M, priority_queue<pair<size_t, Vertex>, 
                                                vector<pair<size_t, Vertex>>,
                                                Cm_pair_compare
                                                > Cm_queue, vector<vector<Vertex>> Cm, const Graph &data, const CandidateSet &cs, Vertex u);
pair<vector<Vertex>, priority_queue<pair<size_t, Vertex>, 
vector<pair<size_t, Vertex>>, Cm_pair_compare>> calculateCm(vector<Vertex> M, priority_queue<pair<size_t, Vertex>, 
                                                vector<pair<size_t, Vertex>>,
                                                Cm_pair_compare
                                                > Cm_queue, const Graph &data, const CandidateSet &cs, Vertex u);
vector<Vertex> findParents(Vertex u);
void initializeParents(Vertex root, size_t numVertice, const Graph &query);
Vertex findRoot(const Graph &query, const CandidateSet &cs);

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  
  size_t numVertice = query.GetNumVertices();
  cout << "t " << numVertice << "\n";

  /* initialize M */
  vector<Vertex> M(numVertice, -1);
  vector<Vertex> M_search;

  /* initialize Cm */
  vector<vector<Vertex>> Cm(numVertice, vector<Vertex>(0));

  /* initialize Cm_queue */
  priority_queue<pair<size_t, Vertex>, 
							 vector<pair<size_t, Vertex>>,
							 Cm_pair_compare
							 > Cm_queue;

  /* initialize parents and childs */
  parents.resize(numVertice);
  childs.resize(numVertice);

  /* find root */
  Vertex root = findRoot(query, cs);

  /* find root's Cm */
  pair<vector<Vertex>, priority_queue<pair<size_t, Vertex>, 
  vector<pair<size_t, Vertex>>, Cm_pair_compare>> returnPair = calculateCm(M, Cm_queue, data, cs, root);
  Cm[root] = returnPair.first;

  /* Cm_queue => (, root) */
  Cm_queue = returnPair.second;
	Cm_queue.pop();

  /* Parents and Childs */
  initializeParents(root, numVertice, query);

	/* Start Tracing! */
	for(size_t i = 0; i < Cm[root].size(); i++) {
		Vertex v = Cm[root][i];
		doTrace(v, root, M, M_search, Cm_queue, Cm, data, cs);
	}
}

void doTrace(Vertex v, Vertex u, vector<Vertex> M, vector<Vertex> M_search, 
              priority_queue<pair<size_t, Vertex>, 
							vector<pair<size_t, Vertex>>,
							Cm_pair_compare
							> Cm_queue, vector<vector<Vertex>> Cm, const Graph &data, const CandidateSet &cs) {

  M[u] = v;
  M_search.insert(upper_bound(M_search.begin(), M_search.end(), v), v);

  /* v's child's Cm */
  pair<vector<vector<Vertex>>, priority_queue<pair<size_t, Vertex>, 
  vector<pair<size_t, Vertex>>, Cm_pair_compare>> 
  returnPair = calculateChildsCm(M, Cm_queue, Cm, data, cs, u);
  Cm = returnPair.first;
  Cm_queue = returnPair.second;

  if(!Cm_queue.empty()) {
		Vertex next = Cm_queue.top().second;
		Cm_queue.pop();

		for(size_t i=0; i < Cm[next].size(); i++) {
		  Vertex candidate = Cm[next][i];

			if(binary_search(M_search.begin(), M_search.end(), candidate)) {
				continue;
			}

			doTrace(candidate, next, M, M_search, Cm_queue, Cm, data, cs);
    }
	}

  bool isFull = true;
  for(size_t i =0; i < M.size(); i++) {
    if(M[i] == -1) {
      isFull = false;
      break;
    }
  }

  if(isFull) {
    cout << "a ";
    printV(M_search);
    cnt++;

    if(cnt >= 100000) {
      exit(0);
    }
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

pair<vector<vector<Vertex>>, priority_queue<pair<size_t, Vertex>, 
vector<pair<size_t, Vertex>>, Cm_pair_compare>> calculateChildsCm(vector<Vertex> M, priority_queue<pair<size_t, Vertex>, 
							 vector<pair<size_t, Vertex>>,
							 Cm_pair_compare
							 > Cm_queue, vector<vector<Vertex>> Cm, const Graph &data, const CandidateSet &cs, Vertex u) {

  vector<Vertex>::iterator iter;
  for(iter=childs[u].begin(); iter!=childs[u].end(); iter++) {
    pair<vector<Vertex>, priority_queue<pair<size_t, Vertex>, 
    vector<pair<size_t, Vertex>>, Cm_pair_compare>> returnPair = calculateCm(M, Cm_queue, data, cs, *iter);
    Cm[*iter] = returnPair.first;
    Cm_queue = returnPair.second;
  }

  pair<vector<vector<Vertex>>, priority_queue<pair<size_t, Vertex>, 
      vector<pair<size_t, Vertex>>, Cm_pair_compare>> p = make_pair(Cm, Cm_queue);

  return p;
}

pair<vector<Vertex>, priority_queue<pair<size_t, Vertex>, 
vector<pair<size_t, Vertex>>, Cm_pair_compare>> calculateCm(vector<Vertex> M, priority_queue<pair<size_t, Vertex>, 
							 vector<pair<size_t, Vertex>>,
							 Cm_pair_compare
							 > Cm_queue, const Graph &data, const CandidateSet &cs, Vertex u) {

  vector<Vertex> Cmu;
  vector<Vertex> parentsList;

  /* Cm can only be calculated when u's all parents are in M. */
  vector<Vertex>::iterator iter;
  for(iter=parents[u].begin(); iter!=parents[u].end(); iter++) {
    if(M[*iter] == -1) {
      pair<vector<Vertex>, priority_queue<pair<size_t, Vertex>, 
      vector<pair<size_t, Vertex>>, Cm_pair_compare>> p = make_pair(Cmu, Cm_queue);
      return p;
    }
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

    if(isCm) Cmu.push_back(v);
  }

	pair<size_t, Vertex> pr = make_pair(Cmu.size(), u);
  Cm_queue.push(pr);

  pair<vector<Vertex>, priority_queue<pair<size_t, Vertex>, 
      vector<pair<size_t, Vertex>>, Cm_pair_compare>> p = make_pair(Cmu, Cm_queue);
  return p;
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