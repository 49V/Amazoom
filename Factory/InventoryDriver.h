#ifndef INVENTORY_DRIVER_H
#define INVENTORY_DRIVER_H

#include <cpen333/thread/thread_object.h>
#include <cpen333/process/semaphore.h>
#include <iostream>
#include <thread>

#include "Customer.h"
#include "Order.h"
#include "OrderQueue.h"
#include "safe_printf.h"
#include "common.h"

/**
 * Inventory drivers take completed orders from the robots and deliver
 * them to customers.
 */
class InventoryDriver : public cpen333::thread::thread_object {
  OrderQueue& queue_;
  OrderQueue& factoryQueue_;
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
  InventoryDriver(int id, OrderQueue& queue,
         OrderQueue& factoryQueue) :
      id_(id), queue_(queue), factoryQueue_(factoryQueue){}

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
	  
	// Define the vector for storing orders
	std::vector<Order> orders;
	
	const int truckSpace = 5;

	// Grab the shared mutex for delivery drivers
    cpen333::process::semaphore parkingSpots(PARKING_SEMAPHORE, PARKING_SPOTS_COUNT);
	
    safe_printf("Inventory Driver %d started\n", id_);

	// If we have no orders, just go deliver yer goods mate
	parkingSpots.wait();
	for(int i = 0; ((i < truckSpace) && !queue_.isEmpty()); ++i) {
		orders.push_back(queue_.get());
	}
	parkingSpots.notify();
	
    while (true) {
	
		for(int i = 0; i < orders.size(); ++i) {
		    // serve order
			safe_printf("Inventory driver %d delivering {%d,%d}\n", id_, orders[i].customer_id, orders[i].item_id);
			// Adds to factory queue so the robots can start stocking the shelves
			factoryQueue_.add(orders[i]);
		}
	  
		  // Clears orders queue for next batch
		  orders.clear();
		  
		  // Get next orders
		  parkingSpots.wait();
		  for(int i = 0; ((i < truckSpace) && !queue_.isEmpty()); ++i) {			
			orders.push_back(queue_.get());
			
			// If we have no orders, just go ahead and move on
			// Check if we have a poison order
			if(orders[i] == poisonOrder){
				safe_printf("Inventory driver %d checking out.\n", id_);
				return 0;
			}
		  }
		  parkingSpots.notify();
	}

  }
};

#endif
