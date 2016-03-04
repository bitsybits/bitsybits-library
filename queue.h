#ifndef QUEUE_H
#define QUEUE_H

template<typename T>
class Queue {
public:
  Queue ();
  ~Queue ();

  void push (const T i);
  T pop (const T errorCode);
  T peek (const T errorCode) const;
  bool isEmpty () const;

private:
  typedef struct node {
    T value;
    node* next;
  }* pnode;

  pnode mHead;
  pnode mTail;
};

template<typename T>
Queue<T>::Queue () {
  mHead = NULL;
  mTail = NULL;
}

template<typename T>
Queue<T>::~Queue () {
  for (pnode cur = mHead; cur != NULL; mHead = cur) {
    cur = mHead->next; 
    delete mHead;
  }

  mHead = NULL;
  mTail = NULL;
}

template<typename T>
void Queue<T>::push (const T value) {
  pnode cur = mTail;
  mTail = (pnode) new node;

  mTail->next = NULL;
  mTail->value = value;
  if (isEmpty ()){
    mHead = mTail;
  } else {
    cur->next = mTail;
  }
}

template<typename T>
T Queue<T>::pop (const T errorCode) {
  if (!isEmpty ()){
    T value = mHead->value;
    pnode cur = mHead->next;
    delete mHead;
    mHead = cur;
    return value;
  }
  return errorCode;
}

template<typename T>
T Queue<T>::peek (const T errorCode) const {
  if (!isEmpty ()){
    return mHead->value;
  }
  return errorCode;
}

template<typename T>
bool Queue<T>::isEmpty () const {
  return mHead == NULL;
}

#endif //QUEUE_H