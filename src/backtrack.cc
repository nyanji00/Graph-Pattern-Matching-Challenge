/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <limits>

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

std::vector<Vertex> M = {-1, };

void tracking(const Graph &query, const CandidateSet &cs);

std::vector<Vertex> findNext(const Graph &query, Vertex r);

std::vector<Vertex> calculateCm(Vertex u, std::vector<Vertex> up, const Graph &data, const Graph &query, const CandidateSet &cs);

Vertex findRoot(const Graph &query, const CandidateSet &cs);

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {

  std::cout << "t " << query.GetNumVertices() << "\n";

  Vertex root = findRoot(query, cs);
}

void tracking(const Graph &query, const CandidateSet &cs) {

}

std::vector<Vertex> findNext(const Graph &query) {
  
}

std::vector<Vertex> calculateCm(Vertex u, std::vector<Vertex> up_set, const Graph &data, const Graph &query, const CandidateSet &cs) {
	
	// complete parent vertex set
	std::vector<Vertex> vp_set;

	std::vector<Vertex>::iterator iter;
	for(iter = up_set.begin(); iter != up_set.end(); iter++) {
		vp_set[*iter] = M[*iter];
	}
	
	// get possible candidates of u
	size_t size_cs_u = cs.GetCandidateSize(u);
	size_t size_cm_u = 0;
	std::vector<Vertex> cs_u(size_cs_u);
	std::vector<Vertex> cm_u;

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