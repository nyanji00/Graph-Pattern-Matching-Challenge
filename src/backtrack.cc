/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <limits>
#include <algorithm>

using namespace std;

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

vector<Vertex> M;
vector<Vertex> M_search;

void tracking(const Graph &query, const CandidateSet &cs);
vector<Vertex> findNext(const Graph &query, Vertex r);
Vertex calculateCm(Vertex u, std::vector<Vertex> up, const CandidateSet &cs);
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

  // M_search.insert(upper_bound(M_search.begin(), M_search.end(), 161), 161);
  // cout << firstCandidate(query, cs, 3) << endl;
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

void tracking(const Graph &query, const CandidateSet &cs, Vertex r) {

  // get first Cs(r)
  int32_t size_cs_r = cs.GetCandidateSize(r);
  Vertex cs_r[size_cs_r] = {0, };

  for(int i=0; i < size_cs_r; ++i) {
	  cs_r[i] = cs.GetCandidate(r, i);
  }

  // Find where to go
  vector<Vertex> next_set = findNext(query, r);

  // calculate cm(u')
  vector<Vertex>::iterator iter;
  for(iter = next_set.begin(); iter != next_set.end(); iter++) {
	  
  }
}

std::vector<Vertex> findNext(const Graph &query, Vertex r) {
  
}

Vertex calculateCm(Vertex u, std::vector<Vertex> up, const CandidateSet &cs) {
	vector<Vertex> vp_set;
    vector<Vertex>::iterator iter;
	for(iter = up.begin(); iter != up.end(); iter++) {
	  //vp_set.push_back(M[*iter]);
	}
	vector<Vertex> cmu;
}

Vertex findRoot(const Graph &query, const CandidateSet &cs) {
  size_t numVertice = query.GetNumVertices();
  Vertex root = (Vertex)numVertice;
  double minNum = std::numeric_limits<double>::max();

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