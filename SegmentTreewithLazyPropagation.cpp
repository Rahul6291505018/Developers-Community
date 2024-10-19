#include <iostream>
using namespace std;

const int N = 1e5;  // Max size of the array
int tree[4 * N], lazy[4 * N];  // Segment tree and lazy array

// Function to build the segment tree
void build(int arr[], int node, int start, int end) {
    if (start == end) {
        tree[node] = arr[start];  // Leaf node
    } else {
        int mid = (start + end) / 2;
        build(arr, 2 * node, start, mid);
        build(arr, 2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1];  // Sum of the children
    }
}

// Function to update the segment tree with lazy propagation
void updateRange(int node, int start, int end, int l, int r, int val) {
    if (lazy[node] != 0) {
        // Apply the pending update
        tree[node] += (end - start + 1) * lazy[node];
        if (start != end) {  // Not a leaf node
            lazy[2 * node] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
        }
        lazy[node] = 0;
    }

    if (start > end || start > r || end < l) return;  // Out of range

    if (start >= l && end <= r) {
        // Segment is fully within the range
        tree[node] += (end - start + 1) * val;
        if (start != end) {
            lazy[2 * node] += val;
            lazy[2 * node + 1] += val;
        }
        return;
    }

    // Otherwise, update the children
    int mid = (start + end) / 2;
    updateRange(2 * node, start, mid, l, r, val);
    updateRange(2 * node + 1, mid + 1, end, l, r, val);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

// Function to query the sum in a range with lazy propagation
int queryRange(int node, int start, int end, int l, int r) {
    if (lazy[node] != 0) {
        // Apply the pending update
        tree[node] += (end - start + 1) * lazy[node];
        if (start != end) {
            lazy[2 * node] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
        }
        lazy[node] = 0;
    }

    if (start > end || start > r || end < l) return 0;  // Out of range

    if (start >= l && end <= r) return tree[node];  // Segment is fully within range

    // Otherwise, query the children
    int mid = (start + end) / 2;
    int p1 = queryRange(2 * node, start, mid, l, r);
    int p2 = queryRange(2 * node + 1, mid + 1, end, l, r);
    return p1 + p2;
}

int main() {
    int arr[] = {1, 3, 5, 7, 9, 11};
    int n = sizeof(arr) / sizeof(arr[0]);

    build(arr, 1, 0, n - 1);

    // Update range (1, 3) by adding 10
    updateRange(1, 0, n - 1, 1, 3, 10);

    // Query sum of range (1, 3)
    cout << "Sum of values in range (1, 3): " << queryRange(1, 0, n - 1, 1, 3) << endl;

    return 0;
}
