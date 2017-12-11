#include "Menu.h"
#include "Robot.h"
#include "Customer.h"
#include "DeliveryDriver.h"
#include "SingleOrderQueue.h"
#include "CircularOrderQueue.h"
#include "DynamicOrderQueue.h"

#define POISON_ID 666

/**
 * Main function to run the restaurant
 * @return
 */
int main() {
  Menu catalogue;
  catalogue.load("../data/amazoom.json");

  // bunch of robots, customers and deliveryDrivers
  std::vector<Robot*> robots;
  std::vector<Customer*> customers;
  std::vector<DeliveryDriver*> deliveryDrivers;

  const int numberOfRobots = 4;
  const int numberOfDeliveryDrivers = 2;
  const int numberOfCustomers = 30;

  //============================================
  // TODO: Change queue types to test all three
  //    - SingleOrderQueue
  //    - CircularOrderQueue
  //    - DynamicOrderQueue
  //============================================
  CircularOrderQueue order_queue;
  CircularOrderQueue serve_queue;

  for (int i=0; i<numberOfRobots; ++i) {
    robots.push_back(new Robot(i, order_queue, serve_queue));
  }

  for (int i=0; i<numberOfDeliveryDrivers; ++i) {
    deliveryDrivers.push_back(new DeliveryDriver(i, serve_queue, customers));
  }

  for (int i=0; i<numberOfCustomers; ++i) {
    customers.push_back(new Customer(i, catalogue, order_queue));
  }

  // start everyone
  for (auto& robot : robots) {
    robot->start();
  }
  for (auto& deliveryDriver : deliveryDrivers) {
    deliveryDriver->start();
  }
  for (auto& customer : customers) {
    customer->start();
  }

  // wait for all customers to leave
  for (auto& customer : customers) {
    customer->join();
  }

  std::cout << "ALL CUSTOMERS ARE GONE BOY" << std::endl;
  
  //==================================================
  // TODO: Signal all robots to leave
  //==================================================
  // Add numberOfRobots poison pills to the order_queue
  for(int i = 0; i < numberOfRobots; ++i){
	  order_queue.add({POISON_ID, POISON_ID});
  }
  
  // wait for all robots to leave
  for (auto& robot : robots) {
    robot->join();
  }

  //==================================================
  // TODO: Signal all deliveryDrivers to leave
  //==================================================
  // Add numberOfDeliveryDrivers poison pills to the serve_queue
  
  for(int i = 0; i < numberOfDeliveryDrivers; ++i){
	  serve_queue.add({POISON_ID, POISON_ID});
  }
  
  // wait for all deliveryDrivers to leave
  for (auto& deliveryDriver : deliveryDrivers) {
    deliveryDriver->join();
  }

  // free all memory
  for (auto& customer : customers) {
    delete customer;
    customer = nullptr;
  }
  for (auto& robot : robots) {
    delete robot;
    robot = nullptr;
  }
  for (auto& deliveryDriver : deliveryDrivers) {
    delete deliveryDriver;
    deliveryDriver = nullptr;
  }

  std::cout << "Amazoom is closing" << std::endl;
  cpen333::pause();

  return 0;
}