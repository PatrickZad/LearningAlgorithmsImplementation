#include "kdtree.h"
#include <cstdlib>
using namespace patrick;

/*
 *Construct KDTree
 *last dimension of a Vector is supposed to be its lable
 */
KDTree::KDTree(std::vector<Vector>& data)
{
    root=new KDNode{};
    buildTree(root,data,0);
}

KDTree::~KDTree()
{
    releaseTree(root);
}

void KDTree::releaseTree(KDNode* root)
{
    if (root->left!=nullptr)
    {
        releaseTree(root->left);
    }
    if (root->right!=nullptr)
    {
        releaseTree(root->right);
    }
    delete root;
}

std::vector<Vector> KDTree::searchNN(Vector& sample, unsigned long k){
    //TODO
}

void KDTree::buildTree(KDNode* parent,std::vector<Vector>& dataLeft,unsigned long depth)
{
    std::vector<Vector> left{};
    std::vector<Vector> right{};
    unsigned long dimension=depth%(dataLeft[0].size()-1);
    parent->partitionDim=dimension;
    double mid=getMid(dataLeft,dimension);
    for (Vector vec : dataLeft)
    {
        if (vec[dimension]>mid)
        {
            right.push_back(vec);
        }else
            if (vec[dimension]<mid)
            {
                left.push_back(vec);
            }else
            {
                parent->data.push_back(vec);
            }
    }
    if (left.size()>0)
    {
        KDNode* leftNode=new KDNode{};
        parent->left=leftNode;
        leftNode->parent=parent;
        buildTree(leftNode,left,depth++);
    }
    if (right.size()>0)
    {
        KDNode* rightNode=new KDNode{};
        parent->right=rightNode;
        rightNode->parent=parent;
        buildTree(rightNode,right,depth++);
    }
}

double patrick::getMid(std::vector<Vector>& collection, unsigned long dimension)
{
    unsigned long start=0;
    unsigned long end=collection.size();
    unsigned long expect=collection.size()/2;
    unsigned long partitioned=partition(collection,start,end,dimension);
    while (partitioned!=expect)
    {
        if (partitioned>expect)
        {
            end=partitioned;
        }else
        {
            start=partitioned;
        }
        partitioned=partition(collection,start,end,dimension);
    }
    return collection[expect][dimension];
}

unsigned long patrick::partition(std::vector<Vector>& collection, unsigned long start, unsigned long end, unsigned long dimension)
{
    if (start==end)
    {
        return start;
    }
    unsigned long randIndex=std::rand()%(end-start);
    Vector temp=collection[start];
    collection[start]=collection[randIndex];
    collection[randIndex]=temp;
    unsigned long left=start+1;
    unsigned long right=end;
    while (left<=end)
    {
        while (collection[left][dimension]<=collection[start][dimension])
        {
            left++;
        }
        while (collection[right][dimension]>collection[start][dimension])
        {
            right--;
        }
        if (left<right)
        {
            temp=collection[left];
            collection[left]=collection[right];
            collection[right]=temp;
        }
    }
    temp=collection[left-1];
    collection[left-1]=collection[start];
    collection[start]=temp;
    return left-1;
}

KDNode* findArea(Vector& sample, KDNode* root){
    KDNode* area=root;
    KDNode* nextArea=root;
    while (nextArea!=nullptr)
    {
        area=nextArea;
        unsigned long dim=area->partitionDim;
        if (sample[dim]<area->data[0][dim])
        {
            nextArea=area->left;
        }else{
            nextArea=area->right;
        }
    }
    return area;
}

DistanceHeap::DistanceHeap(unsigned long length):dataLength{0}, arrayLength{length}
{
    array=new  HeapItem[length];
    for (unsigned long i = 0; i < length; i++)
    {
        (array+i)->distance=-1;
        (array+i)->dataPtr=nullptr;
    }
}
    
DistanceHeap::~DistanceHeap()
{
    delete[] array;
    array=nullptr;
    dataLength=0;
    arrayLength=0;
}

void DistanceHeap::push(double distance, Vector* dataPtr)
{
    HeapItem item{distance,dataPtr};
    if (dataLength<arrayLength)
    {
        *(array+dataLength)=item;
        dataLength++;
        heapifyDownTop(dataLength-1);
    }else{
        *array=item;
        heapifyTopDown(0);
    }
}

double DistanceHeap::topDistance()
{
    return array[0].distance;
}

void DistanceHeap::heapifyTopDown(unsigned long index)
{
    unsigned long largest=index;
    unsigned long left=index*2+1;
    unsigned long right=2*(index+1);
    if (left<dataLength && (array+left)->distance>(array+index)->distance){
        largest=left;
    }
    if (right<dataLength && (array+right)->distance>(array+largest)->distance){
        largest=right;
    }
    if (largest!=index){
        auto temp=array[index];
        array[index]=array[largest];
        array[largest]=temp;
        heapifyTopDown(largest);
    }
}

void DistanceHeap::heapifyDownTop(unsigned long index)
{
    unsigned long i=index;
    while (i>0 && (array+i)->distance>(array+(i-1)/2)->distance){
        HeapItem temp=*(array+(i-1)/2);
        *(array+(i-1)/2)=*(array+i);
        *(array+i)=temp;
        i=(i-1)/2;
    }
}
        