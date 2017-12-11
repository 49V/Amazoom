#ifndef LAB6_CUSTOMER_H
#define LAB6_CUSTOMER_H

#include <cpen333/thread/thread_object.h>
#include <cpen333/thread/semaphore.h>
#include <thread>
#include <vector>

#include "safe_printf.h"

/**
 * Customers place orders into a queue, wait for them to be
 * served, eat, then leave
 */
class Customer : public cpen333::thread::thread_object {
  OrderQueue& queue_;
  Menu& menu_;
  int id_;
  
  // Part 2
  int servedDishCount = 0;
  std::mutex customerMutex;
  std::condition_variable mealServed;

 public:
  /**
   * Creates a customer
   * @param id customer id
   * @param menu menu for customer to order food from
   * @param queue queue to place order into
   */
  Customer(int id, Menu& menu, OrderQueue& queue) :
      id_(id), menu_(menu), queue_(queue) {}

  /**
   * Unique customer id
   * @return customer id
   */
  int id() {
    return id_;
  }

  /**
   * Serve customer an order
   * @param order order that is complete
   */
  void serve(const Order& order) {

    //==================================================
    // TODO: Notify main method that order is ready
    //==================================================
	std::unique_lock<decltype(customerMutex)> lock(customerMutex);
		servedDishCount++;
	lock.unlock();
	
	mealServed.notify_one();
  
  }
  

  /**
   * Main customer function
   * @return 0 when complete
   */
  int main() {

    safe_printf("Customer %d Placed Web Order\n", id_);

    // randomly order items from store
    double cost = 0;
    int items = 0;

    srand((int)std::chrono::system_clock::now().time_since_epoch().count());

    // appetizer
    size_t s = menu_.appetizers().size();
    if (s > 0) {
      MenuItem appy = menu_.appetizers()[rand() % s];
      cost += appy.price;
      ++items;

      safe_printf("Customer %d ordering the %s (%d)\n", id_, appy.item.c_str(), appy.id);
      queue_.add({id_, appy.id});
    }

    //==================================================
    // TODO: Wait for item to be delivered
    //==================================================
	// Keep a condition variable that monitors whether or not item has been 
	// delivered. Need to protect access to reading it.
	safe_printf("Customer %d waiting on order\n", id_);
	std::unique_lock <decltype(customerMutex)> lock(customerMutex);
	mealServed.wait(lock, [&](){return (servedDishCount > 0);});
	lock.unlock();
	safe_printf("Customer %d received order\n", id_);
	
    // stay for some time
    std::this_thread::sleep_for(std::chrono::seconds(5));

    safe_printf("Customer %d is happy as a tree \n", id_, cost);

    return 0;
  }
};

#endif //LAB6_CUSTOMER_H
