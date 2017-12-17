#ifndef INVENTORY_DRIVER_H
#define INVENTORY_DRIVER_H

#include "Driver.h"

/**
 * Inventory drivers take completed orders from the robots and deliver
 * them to factory.
 */
class InventoryDriver : public Driver {

 public:
 OrderQueue& factory_;

	/**
	 * Create a delivery driver
	 * @param id drive ID
	 * @param queue queue of completed orders to server.  It is assumed this vector of factory never changes.
	 * @param factory list of factory for finding who to deliver order to
	 */
	InventoryDriver(int id, OrderQueue& queue, OrderQueue& factory) : Driver(id, queue), factory_(factory) {}

	/**
	 * Main work function
	 * @return 0 when complete
	 */
	int main() {
		
		// Define the vector for storing orders
		std::vector<Order> orders;

		// Grab the shared mutex for delivery drivers
		cpen333::process::semaphore parkingSpots(PARKING_SEMAPHORE, PARKING_SPOTS_COUNT);
		
		safe_printf("Inventory Driver %d started\n", id_);

		// If we have no orders, just go deliver yer goods mate
		getOrders(orders);
		
		while (true) {
		
			parkingSpots.wait();
			for(int i = 0; i < orders.size(); ++i) {

				if(orders[i] == poisonOrder) {
					parkingSpots.notify();
					safe_printf("Inventory Driver Checking Out");
					return 0;
				}

				// serve order
				safe_printf("Inventory driver %d delivering {%d,%d}\n", id_, orders[i].customer_id, orders[i].item_id);
				// Adds to factory queue so the robots can start stocking the shelves
				factory_.add(orders[i]);
			}
			parkingSpots.notify();
		
			// Clears orders queue for next batch
			orders.clear();
			
			// Get next orders
			getOrders(orders);
		}

		return 1;
	}
};

#endif
