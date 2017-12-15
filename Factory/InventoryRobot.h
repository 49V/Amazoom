#ifndef INVENTORY_ROBOT_H
#define INVENTORY_ROBOT_H

#include <cpen333/thread/thread_object.h>

#include "Robot.h"
#include "Manager.h"
#include "OrderQueue.h"
#include "safe_printf.h"

class InventoryRobot : public Robot {

    const std::vector<Manager*>& managers_;

    public:

    InventoryRobot(int id, OrderQueue& orders, OrderQueue& serve, std::vector<Manager*>& managers ) :
        Robot(id, orders, serve), managers_(managers) {}

    /**
     * Main execution function
     * @return 0 if completed
     */
    int main() {

        safe_printf("Inventory robot %d started\n", id_);

        Order order = orders_.get();
        
        //==================================================
        // TODO: ALLOW ROBOT TO PICK UP MULTIPLE OBJECTS
        //================================================
        
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
        // Tell manager that item has been stocked
			for (auto& manager : managers_) {
				if (manager->id() == order.customer_id) {
					manager->deliver(order);
					break;
				}
            }

        // next order
        order = orders_.get();
        }
        safe_printf("Robot %d entering R&R Mode \n", id_);

        return 0;
    }

};

#endif //INVENTORY_ROBOT_H