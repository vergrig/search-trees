#include <bits/stdc++.h>

using namespace std;

struct Node {
    int x, y = ???, sz = 1;
    Node *l = nullptr, *r = nullptr;

    Node(int x) {
        this->x = x;
    }
};

int size(Node* t) {
    if (t != nullptr) return t->sz;
    return 0;
}

void upd(Node* t) {
    if (t != nullptr) t->sz = size(t->l) + size(t->r) + 1;
    return t;
}

Node* find(Node* t, int x) {
    if (!t) return nullptr;

    if (t->x == val) {
        return t;
    } else if (val < t->x) {
        return find(t->l, val);
    } else if (val > t->x) {
        return find(t->r, val);
    }
}

Node* merge(Node* t1, Node* t2) {
    if (t1 == nullptr) return t2;
    if (t2 == nullptr) return t1;

    if (t1->y > t2->y) {
        t1->r = merge(t1->r, t2);
        upd(t1);
        return t1;
    } else {
        t2->l = merge(t1, t2->l);
        upd(t2);
        return t2;
    }
}

Node* kth(Node* t, int k) {
    if (t == nullptr) return t;

    if (size(t->l) == k) {
        return t;
    }else if (size(t->l) > k) {
        return kth(t->l, k);
    } else {
        return kth(t->r, k - 1 - size(t->l));
    }
}

pair<Node*, Node*> split(Node* t, int div) {
    if (t == nullptr) return {nullptr, nullptr};

    if (div <= t->x) {
        auto p = split(t->l, div);
        t->l = p.second;
        upd(t);
        return {p.first, t};
    } else {
        auto p = split(t->r, x);
        t->r = p.first;
        upd(t);
        return {t, p.second};
    }
}

Node* insert(Node* t, int val) {
    Node* new_node = new Node(val);
    auto t1 = split(t, val);
    return merge(t1.first, merge(new_node, t1.second));
}

Node* erase(Node* t, int val) {
    auto t1 = split(t, x);
    auto t2 = split(t1.second, val + 1);
    return merge(t1.first, t2.second);
}

void print_tree(Node* t) {
    if (t == nullptr) return;
    print_tree(t->l);
    cout << "<-" << t->x << "->";
    print_tree(t->r);
}

int main(void) {
    Node* root_node = nullptr;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int q;
        cin >> q;
        root_node = insert(root_node, q);
    }
    
    return 0;
}
