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
  Menu& catalogue_;
  int id_;
  int stockedCount = 0;
  std::mutex managerMutex;
  std::condition_variable orderStocked;

 public:
  /**
   * Creates a manager
   * @param id : manager id
   * @param catalogue : catalogue for manager to order inventory from
   * @param queue : queue to place inventory order into
   * @param stock_queue : queue to check whether or not an item has been stocke
   */
  Manager(int id, Menu& catalogue, OrderQueue& queue, OrderQueue& stock_queue) :
      id_(id), catalogue_(catalogue), queue_(queue), stock_queue_(stock_queue) {}

  /**
   * Unique manager id
   * @return manager id
   */
  int id() {
    return id_;
  }  

  /**
   * Stocks order and notifies manager
   * @param order : Order that is ready to be stocked
  */
  void stock(const Order& order){
    std::unique_lock<decltype(managerMutex)> lock(managerMutex);
        stockedCount++;
    lock.unlock();

    orderStocked.notify_one();

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
    size_t s = catalogue_.catalogue().size();
	
	for(int i = 0; i < numberOfInventoryOrders; ++i) {
		if (s > 0) {
		  MenuItem appy = catalogue_.catalogue()[rand() % s];
		  cost += appy.price;
		  ++items;

		  safe_printf("Manager %d ordering the %s (%d)\n", id_, appy.item.c_str(), appy.id);
		  queue_.add({id_, appy.id});
		}
	}

    // Wait until all items have been stocked.
    safe_printf("Manager %d waiting on stock \n", id_);
    /*
    std::unique_lock <decltype(managerMutex)> lock(managerMutex);
    orderStocked.wait(lock, [&] () {return stockedCount >= numberOfInventoryOrders});
    lock.unlock();
    */
	safe_printf("Manager %d received all stock\n", id_);
	
    // stay for some time
    std::this_thread::sleep_for(std::chrono::seconds(5));

    safe_printf("Manager %d is happy as can be, happier than a tree \n", id_, cost);

    return 0;
  }
};

#endif //LAB6_CUSTOMER_H
