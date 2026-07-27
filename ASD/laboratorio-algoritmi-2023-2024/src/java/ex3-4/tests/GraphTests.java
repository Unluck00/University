package tests;

import exceptions.GraphException;

import structures.Graph;

import org.junit.Test;
import static org.junit.Assert.*;

import java.util.Collection;

public class GraphTests {

    @Test(expected = GraphException.class)
    public void testAddNullNode() throws GraphException {
        Graph<String, Integer> g = new Graph<>(false, true);
        assertTrue(g.addNode(null)); 
        assertTrue(g.containsNode(null));
    }

    @Test(expected = GraphException.class)
    public void testAddNullLabelInLabelledGraph() throws GraphException {
        Graph<String, Integer> g = new Graph<>(false, true);
        assertFalse(g.addEdge("A", "B", null)); 
    }

    @Test
    public void testAddNullLabelInUnlabelledGraph() throws GraphException {
        Graph<String, Integer> g = new Graph<>(false, false);
        assertTrue(g.addEdge("A", "B", null));
        assertTrue(g.containsEdge("A", "B"));
    }

    @Test
    public void testAddAndContainsNode() throws GraphException {
        Graph<String, Integer> g = new Graph<>(true, true);
        assertTrue(g.addNode("A"));
        assertTrue(g.containsNode("A"));
        assertFalse(g.addNode("A"));
    }

    @Test
    public void testAddAndContainsEdgeDirected() throws GraphException {
        Graph<String, Integer> g = new Graph<>(true, true);
        g.addEdge("A", "B", 5);
        assertTrue(g.containsEdge("A", "B"));
        assertFalse(g.containsEdge("B", "A"));
    }

    @Test
    public void testAddAndContainsEdgeUndirected() throws GraphException {
        Graph<String, Integer> g = new Graph<>(false, true);
        g.addEdge("A", "B", 7);
        assertTrue(g.containsEdge("A", "B"));
        assertTrue(g.containsEdge("B", "A"));
    }

    @Test
    public void testRemoveNode() throws GraphException {
        Graph<String, Integer> g = new Graph<>(false, true);
        g.addEdge("A", "B", 3);
        g.addEdge("B", "C", 4);
        assertTrue(g.removeNode("B"));
        assertFalse(g.containsNode("B"));
        assertFalse(g.containsEdge("A", "B"));
        assertFalse(g.containsEdge("B", "C"));
    }

    @Test
    public void testRemoveEdge() throws GraphException {
        Graph<String, Integer> g = new Graph<>(false, true);
        g.addEdge("A", "B", 10);
        assertTrue(g.removeEdge("A", "B"));
        assertFalse(g.containsEdge("A", "B"));
    }

    @Test
    public void testNeighboursAndLabel() throws GraphException {
        Graph<String, Integer> g = new Graph<>(true, true);
        g.addEdge("X", "Y", 7);
        g.addEdge("X", "Z", 9);
        Collection<String> neighbours = g.getNeighbours("X");
        assertTrue(neighbours.contains("Y"));
        assertTrue(neighbours.contains("Z"));
        assertEquals(Integer.valueOf(7), g.getLabel("X", "Y"));
        assertNull(g.getLabel("Y", "X")); 
    }

    @Test
    public void testNumNodesAndEdges() throws GraphException {
        Graph<String, Integer> g = new Graph<>(false, true);
        g.addEdge("A", "B", 1);
        g.addEdge("B", "C", 2);
        assertEquals(3, g.numNodes());
        assertEquals(2, g.numEdges());
    }
} 