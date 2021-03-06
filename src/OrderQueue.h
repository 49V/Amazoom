#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include "Order.h"

/**
 * Thread-safe queue of items
 */
class OrderQueue {
 public: 
  
  /**
   * Adds an order item to the queue
   * @param order item to add
   */
  virtual void add(const Order& order) = 0;

  /**
   * Retrieve the next order item in the queue
   * @return next available item
   */
  virtual Order get() = 0;
  
  /**
   * Checks whether or not a queue is empty
   * @return 1 if empty, 0 if not
   */
  virtual bool isEmpty() = 0;
  
};

#endif //ORDER_QUEUE_H
