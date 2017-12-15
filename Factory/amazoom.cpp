#include "Menu.h"
#include "Robot.h"
#include "DeliveryRobot.h"
#include "InventoryRobot.h"
#include "DeliveryDriver.h"
#include "InventoryDriver.h"
#include "SingleOrderQueue.h"
#include "CircularOrderQueue.h"
#include "DynamicOrderQueue.h"
#include "Manager.h"
#include "Customer.h"
#define POISON_ID 666

/**
 * Main function to run the restaurant
 * @return
 */
int main() {
  Menu catalogue;
  catalogue.load("../data/amazoom.json");

  // bunch of deliveryRobots, customers and deliveryDrivers
  std::vector<Robot*> deliveryRobots;
  std::vector<Robot*> inventoryRobots;
  std::vector<Customer*> customers;
  std::vector<DeliveryDriver*> deliveryDrivers;
  std::vector<InventoryDriver*> inventoryDrivers;
  std::vector<Manager*> managers;
  
  const int numberOfRobots = 2;
  const int numberOfDeliveryDrivers = 2;
  const int numberOfInventoryDrivers = 2;
  const int numberOfCustomers = 30;
  const int numberOfManagers = 1;

  CircularOrderQueue delivery_order_queue;
  CircularOrderQueue delivery_queue;
  CircularOrderQueue inventory_queue;
  CircularOrderQueue inventory_order_queue;
  DynamicOrderQueue stocked_queue;
  
  //Delivery Robots
  for (int i=0; i<numberOfRobots; ++i) {
    deliveryRobots.push_back(new DeliveryRobot(i, delivery_order_queue, delivery_queue));
  }

  //Inventory Robots (Offset by 2 to differentiate from delivery bots)
  for (int i=0; i<numberOfRobots; ++i) {
    inventoryRobots.push_back(new InventoryRobot(i + 2, inventory_order_queue, stocked_queue, managers));
  }
  
  // Delivery Drivers
  for (int i=0; i<numberOfDeliveryDrivers; ++i) {
    deliveryDrivers.push_back(new DeliveryDriver(i, delivery_queue, customers));
  }
  
  // Inventory Drivers (Offset by 2 to differentiate from Delivery Drivers)
  for (int i=0; i<numberOfInventoryDrivers; ++i) {
    inventoryDrivers.push_back(new InventoryDriver(i + 2, inventory_queue, inventory_order_queue));
  }
  
  // Customers
  for (int i=0; i<numberOfCustomers; ++i) {
    customers.push_back(new Customer(i, catalogue, delivery_order_queue));
  }

  //Manager
  for (int i=0; i<numberOfManagers; ++i) {
    managers.push_back(new Manager(i, catalogue, inventory_queue));
  }
  
  // start everyone
  for (auto& robot : deliveryRobots) {
    robot->start();
  }
  for (auto& robot : inventoryRobots) {
    robot->start();
  }
  for (auto& deliveryDriver : deliveryDrivers) {
    deliveryDriver->start();
  }
  for (auto& inventoryDriver : inventoryDrivers) {
    inventoryDriver->start();
  }
  for (auto& customer : customers) {
    customer->start();
  }
  for (auto& manager : managers) {
    manager->start();
  }

  // wait for all customers to leave
  for (auto& customer : customers) {
    customer->join();
  }

  std::cout << "Customers finished ordering for today" << std::endl;
  
  // Wait for all inventory to be stocked
  for (auto& manager : managers) {
    manager->join();
  }

  std::cout << "Manager says it is quitting time" << std::endl;
  
  //==================================================
  // TODO: Signal all deliveryRobots to leave
  //==================================================
  // Add numberOfRobots poison pills to the order_queue
  for(int i = 0; i < numberOfRobots; ++i){
	  delivery_order_queue.add({POISON_ID, POISON_ID});
  }
  // Add numberOfRobots poison pills to the order_queue
  for(int i = 0; i < numberOfRobots; ++i){
	  inventory_order_queue.add({POISON_ID, POISON_ID});
  }
  
  // wait for all deliveryRobots to leave
  for (auto& robot : deliveryRobots) {
    robot->join();
  }
  // wait for all inventoryRobots to leave
  for (auto& robot : inventoryRobots) {
    robot->join();
  }

    // Add numberOfDeliveryDrivers poison pills to the delivery_queue
  for(int i = 0; i < numberOfDeliveryDrivers; ++i){
	  delivery_queue.add({POISON_ID, POISON_ID});
  }
  // Add numberOfInventoryDrivers poison pills to the inventory_queue
  for(int i = 0; i < numberOfInventoryDrivers; ++i){
	  inventory_queue.add({POISON_ID, POISON_ID});
  }
  
  // wait for all deliveryDrivers to leave
  for (auto& deliveryDriver : deliveryDrivers) {
    deliveryDriver->join();
  }
  for (auto& inventoryDriver : inventoryDrivers) {
    inventoryDriver->join();
  }

  // free all memory
  for (auto& customer : customers) {
    delete customer;
    customer = nullptr;
  }
  for (auto& manager : managers) {
    delete manager;
    manager = nullptr;
  }
  for (auto& robot : deliveryRobots) {
    delete robot;
    robot = nullptr;
  }
  for (auto& robot : inventoryRobots) {
    delete robot;
    robot = nullptr;
  }
  for (auto& deliveryDriver : deliveryDrivers) {
    delete deliveryDriver;
    deliveryDriver = nullptr;
  }
  for (auto& inventoryDriver : inventoryDrivers) {
    delete inventoryDriver;
    inventoryDriver = nullptr;
  }
  
  std::cout << "Amazoom is closing" << std::endl;
  cpen333::pause();

  return 0;
}