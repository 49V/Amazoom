#ifndef DELIVERY_DRIVER_H
#define DELIVERY_DRIVER_H


#include "Driver.h"

/**
 * Delivery drivers take completed orders from the robots and deliver
 * them to customers.
 */
class DeliveryDriver : public Driver {

 public:
	const std::vector<Customer*>& customers_;
	/**
	 * Create a delivery driver
	 * @param id drive ID
	 * @param queue queue of completed orders to server.  It is assumed this vector of customers never changes.
	 * @param customers list of customers for finding who to deliver order to
	 */
	DeliveryDriver(int id, OrderQueue& queue, const std::vector<Customer*>& customers) : Driver(id, queue), customers_(customers) {}

	/**
	 * Main work function
	 * @return 0 when complete
	 */
	int main() {
		
		// Define the vector for storing orders
		std::vector<Order> orders;

		// Grab the shared mutex for delivery drivers
		cpen333::process::semaphore parkingSpots(PARKING_SEMAPHORE, PARKING_SPOTS_COUNT);
		
		safe_printf("Delivery Driver %d started\n", id_);

		// Pickup goods from the factory
		parkingSpots.wait();
		getOrders(orders);
		parkingSpots.notify();
		
		while (true) {

			for(int i = 0; i < orders.size(); ++i) {

				if(orders[i] == poisonOrder){
					safe_printf("Delivery driver %d checking out.\n", id_);
					return 0;
				}

				// deliver order
				safe_printf("Delivery driver %d delivering {%d,%d}\n", id_, orders[i].customer_id, orders[i].item_id);

				// Go find customer and deliver
				for (auto& customer : customers_) {
					if (customer->id() == orders[i].customer_id) {
						customer->deliver(orders[i]);
						break;
					}
				}
			}
		
			// Clears orders queue for next batch
			orders.clear();
			
			// Get next orders
			parkingSpots.wait();
			getOrders(orders);
			parkingSpots.notify();
		}

		return 1;

	}
};

#endif
