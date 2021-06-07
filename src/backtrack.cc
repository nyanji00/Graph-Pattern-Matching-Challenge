/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <limits>

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

Vertex findRoot(const Graph &query, const CandidateSet &cs);

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {

  std::cout << "t " << query.GetNumVertices() << "\n";

  Vertex root = findRoot(query, cs);
}

Vertex findRoot(const Graph &query, const CandidateSet &cs) {
  size_t numVertice = query.GetNumVertices();
  Vertex root = (Vertex) numVertice;
  double minNum = std::numeric_limits<double>::max();

  for(Vertex i=0; i<numVertice; i++) {
    size_t candidateSize = cs.GetCandidateSize(i);
    size_t degree = query.GetDegree(i);
    double d = (double)candidateSize / (double)degree;
    if(d < minNum) {
      root = i;
      minNum = d;
    }
  }

  return root;
}