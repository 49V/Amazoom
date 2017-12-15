#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <cpen333/thread/thread_object.h>
#include <cpen333/thread/semaphore.h>
#include <thread>
#include <vector>
#include "User.h"
#include "safe_printf.h"


/**
 * Customers place orders into a queue, wait for them to be
 * served, and then log off
 */
class Customer : public User {

  public:
  Customer(int id, Menu& catalogue, OrderQueue& queue) : User(id, catalogue, queue) {}

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

    // Randomly selects and orders an item from the catalogue
    size_t s = catalogue_.catalogue().size();
    if (s > 0) {
      MenuItem product = catalogue_.catalogue()[rand() % s];
      cost += product.price;
      ++items;

      safe_printf("Customer %d ordering the %s (%d)\n", id_, product.item.c_str(), product.id);
      queue_.add({id_, product.id});
    }

  /**
   * Waits for item to be delivered. Keep a condition variable that monitors 
   * whether or not item has been delivered. Need to protect access to reading it.
   */
	safe_printf("Customer %d waiting on order\n", id_);
	std::unique_lock <decltype(userMutex)> lock(userMutex);
	  userCV.wait(lock, [&](){return (servedCount > 0);});
	lock.unlock();
	safe_printf("Customer %d received order\n", id_);
	
  // stay for some time
  std::this_thread::sleep_for(std::chrono::seconds(5));

  safe_printf("Customer %d is happy as a tree \n", id_, cost);

  return 0;
  }
};

#endif //CUSTOMER_H
