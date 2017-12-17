#ifndef ROBOT_H
#define ROBOT_H

#define CARRYING_CAPACITY 5

#include <cpen333/thread/thread_object.h>
#include <iostream>
#include <thread>

#include "Order.h"
#include "OrderQueue.h"

/**
 * The robot gets Orders from the "order_queue_" queue and places the completed work
 * on "serve_queue_" queue.
 */
class Robot : public cpen333::thread::thread_object {
  protected:
    OrderQueue& order_queue_;
    OrderQueue& serve_queue_;
    int id_;
    Order poisonOrder = {666, 666};
  
  public:
    /**
     * Create a new robot
     * @param id the robot's id
     * @param order_queue queue to read order_queue from
     * @param serve_queue queue to add completed order_queue to
     */
    Robot(int id, OrderQueue& order_queue, OrderQueue& serve_queue) :

        id_(id), order_queue_(order_queue), serve_queue_(serve_queue) {}

   /**
   * Unique robot id
   * @return robot id
   */
    int id() {
      return id_;
    }


    /**
     * Creates a new vector of order_queue for the robot until the carrying capacity is
     * reached or until the queue is empty.
     * @param order_queue The order_queue vector to be populated
     */
    void getOrders(std::vector<Order>& orders) {
      for(int i = 0; ((i < CARRYING_CAPACITY) && (!order_queue_.isEmpty())); ++i) {
        orders.push_back(order_queue_.get());
        // Make sure that you only grab one poison pill
        if(orders[i] == poisonOrder ) {
          break;
        }
      }

      return;
    }
    
    virtual int main() = 0;

};

#endif //ROBOT_H
