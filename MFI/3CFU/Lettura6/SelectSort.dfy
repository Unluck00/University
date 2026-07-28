/** Selection sort
 */

predicate Ordered(a: array<int>)
    reads a
 {
    forall i, j :: 0 <= i < j < a.Length ==> a[i] <= a[j]
 }

method SelectSort(a: array<int>)
    ensures Ordered(a)
    ensures multiset(a[..]) == multiset(old(a[..])) 
    // this implies that the a in the post-state is a permutation
    // of the same a in the pre-state
    modifies a
{
   var n := 0;
   while n != a.Length
      invariant 0 <= n <= a.Length
      invariant forall i, j :: 0 <= i < j < n ==> a[i] <= a[j]
         // a[..n] is ordered
      invariant forall i, j :: 0 <= i < n <= j < a.Length ==> a[i] <= a[j]
         // all values in a[n..] are greter or equal to the elements in a[..n]
      invariant multiset(a[..]) == multiset(old(a[..]))
   {
      var mindex, m := n, n;
      while m != a.Length
         invariant n <= m <= a.Length && n <= mindex < a.Length
         invariant forall i :: n <= i < m ==> a[mindex] <= a[i]
            // mindex is the index of the minimum in a[n..m]
      {
         if a[m] < a[mindex] {
            mindex := m;
         }
         m := m + 1;
      }
      a[n], a[mindex] := a[mindex], a[n];
      n := n + 1;
   }
}