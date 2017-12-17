#ifndef DRIVER_H
#define DRIVER_H

#define TRUCK_SPACE 10

#include <cpen333/thread/thread_object.h>
#include <cpen333/process/semaphore.h>
#include <iostream>
#include <thread>

#include "Common.h"
#include "Customer.h"
#include "Order.h"
#include "OrderQueue.h"
#include "safe_printf.h"

/**
 * Inventory drivers take completed orders from the robots and deliver
 * them to customers.
 */
class Driver : public cpen333::thread::thread_object {

 protected:
    OrderQueue& queue_;
    int id_;
    Order poisonOrder = {666, 666};

 public:
  /**
   * Create a delivery driver
   * @param id drive ID
   * @param queue queue of completed orders to server.  It is assumed this vector of customers never changes.
   * @param customers list of customers for finding who to deliver order to
   */
  Driver(int id, OrderQueue& queue) : id_(id), queue_(queue){}

  /**
   * Unique driver id
   * @return driver id
   */
  int id() {
    return id_;
  }

  /**
   * Creates a new vector of Order for the robot until the truck capacity is
   * reached or until the queue is empty.
   * @param order_queue The order_queue vector to be populated
  */
  void getOrders(std::vector<Order>& orders) {
      for(int i = 0; ((i < TRUCK_SPACE) && !queue_.isEmpty()); ++i) {			
				orders.push_back(queue_.get());
				
				// Only grab one poison pill
				if(orders[i] == poisonOrder){
                    break;
				}
			}
  }

};

#endif
