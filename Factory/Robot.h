#ifndef ROBOT_H
#define ROBOT_H

#include <cpen333/thread/thread_object.h>
#include <iostream>
#include <thread>

#include "Order.h"
#include "OrderQueue.h"

/**
 * The robot gets Orders from the "orders_" queue and places the completed work
 * on "serve_" queue.
 */
class Robot : public cpen333::thread::thread_object {
  protected:
    OrderQueue& orders_;
    OrderQueue& serve_;
    int id_;
    Order poisonOrder = {666, 666};
  
  public:
    /**
     * Create a new robot
     * @param id the robot's id
     * @param orders queue to read orders from
     * @param serve queue to add completed orders to
     */
    Robot(int id, OrderQueue& orders, OrderQueue& serve) :

        id_(id), orders_(orders), serve_(serve) {}

    int id() {
      return id_;
    }

    virtual int main() = 0;

};

#endif //ROBOT_H
