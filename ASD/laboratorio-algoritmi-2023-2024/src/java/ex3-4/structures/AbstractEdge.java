package structures;

public interface AbstractEdge<V,L> {

  /**
   * @return start node of the edge in the graph
   */
  public V getStart();

  /**
   * @return end node of the edge in the graph
   */
  public V getEnd(); // il nodo di arrivo dell'arco

  /**
   * @return label of the edge in the graph
   */
  public L getLabel(); // l'etichetta dell'arco
};
