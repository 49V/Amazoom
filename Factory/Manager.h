#ifndef MANAGER_H
#define MANAGER_H

#include <cpen333/thread/thread_object.h>
#include <cpen333/thread/semaphore.h>
#include <thread>
#include <vector>
#include "User.h"
#include "safe_printf.h"


/**
 * Managers place orders in the inventory queue
 */
class Manager : public User {

  public:
  Manager(int id, Menu& catalogue, OrderQueue& queue) : User(id, catalogue, queue) {}

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
        MenuItem product = catalogue_.catalogue()[rand() % s];
        cost += product.price;
        ++items;

        safe_printf("Manager %d ordering the %s (%d)\n", id_, product.item.c_str(), product.id);
        queue_.add({id_, product.id});
      }
	}

    // Wait until all items have been stocked.
    safe_printf("Manager %d waiting on stock \n", id_);
    std::unique_lock <decltype(userMutex)> lock(userMutex);
    userCV.wait(lock, [&] () {return (servedCount >= numberOfInventoryOrders);});
    lock.unlock();
	  safe_printf("Manager %d received all stock\n", id_);
	
    // stay for some time
    std::this_thread::sleep_for(std::chrono::seconds(5));

    safe_printf("Manager %d is happy as can be, happier than a tree \n", id_, cost);

    return 0;
  }
};

#endif //MANAGER_H
