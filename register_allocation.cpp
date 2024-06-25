#include "register_allocation.hpp"
#include "CSVReader.hpp"
#include "InterferenceGraph.hpp"
#include <iostream>
#include <queue>
#include <vector>

using namespace RA;

// assignRegisters
//
// This is where you implement the register allocation algorithm
// as mentioned in the README. Remember, you must allocate at MOST
// d(G) + 1 registers where d(G) is the maximum degree of the graph G.
// If num_registers is not enough registers to accomodate the passed in
// graph you should return an empty map. You MUST use registers in the
// range [1, num_registers] inclusive.

struct Vertex{
  std::string name;
  unsigned degree = 0;

  Vertex(){};
  Vertex(std::string ver, unsigned deg) {name = ver; degree = deg;}

  bool operator<(const Vertex& other) const {
    return degree < other.degree;
  }

};

RegisterAssignment RA::assignRegisters(const std::string &path_to_graph,
                                          int num_registers) noexcept {
  InterferenceGraph<Variable> ig = CSVReader::load(path_to_graph);
  std::unordered_map<std::string, int> assigned_mapping;
  std::unordered_set<std::string> checked_vertices;

  // create a priority queue that stores all the vertices from the graph
  std::priority_queue<Vertex> pq;
  std::priority_queue<Vertex> inner_pq;

  // store graph vertices and degrees in priority
  // probably not worth doing maybe create your own
  for (auto& vert: ig.vertices()){
    if (static_cast<int>(ig.degree(vert)) + 1 > num_registers) {return {};}
    pq.push(Vertex(vert, ig.degree(vert)));
    inner_pq.push(Vertex(vert, ig.degree(vert)));
  }

  int reg_count = 0;
  std::unordered_set<std::string> current_neighbors;
  std::unordered_set<std::string> interferes_verts;
  while (!pq.empty()){
    if (reg_count > num_registers){return {};}
    Vertex current_v = pq.top();
    pq.pop();
    inner_pq = pq;
    // check if this vertex has been assigned
    if (checked_vertices.count(current_v.name) == 0){
      // if in here, then vertex has not been assigned.
      reg_count++;
      current_neighbors = ig.neighbors(current_v.name);
      assigned_mapping[current_v.name] = reg_count;
      checked_vertices.insert(current_v.name);
      interferes_verts.clear();
      interferes_verts.insert(current_v.name);

      while(!inner_pq.empty()){
        Vertex inner_v = inner_pq.top();
        inner_pq.pop();
        if (checked_vertices.count(inner_v.name) == 0){
            bool no_adjacent = true;
          for (auto& vert: interferes_verts){
            if (ig.interferes(vert, inner_v.name)){
              no_adjacent = false; break;
            }
          }
          if (no_adjacent){
            interferes_verts.insert(inner_v.name);
            checked_vertices.insert(inner_v.name);
            assigned_mapping[inner_v.name] = reg_count;
          }
       }
      }
    }
  }
  // return a unordered_map -> [string, int]
  return assigned_mapping;
}