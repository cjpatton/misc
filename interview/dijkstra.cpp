/* Dijkstra's algorithm! Brushing up on C++ 
 * for my facebook interview. 
 *
 * Chris Patton, 20 Jan 2014
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#define MAX_NEIGHBORS 32

using namespace std;

/** virtual class BaseError
 * Doesn't do much, just wanted to remember 
 * the polymorphism and virtual class stuff.
 */ 
class BaseError 
{
public: 
  BaseError( int n, const char *msg ); 
  virtual ~BaseError();
  virtual const char *get() const = 0;
  virtual int errno() const = 0; 
protected:
  char *message; 
  int   no; 
}; // Pure virtual class BaseError

BaseError::BaseError( int n, const char *msg )
{
  no = n; 
  message = new char [strlen(msg)]; 
  strcpy(message, msg); 
}

BaseError::~BaseError()
{
  delete [] message; 
}

/** class PQError 
 * If I were more sophsiticated, there would be 
 * an enumerated set of errors. 
 */ 
class PQError : public BaseError
{
public:
  PQError( int n, const char *msg ) : BaseError( n, msg )
  { 
  }
  const char *get() const;
  int errno() const; 
}; // class PQError

const char *PQError::get() const 
{
  return message; 
}

int PQError::errno() const 
{
  return no; 
}

/** class NodeError 
 * If I were more sophsiticated, there would be 
 * an enumerated set of errors. 
 */ 
class NodeError : public BaseError
{
public:
  NodeError( int n, const char *msg ) : BaseError( n, msg )
  { 
  }
  const char *get() const;
  int errno() const; 
}; // class NodeError

const char *NodeError::get() const 
{
  return message; 
}

int NodeError::errno() const 
{
  return no; 
}


/* Forward declaration of the main class. */ 
class Graph; 

/** class PriorityQueue 
 * Stores pointers to objects of type T in a binary heap.  
 * When this object goes out of scope, it will deallocate
 * the objects deep_free = true in the constructor. Requires
 * that T has int T::heap_index and 
 * bool T::operator< (const T& obj) const. 
 */ 
template <class T>
class PriorityQueue
{
public: 
  PriorityQueue( int s = 256, bool deep_free = false ); 
  ~PriorityQueue(); 

  void enqueue( T &obj ); 
  void dequeue(); 
  T &top() const;
  bool empty() const; 
  
  /* Check if the key of object at index i has updated and
   * the heap needs to be heapified. */
  void percolate_down( T &obj ); 

  bool _deep_free; 

private:
  int size, ct; 
  T **heap;
  int min_child( int i ); 
  void swap( int i, int j ); 

}; // class PriorityQueue

template<class T>
PriorityQueue<T>::PriorityQueue( int s, bool deep_free ) 
{
  heap = new T* [s]; 
  size = s; 
  ct   = 0; 
  _deep_free = deep_free; 
}

template<class T>
PriorityQueue<T>::~PriorityQueue() 
{
  if (_deep_free)
  {
    for (int i = 0; i < ct; i++) 
      delete heap[i]; 
  }
  delete [] heap; 
}

template<class T>
void PriorityQueue<T>::enqueue( T &obj ) 
{
  int i = ct; 
  obj.heap_index = i; 
  
  if (ct == size)
    throw PQError( 100, "heap is full" ); 

  heap[ct++] = &obj;
  
  while (*heap[i] < *heap[i/2])
  {
    swap(i, i/2); 
    i = i/2;
  }
}

template<class T>
void PriorityQueue<T>::dequeue() 
{
  int i = 0, min;

  if (ct == 0)
    return; 

  else if (ct == 1)
    heap[--ct] = NULL; 

  else
  {
    heap[i] = heap[ct - 1]; 
    heap[--ct] = NULL; 
    min = min_child( i );
    while ((2*i)+1 < ct && *heap[min] < *heap[i]) 
    {
      swap(i, min); 
      i   = min;
      min = min_child( i ); 
    }
  }
}

template<class T>
T &PriorityQueue<T>::top() const
{
  if (ct == 0) 
    throw PQError( 101, "heap is empty" ); 
  else
    return *heap[0];  
}

template<class T>
bool PriorityQueue<T>::empty() const
{
  return (ct == 0); 
}

template<class T>
void PriorityQueue<T>::percolate_down( T &obj )   
/* See if object's key has decreased and needs to be
 * updated. */ 
{
  int i = obj.heap_index; 
  while (*heap[i] < *heap[i/2])
  {
    swap(i, i/2); 
    i = i/2;
  }
}

template<class T>
int PriorityQueue<T>::min_child( int i )
{
  if ((2*i)+1 < ct && *heap[(2*i)+1] < *heap[2*i])
    return (2*i)+1; 
  else 
    return 2*i;  
}

template<class T>
void PriorityQueue<T>::swap( int i, int j )
{
  int tmp_index = heap[i]->heap_index; 
  heap[i]->heap_index = heap[j]->heap_index; 
  heap[j]->heap_index = tmp_index; 

  T *tmp    = heap[i]; 
  heap[i]   = heap[j]; 
  heap[j] = tmp; 
}


/** class Node
 * Holds outgoing edges and their weights, parameters
 * for Dijsktra's shortest path aglorithm, and "int
 * heap_index"  which is used by PriorityQueue.  
 */ 
class Node 
{
friend class Graph; 
friend class PriorityQueue<Node>; 
public:

  Node(); 
  ~Node(); 

  /* In the sense of Disjkstra's shortest-path 
   * algorithm. This is needed by the heap. */ 
  bool operator<( const Node &node ) const; 

  void addEdge( Node *u, int weight );

private:
  int    label;
  Node **adj; 
  int   *adj_w; 
  int    adj_ct;  

  /* Paramaters for Dijsktra's algorithm. */ 
  Node  *dparent;
  int    ddist; 
  bool   dvisited;
  
  /* Index in heap. When shortest path 
   * parameters are updated, we need to 
   * fix heap position. */ 
  int heap_index; 

}; // class Node

Node::Node() 
{
  /* Outgoing neighbor edges. */ 
  adj = new Node* [MAX_NEIGHBORS]; 
  memset(adj, 0, MAX_NEIGHBORS * sizeof(Node*)); 
  adj_w = new int [MAX_NEIGHBORS];
  adj_ct = 0; 

  dparent  = NULL; 
  ddist    = -1; // Initialized when node is queued.
  dvisited = false; 
  heap_index = -1; // Initialized when heaped.  
}

Node::~Node()
{
  delete [] adj;
  delete [] adj_w; 
}

bool Node::operator<( const Node &obj ) const
{
  return ddist < obj.ddist;
}
  
void Node::addEdge( Node *u, int weight )
{
  if (adj_ct == MAX_NEIGHBORS)
    throw NodeError( 102, "node's outgoing edges are at max");
  adj[adj_ct] = u; 
  adj_w[adj_ct] = weight; 
  adj_ct ++; 
}


/** class Graph
 * Top level class. Its methods will implement the various
 * algorithms I want to practice. 
 */
class Graph 
{
public:
  Graph( int size ); 
  ~Graph(); 

  void displayAdjList() const; 
  
  void drawEdge( int u, int v, int weight );
  void drawBidirectionalEdge( int u, int v, int weight ); 
  
  void dijkstra( int s ); 
  void displayShortestPath( int u ) const; 

private:
  Node *nodes; 
  int   node_ct; 

}; // class Graph

Graph::Graph( int size ) 
{
  node_ct = size; 
  nodes = new Node [node_ct]; 
  for (int i = 0; i < node_ct; i++) 
    nodes[i].label = i;  
}

Graph::~Graph()
{
  delete [] nodes;
}

void Graph::drawEdge( int u, int v, int weight )
{
  nodes[u].addEdge( &nodes[v], weight );  
}

void Graph::drawBidirectionalEdge( int u, int v, int weight )
{
  nodes[u].addEdge( &nodes[v], weight );  
  nodes[v].addEdge( &nodes[u], weight );  
}

void Graph::dijkstra( int s )
/* My implementation of Dijkstra's algorithm, refined now 
 * for the third time. Input is the label of the source 
 * node (index in adjacency list.) */ 
{
  int i;  

  /* Reset parameters. */
  for (i = 0; i < node_ct; i++)
  {
    nodes[i].dparent  = NULL; 
    nodes[i].dvisited = false; 
  }

  nodes[s].ddist = 0; 
  nodes[s].dvisited = true; 

  /* Add the source node into the queue */ 
  PriorityQueue<Node> Q( node_ct ); 
  Q.enqueue( nodes[s] ); 

  while (!Q.empty()) 
  {
    Node &u = Q.top(); 
    for (i = 0; i < u.adj_ct; i++)
    {
      Node &v = *u.adj[i]; 
      if (!v.dvisited)
      {
        v.dparent    = &u; 
        v.ddist      = u.ddist + u.adj_w[i]; 
        v.dvisited   = true; 
        Q.enqueue( v ); 
      }

      else if (v.ddist > u.ddist + u.adj_w[i]) 
      {
        v.dparent    = &u; 
        v.ddist      = u.ddist + u.adj_w[i];
        Q.percolate_down( v ); 
      }
    }
    Q.dequeue(); 
  }
}

void Graph::displayShortestPath( int u ) const
{
  Node *node = &nodes[u]; 
  while (node->dparent) 
  {
    printf("%d --> ", node->label); 
    node = node->dparent; 
  }
  printf("%d [w=%d]\n", node->label, nodes[u].ddist);
  /* I noticed a very strnage thing about g++ today. When the line
   * above is changed to node[u].ddist instead of nodes[u].ddist, 
   * not only does it compile, but it doesn't cause a seg fault. 
   * The settings in main() are: srand(198435), num_of_nodes = 8, 
   * graph.dijstra(1), graph.displayShortestPath(3). I compiled
   * and ran this today with gcc version 4.6.3 (Ubuntu/Linaro 
   * 4.6.3-1ubuntu5). Weird. ~ Chris 1/22/14 */
}

void Graph::displayAdjList() const 
{
  char buff[256]; 
  for (int i = 0; i < node_ct; i++) 
  {
    printf("%6d | ", nodes[i].label); 
    for (int j = 0; j < nodes[i].adj_ct; j++)
    {
      sprintf(buff, "%d [w=%d]", nodes[i].adj[j]->label, nodes[i].adj_w[j]);
      printf("%-12s", buff);
    }
    printf("\n"); 
  }
}



int main(int argc, const char **argv) 
{
  try 
  {
    int i, u, v, num_of_nodes = 8; 

    Graph graph ( num_of_nodes ); 
    srand(198435); 
    
    /* Draw a random graph. */
    for (i = 0; i < 2; i++)
    {
      for (u = 0; u < num_of_nodes; u++) 
      {
        v = u; 
        while (u == v)
          v = rand() % num_of_nodes;
        graph.drawBidirectionalEdge( u, v, rand() % 10 ); 
      }
    }

    graph.displayAdjList();
    graph.dijkstra(1);
    graph.displayShortestPath(3); 
      
  } 
  catch (const BaseError &e) 
  {
    fprintf(stderr, "error: [%d] %s.\n", e.errno(), e.get());
  }

  return 0; 
}
