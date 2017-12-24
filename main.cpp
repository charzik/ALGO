#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

struct Node {
    Node(int val, Node* next): val(val), next(next)
        {}
    int val;
    Node* next;
};

class ListIterator {
public:
    ListIterator(Node* node): node(node)
        {}

    int& operator*() const {
        return node->val;
    }

    ListIterator& operator++() {
        node = node->next;
        return *this;
    }

    bool operator==(ListIterator other) const {
        return node == other.node;
    }

    bool operator!=(ListIterator other) const {
        return node != other.node;
    }

private:
    Node* node;
};

class List {
public:
    List(): first(nullptr), last(nullptr)
        {}

    void push(int val) {
        Node* cur = new Node(val, nullptr);
        if (first == nullptr) {
            first = last = cur;
        }
        last->next = cur;
        last = cur;
    }

    ListIterator begin() const {
        return {first};
    }

    ListIterator end() const {
        return {last->next};
    }

    ~List() {
        auto cur = first;
        while (cur != nullptr) {
            auto tmp = cur->next;
            delete cur;
            cur = tmp;
        }
    }

private:
    Node* first;
    Node* last;
};

template<typename Iter, typename OIter>
void Copy(Iter L, Iter R, OIter out) {
    while (L != R) {
        *out = *L;
        ++L;
        ++out;
    }
}

template<typename Iter1, typename Iter2, typename OIter>
void Merge(Iter1 L1, Iter2 R1, Iter2 L2, Iter2 R2, OIter out) {
    while (L1 != R1 && L2 != R2) {
        if (*L2 < *L1) {
            *out = *L2;
            ++L2;
        } else {
            *out = *L1;
            ++L1;
        }
        ++out;
    }
    Copy(L1, R1, out);
    Copy(L2, R2, out);
}

template<typename Iter>
void mergeSort(Iter L, Iter R) {
    auto check = L;
    ++check;
    if (R == check)
        return;

    int n = 0;
    auto l = L;
    auto r = R;
    while (l != r) {
        ++n;
        ++l;
    }
    auto M = L;
    for (int i = 0; i < n / 2; ++i)
        ++M;
    mergeSort(L, M);
    mergeSort(M, R);
    vector<int> c(n);
    Merge(L, M, M, R, c.begin());
    Copy(c.begin(), c.end(), L);
}

int main() {
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    int n;
    cin >> n;
    List L;
    for (int i = 0; i < n; ++i) {
        int a;
        cin >> a;
        L.push(a);
    }
    mergeSort(L.begin(), L.end());
    auto it = L.begin();
    while (it != nullptr) {
        cout << *it << ' ';
        ++it;
    }
    return 0;
}
