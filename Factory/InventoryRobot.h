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

        std::vector<Order> orders;

        safe_printf("Inventory robot %d started\n", id_);

        getOrders(orders);
        
        while (true) {
            
            for(int i = 0; i < orders.size(); ++i) {
                // Check if we have the poison order

                if(i == 0) {
                    // process order
                    safe_printf("Robot %d starting order {%d,%d}\n", id_, orders[i].customer_id, orders[i].item_id);
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    safe_printf("Robot %d completed order {%d,%d}\n", id_, orders[i].customer_id, orders[i].item_id);
                }

                if(orders[i] == poisonOrder){
                    return 0;
                }
                
                // Serve
                serve_queue_.add(orders[i]);
                    
                    // Notify managers
                    for (auto& manager : managers_) {
                        if (manager->id() == orders[i].customer_id) {
                            manager->deliver(orders[i]);
                            break;
                        }
                    }
            }    
        
            // clear the orders vector so new orders can properly be added
            orders.clear();

           getOrders(orders);
        }

        safe_printf("Robot %d entering R&R Mode \n", id_);

        return 1;
    }

};

#endif //INVENTORY_ROBOT_H