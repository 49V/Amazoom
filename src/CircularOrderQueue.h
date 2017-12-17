#ifndef CIRCULARORDERQUEUE_H
#define CIRCULARORDERQUEUE_H

#include "OrderQueue.h"
#include <cpen333/thread/semaphore.h>
#include <mutex>

#define CIRCULAR_BUFF_SIZE 8

/**
 * Queue implementation using a circular buffer
 * (i.e. a fixed-size queue)
 */
class CircularOrderQueue : public virtual OrderQueue {
  Order buffer_[CIRCULAR_BUFF_SIZE];
  cpen333::thread::semaphore producer_;
  cpen333::thread::semaphore consumer_;
  std::mutex producerMutex_;
  std::mutex consumerMutex_;
  std::mutex readerMutex_;
  size_t producerIndex_;
  size_t consumerIndex_;
  size_t queueCount_;

 public:
  /**
   * Creates a queue with provided circular buffer size
   * @param buffsize size of circular buffer
   */
  CircularOrderQueue() :
      buffer_(),
      producer_(CIRCULAR_BUFF_SIZE), consumer_(0),
      producerMutex_(), consumerMutex_(),readerMutex_(), producerIndex_(0), consumerIndex_(0), queueCount_(0){}

  /**
   * Adds an order to the queue if space is available, otherwise it will wait. Notifies writer mutex of additional order
   * @ param order Order to be added to the queue
  */
  void add(const Order& order) {
	
	// wait for the empty slot
	producer_.wait();
	
	// when we acquire it, take the producer index and store it
    int currentProducerIndex;
	std::unique_lock<decltype(producerMutex_)> lock(producerMutex_);
	queueCount_++;
    currentProducerIndex = producerIndex_;
	// update producer index
	producerIndex_ = (producerIndex_ + 1) % CIRCULAR_BUFF_SIZE;
	//Update the buffer to take the current order
    buffer_[currentProducerIndex] = order;
	lock.unlock();
	
	// Update the consumer
	consumer_.notify();

  }

  /**
   * Takes an order from the queue if an order is available, if not will wait. 
   * Notifies the producer queue that another spot has opened for writing.
   * @return Order object from the queue
  */
  Order get() {

	consumer_.wait();
	
    int currentConsumerIndex;
	std::unique_lock<decltype(consumerMutex_)> lock(consumerMutex_);
	queueCount_--;
    currentConsumerIndex = consumerIndex_;
	// update consumer index
	consumerIndex_ = (consumerIndex_ + 1) % CIRCULAR_BUFF_SIZE;
	// Return the order at the current index
    Order out = buffer_[currentConsumerIndex];
	lock.unlock();
	
	producer_.notify();

    return out;
  }
  
  /**
   * Checks whether or not a queue is empty
   * @return bool Returns 1 if empty, 0 otherwise
  */
  bool isEmpty(){
	  
	  std::unique_lock<decltype(producerMutex_)> lock(producerMutex_);
	  bool empty = (queueCount_ == 0);
	  lock.unlock();
	  
	  return empty;
  }

};

#endif //LAB6_CIRCULARORDERQUEUE_H
