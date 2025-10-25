#pragma once
#include <string>
#include <iostream>
using namespace std;

template <typename Key, typename Val>
HashTableOpen<Key, Val>::HashTableOpen(int i) {
    if (i <= 0) i = 100;
    M = i;
    ht = new LinkedList<Record>*[M];
    for (int j = 0; j < M; j++) {
        ht[j] = new LinkedList<Record>();
    }
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>::HashTableOpen(const HashTableOpen<Key, Val>& copyObj)
: M(0), ht(nullptr) {
    copy(copyObj);
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>& HashTableOpen<Key, Val>::operator=
(const HashTableOpen<Key, Val>& rightObj) {
    if (this != &rightObj) {
        clear();
        copy(rightObj);
    }
    return *this;
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>::~HashTableOpen() {
    for (int i = 0; i < M; i++) {
        delete ht[i];
    }
    delete[] ht;
    ht = nullptr;
    M = 0;
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::clear() {
    for (int i = 0; i < M; i++) {
        ht[i]->clear();
    }
}

template <typename Key, typename Val>
Val HashTableOpen<Key, Val>::find(const Key& k) const {
    int slot = hash(k);
    LinkedList<Record>* list = ht[slot];
    int n = list->getLength();
    for (int i = 0; i < n; i++) {
        Record r = list->getElement(i);
        if (r.k == k) {
            return r.v;
        }
    }
    throw string("key not found");
}

template <typename Key, typename Val>
int HashTableOpen<Key, Val>::hash(const Key& k) const {
    const int FOLD_LEN = 4;
    unsigned* ikey = (unsigned*) k.c_str();
    int ilen = (int)k.length() / FOLD_LEN;
    unsigned sum = 0;
    for (int i = 0; i < ilen; i++) {
        sum += ikey[i];
    }
    int extra = (int)k.length() - ilen * FOLD_LEN;
    char temp[FOLD_LEN];
    ikey    = (unsigned*) temp;
    ikey[0] = 0;
    for (int i = 0; i < extra; i++) {
        temp[i] = k[ilen * FOLD_LEN + i];
    }
    sum += ikey[0];
    int slot = (M == 0) ? 0 : (int)(sum % M);
    cout << k << "\thashes to slot " << slot << endl;
    return slot;
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::insert(const Key& k, const Val& v) {
    int slot = hash(k);
    LinkedList<Record>* list = ht[slot];
    int n = list->getLength();
    for (int i = 0; i < n; i++) {
        Record r = list->getElement(i);
        if (r.k == k) {
            throw string("duplicate key");
        }
    }
    Record newRec(k, v);
    list->append(newRec);
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::remove(const Key& k) {
    int slot = hash(k);
    LinkedList<Record>* list = ht[slot];
    int n = list->getLength();
    for (int i = 0; i < n; i++) {
        Record r = list->getElement(i);
        if (r.k == k) {
            list->remove(i);
            return;
        }
    }
    throw string("key not found");
}

template <typename Key, typename Val>
int HashTableOpen<Key, Val>::size() const {
    int total = 0;
    for (int i = 0; i < M; i++) {
        total += ht[i]->getLength();
    }
    return total;
}
