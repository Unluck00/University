package structures;

import exceptions.GraphException;

import java.util.*;

public class Graph<V, L> implements AbstractGraph<V,L> {

    private final boolean directed;
    private final boolean labelled;
    private final Map<V, Map<V, L>> adjacencyMap;

    public Graph(boolean directed, boolean labelled) {
        this.directed = directed;
        this.labelled = labelled;
        this.adjacencyMap = new HashMap<>();
    }

    @Override
    public boolean isDirected() {
        return this.directed;
    }

    @Override
    public boolean isLabelled() {
        return this.labelled;
    }

    @Override
    public boolean addNode(V a) throws GraphException {
        if(a == null) 
            throw new GraphException("addNode: node cannot be null");
        if(this.adjacencyMap.containsKey(a)) return false;
        this.adjacencyMap.put(a, new HashMap<>());
        return true;
    }

    @Override
    public boolean addEdge(V a, V b, L l) throws GraphException {
        if(a==null || b==null) 
            throw new GraphException("addEdge: node a and b cannot be null");
        addNode(a);
        addNode(b);
        if(!this.labelled && l!=null) return false;

        if(this.labelled && l==null)
            throw new GraphException("label cannot be null in labelled graph");

        if(adjacencyMap.get(a).containsKey(b)) return false;
            
        if(this.labelled) { this.adjacencyMap.get(a).put(b, l); } 
        else { this.adjacencyMap.get(a).put(b, null); }

        if(!this.directed) {
            if(this.labelled) { this.adjacencyMap.get(b).put(a, l); } 
            else { this.adjacencyMap.get(b).put(a, null); }
        }

        return true;
    }

    @Override
    public boolean containsNode(V a) { 
        return this.adjacencyMap.containsKey(a);
    }

    @Override
    public boolean containsEdge(V a, V b) {
        return this.adjacencyMap.containsKey(a) && this.adjacencyMap.get(a).containsKey(b);
    }

    @Override
    public boolean removeNode(V a) throws GraphException {
        if(a == null) 
            throw new GraphException("removeNode: node cannot be null");

        if(!this.adjacencyMap.containsKey(a)) return false;
        this.adjacencyMap.remove(a);
        for (Map<V, L> edges : this.adjacencyMap.values()) { // enhanced for loop
            edges.remove(a);
        }

        return true;
    }

    @Override
    public boolean removeEdge(V a, V b) throws GraphException {
        if(a==null || b==null) 
            throw new GraphException("removeEdge: node a and b cannot be null");
        
        if(!containsEdge(a, b)) return false;
        this.adjacencyMap.get(a).remove(b);
        if(!directed) this.adjacencyMap.get(b).remove(a);

        return true;
    }

    @Override
    public int numNodes() {
        return this.adjacencyMap.size();
    }

    @Override
    public int numEdges() {
        // int count = this.adjacencyMap.values().stream().mapToInt(Map::size).sum();
        int count = 0;
        for (Map<V, L> edges : this.adjacencyMap.values()) {
            count += edges.size();
        }

        if(this.directed) {
            return count;
        }
        return count / 2;
    }

    @Override
    public Collection<V> getNodes() {
        return this.adjacencyMap.keySet();
    }

    @Override
    public Collection<? extends AbstractEdge<V, L>> getEdges() {
        List<AbstractEdge<V, L>> edges = new ArrayList<>();
        
        for (var entry : this.adjacencyMap.entrySet()) {
            // V u = entry.getKey(); // node of the graph
            for (var edge : entry.getValue().entrySet()) {
                edges.add(new Edge(entry.getKey(), edge.getKey(), edge.getValue()));
                // entry.getKey() -> node start
                // e.getKey() -> node end
                // e.getValue() -> label of the edge
            }
        }

        return edges;
    }

    @Override
    public Collection<V> getNeighbours(V a) throws GraphException {
        if(a==null) 
            throw new GraphException("getNeighbours: node a cannot be null");

        if(this.adjacencyMap.containsKey(a)) {
            return this.adjacencyMap.get(a).keySet();
        }
        return Collections.emptyList();
    }

    @Override
    public L getLabel(V a, V b) throws GraphException {
        if(a==null || b==null) 
            throw new GraphException("getLabel: node a and b cannot be null");

        if(containsEdge(a, b)) {
            return this.adjacencyMap.get(a).get(b);
        }
        return null;
    }

    /**
     * implementation of AbstractEdge like internal class 
     */
    public class Edge implements AbstractEdge<V,L> {
        private final V start;
        private final V end;
        private final L label;

        public Edge(V start, V end, L label) {
            this.start = start;
            this.end = end;
            this.label = label;
        }
        
        @Override
        public V getStart() {
            return start;
        }

        @Override
        public V getEnd() {
            return end;
        }

        @Override
        public L getLabel() {
            return label;
        }
    }
}