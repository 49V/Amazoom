#ifndef MANAGER_H
#define MANAGER_H

#include <cpen333/thread/thread_object.h>
#include <cpen333/thread/semaphore.h>
#include <thread>
#include <vector>

#include "safe_printf.h"

/**
 * Managers place orders in the inventory queue
 */
class Manager:public cpen333::thread::thread_object {
  OrderQueue& queue_;
  OrderQueue& stock_queue_;
  Menu& menu_;
  int id_;
  
  std::mutex customerMutex;

 public:
  /**
   * Creates a manager
   * @param id manager id
   * @param menu menu for manager to order inventory from
   * @param queue queue to place inventory order into
   */
  Manager(int id, Menu& menu, OrderQueue& queue, OrderQueue& stock_queue) :
      id_(id), menu_(menu), queue_(queue), stock_queue_(stock_queue) {}

  /**
   * Unique manager id
   * @return manager id
   */
  int id() {
    return id_;
  }  

  /**
   * Main manager function
   * @return 0 when complete
   */
  int main() {

    safe_printf("Manager %d Placed Inventory Order\n", id_);

    // randomly order items from store
    double cost = 0;
    int items = 0;
	static int numberOfInventoryOrders = 30;

    srand((int)std::chrono::system_clock::now().time_since_epoch().count());

    // appetizer
    size_t s = menu_.appetizers().size();
	
	for(int i = 0; i < numberOfInventoryOrders; ++i) {
		if (s > 0) {
		  MenuItem appy = menu_.appetizers()[rand() % s];
		  cost += appy.price;
		  ++items;

		  safe_printf("Manager %d ordering the %s (%d)\n", id_, appy.item.c_str(), appy.id);
		  queue_.add({id_, appy.id});
		}
	}

    //==================================================
    // TODO: Wait for all order inventory items to be stocked
    //==================================================
	//The manager could alternatively constantly monitor the inventory levels in
	// a loop, and place orders whenever inventory is low. This is similar to how
	// robots and drivers will wait in the loop until they are doled out poison.
	// The inventory would be shared memory that robots and such would access
	// as they fulfill delivery orders.
    // As soon as the last customer leaves, we could notify the manager that it 
    // is quitting time, and just make the manager wait until all items have
    // been stocked before shutting the factory down	
	// Keep a condition variable that monitors whether or not item has been 
	// stocked. Just use the queue function I created. This would be the alternatively
	
	safe_printf("Manager %d waiting on stock \n", id_);
	// CONDITIONS
	safe_printf("Manager %d received all stock\n", id_);
	
    // stay for some time
    std::this_thread::sleep_for(std::chrono::seconds(5));

    safe_printf("Manager %d is happy as can be, happier than a tree \n", id_, cost);

    return 0;
  }
};

#endif //LAB6_CUSTOMER_H
