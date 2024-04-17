//Allocation done ,deallocation in progress ..

#include <iostream>
#include<map>

class MemoryManager
{
    static char memoryBlock[1000];
    static std::map<int, int> allocBlockMap;//maps starting index and size of alloc space 
    static MemoryManager* memManagerObj;
    MemoryManager()
    {
        allocBlockMap = {};
    }
public:
    
    static MemoryManager* getObject()
    {
        if (memManagerObj == nullptr)
        {
            memManagerObj = new MemoryManager();
        }
        return memManagerObj;
    }
    void printBase()
    {
        void* ptr= &memoryBlock[0];
        std::cout << "base mem. adress:" << ptr;
    }
    void printAllocBlockMap()
    {
        for (auto ele : allocBlockMap)
            std::cout << "address: "<<ele.first <<" size: " <<ele.second<<std::endl;
    }

    int allocate(int bytes)
    {
        if (bytes > sizeof(memoryBlock))
        {
            std::cout << "Requested size too big!!\n";
            //return nullptr;
            return -1;
        }

        if (allocBlockMap.size() == 0)
        {
            allocBlockMap.insert({ memoryBlock[0],bytes });
            //return &memoryBlock[0];
            return 0;
        }

        //calculate best fit free space
        std::pair<int, int> currBestFit = {INT_MAX,INT_MAX};
        int last = 0;
        for (auto block : allocBlockMap)
        {
            int freeBlockSize = block.first - last;
            if (freeBlockSize == bytes)
            {
                allocBlockMap.insert({last,bytes});
                //return &memoryBlock[last];
                return last;
            }
            else if (freeBlockSize > bytes)
            {
                if (freeBlockSize < currBestFit.second)
                {
                    currBestFit = { last,freeBlockSize };
                }
            }
            last += block.second;
        }
        //checking end space
        if (1000 - last == bytes)
        {
            allocBlockMap.insert({ last,bytes });
            //return &memoryBlock[last];
            return last;
        }
        else if (1000 - last > bytes)
        {
            if (1000 - last < currBestFit.second)
            {
                currBestFit = { last,1000 - last };
            }
        }
        
        if (currBestFit == std::make_pair(INT_MAX, INT_MAX))
        {
            std::cout << "no space for current size";
            return -1;
        }

        allocBlockMap.insert({currBestFit.first,bytes});
        //return &memoryBlock[currBestFit.first];
        return currBestFit.first;
    }
};

//initialize to assign memory to static variables
char MemoryManager::memoryBlock[1000] = {};
std::map<int, int> MemoryManager::allocBlockMap;
MemoryManager* MemoryManager::memManagerObj = nullptr;

int main()
{
    MemoryManager* mmObj = MemoryManager::getObject();
    //mmObj->printBase();

    //void* ptr = mmObj->allocate(1000);
    int ind = mmObj->allocate(100);
    std::cout << ind << "\n";
    int ind1 = mmObj->allocate(700);
    std::cout << ind1 << "\n";
    int ind2 = mmObj->allocate(100);
    std::cout << ind2 << "\n";
    int ind3 = mmObj->allocate(50);
    std::cout << ind3 << "\n";
    int ind4 = mmObj->allocate(50);
    std::cout << ind4 << "\n";
    int ind5 = mmObj->allocate(1);
    std::cout << ind5 << "\n";


}
