package structures;

import java.util.Collection;

/**
 * An abstract class representing a Graph with generic Vertexes and Label.
 * 
 * @param <V> type of the elements in the Graph
 * @param <L> type of the label in the Graph
 */

public interface AbstractGraph<V,L> {
  /**
   * @return true if the graph is direct, otherwise false -- O(1)
   */
  public boolean isDirected();

  /**
   * @return true if the graph is label, otherwise false -- O(1)
   */
  public boolean isLabelled(); 

  /**
   * @param a to be added to the graph
   * @return true if added a node -- O(1)
   */
  public boolean addNode(V a) throws Exception;

  /**
   * @param a identifies a node
   * @param b identifies a node
   * @param l identifies label to the edge of the graph
   * @return true if added a edge to the graph -- O(1)
   */
  public boolean addEdge(V a, V b, L l) throws Exception; 

  /**
   * @param a node to be checked
   * @return true if node is in the graph -- O(1)
   */
  public boolean containsNode(V a);

/**
   * @param a node to be checked
   * @param b node to be checked
   * @return true if edge is in the graph -- O(1)
   */
  public boolean containsEdge(V a, V b);

/**
   * @param a node to be checked
   * @return true if node removed in the graph -- O(N)
   */
  public boolean removeNode(V a) throws Exception;

  /**
   * @param a node to be checked
   * @param b node to be checked
   * @return true if edge removed in the graph -- O(1)
   */
  public boolean removeEdge(V a, V b) throws Exception;
  
  /**
   * @return number of the nodes in the graph -- O(1)
   */
  public int numNodes();

  /**
   * @return number of the edge in the graph -- O(N)
   */
  public int numEdges();

  /**
   * @param <V> type of the elements in the graph
   * @return nodes of the graph -- O(N)
   */
  public Collection<V> getNodes();

   /**
   * @param <V> type of the elements in the graph
   * @param <L> type of the labels in the graph
   * @return edges of the graph -- O(N)
   */
  public Collection<? extends AbstractEdge<V,L>> getEdges();

   /**
   * @param a identifies the select node
   * @return nodes adjacent to the select node of the graph -- O(1)
   */
  public Collection<V> getNeighbours(V a) throws Exception;

  /**
   * @param a node to be checked
   * @param b node to be checked
   * @return label of the edge in the graph -- O(1)
   */
  public L getLabel(V a, V b) throws Exception;
};
