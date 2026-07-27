package tests;

import exceptions.PriorityQueueException;

import structures.PriorityQueue;

import org.junit.Test;
import static org.junit.Assert.*;

import java.util.Comparator;

public class PriorityQueueTests {

    @Test
    public void testEmptyQueue() throws PriorityQueueException{
        PriorityQueue<Integer> pq = new PriorityQueue<>(Comparator.naturalOrder());
        assertTrue(pq.empty());
    }

    @Test(expected = PriorityQueueException.class)
    public void testPushNullElement() throws PriorityQueueException{
        PriorityQueue<Integer> pq = new PriorityQueue<>(Comparator.naturalOrder());
        pq.push(null); // dovrebbe sollevare eccezione
    }

    @Test
    public void testPushAndTop() throws PriorityQueueException {
        PriorityQueue<Integer> pq = new PriorityQueue<>(Comparator.naturalOrder());
        pq.push(5);
        assertFalse(pq.empty());
        assertEquals(Integer.valueOf(5), pq.top());
    }

    @Test
    public void testPushMultipleAndTop() throws PriorityQueueException {
        PriorityQueue<Integer> pq = new PriorityQueue<>(Comparator.naturalOrder());
        pq.push(10);
        pq.push(3);
        pq.push(7);
        assertEquals(Integer.valueOf(3), pq.top());
    }

    @Test
    public void testPopReducesSize() throws PriorityQueueException {
        PriorityQueue<Integer> pq = new PriorityQueue<>(Comparator.naturalOrder());
        pq.push(2);
        pq.push(1);
        pq.pop();
        assertEquals(Integer.valueOf(2), pq.top());
    }

    @Test
    public void testContains() throws PriorityQueueException{
        PriorityQueue<Integer> pq = new PriorityQueue<>(Comparator.naturalOrder());
        pq.push(42);
        assertTrue(pq.contains(42));
        assertFalse(pq.contains(99));
    }

    @Test
    public void testRemoveElement() throws PriorityQueueException {
        PriorityQueue<Integer> pq = new PriorityQueue<>(Comparator.naturalOrder());
        pq.push(4);
        pq.push(1);
        pq.push(3);
        assertTrue(pq.remove(1));
        assertEquals(Integer.valueOf(3), pq.top());
        assertFalse(pq.contains(1));
    }
} 