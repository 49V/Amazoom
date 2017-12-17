#ifndef DELIVERY_ROBOT_H
#define DELIVERY_ROBOT_H

#include <cpen333/process/mutex.h>
#include <cpen333/process/shared_memory.h>
#include <cpen333/thread/thread_object.h>

#include "Common.h"
#include "Robot.h"
#include "OrderQueue.h"
#include "safe_printf.h"

class DeliveryRobot : public Robot {

    public:

    DeliveryRobot(int id, OrderQueue& orders, OrderQueue& serve) : Robot(id, orders, serve){}

    /**
     * Main execution function
     * @return 0 if completed
     */
    int main() {

        //Acquire access to shared mutex
        cpen333::process::shared_object<InventoryData> inventory(INVENTORY_MEMORY_NAME);
        cpen333::process::mutex inventory_mutex(INVENTORY_MUTEX_NAME);
        std::vector<Order> orders;

        safe_printf("Delivery robot %d started\n", id_);

        getOrders(orders);

        while (true) {
            
            for(int i = 0; i < orders.size(); ++i) {

                // Check if we have the poison order
                if(orders[i] == poisonOrder){
                    return 0;
                }

                // process order
                safe_printf("Robot %d starting order {%d,%d}\n", id_, orders[i].customer_id, orders[i].item_id);
                // remove item from inventory and update
                inventory_mutex.lock();
                std::this_thread::sleep_for(std::chrono::seconds(1));
                inventory->stock[orders[i].item_id - 1][ITEM_QUANTITY_INDEX]--;
                inventory_mutex.unlock();
                safe_printf("Robot %d completed order {%d,%d}\n", id_, orders[i].customer_id, orders[i].item_id);
                
                // Serve
                serve_queue_.add(orders[i]);
                
            }    

            // clear orders
            orders.clear();

            // next order
            getOrders(orders);
        }

        safe_printf("Robot %d entering R&R Mode \n", id_);

        return 1;
    }

};

#endif //DELIVERY_ROBOT_H