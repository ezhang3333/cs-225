#include "dsets.h"

void DisjointSets::addelements(int num) 
{
    for(int i = 0; i < num; i++)
    {
        set.push_back(-1);
    }
}

int DisjointSets::find(int elem) 
{
    if(set[elem] < 0)
    {
        return elem;
    }
    else
    {
        int root = find(set[elem]);
        set[elem] = root;
        return root;
    }
}

void DisjointSets::setunion(int a, int b) {
    int rootA = find(a);
    int rootB = find(b);

    if (rootA == rootB) {
        return; 
    }

    int newSize = set[rootA] + set[rootB]; 
    if (set[rootA] < set[rootB]) { 
        set[rootA] = newSize;   
        set[rootB] = rootA;      
    } else { 
        set[rootB] = newSize;  
        set[rootA] = rootB;      
    }
}



int DisjointSets::size(int elem) 
{
    int root = find(elem);
    return set[root] * -1;
}

int DisjointSets::getValue(int elem) const 
{
    return set[elem];
}