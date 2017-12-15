#ifndef DELIVERY_ROBOT_H
#define DELIVERY_ROBOT_H

#include <cpen333/thread/thread_object.h>

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

        safe_printf("Delivery robot %d started\n", id_);

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
        // If inventory robot, make sure to update the manager

        // next order
        order = orders_.get();
        }
        safe_printf("Robot %d entering R&R Mode \n", id_);

        return 0;
    }

};

#endif //DELIVERY_ROBOT_H