#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
typedef Matrix;
typedef queue;
typedef Triple;
struct BtreeNd {
    int key;
    struct BtreeNd* parent;
    struct BtreeNd* left;
    struct BtreeNd* right;
};
typedef struct BtreeNd* btree;
typedef struct kTreeNode {
    int key;
    struct kTreeNode* child;
    struct kTreeNode* sibling;
} kTreeNode;
typedef kTreeNode* kTree;
typedef enum { verde, rosso, bianco } Color;
typedef struct ListNode {
    int info;
    struct ListNode* pred;
    struct ListNode* next;
} ListNode;
typedef ListNode* list;
struct ht {
    int dim;
    list* array;
};
typedef struct ht* HashTable;
struct heapFrame {
    int dim;
    int hd;
    int* keys;
};
typedef struct heapFrame* Heap;

void Bandiera(Color B[], int n) {
    int i = 0;
    int k = n - 1;

    for (int j = 0; j <= k; j++) {
        if (B[j] == verde) {
            swap(B, j, i);
            i++;
        } else if (B[j] == rosso) {
            swap(B, j, k);
            k--;
            j--;
        }
    }
}

int countBubbleSort(int A[], int n) {
    int flag;
    int confronti = 0;

    for (int i = 0; i < n - 1; i++) {
        flag = 0;
        for (int j = 0; j < n - i - 1; j++) {
            confronti++;
            if (A[j] > A[j + 1]) {
                swap(A, j, j + 1);
                flag = 1;
            }
        }
        if (flag == 0)
            break;
    }

    return confronti;
}

int find(int A[], int N, int f) {
    int LEFT = 0;
    int RIGHT = N - 1;
    while (LEFT < RIGHT) {
        int left = LEFT;
        int right = RIGHT;
        int r = A[f];
        while (left <= right) {
            while (A[left] < r) left++;
            while (r < A[right]) right--;
            if (left <= right) {
                swap(A, left, right);
                left++;
                right--;
            }
        }
        if (f <= right)
            RIGHT = right;
        else if (f >= left)
            LEFT = left;
        else
            break;
    }

    return A[f];
}

list mergeList(list l, list m) {
    if (l == NULL) {
        return m;
    } else if (m == NULL) {
        return l;
    } else {
        if (l->info <= m->info) {
            l->next = mergeList(l->next, m);
            return l;
        } else {
            m->next = mergeList(m->next, l);
            return m;
        }
    }
}

list mergeListNonDistruttiva(list l, list m) {
    if (l == NULL) {
        return m;
    } else if (m == NULL) {
        return l;
    } else if (l->info <= m->info) {
        return Cons(l->info, mergeListNonDistruttiva(l->next, m));
    } else {
        return Cons(m->info, mergeListNonDistruttiva(l, m->next));
    }
}

void merge(int A[], int a, int p, int b) {
    int n1 = p - a + 1;
    int n2 = b - p;
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));
    arraycpy(A, a, p, L, 0);
    arraycpy(A, p + 1, b, R, 0);
    int i = 0;
    int j = 0;
    int k = a;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            A[k] = L[i];
            i++;
        } else {
            A[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        A[k] = L[i];
        k++;
        i++;
    }
    while (j < n2) {
        A[k] = R[j];
        k++;
        j++;
    }
    free(L);
    free(R);
}

list mergeSort(list l) {
    if (l == NULL) {
        return l;
    } else if (l->next == NULL) {
        return l;
    } else {
        list m = split(l);
        l = mergeSort(l);
        m = mergeSort(m);
        mergeList(l, m);
    }
}

int Partition(int A[], int p, int r) {
    int x = A[p];
    int i = p + 1;
    int j = r;
    while (i <= j) {
        if (A[i] <= x)
            i = i + 1;
        else if (A[j] > x)
            j = j - 1;
        else {
            swap(A, i, j);
            i = i + 1;
            j = j - 1;
        }
    }
    swap(A, p, i - 1);
    return i - 1;
}

int dicotomicSearchRec(int A[], int i, int j, int target) {
    int m = (i + j) / 2;
    if (i > j) {
        return -1;
    } else if (A[m] == target) {
        return m;
    } else if (target > A[m]) {
        return dicotomicSearchRec(A, m + 1, j, target);
    } else {
        return dicotomicSearchRec(A, i, m - 1, target);
    }
}

int binomialRec(int n, int k) {
    if (k == 0 || n == k) {
        return 1;
    } else {
        return binomialRec(n - 1, k - 1) + binomialRec(n - 1, k);
    }
}
int binomialBottUp(int n, int k) {
    int mat[n + 1][k + 1];

    for (int i = 0; i < n + 1; i++) {
        for (int j = 0; j < k + 1; j++) {
            if (j == 0 || j == i) {
                mat[i][j] = 1;
            } else {
                mat[i][j] = -1;
            }
        }
    }

    for (int i = 0; i < n + 1; i++) {
        for (int j = 0; j < k + 1; j++) {
            if (j != 0 && j != i)
                mat[i][j] = mat[i - 1][j - 1] + mat[i - 1][j];
        }
    }

    return mat[n][k];
}

int binomialNoMem(int n, int k) {
    int res = 0;

    for (int i = 0; i < k + 1; i++) {
        if (i == 0 || n == i) {
            res += 1;
        } else {
            res = (res * (n - i + 1)) / i;
        }
    }

    return res;
}

Matrix distance(char* X, char* Y) {
    int m = strlen(X);
    int n = strlen(Y);
    Matrix D = newMatrix(m + 1, n + 1);
    for (int i = 0; i <= m; i++) D->array[i][0] = i;
    for (int j = 0; j <= n; j++) D->array[0][j] = j;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            if (X[i - 1] == Y[j - 1])
                D->array[i][j] = D->array[i - 1][j - 1];
            else
                D->array[i][j] = 1 + min(D->array[i][j - 1], min(D->array[i - 1][j], D->array[i - 1][j - 1]));
    return D;
}

list deleteAll(int n, list as) {
    if (as == NULL) {
        return as;
    } else {
        if (as->info == n) {
            return deleteAll(n, as->next);
        } else {
            as->next = deleteAll(n, as->next);
            return as;
        }
    }
}

list symmDiff(list xs, list ys) {
    if (xs == NULL) {
        return copyList(ys);
    } else if (ys == NULL) {
        return copyList(xs);
    } else if (xs->info == ys->info) {
        return symmDiff(xs->next, ys->next);
    } else if (xs->info < ys->info) {
        return Cons(xs->info, symmDiff(xs->next, ys));
    } else {
        return Cons(ys->info, symmDiff(xs, ys->next));
    }
}

list insertList(list as, list bs, int n) {
    if (n == 0) {
        return concat(as, bs);
    } else {
        bs->next = insertList(as, bs->next, n - 1);
        return bs;
    }
}

bool insertHeapMin(Heap H, int k) {
    if (H->hd == H->dim) {
        return false;
    } else {
        H->keys[H->hd] = k;
        H->hd++;
        int i = H->hd - 1;
        while (i > 0 && H->keys[i] < H->keys[parent(H, i)]) {
            swap(H->keys, i, parent(H, i));
            i = parent(H, i);
        }
        return true;
    }
}

int corankAux(list l, int tot) {
    if (l == NULL) {
        return tot;
    } else {
        tot = tot + l->info;
        l->info = tot;
        return corankAux(l->next, tot);
    }
}
int corank(list l) { return corankAux(l, 0); }

int rank(list l) {
    if (l == NULL) {
        return 0;
    } else {
        l->info = l->info + rank(l->next);
        return l->info;
    }
}

list fast_reverseAux(list l, list appoggio) {
    if (l == NULL) {
        return appoggio;
    } else {
        return fast_reverseAux(l->next, Cons(l->info, appoggio));
    }
}
list fast_reverse(list l) { return fast_reverseAux(l, NULL); }

int height(kTree t) {
    if (t->child == NULL) {
        return 0;
    } else {
        int tot = 0;
        kTree v = t->child;
        while (v != NULL) {
            tot = max(tot, height(v));
            v = v->sibling;
        }
        return tot + 1;
    }
}

int sumLeaf(kTree t) {
    if (t == NULL) {
        return 0;
    } else if (t->child == NULL)
        return t->key;
    else {
        kTree v = t->child;
        int sum = 0;
        while (v) {
            sum += sumLeaf(v);
            v = v->sibling;
        }
        return sum;
    }
}

list kTreeBFS(kTree t) {
    if (t == NULL)
        return NULL;
    queue q = NewQueue();
    EnQueue(t, q);
    list l = NULL;
    while (!isEmptyQueue(q)) {
        kTree node = DeQueue(q);
        l = Cons(node->key, l);
        node = node->child;
        while (node) {
            EnQueue(node, q);
            node = node->sibling;
        }
    }

    return reverse(l);
}

Triple isOrderedBTreeAux(btree bt) {
    Triple t;
    t.isOrdered = true;
    t.min = bt->key;
    t.max = bt->key;

    if (bt->right) {
        Triple check_right = isOrderedBTreeAux(bt->right);
        if (check_right.isOrdered && bt->key <= check_right.min) {
            t.max = check_right.max;
        } else {
            t.isOrdered = false;
        }
    }

    if (bt->left) {
        Triple check_left = isOrderedBTreeAux(bt->left);
        if (check_left.isOrdered && bt->key >= check_left.max) {
            t.min = check_left.min;
        } else {
            t.isOrdered = false;
        }
    }

    return t;
}
bool isOrdered(btree bt) {
    if (!bt)
        return true;
    Triple ret = isOrderedBTreeAux(bt);
    return ret.isOrdered;
}

list DescList_aux(btree bt, list l) {
    if (bt == NULL) {
        return l;
    } else {
        return DescList_aux(bt->right, Cons(bt->key, DescList_aux(bt->left, l)));
    }
}
list DescList(btree bt) { return DescList_aux(bt, NULL); }

list CrescList_aux(btree bt, list l) {
    if (bt == NULL) {
        return l;
    } else {
        return CrescList_aux(bt->left, Cons(bt->key, CrescList_aux(bt->right, l)));
    }
}

btree minInBtree(btree bt) {
    btree res = bt;
    while (res->left) {
        res = res->left;
    }
    return res;
}
btree rightAncestor(btree nd) {
    if (nd->parent == NULL) {
        return NULL;
    } else if (nd == nd->parent->left) {
        return nd->parent;
    } else {
        return rightAncestor(nd->parent);
    }
}
btree successor(btree nd) {
    if (nd->right == NULL) {
        return rightAncestor(nd);
    } else {
        return minInBtree(nd->right);
    }
}

btree antenatoComune(btree bt, int a, int b) {
    if (bt == NULL) {
        return NULL;
    } else if (bt->key >= a && bt->key <= b) {
        return bt;
    } else if (bt->key < a) {
        return antenatoComune(bt->right, a, b);
    } else {
        return antenatoComune(bt->left, a, b);
    }
}

void maxHeapify(int H[], int i, int hd) {
    int leftindex = left(i, hd);
    int rightindex = right(i, hd);
    int max = i;
    if (leftindex != i && H[leftindex] > H[max]) {
        max = leftindex;
    }
    if (rightindex != i && H[rightindex] > H[max]) {
        max = rightindex;
    }
    if (max != i) {
        swap(H, i, max);
        maxHeapify(H, max, hd);
    }
}
void buildMaxHeap(int A[], int dim) {
    int m = dim / 2 + 1;
    for (int i = m; i >= 0; i--) {
        maxHeapify(A, i, dim);
    }
}
void heapSort(int A[], int dim) {
    buildMaxHeap(A, dim - 1);
    for (int i = dim - 1; i > 0; i--) {
        swap(A, 0, i);
        maxHeapify(A, 0, i - 1);
    }
}

list oddList(list l) {
    if (l == NULL)
        return NULL;
    else if (l->info % 2 == 0)
        return oddList(l->next);
    else
        return Cons(l->info, oddList(l->next));
}
list evenList(list l) {
    if (l == NULL)
        return NULL;
    else if (l->info % 2 == 0)
        return Cons(l->info, evenList(l->next));
    else
        return evenList(l->next);
}
list oddEven(list l) { return concat(oddList(l), evenList(l)); }

list ordInsert(int k, list p, list l) {
    if (l == NULL) {
        return Cons(k, p, l);
    } else if (k == l->info)
        return l;
    else if (k < l->info) {
        return Cons(k, p, l);
    } else {
        l->next = ordInsert(k, l, l->next);
        return l;
    }
}
void hashInsert(HashTable T, int k) {
    int pos = hashFun(k, T->dim);
    T->array[pos] = ordInsert(k, NULL, T->array[pos]);
}

int hashInsertLinear(HashTable T, int k) {
    for (int i = 0; i < T->dim; i++) {
        int pos = linearProbing(k, i, T->dim);
        if (T->array[pos] == -1) {
            T->array[pos] = k;
            return pos;
        } else if (T->array[pos] == k) {
            return -1;
        }
    }
    return -2;
}

int hashSearch(HashTable T, int k) {
    for (int i = 0; i < T->dim; i++) {
        int pos = linearProbing(k, i, T->dim);
        if (T->array[pos] == k) {
            return k;
        } else if (T->array[pos] == -1) {
            return -1;
        }
    }
    return -2;
}

kTree complete(int key, int dg, int ht) {
    if (ht == 0) {
        return consTree(key, NULL, NULL);
    }
    kTree child = complete(key, dg, ht - 1);
    kTree temp = child;
    for (int i = 0; i < dg - 1; i++) {
        temp->sibling = complete(key, dg, ht - 1);
        temp = temp->sibling;
    }
    return consTree(key, child, NULL);
}

int countInt(kTree t) {
    if (t == NULL || t->child == NULL)
        return 0;
    int tot = 1;
    kTree temp = t->child;
    while (temp) {
        tot = tot + countInt(temp);
        temp = temp->sibling;
    }
    return tot;
}

list differenza(list l, list m) {
    if (l == NULL) {
        return NULL;
    } else if (m == NULL) {
        return l;
    } else if (l->info == m->info) {
        return differenza(l->next, m->next);
    } else if (l->info < m->info) {
        return Cons(l->info, differenza(l->next, m));
    } else {
        return differenza(l, m->next);
    }
}

list unione(list l, list m) {
    if (l == NULL) {
        return m;
    } else if (m == NULL) {
        return l;
    } else if (m->info == l->info) {
        return Conc(m->info, unione(l->next, m->next));
    } else if (m->info < l->info) {
        return Conc(m->info, unione(l, m->next));
    } else {
        return Conc(l->info, unione(l->next, m));
    }
}

list split(list l) {
    list slow = l;
    list fast = l->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    list ret = slow->next;
    slow->next = NULL;
    return ret;
}

bool subset(list l, list m) {
    if (l == NULL)
        return true;
    if (m == NULL)
        return false;
    if (l->info == m->info)
        return subset(l->next, m->next);
    if (l->info > m->info)
        return subset(l, m->next);
    return false;
}

bool equal(list l, list r) {
    if (l == NULL) {
        if (r == NULL) {
            return true;
        } else {
            return false;
        }
    } else if (r == NULL) {
        if (l == NULL) {
            return true;
        } else {
            return false;
        }
    } else if (l->info == r->info) {
        return true && equal(l->next, r->next);
    } else {
        return false;
    }
}
bool palindrome(list l) {
    if (l == NULL) {
        return true;
    }
    else {
        list reversed = fast_reverse(l);
        return equal(l, reversed);
    }
}

btree insertTree(int k, btree bt) {
    if (bt == NULL) {
        return ConsTree(k, NULL, NULL);
    } else if (k == bt->key) {
        return bt;
    } else if (k > bt->key) {
        bt->right = insertTree(k, bt->right);
        return bt;
    } else {
        bt->left = insertTree(k, bt->left);
        return bt;
    }
}

int maxDecSeq(int A[], int n) {
    if (n == 0)
        return 0;
    int max = 1;
    int current = 1;
    for (int i = 1; i < n; i++) {
        if (A[i] < A[i - 1]) {
            current++;
        } else {
            if (current > max) {
                max = current;
                current = 1;
            }
        }
    }
    if (current > max)
        max = current;
    return max;
}

int searchRec(int a[], int i, int target) {
    int j = dim(a);
    if (i > j) {
        return -1;
    } else {
        if (a[i] == target) {
            return i;
        } else {
            return searchRec(a, i + 1, target);
        }
    }
}

int searchIter(int a[], int i, int target) {
    int j = dim(a);
    for (int k = 0; k < j; k++) {
        if (a[k] == target) {
            return k;
        }
    }
    return -1;
}

int expRic(int x, int n) {
    if (n == 0) {
        return 1;
    }
    if (n % 2 == 0) {
        int half = expRic(x, n / 2);
        return half * half;
    } else {
        return x * expRic(x, n - 1);
    }
}
int expIter(int x, int n) {
    int z = 1;
    while (n > 0) {
        if (n % 2 == 1) {
            z = z * x;
        }
        x = x * x;
        n = n / 2;
    }
    return z;
}

list fringe(kTree t) {
    if (t->child == NULL) {
        return Cons(t->key, NULL);
    } else {
        list result = NULL;
        kTree temp = t->child;
        while (temp) {
            result = concat(result, fringe(temp));
            temp = temp->sibling;
        }
        return result;
    }
}

btree maxInBtree(btree bt) {
    if (bt->right == NULL)
        return bt;
    else
        return maxInBtree(bt->right);
}
btree leftAncestor(btree node) {
    if (node->parent == NULL) {
        return NULL;
    } else if (node == node->parent->right) {
        return node->parent;
    } else {
        return leftAncestor(node->parent);
    }
}
btree predecessor(btree node) {
    if (node->left) {
        return maxInBtree(node->left);
    } else {
        return leftAncestor(node);
    }
}

void decreaseKey(Heap H, int i, int newKey) {
    H->keys[i] = newKey;
    int par = parent(H, i);
    if (par != i && H->keys[i] < H->keys[par]) {
        swap(H, i, par);
        decreaseKey(H, par, H->keys[par]);
    }
}

list intersect(list l, list m) {
    if (l == NULL) {
        return NULL;
    } else if (m == NULL) {
        return NULL;
    } else if (l->info == m->info) {
        return Cons(l->info, intersect(l->next, m->next));
    } else if (l->info < m->info) {
        return intersect(l->next, m);
    } else {
        return intersect(l, m->next);
    }
}

int maxSumBranch(kTree t) {
    if (t->child == NULL) {
        return t->key;
    } else {
        int massimo = INT_MIN;
        kTree c = t->child;
        while (c) {
            massimo = max(massimo, maxSumBranch(c));
            c = c->sibling;
        }
        return t->key + massimo;
    }
}

int degree(kTree t) {
    if (t->child == NULL) {
        return 0;
    } else {
        kTree c = t->child;
        int contaAttuale = 0;
        int massimo = 0;
        while (c) {
            contaAttuale++;
            massimo = max(massimo, degree(c));
            c = c->sibling;
        }
        return max(contaAttuale, massimo);
    }
}
int degree(kTree t) {
    if (t == NULL)
        return 0;
    else {
        int degree_child = degree(t->child) + 1;
        int degree_sibling = degree(t->sibling);
        return degree_child > degree_sibling ? degree_child : degree_sibling;
    }
}