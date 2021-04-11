#ifndef SET
#define SET

#include <algorithm>
#include <iostream>
 
namespace HseAlgo {

    template <class ValueType>
    int comp(ValueType a, ValueType b) {
        if (a < b) return -1;
        if (b < a) return 1;
        return 0;
    }


    template <class ValueType>
    class AVLnode {
    private:
        using node = AVLnode*;
        using constnode = const AVLnode*;
    public:
        ValueType key;
        int balance = 0, height = 1;
        node left = NULL, right = NULL, parent = NULL;
     
        AVLnode(ValueType k, node p = NULL) : key(k), parent(p) {}

        AVLnode(ValueType k, int h, int b) : key(k), height(h), balance(b) {}
     
        ~AVLnode() {
            delete left;
            delete right;
        }

        void nullifyChildren() {
            left = NULL;
            right = NULL;
        }

        constnode nextNode() const {
            constnode ans = this;
            if (right != NULL) {
                ans = right;
                while (ans->left != NULL) ans = ans->left;
            } else {
                node cur = ans->parent;
                while (cur != NULL && ans == cur->right) {
                    ans = cur; 
                    cur = cur->parent;
                }
                ans = cur;
            }
            return ans;
        }

        constnode prevNode() const {
            constnode ans = this;
            if (left != NULL) {
               ans = left;
               while (ans->right != NULL) ans = ans->right;
            } else {
                node cur = ans->parent;
                while (cur != NULL && ans == cur->left) {
                    ans = cur; 
                    cur = cur->parent;
                }
                ans = cur;
            }
            return ans;
        }
    };



    template <class ValueType>
    class AVLtree {
    private:
        using node = AVLnode<ValueType>*;


    public:

        AVLtree(void) {}
     
        ~AVLtree(void) {}
     
    private:

        // Rotations

        node rotateLeft(node a) {
            if (a == NULL) return a;
            node b = a->right;
            a->right = b->left;
            b->left = a;

            update(a);
            update(b);

            return b;
        }
         
        node rotateRight(node a) {
            if (a == NULL) return a;
            node b = a->left;
            a->left = b->right;
            b->right = a;

            update(a);
            update(b);

            return b;
        }
         
        node rotateLeftThenRight(node cur) {
            cur->left = rotateLeft(cur->left);
            return rotateRight(cur);
        }
         
        node rotateRightThenLeft(node cur) {
            cur->right = rotateRight(cur->right);
            return rotateLeft(cur);
        }

        // Auxiliary

        int height(node cur) {
            if (cur == NULL) return 0;
            return cur->height;
        }
         
        void update(node cur) {
            if (cur == NULL) return;
            if (cur->left) cur->left->parent = cur;
            if (cur->right) cur->right->parent = cur;
            cur->height = std::max(height(cur->left), height(cur->right)) + 1;
            cur->balance = height(cur->right) - height(cur->left);
        }

        node rebalance(node cur) {
            if (cur == NULL) return NULL;
            update(cur);
         
            if (cur->balance == -2) {
                if (cur->left->balance < 0) {
                    cur = rotateRight(cur);
                } else {
                    cur = rotateLeftThenRight(cur);
                }
            } else if (cur->balance == 2) {
                if (cur->right->balance > 0) {
                    cur = rotateLeft(cur);
                } else {
                    cur = rotateRightThenLeft(cur);
                }
            }
            return cur;
        }

    public:

        // Main functionality 

        node insertNode(node cur, const ValueType& key) {
            if (cur == NULL) {
                node tmp = nullptr;
                tmp = new AVLnode<ValueType>(key);
                return tmp;
            }
            if (comp(cur->key, key) == 1) {
                cur->left = insertNode(cur->left, key);
            } else {
                cur->right = insertNode(cur->right, key);
            }
            cur = rebalance(cur);
            return cur;
        }

        node removeMin(node cur) {
            if (cur->left == NULL) return cur->right;
            cur->left = removeMin(cur->left);
            cur = rebalance(cur);
            return cur;
        }

        node removeNode(node cur, const ValueType& key) {
            if (cur == NULL) return cur;
         
            if (comp(cur->key, key) > 0) {
                cur->left = removeNode(cur->left, key);
            } else if (comp(cur->key, key) < 0) {
                cur->right = removeNode(cur->right, key);
            } else {
                node lchild = cur->left;
                node rchild = cur->right;
                cur->nullifyChildren();
                delete cur;

                if (rchild == NULL) return lchild;
                cur = rchild;
                while (cur->left != NULL) cur = cur->left;
                cur->right = removeMin(rchild);
                cur->left = lchild;
            }
            cur = rebalance(cur);
            return cur;
        }

        // Working with iterators

        node smallest(node cur) const {
            while (cur->left != NULL) cur = cur->left;
            return cur;
        }

        node biggest(node cur) const {
            while (cur->right != NULL) cur = cur->right;
            return cur;
        }

        node begin(node root) const {
            if (root == NULL) return NULL;
            node tmp = smallest(root);
            return tmp;
        }

        node rbegin(node root) const {
            if (root == NULL) return NULL;
            node tmp = biggest(root);
            return tmp;
        }

        node lower_bound(node cur, const ValueType& key) const {
            if (cur == NULL) return NULL;

            if (comp(cur->key, key) == 1) {
                node tmp = lower_bound(cur->left, key);
                if (tmp == NULL) return cur;
                else return tmp;
            } else if (comp(cur->key, key) == -1) {
                return lower_bound(cur->right, key);
            }

            return cur;
        }

        node find(node cur, const ValueType& key) const {
            node tmp = lower_bound(cur, key);
            if (tmp == NULL || comp(tmp->key, key) != 0) return NULL;
            return tmp;
        }

        void outNode(node cur) {
            if (cur == NULL) std::cout << "none ";
            else std::cout << cur->key;
        }

        void printAll(node a) {
            if (a == NULL) {
                //std::cout << " - ";
                return;
            }
            //std::cout << " l ";
            printAll(a->left);
            std::cout << a->key << "(par=";
            outNode(a->parent);
            std::cout << "), ";
            //std::cout << " r ";
            printAll(a->right);

        }

    };


    template <class ValueType>
    class Set {
    private:
        using node = AVLnode<ValueType>*;

        AVLtree<ValueType> body;
        node root = NULL;
        size_t sz = 0;

        void rooted() {
            if (root != NULL) root->parent = NULL;
        }

    public:

        class iterator {
        private:
            using node = const AVLnode<ValueType>*;
            node _node = NULL; 
            const Set* owner = NULL;

        public:
            iterator() {}

            iterator(const node in, const Set* own) : _node(in), owner(own) {}

            iterator(const iterator& other) : _node(other._node), owner(other.owner) {}
         
            const ValueType& operator*() const {
                return _node->key;
            }
         
            const ValueType* operator->() const {
                return &(_node->key);
            }
         
            const ValueType* operator&() const {
                return (&_node)->key;
            }
         
            iterator& operator++() {
                if (_node == NULL) return *this;
                _node = _node->nextNode();
                return *this;
            }
         
            iterator& operator--() {
                if (_node == NULL) {
                    *this = owner->rbegin();
                    return *this;
                }
                node tmp = _node->prevNode();
                if (tmp != NULL) {
                    _node = tmp;
                }
                return *this;
            }

    		iterator operator++(int) {
    	        iterator cur = *this;
    	        ++*this;
    	        return cur;
    	    }

            iterator operator--(int) {
    	        iterator cur = *this;
    	        --*this;
    	        return cur;
    	    }

            bool operator==(const iterator& other) const {
                return (_node == other._node && owner == other.owner);
            }
         
            bool operator!=(const iterator& other) const {
                return (_node != other._node || owner != other.owner);
            }
        };

        Set() {}

        template<class InputIter>
        Set(InputIter st, InputIter en) {
            while(st != en) {
                try {
                    insert(*st);
                } catch (...) {
                    if (root != NULL) {
                        delete root;
                    }
                }
                ++st;
            }
        }

        Set(const std::initializer_list<ValueType>& inp) {
            *this = Set(inp.begin(), inp.end());
        }

        Set(const Set& other) {
            *this = Set(other.begin(), other.end());
        }

        ~Set() {
            delete root;
        }

        Set& operator=(const Set& other) {
            Set tmp(other); //possible problem
            std::swap(tmp.root, root);
            sz = tmp.sz;
            return *this;
        }

        iterator begin() const {
            return iterator(body.begin(root), this);
        }

        iterator end() const {
            return iterator(NULL, this);
        }

        iterator rbegin() const {
            return iterator(body.rbegin(root), this);
        }

        void print() {
            body.printAll(root);
            std::cout << "\n";
        }   

        iterator find(const ValueType& key) const {
            return iterator(body.find(root, key), this);
        }

        iterator lower_bound(const ValueType& key) const {
            return iterator(body.lower_bound(root, key), this);
        }

        void insert(const ValueType& key) {
            if (find(key) != end()) return;
            root = body.insertNode(root, key);
            rooted();
            ++sz;
        }

        void erase(const ValueType& key) {
            if (find(key) == end()) return;
            root = body.removeNode(root, key);
            rooted();
            --sz;
        }

        bool empty() const {
            return (sz == 0);
        }

        size_t size() const {
            return sz;
        }

    };
}

#endif