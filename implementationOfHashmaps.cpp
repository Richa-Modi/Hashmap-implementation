#include <iostream>
#include <string>
using namespace std;
// For the nodes of the linked list whose address of head will be stored in the bucketArray
template <typename V>
class mapNode
{
public:
    string key;
    V value;
    mapNode *next;

    mapNode(string key, V value)
    {
        this->key = key;
        this->value = value;
        next = NULL;
    }
    ~mapNode()
    {
        delete next; //recursive destructor
    }
};
//For our hashmap implementation
template <typename V>
class ourmap
{
    mapNode<V> **buckets;
    int count;
    int numBuckets;

    int getBucketIndex(string key)
    {
        int hashCode;
        int currentCoeff = 1;
        for (int i = key.size() - 1; i >= 0; i--)
        {
            hashCode += key[i] * currentCoeff; // hash function
            hashCode %= numBuckets;
            currentCoeff *= 37; // base
            currentCoeff %= numBuckets;
        }

        return hashCode % numBuckets; //compression function
    }

public:
    ourmap()
    {
        count = 0;
        numBuckets = 5;
        buckets = new mapNode<V> *[numBuckets];
        for (int i = 0; i < numBuckets; i++)
        { //because garbage addresses are dangerous
            buckets[i] = NULL;
        }
    }
    ~ourmap()
    {
        for (int i = 0; i < numBuckets; i++)
        {
            delete buckets[i];
        }
        delete[] buckets;
    }
    void getLoadFactor()
    {
        cout << "Load Factor : " << (1.0) * count / numBuckets << endl;
    }
    int size()
    {
        return count;
    }
    void rehash()
    {
        cout << "\n---------REHASHING: NEW ADDRESSES WILL NOW BE ALLOTTED----------" << endl;
        mapNode<V> **temp = buckets;
        buckets = new mapNode<V> *[2 * numBuckets];
        for (int i = 0; i < 2 * numBuckets; i++)
        {
            buckets[i] = NULL;
        }
        int oldNumBuckets = numBuckets;
        numBuckets *= 2;
        count = 0;

        for (int i = 0; i < oldNumBuckets; i++)
        {
            mapNode<V> *head = temp[i];
            while (head != NULL)
            {
                string key = head->key;
                V value = head->value;
                insert(key, value);
                head = head->next;
            }
        }
        for (int i = 0; i < oldNumBuckets; i++)
        {
            mapNode<V> *head = temp[i];
            delete head;
        }
        delete[] temp;
    }
    void insert(string key, V value)
    {
        int bucketIndex = getBucketIndex(key);
        mapNode<V> *head = buckets[bucketIndex];
        while (head != NULL)
        {
            if (head->key == key)
            {
                head->value = value;
                return;
            }
            head = head->next;
        }
        head = buckets[bucketIndex];
        mapNode<V> *node = new mapNode<V>(key, value);
        node->next = head;
        buckets[bucketIndex] = node;
        cout << endl;
        cout << " new element with hash code: " << bucketIndex << " inserted at address: " << node << endl;
        count++; //total number of entries in the hashmap
        double loadFactor = (double)count / numBuckets;
        if (loadFactor > 0.7)
        {
            rehash();
        }
    }
    V getValue(string key)
    {
        int bucketIndex = getBucketIndex(key);
        mapNode<V> *head = buckets[bucketIndex];
        while (head != NULL)
        {
            if (head->key == key)
            {
                return head->value;
            }
            head = head->next;
        }

        return -1;
    }
    V remove(string key)
    {
        int bucketIndex = getBucketIndex(key);
        mapNode<V> *head = buckets[bucketIndex];
        mapNode<V> *previous = NULL;
        while (head != NULL)
        {
            if (head->key == key)
            {
                if (previous == NULL)
                {
                    buckets[bucketIndex] = head->next;
                }
                else
                {
                    previous->next = head->next;
                }
                V value = head->value;
                head->next = NULL;
                delete head;
                count--;

                return value;
            }
            previous = head;
            head = head->next;
        }

        return -1;
    }
};

int main()
{
    ourmap<int> map;
    for (int i = 0; i < 10; i++)
    {
        char c = '0' + i;
        string key = "abc";
        key = key + c;
        int value = i + 1;
        map.insert(key, value);
        map.getLoadFactor();
    }

    map.remove("abc1");
    map.remove("abc5");
    for (int i = 0; i < 10; i++)
    {
        char c = '0' + i;
        string key = "abc";
        key = key + c;
        cout << "Key: " << key << "  "
             << "Value: " << map.getValue(key) << endl;
    }

    cout << "Size of the hashmap: " << map.size() << endl;
}