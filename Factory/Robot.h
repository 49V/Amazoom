#ifndef ROBOT_H
#define ROBOT_H

#include <cpen333/thread/thread_object.h>
#include <iostream>
#include <thread>

#include "Order.h"
#include "OrderQueue.h"
#include "safe_printf.h"

/**
 * The robot gets Orders from the "orders_" queue and places the completed work
 * on "serve_" queue.
 */
class Robot : public cpen333::thread::thread_object {
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

  /**
   * Main execution function
   * @return 0 if completed
   */
  int main() {

    safe_printf("Robot %d started\n", id_);

    
    Order order = orders_.get();
	
	
	//==================================================
    // TODO: ALLOW ROBOT TO PICK UP MULTIPLE OBJECTS
    //==================================================
	
    while (true) {
		
	  // Check if we have the poison order
	  if(order == poisonOrder){
		  break;
	  }
	  
      // process order
      safe_printf("Robot %d starting order {%d,%d}\n", id_, order.customer_id, order.item_id);
      std::this_thread::sleep_for(std::chrono::seconds(5));
      safe_printf("Robot %d completed order {%d,%d}\n", id_, order.customer_id, order.item_id);

      // add to those to serve
      serve_.add(order);

      // next order
      order = orders_.get();
    }
    safe_printf("Robot %d entering R&R Mode \n", id_);

    return 0;
  }
};

#endif //ROBOT_H
