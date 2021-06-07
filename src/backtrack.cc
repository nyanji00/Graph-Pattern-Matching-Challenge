/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void tracking(const Graph &query, const CandidateSet &cs);
std::vector<Vertex> findNext(const Graph &query, Vertex r);
Vertex calculateCm(Vertex u, std::vector<Vertex> up, const CandidateSet &cs);

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  std::cout << "t " << query.GetNumVertices() << "\n";

  // Must not use pruning by failing sets
}

void tracking(const Graph &query, const CandidateSet &cs) {
  Vertex r; // = u1
  // r = findRoot(const Graph &query);

  // get first Cm(r)
  int32_t size_cm_r = cs.GetCandidateSize(r);
  Vertex cm_r[size_cm_r] = {0, };
  for(int i=0; i < size_cm_r; ++i) {
	cm_r[i] = cs.GetCandidate(r, i);
  }

  // Find where to go
  std::vector<Vertex> next_set = findNext(query, r);

  // calculate cm(u')
  std::vector<Vertex>::iterator iter;
  for(iter = next_set.begin(); iter != next_set.end(); iter++) {
	  
  }
}

std::vector<Vertex> findNext(const Graph &query, Vertex r) {
  
}

Vertex calculateCm(Vertex u, std::vector<Vertex> up, const CandidateSet &cs) {
	std::vector<Vertex> vp_set;
    std::vector<Vertex>::iterator iter;
	for(iter = up.begin(); iter != up.end(); iter++) {
	  //vp_set.push_back(M[*iter]);
	}
	std::vector<Vertex> cmu;

}