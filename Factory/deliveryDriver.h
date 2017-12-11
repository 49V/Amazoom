#ifndef LAB6_SERVER_H
#define LAB6_SERVER_H

#include <cpen333/thread/thread_object.h>
#include <iostream>
#include <thread>

#include "Customer.h"
#include "Order.h"
#include "OrderQueue.h"
#include "safe_printf.h"

/**
 * Delivery drivers take completed orders from the robots and deliver
 * them to customers.
 */
class DeliveryDriver : public cpen333::thread::thread_object {
  OrderQueue& queue_;
  const std::vector<Customer*>& customers_;
  int id_;
  
  //Part 3
  Order poisonOrder = {666, 666};

 public:
  /**
   * Create a delivery driver
   * @param id drive ID
   * @param queue queue of completed orders to server.  It is assumed this vector of customers never changes.
   * @param customers list of customers for finding who to deliver order to
   */
  DeliveryDriver(int id, OrderQueue& queue,
         const std::vector<Customer*>& customers) :
      id_(id), queue_(queue), customers_(customers) {}

  /**
   * Unique driver id
   * @return driver id
   */
  int id() {
    return id_;
  }

  /**
   * Main work function
   * @return 0 when complete
   */
  int main() {
	  
	  
	// Need to add functionality such that delivery driver waits until the truck
	// is full or until the order queue is empty.

    safe_printf("Delivery Driver %d started\n", id_);

    Order order = queue_.get();
    while (true) {

	  // Check if we have a poison order
	  if(order == poisonOrder){
		  break;
	  }
	
      // serve order
      safe_printf("Delivery driver %d delivering {%d,%d}\n", id_, order.customer_id, order.item_id);

      // Go find customer and serve
      for (auto& customer : customers_) {
        if (customer->id() == order.customer_id) {
          customer->serve(order);
          break;
        }
      }

      // next order
      order = queue_.get();
    }

    safe_printf("Delivery driver %d checking out.\n", id_);

    return 0;
  }
};

#endif //LAB6_SERVER_H
