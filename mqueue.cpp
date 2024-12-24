/***********************************************************
 ** // UMBC - CMSC 341 - Fall 2024 - Proj3
 ** File:    Swarm.cpp
 ** Project: Fall 2024 - Proj2
 ** Author:  Hazael Magino
 ** Date:    10/30/2022
 ** This file contains the proper implementations for mqueue.cpp
 **********************************************************/
#include "mqueue.h"
// Name: MQueue() - Overloaded Constructor
// Desc: Used to build a new empty MQueue object (for invalid parameters)
// parameters: priFn, heapType,structure
// Preconditions: None
// Postconditions: Creates a new empty MQueue object with assigned praamerters

MQueue::MQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure) {

  // assign values
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_structure = structure;
  m_heap = nullptr;
  m_size = 0;
}

// Name: MQueue() - Destructor
// Desc: The destructor performs the required cleanup including memory
// deallocations and re-initializing. Preconditions: MQueue object must exist
// Postconditions: Dynamic memory is cleared, member variables to default values
MQueue::~MQueue() { clear(); }

// Name: Clear()
// Desc: Deallocates all memory in the tree and makes it an MQueue object empty.
// parameters: NONE
// Preconditions: MQueue must exist
// Postconditions: Dynamic memory is cleared, MQueue is now an empty object
void MQueue::clear() {

  deleteNodes(m_heap);
  m_heap = nullptr;
  m_size = 0;
}

// Name: deleteNodes()
// Desc: Deallocates all memory in the tree and makes it an MQueue object empty.
// parameters: NONE
// Preconditions: MQueue must exist
// Postconditions: Dynamic memory is cleared, MQueue is now an empty object
void MQueue::deleteNodes(Node *aNode) {

  // Post order traversal
  // 1st go to left child
  // 2nd go to right child
  // 3rd go to parents child

  if (aNode != nullptr) { // nullptr is base case
    deleteNodes(aNode->m_left);
    deleteNodes(aNode->m_right);
    delete aNode;
    aNode = nullptr;
  }
  m_heap = nullptr;
  m_size = 0;
}

// Name: MQueue()
// Desc: Copy constructor
// parameters: Rhs
// Preconditions: rhs must satisfy valid conditions to copy
// Postconditions: MQueue is copied
MQueue::MQueue(const MQueue &rhs) {

  if (rhs.m_heap != nullptr) {
    // Copy the heap type from rhs to the object
    m_heapType = rhs.m_heapType;

    // Copy the priority function from rhs to the object
    m_priorFunc = rhs.getPriorityFn();

    // Copy the size from rhs
    m_size = rhs.m_size;

    // Copy the structure type from rhs
    m_structure = rhs.m_structure;

    // Call copyTree to perform a deep copy of the heap
    copyTree(m_heap, rhs.m_heap);
  } else {
    // If rhs.m_heap is nullptr, initialize members appropriately
    m_heap = nullptr;
    m_heapType = rhs.m_heapType;
    m_priorFunc = rhs.getPriorityFn();
    m_size = 0; // If rhs.m_heap is nullptr, size should be 0
    m_structure = rhs.m_structure;
  }
}

// Name: MQueue()
// Desc: Overloaded assignment
// parameters: rhs
// Preconditions: rhs must be satisfy valid conditions to copy
// Postconditions: Mqueue is copied
MQueue &MQueue::operator=(const MQueue &rhs) {

  // protect against self-assignment
  if (this == &rhs) {
    return *this;
  }

  clear();

  // Check if rhs's heap is not null
  if (rhs.m_heap != nullptr) {

    // Copy the heap type from rhs to the object
    m_heapType = rhs.m_heapType;

    // Copy the priority function from the object
    m_priorFunc = rhs.getPriorityFn();

    // Copy the size from rhs
    m_size = rhs.m_size;

    // Copy the structure type from rhs
    m_structure = rhs.m_structure;

    // Call copyTree to perform a deep copy of the heap
    copyTree(m_heap, rhs.m_heap);
  } else {
    m_heap = nullptr;

    m_heapType = rhs.m_heapType;

    m_priorFunc = rhs.getPriorityFn();

    m_size = 0;

    m_structure = rhs.getStructure();
  }

  return *this;
}

// Name: copyTree()
// Desc: helper function for deep copu
// parameters: rhs provided
// Preconditions: rhs must be satisfy valid conditions to copy
// Postconditions: Mqueue is a deep copy
// Name: copyTree()
// Desc: helper function for deep copy
// Parameters: rhs provided
// Preconditions: rhs must satisfy valid conditions to copy
// Postconditions: Mqueue is a deep copy
void MQueue::copyTree(Node *&newNode, const Node *oldNode) {

  // Base case: if the old node is null, set the new node to null and return
  if (oldNode == nullptr) {
    newNode = nullptr;
    return;
  }

  // Create a new node for the current old node
  newNode = new Node(oldNode->getOrder());
  newNode->setNPL(oldNode->getNPL());

  // Recursively copy the left subtree
  if (oldNode->m_left != nullptr) {
    copyTree(newNode->m_left, oldNode->m_left);
  }

  // Recursively copy the right subtree
  if (oldNode->m_right != nullptr) {
    copyTree(newNode->m_right, oldNode->m_right);
  }
}

// Name: mergeWithQueue()
// Desc: Used to merge two heaps together
// parameters: rhs of the heap
// Preconditions: heaps must exist
// Postconditions: Creates a singular heap with heap characteristics dependent
// if it is SKEW or LEFTIST
void MQueue::mergeWithQueue(MQueue &rhs) {

  // Retrieve the priority function from the rhs object
  prifn_t funcRhs = rhs.getPriorityFn();

  // Retrieve the priority function from the current object
  prifn_t funcCurr = getPriorityFn();

  // Retrieve the structure type from the rhs object (SKEW or LEFTIST)
  STRUCTURE rhsStruct = rhs.getStructure();

  // Retrieve the structure type from the current object
  STRUCTURE currStruct = getStructure();

  // Check if the priority functions of the two heaps are different
  if (funcRhs != funcCurr) {

    throw domain_error("Cannot merge with different priority functions");
  } else if (rhsStruct != currStruct) {
    // Check if the structure types of the two heaps are different

    throw domain_error("Cannot merge with different data structures");
  } else if (rhs.m_heap == m_heap) {
    // Check if the rhs heap is the same as the current heap

    throw domain_error("Cannot merge a heap with itself");
  } else {

    // Check if both heaps are non-null
    if (m_heap != nullptr && rhs.m_heap != nullptr) {

      // Merge the two heaps
      m_heap = mergeRecurse(m_heap, rhs.m_heap);
      m_size = m_size + rhs.m_size;
      // rhs is now an unneeded empty heap
      rhs.m_heap = nullptr;
      rhs.m_size = 0;

      // check if only one m_heap is null
    } else if (m_heap == nullptr && rhs.m_heap != nullptr) {

      m_heap = mergeRecurse(nullptr, rhs.m_heap);
      m_size = rhs.m_size;
      rhs.m_heap = nullptr;
      rhs.m_size = 0;

      // check if only m_heap is null
    } else if (m_heap != nullptr && rhs.m_heap == nullptr) {

      // don't need to do anything with this case nothing to merge

    } else {

      m_heap = nullptr;
      m_size = 0;
    }
  }
}

// Name: mergeRecurse()
// Desc: Used as helper function to merge two tree together
// parameters: Node pointer newTree, and Node pointer rhs tree
// Preconditions: heaps must exist
// Postconditions: Creates a singular heap with heap characteristics dependent
// if it is SKEW or LEFTIST
Node *MQueue::mergeRecurse(Node *newHeap, Node *rhsHeap) {

  if (newHeap == nullptr) {
    return rhsHeap;
  }
  if (rhsHeap == nullptr) {
    return newHeap;
  }

  HEAPTYPE type = getHeapType();
  STRUCTURE currStruct = getStructure();
  int newPri = m_priorFunc(newHeap->getOrder());
  int rhsPri = m_priorFunc(rhsHeap->getOrder());

  // Ensure the correct root node based on heap type
  if (((type == MINHEAP) && (newPri > rhsPri)) ||
      ((type == MAXHEAP) && (newPri < rhsPri))) {
    swap(newHeap, rhsHeap);
  }

  Node *nRight = newHeap->m_right;

  // Merge right child with rhsHeap and update left and right children
  newHeap->m_right = mergeRecurse(nRight, rhsHeap);

  if (currStruct == LEFTIST) {
    checkNPL(newHeap);
  } else if (currStruct == SKEW) {
    // Swap children for skew heaps
    swap(newHeap->m_left, newHeap->m_right);
  }

  return newHeap;
}

// Name: swap()
// Desc: Used as helper function to swap children of nodes
// parameters: left child and right child of node
// Preconditions: heaps must exist
// Postconditions: Switches nodes of children in heap
void MQueue::swap(Node *&left, Node *&right) {

  // store left so we don't lose the pointer
  Node *temp = left;

  // switch the pointers
  left = right;
  right = temp;
}

// Name: insertOrder()
// Desc: insert node into a heap
// parameters: priorty of heap node
// Preconditions: heaps must exist
// Postconditions: a new node is inserted into heap
bool MQueue::insertOrder(const Order &order) {

  int newPri = m_priorFunc(order);

  if (newPri < 1) {
    return false;
  }

  Node *newNode = new Node(order);

  m_heap = mergeRecurse(m_heap, newNode);

  m_size++;

  return true;
}

// parameters: Node pointer
// Preconditions: node must be inserted
// Postconditions: npl value exists
void MQueue::checkNPL(Node *node) {
  // Base case for recursion
  if (node == nullptr) {
    return;
  }

  // If the left child is null or the NPL of the right child is greater than the
  // NPL of the left child
  if (node->m_left == nullptr ||
      (node->m_right != nullptr &&
       node->m_right->getNPL() > node->m_left->getNPL())) {
    // Swap the left and right children to maintain the leftist heap property
    swap(node->m_left, node->m_right);
  }

  // If the right child is null
  if (node->m_right == nullptr) {
    node->setNPL(0);
  } else {
    // If the left child's NPL is greater than the right child's NPL
    if (node->m_left != nullptr &&
        node->m_left->getNPL() > node->m_right->getNPL()) {
      // Set the NPL of the current node to the right child's NPL + 1
      node->setNPL(node->m_right->getNPL() + 1);
    } else {
      // Set the NPL of the current node to the left child's NPL + 1
      if (node->m_left != nullptr) {
        node->setNPL(node->m_left->getNPL() + 1);
      } else {
        node->setNPL(0);
      }
    }
  }

  // Recursively check and maintain NPL for the left child
  if (node->m_left != nullptr) {
    checkNPL(node->m_left);
  }

  // Recursively check and maintain NPL for the right child
  if (node->m_right != nullptr) {
    checkNPL(node->m_right);
  }
}

// Name: numOrders()
// Desc: size of m_heap
// parameters: mQueue exists
// Preconditions:
// Postconditions: returns size of of mQueue object
int MQueue::numOrders() const { return m_size; }

// Name: getPriorityFn()
// Desc: priority function
// parameters: mQueue exists
// Preconditions:
// Postconditions: uses new functior detirmining priority
prifn_t MQueue::getPriorityFn() const { return m_priorFunc; }

// Name: getNextOrder()
// Desc: returns highest priorty node
// parameters: mQueue exists
// Preconditions:
// Postconditions: a new heap is created, and highest priorty value is returned
Order MQueue::getNextOrder() {
  // Throw exception if the queue is empty
  if (m_size == 0) {
    throw std::out_of_range("Queue is empty");
  }

  // Store the highest priority order from the root node
  Order highestPriorityOrder = m_heap->getOrder();

  // Debug statement to print the root order details

  // Hold the old root node in a temporary variable
  Node *temp = m_heap;

  // Merge the left and right children to form the new heap
  m_heap = mergeRecurse(m_heap->m_left, m_heap->m_right);



  // Delete the old root node
  delete temp;

  // Decrease the size of the heap
  m_size--;

  // Return the highest priority order
  return highestPriorityOrder;
}

// Name: setPriorityFn()
// Desc: sets a new priorty function and rebuilds a new heap
// parameters: mQueue exists
// Preconditions:
// Postconditions: a new heap is created, and heap type is changed heap is
// rebuilt
void MQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {

  // Check if there is a change in priority function or heap type
  if ((m_priorFunc != priFn) || (m_heapType != heapType)) {

    // Update the priority function to the new one
    m_priorFunc = priFn;

    // Update the heap type to the new one
    m_heapType = heapType;

    // Rebuild the heap to reflect the new priority function and heap type
    m_heap = rebuild(m_heap);
  }
}

// Name: rebuild()
// Desc: Helper function that rebuilds heap
// parameters: mQueue exists
// Preconditions:
// Postconditions: Old heap is changed to new heap type
Node *MQueue::rebuild(Node *oldHeap) {

  // Initialize a new heap
  Node *newHeap = nullptr;

  // make sure oldHeap is not null
  if (oldHeap != nullptr) {

    // Temporary size variable to track the transfer of nodes
    int tempSize = m_size;

    // Loop until all nodes from the original heap have been processed
    while (tempSize != 0) {

      // Store the current root node of the old heap in a temp
      Node *temp = oldHeap;

      // Merge the left and right children of the old root to form a new heap
      oldHeap = mergeRecurse(oldHeap->m_left, oldHeap->m_right);

      // Remove the children
      temp->m_left = nullptr;
      temp->m_right = nullptr;

      // Insert the  node into the new heap
      if (newHeap == nullptr) {
        // If the new heap is empty, initialize it with the extracted node
        newHeap = temp;
      } else {
        // Otherwise, merge the extracted node into the new heap
        newHeap = mergeRecurse(newHeap, temp);
      }

      // Decrement the size counter to ensure the loop terminates
      tempSize--;
    }
  }
  // Return the new heap
  return newHeap;
}

// Name: setStructure()
// Desc: Function that changes the structure of the heap and rebuilds heap
// parameters: mQueue exists
// Preconditions:
// Postconditions: Old heap is changed to new heap type
void MQueue::setStructure(STRUCTURE structure) {

  // make sure structure is different to intiate change
  if (structure != m_structure) {
    m_structure = structure;
  }

  // rebuild heap
  m_heap = rebuild(m_heap);
}

STRUCTURE MQueue::getStructure() const { return m_structure; }
HEAPTYPE MQueue::getHeapType() const { return m_heapType; }

// Name: printOrderQueue()
// Desc: Function that outputs the priorty queue
// parameters: mQueue exists
// Preconditions:
// Postconditions: Node information of priority is displayed
void MQueue::printOrderQueue() const { printHeap(m_heap); }

// Name: printHeap()
// Desc: Helper Function that outputs the priorty queue
// parameters: mQueue exists
// Preconditions:
// Postconditions: Node information of priority is displayed
void MQueue::printHeap(Node *currNode) const {

  if (currNode != nullptr) {
    cout << "[" << m_priorFunc(currNode->getOrder()) << "]" << *currNode
         << endl;
    printHeap(currNode->m_left);
    printHeap(currNode->m_right);
  }
}

void MQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n";
  } else {
    dump(m_heap);
  }
  cout << endl;
}

void MQueue::dump(Node *pos) const {

  if (pos != nullptr) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
      cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer;
    else
      cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer << ":"
           << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream &operator<<(ostream &sout, const Order &order) {
  sout << "Customer: " << order.getCustomer()
       << ", importance: " << order.getImportance()
       << ", quantity: " << order.getQuantity()
       << ", order placement time: " << order.getFIFO();
  return sout;
}

ostream &operator<<(ostream &sout, const Node &node) {
  sout << node.m_order;
  return sout;
}