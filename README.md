Least Frequently Used Cache:
Implement a LFU Cache

Step1: a Cache should provide get() and put() methods 
Step2: Select a data structure to implement a cache
             selected a 'map<key, <value, frequency>> ' data structure
Step3: Select a data structure in co-ordination with cache data structure to implement LFU page replacement policy
            selected a 'map<frequency, list<key>>'


Cache get() takes key as input and returns value(returns NULL if page is not found)

Cache put() takes key and value as inputs and returns nothing(could be improved to return any error if insertion fails for any reason)
