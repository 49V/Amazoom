#ifndef USER_H
#define USER_H

#include <cpen333/thread/thread_object.h>
#include <cpen333/thread/semaphore.h>
#include <thread>
#include <vector>

class User : public cpen333::thread::thread_object {

protected:
    OrderQueue& queue_;
    Menu& catalogue_;
    int id_;
    int servedCount = 0;
    std::mutex userMutex;
    std::condition_variable userCV;

public:
    /**
     * Creates a user
     * @param id : User id
     * @param catalogue : Catalogue for user to order product from
     * @param queue : Queue to place order into
     */
    User(int id, Menu& catalogue, OrderQueue& queue) :
        id_(id), catalogue_(catalogue), queue_(queue) {}

    /**
     * Unique user id
     * @return : User id
     */
    int id() {
        return id_;
    }

    /**
     * Delivers user order and notifies the user 
     * @param order : Order that is ready to be delivered
     */
    void deliver(const Order& order) {
        std::unique_lock<decltype(userMutex)> lock(userMutex);
            servedCount++;
        lock.unlock();
        
        userCV.notify_one();
    }

    virtual int main() = 0;

};

#endif //USER_H