package structures;

import java.io.File;
import java.util.*;

public class Prim {
  public static <V, L extends Number> Collection<? extends AbstractEdge<V, L>> minimumSpanningForest(Graph<V, L> graph) throws Exception {
    
    PriorityQueue<AbstractEdge<V, L>> queue = new PriorityQueue<>(Comparator.comparingDouble(e -> e.getLabel().doubleValue())); // order edges for increase weight
    List<AbstractEdge<V, L>> minForest = new ArrayList<>();
    Set<V> visited = new HashSet<>();

    // Cicliamo su tutti i nodi del grafo
    for (V start : graph.getNodes()) { // Se un nodo non è ancora stato visitato → significa che siamo in una nuova componente connessa.
        if(visited.contains(start)) continue;
        visited.add(start); // Lo aggiungiamo a visited 
        
        // Inseriamo i suoi archi iniziali nella queue
        for (V node : graph.getNeighbours(start)) {
            L label = graph.getLabel(start, node);
            queue.push(graph.new Edge(start, node, label));
        }

        // Finché la coda non è vuota, prendiamo l’arco di peso minimo
        while(!queue.empty()) {
            AbstractEdge<V, L> edge = queue.top();
            queue.pop();

            // V node_start = edge.getStart();
            V node_end = edge.getEnd();

            if(visited.contains(node_end)) continue; // Se il nodo di arrivo node_end è già visitato → scartiamo l’arco (evita cicli)
            // Altrimenti, significa che node_end è il nuovo nodo da aggiungere al MST.
            minForest.add(edge);
            visited.add(node_end);

            // Si marca node_end come visitato.
            // Si aggiungono tutti i suoi vicini non ancora visitati alla queue.
            for (V node_neighbour_to_end : graph.getNeighbours(node_end)) {
                if(!visited.contains(node_neighbour_to_end)) {
                    L label_neighbour = graph.getLabel(node_end, node_neighbour_to_end);
                    queue.push(graph.new Edge(node_end, node_neighbour_to_end, label_neighbour));
                }
            }
        }
    }

    return minForest;
  }


  public static void main(String[] args) throws Exception {
    if(args.length != 1) {
        System.err.println("Usage: java Prim <file.csv>");
        return;
    }

    String filePath = args[0];
    File file = new File(filePath);
    Graph<String, Float> graph = new Graph<>(false, true);

    try (Scanner scanner = new Scanner(file)) {
        while (scanner.hasNextLine()) {
            String line = scanner.nextLine();
            String[] tokens = line.split(",");
            if (tokens.length != 3) continue;

            String place1 = tokens[0];
            String place2 = tokens[1];
            float distance = Float.parseFloat(tokens[2]);
            graph.addEdge(place1, place2, distance);
        }
        scanner.close();
    } catch (Exception exception) {
        exception.printStackTrace();
        return;
    }

    Collection<? extends AbstractEdge<String, Float>> forest = minimumSpanningForest(graph);

    float totalWeight = 0f;
    for (AbstractEdge<String, Float> edge : forest) {
        System.out.println(edge.getStart() + "," + edge.getEnd() + "," + edge.getLabel());
        totalWeight += edge.getLabel();
    }

    System.err.println("Nodes of the graph: " + graph.numNodes());
    System.err.println("Edges of the graph: " + forest.size());
    System.err.println("Total weight: " + totalWeight / 1000f + " Km");
  }
}       