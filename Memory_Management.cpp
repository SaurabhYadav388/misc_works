//Allocation done ,deallocation in progress ..

#include <iostream>
#include<map>

const int memBlockSize = 1000;
class MemoryManager
{
    static char memoryBlock[memBlockSize];
    static std::map<int, int> allocBlockMap;//maps starting index and size of alloc space 
    static MemoryManager* memManagerObj;
    MemoryManager()
    {
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
        void* ptr = &memoryBlock[0];
        std::cout << "Base memory adress:" << ptr;
    }
    void printAllocBlockMap()
    {
        for (auto ele : allocBlockMap)
            std::cout << "address: " << ele.first << " size: " << ele.second << std::endl;
    }

    void* allocate(int bytes)
    {
        if (bytes > sizeof(memoryBlock))
        {
            std::cout << "Requested size too big!!\n";
            return nullptr;
            //return -1;
        }

        //if full block is empty
        if (allocBlockMap.size() == 0)
        {
            allocBlockMap.insert({ memoryBlock[0],bytes });
            return &memoryBlock[0];
            //return 0;
        }

        //calculate best fit free space
        std::pair<int, int> currBestFit = { INT_MAX,INT_MAX };
        int startFreeSpace = 0;
        for (auto block : allocBlockMap)
        {
            int freeBlockSize = block.first - startFreeSpace;
            
            if (freeBlockSize == bytes)
            {
                allocBlockMap.insert({ startFreeSpace,bytes });
                return &memoryBlock[startFreeSpace];
                //return startFreeSpace;
            }
            else if (freeBlockSize > bytes)
            {
                if (freeBlockSize < currBestFit.second)
                {
                    currBestFit = { startFreeSpace,freeBlockSize };
                }
            }
            //startFreeSpace += block.second;
            startFreeSpace = block.first+block.second;
        }

        //checking end space
        if (memBlockSize - startFreeSpace == bytes)
        {
            allocBlockMap.insert({ startFreeSpace,bytes });
            return &memoryBlock[startFreeSpace];
            //return startFreeSpace;
        }
        else if (memBlockSize - startFreeSpace > bytes)
        {
            if (memBlockSize - startFreeSpace < currBestFit.second)
            {
                currBestFit = { startFreeSpace,memBlockSize - startFreeSpace };
            }
        }

        if (currBestFit == std::make_pair(INT_MAX, INT_MAX))
        {
            std::cout << "no space for current size";
            return nullptr;
            //return -1;
        }

        allocBlockMap.insert({ currBestFit.first,bytes });
        return &memoryBlock[currBestFit.first];
        //return currBestFit.first;
    }

    int deallocate(void* blockBasePtr)
    {
        int index = (char*)blockBasePtr - &memoryBlock[0];
        if (allocBlockMap.find(index) == allocBlockMap.end())
        {
            std::cout << "Given allocation doesnt exist";
            return -1;
        }
        allocBlockMap.erase(index);
    }
};

//initialize to assign memory to static variables
char MemoryManager::memoryBlock[memBlockSize] = {};
std::map<int, int> MemoryManager::allocBlockMap;
MemoryManager* MemoryManager::memManagerObj = nullptr;

int main()
{
    MemoryManager* mmObj = MemoryManager::getObject();

    //void* ptr = mmObj->allocate(1000);
    /*int ind = mmObj->allocate(100);
    std::cout << ind << "\n";
    int ind1 = mmObj->allocate(700);
    std::cout << ind1 << "\n";
    int ind2 = mmObj->allocate(100);
    std::cout << ind2 << "\n";
    int ind3 = mmObj->allocate(50);
    std::cout << ind3 << "\n";
    int ind4 = mmObj->allocate(50);
    std::cout << ind4 << "\n";
    
    mmObj->printAllocBlockMap();
    std::cout << std::endl;
    mmObj->deallocate(ind1);
    mmObj->deallocate(ind4);

    mmObj->printAllocBlockMap();
    std::cout << std::endl;
    
    mmObj->allocate(50);
    mmObj->printAllocBlockMap();
    std::cout << std::endl;*/

    void* ind = mmObj->allocate(100);
    std::cout << ind << "\n";
    void* ind1 = mmObj->allocate(700);
    std::cout << ind1 << "\n";
    void* ind2 = mmObj->allocate(100);
    std::cout << ind2 << "\n";
    void* ind3 = mmObj->allocate(50);
    std::cout << ind3 << "\n";
    void* ind4 = mmObj->allocate(50);
    std::cout << ind4 << "\n";

    mmObj->printAllocBlockMap();
    std::cout << std::endl;
    mmObj->deallocate(ind1);
    mmObj->deallocate(ind4);

    mmObj->printAllocBlockMap();
    std::cout << std::endl;

    mmObj->allocate(50);
    mmObj->printAllocBlockMap();
    std::cout << std::endl;

   /* int* ptr = (int*)mmObj->allocate(5 * sizeof(int));

    for (int i = 0; i < 5; i++)
        ptr[i] = i+1;
    for (int i = 0; i < 5; i++)
        std::cout << ptr[i];*/



}
