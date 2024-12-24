# mqueue-Cplus
This implementation contains the implementation and development of a scheduler software application that can be used by production managers to prioritize the production of orders. This was developed utilizing C++ language.

Objectives:

  -Practice constructing and using heap data structure as a priority queue ADT.
  -Practice writing merge operation in skew and leftist heap data structures.
  -Gain additional experience constructing and using binary trees.
  -Practice using recursion in programs.
  -Learn to use function pointers.

Introduction:

  -To mitigate manufacturing plants need to prioritize their productionThe application automatically prioritizes the production of orders. The prioritization is based on some specific criteria. The algorithm that uses the criteria to calculate the priorities can change. Therefore, the application should has the possibility of changing the order priorities. An algorithm that can determine the priority will be implemented in a function. This desgin implements a priority queue data structure that takes different priority functions. Such an architecture allows us to use different priority functions.

Specification:
  -Priority queue class is implemented based on a skew-heap or a Leftist-heap data structure; it can maintain a min-heap or a max-heap based on the computed priority for every node, where the priority function is provided to the queue constructor via a function pointer. Inserting to and extracting from the skew/leftist heap uses a heap merge function which guarantees that the heap property (min-heap or max-heap) is maintained; the comparisons that are part of the merge process will be made on the computed priorities for the objects in the heap. The queue class allows for the priority function to be changed; in which case the heap must be rebuilt. Moreover, the data structure can switch between a skew heap or a leftist heap at the request of the class user. Such a request will trigger reconstruction of the heap.


This project includes the following files:
  mqueue.h – The interface for the Order, Node, and MQueue classes.
  mqueue.cpp – A skeleton for the implementation of the class queue
  driver.cpp – A sample driver program. It contains sample use of the queue class.
  driver.txt – A sample output produced by driver.cpp.
  mytest.cpp - A test file used to test different menthods and functionality of        implementation.

There are three classes in this project. The class MQueue has a member variable of type Node, and the class Node has a member variable of type Order.

Class Order:
The implementation of this class is provided to you. You are not allowed to modify the class. This class stores the following information about an order:

-m_customer stores the customer's name. The name does not need to be unique.

-m_FIFO stores the order's ordinal position in the list of orders. It shows how soon or how late the customer placed the order. The manufacturer keeps a maximum number of 50 orders in the list at any time. The position 1 indicates that the order was placed before every other order. The 50th position indicates that the order was the last one received.

-m_processTime indicates how long it takes to complete the production of this order. The production can take between 1 to 12 months.

-m_dueTime indicates when the order will be ready for delivery. It might be between 1 and 12 months.

-m_slackTime is the remaining time until the due date minus the required time to complete the production. It might be between 1 and 12 months.

-m_importance stores the importance of the customer to the manufacturer. The customers are rated form 1 to 100 percent. A value of 100 percent indicates the most important customer.

-m_workForce stores the number of available personnel on the production line. This number varies between 100 and 200.

-m_quantity stores the quantity of the order. The production line accepts order of 1000 to 10000 units.

Priority Functions:

int priorityFn1(const Order & order);

This function determines a priority value for the argument and returns the priority value. The algorithm in this function uses the information in the Order class. In this algorithm a greater value means a higher priority. To use this function we need to build a max-heap. If the function cannot calculate a valid priority value it returns zero.


int priorityFn2(const Order & order);

This function returns the priority value for the argument and returns the priority value. The algorithm in this function uses the information in the Order class. In this algorithm a smaller value means a higher priority. To use this function we need to build a min-heap. If the function cannot calculate a valid priority value it returns zero.

Class Node:
This class constructs a node in the heap data structure. It has a member variable of the type Order. The member variable is initialized through the Node constructor. The class MQueue is a friend of Node class, it means it has access to private members of Node class. You are not allowed to modify this class.

Class Mqueue:
The class MQueue constructs a skew or a leftist data structure of the type min-heap or max-heap. This class has a member variable called m_heap. The member variable m_heap presents the root node of the heap data structure and it is of the type Node. The following table presents the list of member functions that need implementation.

MQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure);
This is the constructor. It must be provided with a pointer to the prioritization function, the type of heap, and the desired data structure.

MQueue::~MQueue();
The destructor deallocates the memory and re-initializes the member variables.

MQueue::MQueue(const MQueue& rhs);
The copy constructor must make a deep copy of the rhs object. This function creates an exact same copy of rhs.

MQueue& MQueue::operator=(const MQueue& rhs);
The assignment operator creates an exact same copy of rhs.

bool MQueue::insertOrder(const Order& input);
Inserts an order into the queue. It must maintain the min-heap or the max-heap property depending on the settings. Moreover, if the selected data structure is leftist heap, it needs to maintain a correct value of Null Path Length (NPL) in the node. If the order does not provide a valid priority value the object is not inserted and the function returns false, otherwise the function returns true.

Order MQueue::getNextOrder();
This function extracts (removes the node) and returns the highest priority order from the queue. It must maintain the min-heap or max-heap property. The function throws an out_of_range exception if the queue is empty when the function is called.

void MQueue::mergeWithQueue(MQueue& rhs);
This function merges the host queue with the rhs; it leaves rhs empty; it transfers all nodes from rhs to the current heap. Two heaps can only be merged if they have the same priority function and they are of the same data structure. If the user attempts to merge queues with different priority functions, or two different data structures a domain_error exception is thrown. This function requires protection against self-merging. Merging a heap with itself should not allowed.

void MQueue::clear();
This function clears the queue. It must delete all the nodes in the heap, leaving the heap empty. Moreover, it re-initializes the member variables.

int MQueue::numOrders() const;
It returns the current number of orders in the queue.

void MQueue::printOrderQueue() const;
It prints the contents of the queue using preorder traversal. Although the first order printed should have the highest priority, the remaining orders will not necessarily be in priority order.

prifn_t MQueue::getPriorityFn() const;
This function returns the current priority function.

void MQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType);
This function sets a new priority function and its corresponding heap type (min-heap or max-heap). It must rebuild the heap!

HEAPTYPE MQueue::getHeapType() const;
This function returns the heap type, i.e. it is either MINHEAP or MAXHEAP.

STRUCTURE MQueue::getStructure() const;
This function returns the structure of the current heap, i.e. it is either SKEW or LEFTIST.

void MQueue::setStructure(STRUCTURE structure);
This function sets the data structure, i.e. it is either SKEW or LEFTIST. It must rebuild a heap with the new structure using the nodes in the current data structure.
Note: rebuild means transferring nodes not recreating nodes.

void MQueue::dump();
This function prints out the nodes information in an in-order traversal. For every node, it prints the priority followed by the order's name of the node; and in the case of a leftist heap the output is followed by the value of NPL for the node. 
The tree viewer tool shows a graphical representation of the output of the dump function. You can copy and paste the dump() output in the viewer. This tool facilitates debugging. 

Tree Viewer tool: 
https://swe.umbc.edu/~donyaee/res/tree_viewer.html
