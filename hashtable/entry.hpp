template<typename K, typename V>
struct Entry{
    K key;
    V value;
    Entry(){key = K(); value = V();};
    Entry(Entry<K, V> const& e)
    {
        key = e.key;
        value = e.value;
    }

    //Assume that class K has an order
    bool operator<(Entry<K,V> const& e) {return key < e.key;}
    bool operator>(Entry<K,V> const& e) {return key > e.key;}
    bool operator==(Entry<K,V> const& e) {return key == e.key;}
    bool operator!=(Entry<K,V> const& e) {return key != e.key;}
};