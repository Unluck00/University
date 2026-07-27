package structures;

import exceptions.PriorityQueueException;

import java.util.*;

public class PriorityQueue<E> implements AbstractQueue<E> {
    private final ArrayList<E> heap;
    private final Map<E, Integer> indexMap;
    private final Comparator<? super E> comparator;

    public PriorityQueue(Comparator<? super E> comparator) throws PriorityQueueException {
        if(comparator == null)
            throw new PriorityQueueException("PriorityQueue: parameter comparator cannot be null");
        this.heap = new ArrayList<>();
        this.indexMap = new HashMap<>();
        this.comparator = comparator;
    }

    @Override
    public boolean empty() {
        return this.heap.isEmpty();
    }

    @Override
    public boolean push(E e) throws PriorityQueueException {
        if(e == null)
            throw new PriorityQueueException("push: parameter e cannot be null");
        if(this.indexMap.containsKey(e)) return false; // for consistence between heap and indexMap
        
        this.heap.add(e);
        int pos = this.heap.size()-1;
        this.indexMap.put(e, pos);
        moveUp(pos);
        return true;
    }

    @Override
    public boolean contains(E e) {
        return this.indexMap.containsKey(e);
    }

    @Override
    public E top() {
        if(this.heap.isEmpty()) return null;
        else return this.heap.get(0);
    }

    @Override
    public void pop() throws PriorityQueueException {
        if(this.heap.isEmpty()) return;
        else remove(this.heap.get(0));
    }

    @Override
    public boolean remove(E e) throws PriorityQueueException { 
        if(e == null)
            throw new PriorityQueueException("remove: parameter e cannot be null");
        if(this.heap.isEmpty())
            throw new PriorityQueueException("remove: heap is empty");
                
        Integer pos = this.indexMap.get(e);
        if(pos == null) return false;

        // swap last element in ArrayList with element to remove
        int last_pos = this.heap.size()-1;
        swap(pos, last_pos); 
        
        // remove last element on the list (which is the element we want to eliminate)
        this.heap.remove(last_pos);
        // remove element on the HashMap (update map)
        this.indexMap.remove(e);
        // swap + remove from ArrayList -> O(1)

        // rule of Heap 
        if(pos < heap.size()) {
            moveUp(pos);
            moveDown(pos);
        }
        // moveUp/moveDown -> O(log N)

        return true;
    }

    private void moveUp(int pos) {
        while (pos > 0) {
            int pos_up = (pos-1)/2;
            E e_pos = this.heap.get(pos);
            E e_pos_up = this.heap.get(pos_up);
            if(this.comparator.compare(e_pos, e_pos_up) < 0) {
                swap(pos, pos_up);
                pos = pos_up;
            }
            else {
                break;
            }
        }
    }

    private void moveDown(int pos) {
        int size = this.heap.size();
        while (true) {
            int left = 2*pos+1; 
            int right = 2*pos+2;
            int smallest = pos;

            if(left < size && this.comparator.compare(this.heap.get(left), this.heap.get(smallest)) < 0) {
                smallest = left;
            }
            if(right < size && this.comparator.compare(this.heap.get(right), this.heap.get(smallest)) < 0) {
                smallest = right;
            }
            if(smallest == pos) break;
            swap(pos, smallest);
            pos = smallest;
        }
    }

    private void swap(int pos, int last_pos) {
        E e_pos = this.heap.get(pos);
        E e_last_pos = this.heap.get(last_pos);
        this.heap.set(pos, e_last_pos);
        this.heap.set(last_pos, e_pos);
        this.indexMap.put(e_pos, last_pos);
        this.indexMap.put(e_last_pos, pos);
    }
}