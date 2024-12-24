// CMSC 341 - Fall 2024 - Project 3
#include "mqueue.h"
#include <algorithm>
#include <math.h>
#include <random>
#include <vector>

const int DEF_MINVALUE = -100000000;
const int DEF_MAXVALUE = 1000000000;

using namespace std;
enum RANDOM { UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE };
class Random {
public:
  Random() {}
  Random(int min, int max, RANDOM type = UNIFORMINT, int mean = 50,
         int stdev = 20)
      : m_min(min), m_max(max), m_type(type) {
    if (type == NORMAL) {
      // the case of NORMAL to generate integer numbers with normal distribution
      m_generator = std::mt19937(m_device());
      // the data set will have the mean of 50 (default) and standard deviation
      // of 20 (default) the mean and standard deviation can change by passing
      // new values to constructor
      m_normdist = std::normal_distribution<>(mean, stdev);
    } else if (type == UNIFORMINT) {
      // the case of UNIFORMINT to generate integer numbers
      //  Using a fixed seed value generates always the same sequence
      //  of pseudorandom numbers, e.g. reproducing scientific experiments
      //  here it helps us with testing since the same sequence repeats
      m_generator = std::mt19937(10); // 10 is the fixed seed value
      m_unidist = std::uniform_int_distribution<>(min, max);
    } else if (type == UNIFORMREAL) { // the case of UNIFORMREAL to generate
                                      // real numbers
      m_generator = std::mt19937(10); // 10 is the fixed seed value
      m_uniReal =
          std::uniform_real_distribution<double>((double)min, (double)max);
    } else { // the case of SHUFFLE to generate every number only once
      m_generator = std::mt19937(m_device());
    }
  }
  void setSeed(int seedNum) {
    // we have set a default value for seed in constructor
    // we can change the seed by calling this function after constructor call
    // this gives us more randomness
    m_generator = std::mt19937(seedNum);
  }
  void init(int min, int max) {
    m_min = min;
    m_max = max;
    m_type = UNIFORMINT;
    m_generator = std::mt19937(10); // 10 is the fixed seed value
    m_unidist = std::uniform_int_distribution<>(min, max);
  }
  void getShuffle(vector<int> &array) {
    // this function provides a list of all values between min and max
    // in a random order, this function guarantees the uniqueness
    // of every value in the list
    // the user program creates the vector param and passes here
    // here we populate the vector using m_min and m_max
    for (int i = m_min; i <= m_max; i++) {
      array.push_back(i);
    }
    shuffle(array.begin(), array.end(), m_generator);
  }

  void getShuffle(int array[]) {
    // this function provides a list of all values between min and max
    // in a random order, this function guarantees the uniqueness
    // of every value in the list
    // the param array must be of the size (m_max-m_min+1)
    // the user program creates the array and pass it here
    vector<int> temp;
    for (int i = m_min; i <= m_max; i++) {
      temp.push_back(i);
    }
    std::shuffle(temp.begin(), temp.end(), m_generator);
    vector<int>::iterator it;
    int i = 0;
    for (it = temp.begin(); it != temp.end(); it++) {
      array[i] = *it;
      i++;
    }
  }

  int getRandNum() {
    // this function returns integer numbers
    // the object must have been initialized to generate integers
    int result = 0;
    if (m_type == NORMAL) {
      // returns a random number in a set with normal distribution
      // we limit random numbers by the min and max values
      result = m_min - 1;
      while (result < m_min || result > m_max)
        result = m_normdist(m_generator);
    } else if (m_type == UNIFORMINT) {
      // this will generate a random number between min and max values
      result = m_unidist(m_generator);
    }
    return result;
  }

  double getRealRandNum() {
    // this function returns real numbers
    // the object must have been initialized to generate real numbers
    double result = m_uniReal(m_generator);
    // a trick to return numbers only with two deciaml points
    // for example if result is 15.0378, function returns 15.03
    // to round up we can use ceil function instead of floor
    result = std::floor(result * 100.0) / 100.0;
    return result;
  }

  string getRandString(int size) {
    // the parameter size specifies the length of string we ask for
    // to use ASCII char the number range in constructor must be set to 97 - 122
    // and the Random type must be UNIFORMINT (it is default in constructor)
    string output = "";
    for (int i = 0; i < size; i++) {
      output = output + (char)getRandNum();
    }
    return output;
  }

  int getMin() { return m_min; }
  int getMax() { return m_max; }

private:
  int m_min;
  int m_max;
  RANDOM m_type;
  std::random_device m_device;
  std::mt19937 m_generator;
  std::normal_distribution<> m_normdist;     // normal distribution
  std::uniform_int_distribution<> m_unidist; // integer uniform distribution
  std::uniform_real_distribution<double> m_uniReal; // real uniform distribution
};

int priorityFn1(const Order &order);
int priorityFn2(const Order &order);
/* Priority functions */
int priorityFn1(const Order &order) {
  // this function works with a MAXHEAP
  // priority value is determined based on some criteria
  // priority value falls in the range [1101-10400]
  int minValue = MIN1000 + MAX100 + MINONE + MINZERO;
  int maxValue = MAX100 + MAX100 + MAX200 + MAX10000;
  // the larger value means the higher priority
  int priority = order.getMaterial() + order.getImportance() +
                 order.getWorkForce() + order.getQuantity();
  if (priority >= minValue && priority <= maxValue)
    return priority;
  else
    return 0; // this is an invalid order object
}

int priorityFn2(const Order &order) {
  // this function works with a MINHEAP
  // priority value is determined based on some criteria
  // priority value falls in the range [4-86]
  int minValue = MINONE + MINONE + MINONE + MINONE;
  int maxValue = MAX12 + MAX12 + MAX12 + MAX50;
  // the smaller value means the higher priority
  int priority = order.getFIFO() + order.getProcessTime() + order.getDueTime() +
                 order.getSlackTime();
  if (priority >= minValue && priority <= maxValue)
    return priority;
  else
    return 0; // this is an invalid order object
}

class Tester {
public:
  Tester() {
    m_highestOrder = DEF_MINVALUE; // set purposely to be changed
    m_lowestOrder = DEF_MAXVALUE;  // set purposely to be changed
  }

  MQueue generateTree(int size, HEAPTYPE type, STRUCTURE structr,
                      int prior); // generates a heap based off given parameters
  bool testIsMinHeap(
      const Node *node,
      const MQueue *queue); // helper function to aid in testing MIN heap
  bool testIsMAXHeap(
      const Node *node,
      const MQueue *queue); // helper function to aid in testing MAX heap
  bool testIsSkewHeap(const Node *node,
                      const MQueue *queue); // helper function to aid in
                                            // testingEFF skew heap properties
  bool testIsLeftHeap(const Node *node,
                      const MQueue *queue); // helper function to aid in testing
                                            // leftist heap properties
  bool testCorrectNpl(const Node *start);   // helper function to aid in testing
                                            // npl value of leftist heap
  bool
  validateHeap(const MQueue &aHeap, int expectedSize,
               HEAPTYPE type); // function to test validity of heap properties
  void resetVariables(); // helper function to aid in reseting tester member
                         // variables
  int getMAXInserted() const; // return highest order
  int getMINInserted() const; // return lowest order
  bool testNormalInsertMax(int size, HEAPTYPE type, STRUCTURE struc,
                           int prior); // function to test max heap insertion
  bool testNormalInsertMin(int size, HEAPTYPE type, STRUCTURE struc,
                           int prior); // function to test min heap insertion
  bool testRemovalMin(int size, HEAPTYPE type, STRUCTURE struc,
                      int prior); // function to test removals in min heap
  bool testRemovalMax(int size, HEAPTYPE type, STRUCTURE struc,
                      int prior); // function to test removals in max heap
  bool testChangePriorFunc(
      int size, HEAPTYPE type, STRUCTURE struc,
      int prior); // function to test the change of priority functions
  bool testMergeEmptyQueueEdge(const MQueue &queue);
  bool testMergeNormalRhs(Node *start, const MQueue *queue);
  bool testCopyNormal(const MQueue &queue, int size);
  bool testCopyEdge(const MQueue &queue, int size);
  bool testOverloadedAssNormal(const MQueue &queue, int size);
  bool testOverloadedAssEdge(const MQueue &queue, int size);
  bool testDequeEmpty();
  bool testMergeDiffPrior();
  bool testStressLargeInsert(int size, HEAPTYPE type, STRUCTURE struc,
                             int prior);
  bool testAssignWithDiffSize();
  void updateHighestAndLowest(Node *prior, const MQueue *queue);
  bool testInsertInvalidOrder();

private:
  int m_highestOrder;
  int m_lowestOrder;
};

void Tester::resetVariables() {
  m_highestOrder = DEF_MINVALUE;
  m_lowestOrder = DEF_MAXVALUE;
}

// Helper Function: updateHighestAndLowest(Node *aNode, const MQueue *queue)
// Purpose: Update the highest and lowest priority values in the heap.
// Description: Traverse the heap to find and update the highest and lowest
// priority values.
//              This ensures the member variables m_highestOrder and
//              m_lowestOrder are accurate after operations that modify the
//              heap.
// Expected Result:
// 1. The highest priority value in the heap is correctly stored in
// m_highestOrder.
// 2. The lowest priority value in the heap is correctly stored in
// m_lowestOrder.
// 3. The function correctly traverses the entire heap and updates the values
// dynamically.
// 4. The function handles null nodes gracefully and updates values only for
// valid nodes.
void Tester::updateHighestAndLowest(Node *aNode, const MQueue *queue) {

  if (aNode == nullptr) {
    return;
  }

  int currentPriority = queue->m_priorFunc(aNode->getOrder());

  // Update the highest order
  if (currentPriority > m_highestOrder) {
    m_highestOrder = currentPriority;
  }

  // Update the lowest order
  if (currentPriority < m_lowestOrder) {
    m_lowestOrder = currentPriority;
  }

  // Recursively update the left and right subtrees
  updateHighestAndLowest(aNode->m_left, queue);
  updateHighestAndLowest(aNode->m_right, queue);
}

// Function: generateTree(int size, HEAPTYPE type, STRUCTURE struc, int prior)
// Purpose: Generate a tree (heap) with specified parameters.
// Case: Tree Generation
// Description: Creates a heap of the given size, type (min-heap or max-heap),
// and structure (skew or leftist).
//              It uses the priority function defined by `prior` to assign
//              priorities to the nodes. The generated heap is validated to
//              ensure it adheres to the specified properties.
// Parameters:
//  - size: The number of nodes in the heap.
//  - type: The type of heap (MINHEAP or MAXHEAP).
//  - struc: The structure type of the heap (SKEW or LEFTIST).
//  - prior: The priority function to use for assigning priorities to the nodes.
// Expected Result:
// 1. Tree is generated
MQueue Tester::generateTree(int size, HEAPTYPE type, STRUCTURE struc,
                            int prior) {

  Random nameGen(97, 122);
  Random FIFO(MINONE, MAX50);
  Random processTime(MINONE, MAX12);
  Random dueTime(MINONE, MAX12);
  Random slackTime(MINONE, MAX12);
  Random material(MINZERO, MAX100);
  Random importance(MINONE, MAX100);
  Random workers(MAX100, MAX200);
  Random quantity(MIN1000, MAX10000);

  prifn_t func = priorityFn1;
  if (prior == 2) {
    func = priorityFn2;
  }

  MQueue queue1(func, type, struc);
  for (int i = 0; i < size; i++) {
    Order order(
        nameGen.getRandString(5), FIFO.getRandNum(), processTime.getRandNum(),
        dueTime.getRandNum(), slackTime.getRandNum(), material.getRandNum(),
        importance.getRandNum(), workers.getRandNum(), quantity.getRandNum());
    queue1.insertOrder(order);
  }
  updateHighestAndLowest(queue1.m_heap, &queue1);

  return queue1;
}
// Function: testIsMinHeap(const Node *node, const MQueue *queue)
// Purpose: Validate if the given heap maintains the min-heap property.
// Case: Min-Heap Validation
// Description: This function checks whether the specified heap adheres to the
// min-heap property,
//              where each parent node's priority is less than or equal to its
//              children's priorities. It recursively traverses the heap to
//              validate this property at each node.
// Parameters:
//  - node: The current node being validated.
//  - queue: The heap (MQueue) being validated.
// Expected Result:
// 1. The function returns true if the heap maintains the min-heap property.
// 2. The function returns false if any node in the heap violates the min-heap
// property.
// 3. The function correctly traverses the entire heap, validating each node's
// priority against its children.
// 4. The function handles null nodes gracefully, considering them as valid in
// the min-heap context.

bool Tester::testIsMinHeap(const Node *node, const MQueue *queue) {

  // Base case: if the node is null, it is a valid subtree
  if (node == nullptr) {
    return true;
  }

  // Check heap property with the left child
  if (node->m_left != nullptr &&
      queue->m_priorFunc(node->getOrder()) >
          queue->m_priorFunc(node->m_left->getOrder())) {
    return false;
  }

  // Check heap property with the right child
  if (node->m_right != nullptr &&
      queue->m_priorFunc(node->getOrder()) >
          queue->m_priorFunc(node->m_right->getOrder())) {
    return false;
  }

  // Recursively validate left and right subtrees
  return testIsMinHeap(node->m_left, queue) &&
         testIsMinHeap(node->m_right, queue);
}

// Function: testIsMAXHeap(const Node *node, const MQueue *queue)
// Purpose: Validate if the given heap maintains the max-heap property.
// Case: Max-Heap Validation
// Description: This function checks whether the specified heap adheres to the
// max-heap property,
//              where each parent node's priority is greater than or equal to
//              its children's priorities. It recursively traverses the heap to
//              validate this property at each node.
// Parameters:
//  - node: The current node being validated.
//  - queue: The heap (MQueue) being validated.
// Expected Result:
// 1. The function returns true if the heap maintains the max-heap property.
// 2. The function returns false if any node in the heap violates the max-heap
// property.
// 3. The function correctly traverses the entire heap, validating each node's
// priority against its children.
// 4. The function handles null nodes gracefully, considering them as valid in
// the max-heap context.
bool Tester::testIsMAXHeap(const Node *node, const MQueue *queue) {
  // Base case: if the node is null, it is a valid subtree
  if (node == nullptr) {
    return true;
  }

  // Check heap property with the left child
  if (node->m_left != nullptr &&
      queue->m_priorFunc(node->getOrder()) <
          queue->m_priorFunc(node->m_left->getOrder())) {
    return false;
  }

  // Check heap property with the right child
  if (node->m_right != nullptr &&
      queue->m_priorFunc(node->getOrder()) <
          queue->m_priorFunc(node->m_right->getOrder())) {
    return false;
  }

  // Recursively validate left and right subtrees
  return testIsMAXHeap(node->m_left, queue) &&
         testIsMAXHeap(node->m_right, queue);
}

// Function: testIsSkewHeap(const Node *node, const MQueue *queue)
// Purpose: Validate if the given heap maintains the skew heap property.
// Case: Skew Heap Validation
// Description: This function checks whether the specified heap adheres to the
// skew heap property.
//              It recursively traverses the heap to validate that each node and
//              its subtrees maintain the skew heap structure.
// Parameters:
//  - node: The current node being validated.
//  - queue: The heap (MQueue) being validated.
// Expected Result:
// 1. The function returns true if the heap maintains the skew heap property.
// 2. The function returns false if any node in the heap violates the skew heap
// property.
// 3. The function correctly traverses the entire heap, validating the structure
// at each node.
bool Tester::testIsSkewHeap(const Node *node, const MQueue *queue) {
  if (node == nullptr) {
    return true;
  }

  bool leftCheck = true, rightCheck = true;

  // Determine if we are working with a min-heap or max-heap
  bool isMinHeap = (queue->getHeapType() == MINHEAP);

  // Check heap property with the left child
  if (node->m_left != nullptr) {
    if (isMinHeap) {
      if (queue->m_priorFunc(node->getOrder()) >
          queue->m_priorFunc(node->m_left->getOrder())) {
        return false;
      }
    } else {
      if (queue->m_priorFunc(node->getOrder()) <
          queue->m_priorFunc(node->m_left->getOrder())) {

        return false;
      }
    }
    leftCheck = testIsSkewHeap(node->m_left, queue);
  }

  // Check heap property with the right child
  if (node->m_right != nullptr) {
    if (isMinHeap) {
      if (queue->m_priorFunc(node->getOrder()) >
          queue->m_priorFunc(node->m_right->getOrder())) {

        return false;
      }
    } else {
      if (queue->m_priorFunc(node->getOrder()) <
          queue->m_priorFunc(node->m_right->getOrder())) {

        return false;
      }
    }
    rightCheck = testIsSkewHeap(node->m_right, queue);
  }

  return leftCheck && rightCheck;
}

// Function: testIsLeftHeap(const Node *node, const MQueue *queue)
// Purpose: Validate if the given heap maintains the leftist heap property.
// Case: Leftist Heap Validation
// Description: This function checks whether the specified heap adheres to the
// leftist heap property,
//              which involves validating the Null Path Length (NPL) property at
//              each node. It recursively traverses the heap to ensure that the
//              NPL of each node is accurate and that the overall structure
//              satisfies the leftist heap conditions.
// Parameters:
//  - node: The current node being validated.
//  - queue: The heap (MQueue) being validated.
// Expected Result:
// 1. The function returns true if the heap maintains the leftist heap property.
// 2. The function returns false if any node in the heap violates the leftist
// heap property.
// 3. The function correctly traverses the entire heap, validating each node's
// NPL and structure.
bool Tester::testIsLeftHeap(const Node *node, const MQueue *queue) {

  // Base case: if the node is null, it is trivially a leftist heap
  if (node == nullptr) {
    return true;
  }

  // Validate the NPL property of the current node
  if (!testCorrectNpl(node)) {
    return false;
  }

  // Recursively validate the left and right subtrees
  bool leftSubtreeValid = testIsLeftHeap(node->m_left, queue);
  bool rightSubtreeValid = testIsLeftHeap(node->m_right, queue);

  // The node is a valid leftist heap if its NPL is valid and both subtrees are
  // valid leftist heaps
  return leftSubtreeValid && rightSubtreeValid;
}

// Function: testCorrectNpl(const Node *node)
// Purpose: Validate if the given node maintains the correct Null Path Length
// (NPL). Case: NPL Validation Description: This function checks whether the
// specified node adheres to the Null Path Length (NPL) property of a leftist
// heap.
//              It calculates the expected NPL based on its children and
//              compares it to the actual NPL of the node.
// Parameters:
//  - node: The current node being validated.
// Expected Result:
// 1. The function returns true if the node maintains the correct NPL property.
// 2. The function returns false if the node's NPL is incorrect.
// 3. The function correctly calculates the expected NPL based on the left and
// right children's NPL values.
bool Tester::testCorrectNpl(const Node *node) {
  // Base case: if the node is null, the NPL is trivially correct
  if (node == nullptr) {
    return true;
  }

  // Calculate NPL of left and right children
  int leftNPL;
  if (node->m_left != nullptr) {
    leftNPL = node->m_left->getNPL();
  } else {
    leftNPL = -1;
  }

  int rightNPL;
  if (node->m_right != nullptr) {
    rightNPL = node->m_right->getNPL();
  } else {
    rightNPL = -1;
  }

  // Calculate the expected NPL of the current node
  int expectedNPL;
  if (leftNPL < rightNPL) {
    expectedNPL = leftNPL + 1;
  } else {
    expectedNPL = rightNPL + 1;
  }

  // Check if the current node's NPL matches the expected NPL
  if (node->getNPL() != expectedNPL) {
    return false;
  }

  // If the current node's NPL is correct, return true
  return true;
}

// Expected Result:
// 1. The function returns true if the highest order of the heap
int Tester::getMAXInserted() const { return m_highestOrder; }

// Expected Result:
// 1. The function returns true if the lowest order of the heap
int Tester::getMINInserted() const { return m_lowestOrder; }

// Function: testNormalInsertMax(int min, int max, int size, HEAPTYPE type,
// STRUCTURE struc, int prior)

// Case(Normal Insert): Attempt to insert a range of orders into a max heap
// Expected Result:
// 1. The heap maintains the max heap property.
// 2. The heap size matches the expected size.
// 3. The highest priority value is correctly identified as the root node.
// 4. The struct type of heap is a valid struct type
bool Tester::testNormalInsertMax(int size, HEAPTYPE type, STRUCTURE struc,
                                 int prior) {

  MQueue aHeap = generateTree(size, type, struc, prior);
  // Check if the heap maintains the max heap property
  bool isMaxHeap = testIsMAXHeap(aHeap.m_heap, &aHeap);

  // Verify if the heap size matches the expected size
  bool isCorrectSize = false;
  if (aHeap.m_size == size) {
    isCorrectSize = true;
  }
  int rootPriority = aHeap.m_priorFunc(aHeap.m_heap->getOrder());

  // Verify if the heap priority is correct
  bool isValidPriority = false;
  if (rootPriority == getMAXInserted()) {
    isValidPriority = true;
  }

  bool isValidType = false;

  // Verify the structure type of the heap
  if (aHeap.m_structure == SKEW) {
    isValidType = testIsSkewHeap(aHeap.m_heap, &aHeap);
  } else {
    isValidType = testIsLeftHeap(aHeap.m_heap, &aHeap);
  }

  if (isMaxHeap && isCorrectSize && isValidPriority && isValidType) {
    return true;
  } else {
    return false;
  }
}

// Function: validateHeap(const MQueue &aHeap, int expectedSize, HEAPTYPE type)
// Purpose: Validate if the given heap maintains the specified properties and
// structure. Case: Heap Validation Description: This function checks whether
// the specified heap adheres to its properties,
//              which includes validating if it's a min-heap or max-heap,
//              checking the heap size, verifying the priority values, and
//              ensuring the structural integrity (skew or leftist).
// Parameters:
//  - aHeap: The heap (MQueue) being validated.
//  - expectedSize: The expected number of nodes in the heap.
//  - type: The type of heap (MINHEAP or MAXHEAP) to validate.
// Expected Result:
// 1. The function returns true if the heap maintains the specified properties.
// 2. The function returns false if any of the properties (heap property, size,
// priority, structure) are violated.
// 3. The function correctly traverses the entire heap to validate its
// properties and structure.
// 4. The function handles various heap types (min-heap, max-heap) and
// structures (skew, leftist) appropriately.
bool Tester::validateHeap(const MQueue &aHeap, int expectedSize,
                          HEAPTYPE type) {
  // Debugging statement

  if (aHeap.m_heap == nullptr) {
    return true;
  }

  bool isMinOrMaxHeap = false;
  int rootPriority = aHeap.m_priorFunc(aHeap.m_heap->getOrder());
  bool isValidPriority = false;

  // Test for valid type heap and priority of heap
  if (type == MINHEAP) {
    isMinOrMaxHeap = testIsMinHeap(aHeap.m_heap, &aHeap);
    isValidPriority = (rootPriority == getMINInserted());
  } else {
    isMinOrMaxHeap = testIsMAXHeap(aHeap.m_heap, &aHeap);
    isValidPriority = (rootPriority == getMAXInserted());
  }

  // Verify if the heap size matches the expected size
  bool isCorrectSize = (aHeap.m_size == expectedSize);

  bool isValidType = false;
  // Verify the structure type of the heap
  if (aHeap.m_structure == SKEW) {
    isValidType = testIsSkewHeap(aHeap.m_heap, &aHeap);
  } else {
    isValidType = testIsLeftHeap(aHeap.m_heap, &aHeap);
  }

  // Debugging statements to trace validation steps
  // cout << "Debug: isMinOrMaxHeap: " << isMinOrMaxHeap << std::endl;
  // cout << "Debug: isCorrectSize: " << isCorrectSize << " (Expected: "
  //           << expectedSize << ", Actual: " << aHeap.m_size << ")" <<
  //           endl;
  // cout << "Debug: isValidPriority: " << isValidPriority
  //           << " (Root Priority: " << rootPriority << ")" << endl;
  // cout << "Debug: isValidType: " << isValidType << " (Structure: "
  //           << (aHeap.m_structure == SKEW ? "SKEW" : "LEFTIST") << ")" <<
  //           endl;

  return isMinOrMaxHeap && isCorrectSize && isValidPriority && isValidType;
}

// Function: testNormalInsertMin(int size, HEAPTYPE type, STRUCTURE struc, int
// prior) Purpose: Test the insertion of nodes into a min-heap and validate its
// properties. Case: Normal Insert into Min-Heap Description: This function
// generates a heap of the specified size, type, and structure.
//              It then inserts nodes into the heap using the provided priority
//              function. After the insertions, the function validates whether
//              the heap maintains the min-heap property, has the correct size,
//              and adheres to the expected structure.
// Parameters:
//  - size: The number of nodes to insert into the heap.
//  - type: The type of heap (should be MINHEAP for this test).
//  - struc: The structure type of the heap (SKEW or LEFTIST).
//  - prior: The priority function to use for inserting nodes into the heap.
// Expected Result:
// 1. The heap is generated with the correct number of nodes (matching the
// specified size).
// 2. The heap maintains the min-heap property, where each parent node's
// priority is less than or equal to its children's priorities.
// 3. The heap size matches the expected size after insertions.
// 4. The structure of the heap is consistent with the specified type (skew or
// leftist).
// 5. The heap's highest and lowest priority values are correctly updated and
// maintained.
bool Tester::testNormalInsertMin(int size, HEAPTYPE type, STRUCTURE struc,
                                 int prior) {
  MQueue aHeap = generateTree(size, type, struc, prior);

  // Check if the heap maintains the min heap property
  bool isMinHeap = testIsMinHeap(aHeap.m_heap, &aHeap);

  // Verify if the heap size matches the expected size
  bool isCorrectSize = false;
  if (aHeap.m_size == size) {
    isCorrectSize = true;
  }

  int rootPriority = aHeap.m_priorFunc(aHeap.m_heap->getOrder());
  bool isValidPriority = false;
  // Check if the root node's priority matches the lowest inserted priority
  if (rootPriority == getMINInserted()) {
    isValidPriority = true;
  }

  bool isValidType = false;
  // Verify the structure type of the heap
  if (aHeap.m_structure == SKEW) {
    isValidType = testIsSkewHeap(aHeap.m_heap, &aHeap);
  } else {
    isValidType = testIsLeftHeap(aHeap.m_heap, &aHeap);
  }

  if (isMinHeap && isCorrectSize && isValidPriority && isValidType) {
    return true;
  } else {
    return false;
  }
}
// Function: testRemovalMin(int size, HEAPTYPE type, STRUCTURE struc, int prior)
// Purpose: Test the removal of nodes from a min-heap and validate its
// properties. Case: Min-Heap Removal Description: This function generates a
// min-heap and removes nodes to validate that
//              the heap maintains its properties after each removal.
// Parameters:
//  - size: The initial number of nodes in the heap.
//  - type: The type of heap (MINHEAP).
//  - struc: The structure type of the heap (SKEW or LEFTIST).
//  - prior: The priority function to use for the heap.
// Expected Result:
// 1. The heap maintains the min-heap property after each removal.
// 2. The heap size decreases correctly with each removal.
// 3. The removals are performed in the correct order based on priority.
// 4. The structure of the heap remains consistent with the specified type.
bool Tester::testRemovalMin(int size, HEAPTYPE type, STRUCTURE struc,
                            int prior) {
  // Generate a heap with the specified parameters
  MQueue aHeap = generateTree(size, type, struc, prior);

  // Variable to track if the removals maintain the correct order
  bool isCorrectOrderRemoval = true;
  // Number of removals to perform (half of the initial size)
  int removals = size / 2;

  try {
    // Get the first order (root) from the heap
    Order previousOrder = aHeap.getNextOrder();

    for (int i = 1; i < removals; ++i) {
      // Get the next order from the heap
      Order currentOrder = aHeap.getNextOrder();

      // Update the lowest order if necessary
      if (aHeap.m_priorFunc(currentOrder) == m_lowestOrder) {
        m_lowestOrder = aHeap.m_priorFunc(aHeap.m_heap->getOrder());
      }
      // Update the highest order if necessary
      else if (aHeap.m_priorFunc(currentOrder) == m_highestOrder) {
        m_highestOrder = aHeap.m_priorFunc(aHeap.m_heap->getOrder());
      }

      // Check if the current order priority is less than the previous one
      if (aHeap.m_priorFunc(currentOrder) < aHeap.m_priorFunc(previousOrder)) {
        isCorrectOrderRemoval = false;
      }
      // Update the previous order to the current one
      previousOrder = currentOrder;
    }
  } catch (const out_of_range &e) {
    // Handle the case where there are no elements to remove
    isCorrectOrderRemoval = false;
  }

  // Validate the heap if the order removals were correct
  if (isCorrectOrderRemoval) {
    return validateHeap(aHeap, size - removals, type);
  } else {
    return false;
  }
}

// Function: testRemovalMax(int size, HEAPTYPE type, STRUCTURE struc, int prior)
// Purpose: Test the removal of nodes from a max-heap and validate its
// properties. Case: Max-Heap Removal Description: This function generates a
// max-heap and removes nodes to validate that
//              the heap maintains its properties after each removal.
// Parameters:
//  - size: The initial number of nodes in the heap.
//  - type: The type of heap (MAXHEAP).
//  - struc: The structure type of the heap (SKEW or LEFTIST).
//  - prior: The priority function to use for the heap.
// Expected Result:
// 1. The heap maintains the max-heap property after each removal.
// 2. The heap size decreases correctly with each removal.
// 3. The removals are performed in the correct order based on priority.
// 4. The structure of the heap remains consistent with the specified type.
bool Tester::testRemovalMax(int size, HEAPTYPE type, STRUCTURE struc,
                            int prior) {
  // Generate a heap with the specified parameters
  MQueue aHeap = generateTree(size, type, struc, prior);

  // Variable to track if the removals maintain the correct order
  bool isCorrectOrderRemoval = true;
  // Number of removals to perform (half of the initial size)
  int removals = size / 2;

  try {
    // Get the first order (root) from the heap
    Order previousOrder = aHeap.getNextOrder();

    for (int i = 1; i < removals; ++i) {
      // Get the next order from the heap
      Order currentOrder = aHeap.getNextOrder();

      // Update the lowest order if necessary
      if (aHeap.m_priorFunc(currentOrder) != m_lowestOrder) {
        m_lowestOrder = aHeap.m_priorFunc(aHeap.m_heap->getOrder());
      }
      // Update the highest order if necessary
      else if (aHeap.m_priorFunc(currentOrder) != m_highestOrder) {
        m_highestOrder = aHeap.m_priorFunc(aHeap.m_heap->getOrder());
      }

      // Check if the current order priority is greater than the previous one
      if (aHeap.m_priorFunc(currentOrder) > aHeap.m_priorFunc(previousOrder)) {
        isCorrectOrderRemoval = false;
      }
      // Update the previous order to the current one
      previousOrder = currentOrder;
    }
  } catch (const out_of_range &e) {
    // Handle the case where there are no more elements to remove
    isCorrectOrderRemoval = false;
  }

  // Validate the heap if the order removals were correct
  if (isCorrectOrderRemoval) {
    return validateHeap(aHeap, size - removals, type);
  } else {
    return false;
  }
}

// Function: testChangePriorFunc(int size, HEAPTYPE type, STRUCTURE struc, int
// prior)
// Purpose: Test the change of priority function and validate heap
// properties after the change. Case: Change Priority Function Description: This
// function generates a heap with the specified parameters and then changes its
// priority function and type.
//              After the changes, it validates whether the heap maintains the
//              correct properties, including heap order, size, root priority,
//              and structure type.
// Parameters:
//  - size: The number of nodes in the heap.
//  - type: The initial type of heap (MINHEAP or MAXHEAP).
//  - struc: The structure type of the heap (SKEW or LEFTIST).
//  - prior: The initial priority function to use for the heap.
// Expected Result:
// 1. The heap correctly rebuilds with the new priority function and type.
// 2. The heap maintains the correct order (min-heap or max-heap) after the
// priority function change.
// 3. The heap size matches the expected size after the rebuild.
// 4. The root node has the correct priority according to the new priority
// function.
// 5. The structure of the heap remains consistent with the specified type.
bool Tester::testChangePriorFunc(int size, HEAPTYPE type, STRUCTURE struc,
                                 int prior) {
  // Generate an initial heap with the specified parameters
  MQueue aHeap = generateTree(size, type, struc, prior);
  // Display the initial heap's properties for debugging
  updateHighestAndLowest(
      aHeap.m_heap,
      &aHeap); // Update the highest and lowest with the new priority function

  bool isInitialValid = validateHeap(aHeap, size, type);
  // Set a new priority function and heap type
  prifn_t newFunc = aHeap.getPriorityFn();

  // change function
  if (prior == 1) {
    newFunc = priorityFn2;
  } else {
    newFunc = priorityFn1;
  }

  HEAPTYPE newType;
  // change type
  if (type == MINHEAP) {
    newType = MAXHEAP;
  } else {
    newType = MINHEAP;
  }

  // Rebuild the heap with the new priority function and heap type
  aHeap.setPriorityFn(newFunc, newType);

  // Validate the rebuilt heap
  resetVariables();
  updateHighestAndLowest(
      aHeap.m_heap,
      &aHeap); // Update the highest and lowest with the new priority function

  // Use validateHeap function to ensure the heap maintains its properties
  bool isValid = validateHeap(aHeap, size, newType);
  return isValid && isInitialValid;
}

// Function: testMergeEmptyQueueEdge(const MQueue &queue)
// Purpose: Test merging an empty queue with another queue and validate the
//          resulting queue properties.
// Case: Merge Empty Queue
// Description: This function creates an empty heap and attempts to merge it
// with
//              another queue. It validates whether the resulting queue
//              maintains the correct properties, including heap order, size,
//              and structure type.
// Parameters:
//  &Queue - queue object passed in
// Expected Result:
// 1. The resulting queue correctly integrates the elements from the non-empty
// queue.
// 2. The resulting queue maintains the correct order (min-heap or max-heap)
// after the merge.
// 3. The resulting queue size matches the expected size after the merge.
// 4. The structure of the resulting queue remains consistent with the specified
// type.
bool Tester::testMergeEmptyQueueEdge(const MQueue &queue) {

  // Create a copy of the input heap
  MQueue newHeap = queue;

  // initialize an empty heap
  prifn_t func = queue.getPriorityFn();
  MQueue emptyHeap(func, queue.getHeapType(), queue.getStructure());

  // merge the non empty heap with an empty heap
  newHeap.mergeWithQueue(emptyHeap);

  // Validate that the resultingg heap has the same properties as the original
  // heap
  bool isValid = validateHeap(newHeap, newHeap.m_size, newHeap.getHeapType());

  // Check if the size of the newHeap is correct after merge
  bool sizeCheck = (newHeap.m_size == queue.m_size);

  // Check if the structure and type remain consistent

  bool isValidType = (newHeap.getHeapType() == queue.getHeapType());

  // Return the result of the validation
  return isValid && sizeCheck && isValidType;
}

// Function: testOverloadedAssNormal(const MQueue &queue, int size)
// Purpose: Test the overloaded assignment operator (operator=) under normal
// conditions.
//          Validate that the assigned queue maintains the correct properties
//          after the assignment.
// Case: Overloaded Assignment Operator Normal Condition
// Description: This function creates a heap, assigns the provided queue to it
// using the overloaded
//              assignment operator, and then validates whether the assigned
//              heap maintains the correct properties, including heap order,
//              size, root priority, and structure type.
// Parameters:
//  - queue: A reference to the queue that will be assigned to another queue.
//  - size: The expected size of the assigned queue after the assignment.
// Expected Result:
// 1. The assigned queue correctly reflects the properties of the source queue.
// 2. The assigned queue maintains the correct order (min-heap or max-heap)
// after the assignment.
// 3. The assigned queue size matches the expected size.
// 4. The root node of the assigned queue has the correct priority according to
// the heap's priority function.
// 5. The structure of the assigned queue remains consistent with the specified
// type.
bool Tester::testOverloadedAssNormal(const MQueue &queue, int size) {

  // Random generators for different properties of Order
  Random nameGen(97, 122);
  Random FIFO(MINONE, MAX50);
  Random processTime(MINONE, MAX12);
  Random dueTime(MINONE, MAX12);
  Random slackTime(MINONE, MAX12);
  Random material(MINZERO, MAX100);
  Random importance(MINONE, MAX100);
  Random workers(MAX100, MAX200);
  Random quantity(MIN1000, MAX10000);

  // Create a new heap using the assignment operator
  MQueue newHeap = queue;

  // Check if new heap has the same properties as the original queue
  bool isValid = validateHeap(newHeap, queue.m_size, queue.m_heapType);

  // Number of new insertions to be made to the new heap
  int newInsert = queue.m_size / 2;

  // Insert new elements into newHeap and check its properties
  for (int i = 0; i < newInsert; i++) {
    Order order(
        nameGen.getRandString(5), FIFO.getRandNum(), processTime.getRandNum(),
        dueTime.getRandNum(), slackTime.getRandNum(), material.getRandNum(),
        importance.getRandNum(), workers.getRandNum(), quantity.getRandNum());
    newHeap.insertOrder(order);
  }

  // Check if the size of newHeap is correct after insertions
  bool checkSize = (newHeap.m_size == queue.m_size + newInsert);

  // Validate the properties of newHeap after insertions
  bool isValidInsert =
      validateHeap(newHeap, newHeap.m_size, newHeap.m_heapType);

  // Check if the original queue size remains unchanged
  bool checkOriginal = (queue.m_size == size);

  return isValid && checkSize && checkOriginal && isValidInsert;
}

// Function: testOverloadedAssEdge(const MQueue &queue, int size)
// Purpose: Test the overloaded assignment operator (operator=) under edge
// conditions.
//          Validate that the assigned queue maintains the correct properties
//          after the assignment.
// Case: Overloaded Assignment Operator Edge Condition
// Description: This function creates a heap, assigns the provided queue to it
// using the overloaded
//              assignment operator, and then validates whether the assigned
//              heap maintains the correct properties, including heap order,
//              size, root priority, and structure type, under edge conditions
//              (e.g., empty queues, single-element queues).
// Parameters:
//  - queue: A reference to the queue that will be assigned to another queue.
//  - size: The expected size of the assigned queue after the assignment.
// Expected Result:
// 1. The assigned queue correctly reflects the properties of the source queue.
// 2. The assigned queue maintains the correct order (min-heap or max-heap)
// after the assignment.
// 3. The assigned queue size matches the expected size.
// 4. The root node of the assigned queue has the correct priority according to
// the heap's priority function.
// 5. The structure of the assigned queue remains consistent with the specified
// type, even under edge conditions.
bool Tester::testOverloadedAssEdge(const MQueue &queue, int size) {

  // Random generators for different properties of Order
  Random nameGen(97, 122);
  Random FIFO(MINONE, MAX50);
  Random processTime(MINONE, MAX12);
  Random dueTime(MINONE, MAX12);
  Random slackTime(MINONE, MAX12);
  Random material(MINZERO, MAX100);
  Random importance(MINONE, MAX100);
  Random workers(MAX100, MAX200);
  Random quantity(MIN1000, MAX10000);

  // Create an empty heap
  prifn_t func = queue.getPriorityFn();
  MQueue emptyHeap(func, queue.getHeapType(), queue.getStructure());

  // Assign the non-empty heap to empty heap
  emptyHeap = queue;

  // validate the empty heap has the same properiteis as original heap
  bool isValid = validateHeap(emptyHeap, queue.m_size, queue.getHeapType());

  // validate the sizes are the same
  bool sizeCheck = (emptyHeap.m_size == queue.m_size);

  // Number of new insertions to be made to the emptyHeap
  int newInsert = queue.m_size / 2;

  // Insert new elements into emptyHeap and check its properties and see if
  // modificaiotns affect orignal heap
  for (int i = 0; i < newInsert; i++) {
    Order order(
        nameGen.getRandString(5), FIFO.getRandNum(), processTime.getRandNum(),
        dueTime.getRandNum(), slackTime.getRandNum(), material.getRandNum(),
        importance.getRandNum(), workers.getRandNum(), quantity.getRandNum());
    emptyHeap.insertOrder(order);
  }

  // Check that the size of the emptyHeap is incremented
  bool newSizeCheck = (emptyHeap.m_size == queue.m_size + newInsert);

  // Check the original queu's size is still the same
  bool originalSizeCheck = (queue.m_size == size);

  return isValid && sizeCheck && newSizeCheck && originalSizeCheck;
}

// Function: testCopyNormal(const MQueue &queue, int size)
// Purpose: Test the copy constructor under normal conditions. Validate that the
// copied queue
//          maintains the correct properties after the copy operation.
// Case: Copy Constructor Normal Condition
// Description: This function creates a heap, copies the provided queue to it
// using the copy constructor,
//              and then validates whether the copied heap maintains the correct
//              properties, including heap order, size, root priority, and
//              structure type.
// Parameters:
//  - queue: A reference to the queue that will be copied to another queue.
//  - size: The expected size of the copied queue after the copy operation.
// Expected Result:
// 1. The copied queue correctly reflects the properties of the source queue.
// 2. The copied queue maintains the correct order (min-heap or max-heap) after
// the copy operation.
// 3. The copied queue size matches the expected size.
// 4. The root node of the copied queue has the correct priority according to
// the heap's priority function.
// 5. The structure of the copied queue remains consistent with the specified
// type.
bool Tester::testCopyNormal(const MQueue &queue, int size) {
  // Random generators for different properties of Order
  Random nameGen(97, 122);
  Random FIFO(MINONE, MAX50);
  Random processTime(MINONE, MAX12);
  Random dueTime(MINONE, MAX12);
  Random slackTime(MINONE, MAX12);
  Random material(MINZERO, MAX100);
  Random importance(MINONE, MAX100);
  Random workers(MAX100, MAX200);
  Random quantity(MIN1000, MAX10000);

  // Create new heap using copy constructor
  MQueue newHeap(queue);

  // Validate the new heap has the same properties as the original queue
  bool isValid = validateHeap(newHeap, queue.m_size, queue.getHeapType());

  // Ensure sizes are the same
  bool sizeCheck = (newHeap.m_size == queue.m_size);

  // Ensure structure types are the same
  bool structureCheck = (newHeap.getStructure() == queue.getStructure());

  // Number of new insertions to be made to the newHeap
  int newInsert = queue.m_size / 2;

  // Insert new elements into newHeap and check its properties and see if
  // modifications affect original heap
  for (int i = 0; i < newInsert; i++) {
    Order order(
        nameGen.getRandString(5), FIFO.getRandNum(), processTime.getRandNum(),
        dueTime.getRandNum(), slackTime.getRandNum(), material.getRandNum(),
        importance.getRandNum(), workers.getRandNum(), quantity.getRandNum());
    newHeap.insertOrder(order);
  }

  // Check that the size of newHeap is incremented
  bool newSizeCheck = (newHeap.m_size == queue.m_size + newInsert);

  // Check that original queue's size remains unchanged (deep copy)
  bool originalSizeCheck = (queue.m_size == size);

  return isValid && sizeCheck && structureCheck && newSizeCheck &&
         originalSizeCheck;
}

// Function: testCopyEdge(const MQueue &queue, int size)
// Purpose: Test the copy constructor under edge conditions. Validate that the
// copied queue
//          maintains the correct properties after the copy operation.
// Case: Copy Constructor Edge Condition
// Description: This function creates a heap, copies the provided queue to it
// using the copy constructor,
//              and then validates whether the copied heap maintains the correct
//              properties, including heap order, size, root priority, and
//              structure type, under edge conditions such as empty queues.
// Parameters:
//  - queue: A reference to the queue that will be copied to another queue.
//  - size: The expected size of the copied queue after the copy operation.
// Expected Result:
// 1. The copied queue correctly reflects the properties of the source queue.
// 2. The copied queue maintains the correct order (min-heap or max-heap) after
// the copy operation.
// 3. The copied queue size matches the expected size.
// 4. The root node of the copied queue has the correct priority according to
// the heap's priority function.
// 5. The structure of the copied queue remains consistent with the specified
// type, even under edge conditions.
bool Tester::testCopyEdge(const MQueue &queue, int size) {

  // Random generators for different properties of Order
  Random nameGen(97, 122);
  Random FIFO(MINONE, MAX50);
  Random processTime(MINONE, MAX12);
  Random dueTime(MINONE, MAX12);
  Random slackTime(MINONE, MAX12);
  Random material(MINZERO, MAX100);
  Random importance(MINONE, MAX100);
  Random workers(MAX100, MAX200);
  Random quantity(MIN1000, MAX10000);

  // Create new heap using copy constructor
  MQueue newHeap(queue);

  // Validate the new heap has the same properties as the original queue
  bool isValid = validateHeap(newHeap, queue.m_size, queue.getHeapType());

  // Ensure sizes are the same
  bool sizeCheck = (newHeap.m_size == queue.m_size);

  // Ensure structure types are the same
  bool structureCheck = (newHeap.getStructure() == queue.getStructure());

  // Number of new insertions to be made to the newHeap
  int newInsert = queue.m_size / 2;

  // Insert new elements into newHeap and check its properties and see if
  // modifications affect original heap
  for (int i = 0; i < newInsert; i++) {
    Order order(
        nameGen.getRandString(5), FIFO.getRandNum(), processTime.getRandNum(),
        dueTime.getRandNum(), slackTime.getRandNum(), material.getRandNum(),
        importance.getRandNum(), workers.getRandNum(), quantity.getRandNum());
    newHeap.insertOrder(order);
  }

  bool isNewHeapValid =
      validateHeap(newHeap, newHeap.m_size + newInsert, newHeap.getHeapType());

  // Check that the size of newHeap is incremented
  bool newSizeCheck = (newHeap.m_size == queue.m_size + newInsert);

  // Check that original queue's size remains unchanged (deep copy)
  bool originalSizeCheck = (queue.m_size == size);

  return isValid && sizeCheck && structureCheck && newSizeCheck &&
         originalSizeCheck && isNewHeapValid;
}
// Function: testDequeEmpty()
// Purpose: Test if dequeuing from an empty queue throws an out_of_range
// exception. Case: Dequeue from Empty Queue Description: This function creates
// an empty heap and attempts to dequeue an order from it using
//              the getNextOrder method. It then validates whether the operation
//              correctly throws an out_of_range exception.
// Parameters: None
// Expected Result:
// 1. An out_of_range exception is thrown when attempting to dequeue from the
// empty heap.
// 2. The function returns true if the exception is thrown, indicating the test
// passed.
// 3. The function returns false if no exception is thrown, indicating the test
// failed.
bool Tester::testDequeEmpty() {

  // Create an empty heap with the specified priority function and type
  MQueue emptyHeap(priorityFn1, MINHEAP, LEFTIST);

  try {
    // Attempt to get the next order from the empty heap
    emptyHeap.getNextOrder();

    // If no exception is thrown, return false indicating the test failed
    return false;
  } catch (const out_of_range &e) {

    // If an out_of_range exception is caught, return true indicating the test
    // passed
    return true;
  }
}

// Function: testMergeDiffPrior()
// Purpose: Test merging queues with different priority functions and ensure
// that a domain_error exception is thrown. Case: Merge Queues with Different
// Priority Functions Description: This function creates two heaps with
// different priority functions and attempts to merge them.
//              It validates whether the operation correctly throws a
//              domain_error exception when the priority functions of the two
//              queues differ.
// Parameters: None
// Expected Result:
// 1. A domain_error exception is thrown when attempting to merge queues with
// different priority functions.
// 2. The function returns true if the exception is thrown, indicating the test
// passed.
// 3. The function returns false if no exception is thrown, indicating the test
// failed.
bool Tester::testMergeDiffPrior() {

  // Generate a MINHEAP of LEFTIST type with 10 elements and priority 2
  MQueue firstH = generateTree(10, MINHEAP, LEFTIST, 2);

  // Generate a MAXHEAP of SKEW type with 10 elements and priority 1
  MQueue secondH = generateTree(10, MAXHEAP, SKEW, 1);

  try {
    // Attempt to merge the two queues with different priority functions
    firstH.mergeWithQueue(secondH);

    // If no exception is thrown, return false indicating the test failed
    return false;
  } catch (const domain_error &e) {
    // If a domain_error is caught, return true indicating the test passed
    return true;
  } catch (...) {
    // If any other exception is caught, return false indicating the test failed
    return false;
  }
}

bool Tester::testStressLargeInsert(int size, HEAPTYPE type, STRUCTURE struc,
                                   int prior) {

  MQueue aHeap = generateTree(size, type, struc, prior);
  bool isValidHeap = validateHeap(aHeap, size, type);

  if (isValidHeap) {
    return true;
  } else {
    return false;
  }
}

bool Tester::testInsertInvalidOrder() {

  MQueue queue1(priorityFn1, MINHEAP, SKEW);

  bool isValid = false;

  for (int i = 0; i < 1; i++) {
    Order order(DEFAULTCUSTOMER, 0, 0, 0, 0, 0, 0, 0, 0);
    isValid = queue1.insertOrder(order);
  }

  // If true the test failed, otherwise it passed
  if (!isValid) {
    return true;
  } else {
    return false;
  }
}

bool Tester::testAssignWithDiffSize() {

  int larger = 40;
  int small = 25;

  // Create a larger queue
  MQueue largerQueue = generateTree(larger, MAXHEAP, LEFTIST, 1);

  // Create a smaller queue
  MQueue smallerQueue = generateTree(small, MINHEAP, SKEW, 2);

  // Assign the smaller queue to the larger queue
  largerQueue = smallerQueue;

  // test if a valid heaptype
  bool isValid = validateHeap(largerQueue, smallerQueue.m_size, MINHEAP);

  // Validate the properties of the larger queue
  return isValid;
}

int main() {

  Tester aTester;
  cout << "Testing Max Heap Insert Normal" << endl;
  if (aTester.testNormalInsertMax(500, MAXHEAP, SKEW, 1)) {
    cout << "Testing Max Heap Insert Normal case passed!: Heap correctly "
            "handles normal insertions."
         << endl;
  } else {
    cout << "\nTesting Max Heap Insert Normal case failed!:" << endl;
    cout << "Heap properties not maintained after insertion." << endl;
  }
  aTester.resetVariables();

  cout << "Testing Min Heap Insert Normal" << endl;
  if (aTester.testNormalInsertMin(500, MINHEAP, LEFTIST, 2)) {
    cout << "Testing Min Heap Insert Normal case passed!: Heap correctly "
            "handles normal insertions."
         << endl;
  } else {
    cout << "\nTesting Min Heap Insert Normal case failed!:" << endl;
    cout << "Heap properties not maintained after insertion." << endl;
  }
  aTester.resetVariables();

  cout << "Testing Removal Min Heap  Normal" << endl;
  if (aTester.testRemovalMin(300, MINHEAP, LEFTIST, 2)) {
    cout << "Testing Min Heap Removal Normal case passed!: Removals correctly "
            "handled in order."
         << endl;
  } else {
    cout << "\nTesting Min Heap Removal Normal case failed!:" << endl;
    cout << "Removals did not maintain correct order." << endl;
  }
  aTester.resetVariables();
  cout << "Testing Removal Max Heap Normal" << endl;
  if (aTester.testRemovalMax(300, MAXHEAP, SKEW, 1)) {
    cout << "Testing Max Heap Removal Normal case passed!: Removals correctly "
            "handled in order."
         << endl;
  } else {
    cout << "\nTesting Max Heap Removal Normal case failed!:" << endl;
    cout << "Removals did not maintain correct order." << endl;
  }
  aTester.resetVariables();
  cout << "Testing change of Priority function" << endl;
  if (aTester.testChangePriorFunc(300, MINHEAP, LEFTIST, 1)) {
    cout << "Testing change of Priority function case passed!: Heap correctly "
            "rebuilds with new priority function and type."
         << endl;
  } else {
    cout << "\nTesting change of Priority function case failed!:" << endl;
    cout << "Heap properties not maintained after changing priority function."
         << endl;
  }
  aTester.resetVariables();

  // Define the size for the heap
  int size = 300;
  // Generate a heap with specified parameters
  MQueue aHeap = aTester.generateTree(size, MAXHEAP, SKEW, 1);
  cout << "Testing for overloaded assignment of a heap (Normal) \n" << endl;
  if (aTester.testOverloadedAssNormal(aHeap, size)) {
    cout << "Test Overloaded Assignment Operator (Normal Case) Passed: "
         << "The heap was correctly copied and maintained its properties."
         << endl;
  } else {
    std::cout << "Test Overloaded Assignment Operator (Normal Case) Failed: "
              << "The heap copy did not maintain its properties correctly."
              << endl;
  }

  aHeap.clear();
  aTester.resetVariables();
  MQueue aHeap2 = aTester.generateTree(size, MAXHEAP, SKEW, 1);
  cout << "Testing for overloaded assignment of an empty heap (Edge) \n"
       << endl;
  if (aTester.testOverloadedAssEdge(aHeap2, size)) {
    cout << "Test Overloaded Assignment Operator (Edge Case) Passed: "
         << "The heap was correctly copied to an empty heap and maintained "
            "its "
            "properties."
         << endl;
  } else {
    cout << "Test Overloaded Assignment Operator (Edge Case) Failed: "
         << "The heap copy did not maintain its properties correctly." << endl;
  }
  aHeap2.clear();
  aTester.resetVariables();

  MQueue aHeap1 = aTester.generateTree(size, MINHEAP, LEFTIST, 2);
  cout << "Testing for merge of an empty queue (Edge) \n" << endl;
  if (aTester.testMergeEmptyQueueEdge(aHeap1)) {
    cout << "Test Merge with Empty Queue (Edge Case) Passed: "
         << "The heap correctly maintained its properties after merging with "
            "an empty queue."
         << endl;
  } else {
    cout << "Test Merge with Empty Queue (Edge Case) Failed: "
         << "The heap did not maintain its properties correctly after merging "
            "with an empty queue."
         << endl;
  }
  aTester.resetVariables();

  // Generate a heap with specified parameters
  MQueue aHeap3 = aTester.generateTree(size, MINHEAP, SKEW, 2);
  cout << "Testing for normal copy of an object (Normal) \n" << endl;
  if (aTester.testCopyNormal(aHeap3, size)) {
    cout << "Test Copy Constructor (Normal Case) Passed: "
         << "The copied heap maintained its properties and is independent "
            "of the original."
         << endl;
  } else {
    cout << "Test Copy Constructor (Normal Case) Failed: "
         << "The copied heap did not maintain its properties correctly or "
            "was not independent of the original."
         << endl;
  }
  aTester.resetVariables();
  // Generate a heap with specified parameter
  int emptySize = 0;
  MQueue emptyHeap = aTester.generateTree(0, MAXHEAP, SKEW, 1);
  cout << "Testing if copy of an empty heap (Edge) \n" << endl;
  if (aTester.testCopyEdge(emptyHeap, emptySize)) {
    cout << "Test Copy Constructor (Edge Case) Passed: "
         << "The copied empty heap maintained its properties and is "
            "independent "
            "of the original."
         << endl;
  } else {
    cout << "Test Copy Constructor (Edge Case) Failed: "
         << "The copied heap did not maintain its properties correctly or "
            "was not independent of the original."
         << endl;
  }

  cout << "Testing if exception was thrown for dequeing empty queue (Error) \n"
       << endl;
  if (aTester.testDequeEmpty()) {
    cout << "Test passed: Exception thrown for empty queue." << endl;
  } else {
    cout << "Test failed: No exception thrown for empty queue." << endl;
  }

  cout << "Testing if exception was thrown for different priority (Error) \n"
       << endl;
  if (aTester.testMergeDiffPrior()) {
    cout << "Test passed: Exception thrown for merging queues with different "
            "priority functions."
         << endl;
  } else {
    cout << "Teset failed: No exception thrown for merging with different "
            "priority functions"
         << endl;
  }
  cout << "Testing invalid insertion (Error) \n" << endl;
  if (aTester.testInsertInvalidOrder()) {
    cout << "Error case Test passed: invalid order detected and insert not made"
         << endl;
  } else {
    cout << "Error case Test passed: invalid was not detected and insert was "
            "made"
         << endl;
  }
  cout << "Testing large number of insertions with different sizes (Edge) \n"
       << endl;
  if (aTester.testStressLargeInsert(5000, MINHEAP, LEFTIST, 2)) {
    cout << "Edge Test passed: Heap property maintained after large insertions."
         << endl;
  } else {
    cout << "Edge Test failed: Heap property not maintained after large "
            "insertions."
         << endl;
  }
  cout << "Testing Overloaded assignment with different sizes \n" << endl;
  if (aTester.testAssignWithDiffSize()) {
    cout << "Normal Test passed: Assignment operator correctly handles "
            "different sizes."
         << endl;
  } else {
    cout << "Normal Test failed: Assignment operator did not handle different "
            "sizes correctly."
         << endl;
  }

  return 0;
}
