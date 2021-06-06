/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  std::cout << "t " << query.GetNumVertices() << "\n";

  // 1. Make DAG q_d
  // 1-1. For DAG-Ordering, *Alway choose extendable vertex*
  // 1-2. From M{ (u0, v0) } when CS{ (u0, v0), ... }

  // 1-3. Choose next Candidate vertices... ex) u1 and u2 
  // 1-4. ¤¤> By choosing from next Extendable Candidate... 
  //        ex) N(u0, u1, v1) = {v2, v3}
  // 		ex) N(u0, u2, v1) = {v4, v5, v6}
  // 1-5. Choose what vertex will be next is your choice... (look PDF!)

  // 2. Let D[u, v]=1 for all CS
  
  // 3. Use q_d and q_d^-1 alternatively, calculate D'[u, v] reculsively.

  // 4. Make CS compact

  // 5. Check Subgraph from CS
}
