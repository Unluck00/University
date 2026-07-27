package structures;

public interface AbstractQueue<E> {
  /**
   * @return true if queue is empty -- O(1)
   */
  public boolean empty();

  /**
   * @param e to be added to the queue
   * @return true if an element has been added to the queue -- O(logN)
   * @throws Exception throws an Exception when element is null
   */
  public boolean push(E e) throws Exception;

  /**
   * @param e element to be checked
   * @return true if element is in the queue -- O(1)
   */
  public boolean contains(E e);

  /**
   * access to the element on the top of the queue -- O(1)
   * @return E element 
   */
  public E top();

  /**
   * eliminate an element on the top of the queue -- O(logN)
   * @throws Exception throws an Exception when element is null or heap is empty
   */
  public void pop() throws Exception;

  /**
   * @param e to be eliminate to the queue
   * @return true if an element has been eliminate to the queue -- O(logN)
   * @throws Exception throws an Exception when element is null or heap is empty
   */
  public boolean remove(E e) throws Exception;
};
