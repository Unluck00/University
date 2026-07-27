package tests;

import exceptions.PrimException;
import structures.AbstractEdge;
import structures.Graph;
import structures.Prim;

import org.junit.Test;
import static org.junit.Assert.*;

import java.util.Collection;

public class PrimTests {

    @Test
    public void testMSTSingleComponent() throws PrimException, Exception {
        Graph<String, Integer> g = new Graph<>(false, true);
        g.addEdge("A", "B", 1);
        g.addEdge("B", "C", 2);
        g.addEdge("A", "C", 3);

        Collection<? extends AbstractEdge<String, Integer>> mst = Prim.minimumSpanningForest(g);
        int totalWeight = mst.stream().mapToInt(e -> e.getLabel()).sum();
        assertEquals(3, totalWeight); // 1 + 2 = 3
        assertEquals(2, mst.size()); // 3 nodes -> 2 edges
    }

    @Test
    public void testMSTMultipleComponents() throws PrimException, Exception {
        Graph<String, Integer> g = new Graph<>(false, true);
        g.addEdge("A", "B", 1);
        g.addEdge("C", "D", 2);

        Collection<? extends AbstractEdge<String, Integer>> mst = Prim.minimumSpanningForest(g);
        int totalWeight = mst.stream().mapToInt(e -> e.getLabel()).sum();
        assertEquals(3, totalWeight); 
        assertEquals(2, mst.size()); 
    }

    @Test
    public void testEmptyGraph() throws Exception {
        Graph<String, Integer> g = new Graph<>(false, true);
        Collection<? extends AbstractEdge<String, Integer>> mst = Prim.minimumSpanningForest(g);
        assertTrue(mst.isEmpty());
    }

    @Test
    public void testGraphWithIsolatedNodes() throws PrimException, Exception {
        Graph<String, Integer> g = new Graph<>(false, true);
        g.addNode("A");
        g.addNode("B");
        g.addNode("C");
        Collection<? extends AbstractEdge<String, Integer>> mst = Prim.minimumSpanningForest(g);
        assertTrue(mst.isEmpty()); 
    }

    @Test
    public void testGraphWithEqualWeightEdges() throws PrimException, Exception {
        Graph<String, Integer> g = new Graph<>(false, true);
        g.addEdge("A", "B", 1);
        g.addEdge("B", "C", 1);
        g.addEdge("A", "C", 1);
        Collection<? extends AbstractEdge<String, Integer>> mst = Prim.minimumSpanningForest(g);
        assertEquals(2, mst.size());
        int totalWeight = mst.stream().mapToInt(e -> e.getLabel()).sum();
        assertEquals(2, totalWeight); 
    }
}