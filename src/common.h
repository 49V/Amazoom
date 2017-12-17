#ifndef MUTEX_PROCESS_H
#define MUTEX_PROCESS_H

// common names for "named" resources between drivers
#define PARKING_SEMAPHORE "mutex_parking_semaphore"
#define PARKING_SPOTS_COUNT 2
#define INVENTORY_MEMORY_NAME "inventory_memory"
#define INVENTORY_MUTEX_NAME "inventory_mutex"
#define ITEM_ID_INDEX 0
#define ITEM_QUANTITY_INDEX 1
#define MAX_NUMBER_OF_ITEMS 100
#define MAX_NUMBER_OF_PRODUCTS 5


// Shared memory for the inventory

struct InventoryData {
    int numberOfProducts;
    int stock[MAX_NUMBER_OF_PRODUCTS][2]; // to store the product ID and quantity
};

#endif //MUTEX_PROCESS_H
