#ifndef LAB6_CHEF_H
#define LAB6_CHEF_H

#include <cpen333/thread/thread_object.h>
#include <iostream>
#include <thread>

#include "Order.h"
#include "OrderQueue.h"
#include "safe_printf.h"

/**
 * The robot grabs an order from the queue and loads them onto the delivery truck
 */
class Robot : public cpen333::thread::thread_object {
  OrderQueue& orders_;
  OrderQueue& serve_;
  int id_;
  //Part 3
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
	// Talk about what you did with the delivery trucks, and how it would be 
	// nearly identically the same thing with robots.
	
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

#endif //LAB6_CHEF_H
