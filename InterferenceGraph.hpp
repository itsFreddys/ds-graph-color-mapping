#ifndef __INTERFERENCE_GRAPH__HPP
#define __INTERFERENCE_GRAPH__HPP

#include <exception>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>

class UnknownVertexException : public std::runtime_error {
public:
    UnknownVertexException(const std::string &v)
            : std::runtime_error("Unknown vertex " + v) {}
};

class UnknownEdgeException : public std::runtime_error {
public:
    UnknownEdgeException(const std::string &v, const std::string &w)
            : std::runtime_error("Unknown edge " + v + " - " + w) {}
};

// InterferenceGraph
//
// This is a class representing an interference graph
// as described in "Part 1: Interference Graph" of the README.md
// file. Though this class is templated, because of the usage of exceptions
// UnknownVertexException and UnknownEdgeException it will
// ONLY be tested with strings.
template <typename T> class InterferenceGraph {
public:
    // Custom type used to represent edges. This is mainly
    // used in the utility function for reading and writing
    // the graph structure to/from files. You don't need to use it.
    using EdgeTy = std::pair<T, T>;

    InterferenceGraph();

    ~InterferenceGraph();

    void addEdge(const T &v, const T &w);

    void addVertex(const T &vertex);

    void removeEdge(const T &v, const T &w);

    void removeVertex(const T &vertex);

    std::unordered_set<T> vertices() const noexcept;

    std::unordered_set<T> neighbors(const T &vertex) const;

    unsigned numVertices() const noexcept;

    unsigned numEdges() const noexcept;

    bool interferes(const T &v, const T &w) const;

    unsigned degree(const T &v) const;

private:
    std::unordered_map<T,std::unordered_set<T>> graph;
    size_t num_vertices;
    size_t num_edges;

    // Private member variables here.
};

template <typename T> InterferenceGraph<T>::InterferenceGraph() : num_vertices(0), num_edges(0) {}

template <typename T> InterferenceGraph<T>::~InterferenceGraph() {}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::neighbors(const T &vertex) const {
    if (graph.count(vertex) == 0) { throw UnknownVertexException(vertex);}
    return graph.at(vertex);
}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::vertices() const noexcept {
    std::unordered_set<T> temp_vert;
    for (const auto& pair : graph) {
        temp_vert.insert(pair.first);
    }
    return temp_vert;
}

template <typename T>
unsigned InterferenceGraph<T>::numVertices() const noexcept {
    return num_vertices;
}

template <typename T> unsigned InterferenceGraph<T>::numEdges() const noexcept {
    return num_edges;
}

template <typename T>
void InterferenceGraph<T>::addEdge(const T &v, const T &w) {
    if (graph.count(v) == 0 || graph.count(w) == 0) {
        throw UnknownVertexException(v);
    }
    // check if there is already an edge. using interferes
    if (interferes(v,w)){ return;}

    // else create edge
    graph[v].insert(w);
    graph[w].insert(v);

    num_edges++;
}

template <typename T>
void InterferenceGraph<T>::removeEdge(const T &v, const T &w) {
    if (!interferes(v,w)) { throw UnknownEdgeException(v,w);}
    graph[v].erase(w);
    graph[w].erase(v);
    num_edges--;
}

template <typename T>
void InterferenceGraph<T>::addVertex(const T &vertex) {
    if (graph.count(vertex) == 1) { return;}
    graph[vertex] = std::unordered_set<T>();
    num_vertices++;
}

template <typename T>
void InterferenceGraph<T>::removeVertex(const T &vertex) {
    if (graph.count(vertex) == 0) { throw UnknownVertexException(vertex);}
    const std::unordered_set<T>& temp_s = graph.at(vertex);
    std::unordered_set<T> *temp_del;

    for (const T& element : temp_s){
        temp_del = &graph.at(element);
        temp_del->erase(vertex);
        num_edges--;
    }
    graph.erase(vertex);
    num_vertices--;
}

template <typename T>
bool InterferenceGraph<T>::interferes(const T &v, const T &w) const {
    if (graph.count(v) == 0 || graph.count(w) == 0) {
        if (graph.count(v) == 0){throw UnknownVertexException(v);}
        else throw UnknownVertexException(w);
        }
    const std::unordered_set<T>& temp_v = graph.at(v);
    const std::unordered_set<T>& temp_w = graph.at(w);
    if (temp_v.count(w) == 1 || temp_w.count(v) == 1){
        return true;
    }
    return false;
}

template <typename T> unsigned InterferenceGraph<T>::degree(const T &v) const {
    if (graph.count(v) == 0) { throw UnknownVertexException(v);}
    const std::unordered_set<T>& temp_s = graph.at(v);
    return temp_s.size();
}

#endif
