#include <bits/stdc++.h>

using namespace std;
vector <int> tree;
vector <int> add;
vector <int> a;

void build(int v, int l, int r) {
    if (r - l == 1) {
        tree[v] = a[l];
        return;
    }
    int m = (l + r) / 2;
    build(v * 2 + 1, l, m);
    build(v * 2 + 2, m, r);
    tree[v] += tree[v * 2 + 1] + tree[v * 2 + 2];
}

void push(int v, int l, int r) {
    tree[v] += add[v] * (r - l);
    if (r - l == 1) {
        add[v] = 0;
        return;
    }
    add[v * 2 + 1] += add[v];
    add[v * 2 + 2] += add[v];
    add[v] = 0;
}

int get(int v, int l, int r, int tl, int tr) {
    push(v, l, r);
   // if (tr <= l && tl >= r) return 0;
    if (r <= l || tl >= tr) return 0;
    if (tl <= l && tr >= r) return tree[v];

    int m = (l + r) / 2;
    int left = get(v * 2 + 1, l, m, tl, min(tr, m));
    int right = get(v * 2 + 2, m, r, max(tl, m), tr);
    return left + right;
}

void pl(int v, int l, int r, int tl, int tr, int val) {
    //cout << v << " " << l << " " << r << ", " << tl << " " << tr << "\n";
    push(v, l, r);
   // if (tr <= l && tl >= r) return;
    if (r <= l || tl >= tr) return;
    if (tl <= l && tr >= r) {
        add[v] += val;
        return;
    }

    int m = (l + r) / 2;
    pl(v * 2 + 1, l, m, tl, min(tr, m), val);
    pl(v * 2 + 2, m, r, max(tl, m), tr, val);
    tree[v] = tree[v * 2 + 1] + add[v * 2 + 1] + tree[v * 2 + 2] + add[v * 2 + 2];
}

int main() {
    int n;
    cin >> n;
    tree.assign(4 * n, 0);
    add.resize(4 * n, 0);
    int q;
    cin >> q;
    for (int j = 0; j < q; ++q) {
        int x, y, z;
        cin >> x >> y >> z;
        if (x == 0) cout << get(0, 0, n, y - 1, z);
        else pl(0, 0, n, y - 1, z, x);
    }
}
