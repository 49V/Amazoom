#ifndef LAB6_CIRCULARORDERQUEUE_H
#define LAB6_CIRCULARORDERQUEUE_H

#include "OrderQueue.h"
#include <cpen333/thread/semaphore.h>
#include <mutex>

#define CIRCULAR_BUFF_SIZE 8

/**
 * Queue implementation using a circular buffer
 * (i.e. a fixed-size queue)
 */
class CircularOrderQueue : public virtual OrderQueue {
  Order buff_[CIRCULAR_BUFF_SIZE];
  cpen333::thread::semaphore producer_;
  cpen333::thread::semaphore consumer_;
  std::mutex pmutex_;
  std::mutex cmutex_;
  std::mutex rmutex_;
  size_t pidx_;
  size_t cidx_;
  size_t orderCount_;

 public:
  /**
   * Creates a queue with provided circular buffer size
   * @param buffsize size of circular buffer
   */
  CircularOrderQueue() :
      buff_(),
      producer_(CIRCULAR_BUFF_SIZE), consumer_(0),
      pmutex_(), cmutex_(),rmutex_(), pidx_(0), cidx_(0), orderCount_(0){}

  void add(const Order& order) {
	
	// Wait for the empty slot
	producer_.wait();
	
	// When we acquire it, take the producer index and store it
    int currentProducerIndex;
	std::unique_lock<decltype(pmutex_)> lock(pmutex_);
	orderCount_++;
    currentProducerIndex = pidx_;
	// update producer index
	pidx_ = (pidx_ + 1) % CIRCULAR_BUFF_SIZE;
	//Update the buffer to take the current order
    buff_[currentProducerIndex] = order;
	lock.unlock();
	
	// Update the consumer
	consumer_.notify();

  }

  Order get() {

	consumer_.wait();
	
    int currentConsumerIndex;
	std::unique_lock<decltype(cmutex_)> lock(cmutex_);
	orderCount_--;
    currentConsumerIndex = cidx_;
	// update consumer index
	cidx_ = (cidx_ + 1) % CIRCULAR_BUFF_SIZE;
	// Return the order at the current index
    Order out = buff_[currentConsumerIndex];
	lock.unlock();
	
	producer_.notify();

    return out;
  }
  
  bool isEmpty(){
	  
	  std::unique_lock<decltype(pmutex_)> lock(pmutex_);
	  bool empty = (orderCount_ == 0);
	  lock.unlock();
	  
	  return empty;
  }

};

#endif //LAB6_CIRCULARORDERQUEUE_H
