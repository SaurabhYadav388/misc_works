#include <iostream>
#include<map>

const int memBlockSize = 1000;
class MemoryManager
{
    static char memoryBlock[memBlockSize];
    
    //maps starting index and size of allocated space
    static std::map<int, int> allocBlockMap; 
    
    static MemoryManager* memManagerObj;

    MemoryManager()
    {
    }

    //calculate the best fit free space block in complete memory
    std::pair<int, int> calcBestFitSpace(int bytes)
    {
        std::pair<int, int> currBestFit = { INT_MAX,INT_MAX };
        int startFreeSpace = 0;
        for (auto block : allocBlockMap)
        {
            int freeBlockSize = block.first - startFreeSpace;

            if (freeBlockSize == bytes)
            {
                return { startFreeSpace,bytes };
            }
            else if (freeBlockSize > bytes)
            {
                if (freeBlockSize < currBestFit.second)
                {
                    currBestFit = { startFreeSpace,freeBlockSize };
                }
            }
            startFreeSpace = block.first + block.second;
        }

        //checking free space at the end
        if (memBlockSize - startFreeSpace == bytes)
        {
            return { startFreeSpace,bytes };
        }
        else if (memBlockSize - startFreeSpace > bytes)
        {
            if (memBlockSize - startFreeSpace < currBestFit.second)
            {
                currBestFit = { startFreeSpace,memBlockSize - startFreeSpace };
            }
        }

        return currBestFit;
    }

public:

    //return the singleton object
    static MemoryManager* getObject()
    {
        if (memManagerObj == nullptr)
        {
            memManagerObj = new MemoryManager();
        }
        return memManagerObj;
    }

    //prints the current memory allocation map
    void printAllocBlockMap()
    {
        std::cout << "Allocation Map Entries:" << std::endl;
        for (auto ele : allocBlockMap)
            std::cout << "index: " << ele.first << " size: " << ele.second << std::endl;

        std::cout << std::endl;
    }

    /*
    allocates the 'bytes' size of block
    returns the base address of block if sucessfull and nullptr otherwise
    */
    void* allocate(int bytes)
    {
        if (bytes > sizeof(memoryBlock))
        {
            std::cout << "Requested size too big!!" << std::endl;
            return nullptr;
        }

        //if full block is empty
        if (allocBlockMap.size() == 0)
        {
            allocBlockMap.insert({ memoryBlock[0],bytes });
            return &memoryBlock[0];
        }

        //calculate best fit free space
        std::pair<int, int> currBestFit = calcBestFitSpace(bytes);

        //if no free space for current size
        if (currBestFit == std::make_pair(INT_MAX, INT_MAX))
        {
            std::cout << "No space for current requested size!!"<<std::endl;
            return nullptr;
        }

        //allocate and return the best fit free space
        allocBlockMap.insert({ currBestFit.first,bytes });
        return &memoryBlock[currBestFit.first];
    }

    /*
    allocates the block starting from address 'ptr'
    returns 0 if sucessfull and -1 otherwise
    */
    int deallocate(void* ptr)
    {
        int index = (char*)ptr - &memoryBlock[0];
        if (allocBlockMap.find(index) == allocBlockMap.end())
        {
            std::cout << "Given allocation doesnt exist" << std::endl;
            return -1;
        }
        allocBlockMap.erase(index);
        return 0;
    }

    /*
    reallocates the block starting from address 'ptr' to a new size ('newBytes')
    and returns the new base address of block if sucessfull and nullptr otherwise
    */
    void* reallocate(void* ptr, int newBytes)
    {
        //if the ptr is nullptr,it reserves a block of storage of 'newBytes' bytes.
        if (ptr == nullptr)
            return allocate(newBytes);

        //if pointer(ptr) used that doesn't point to a ptr created previously by allocate(),then return nullptr :
        int index = (char*)ptr - &memoryBlock[0];
        if (allocBlockMap.find(index) == allocBlockMap.end())
        {
            std::cout << "Given allocation doesnt exist" << std::endl;
            return nullptr;
        }
        //if size is 0 and ptr is not nullptr, the ptr memory block is freed and nullptr is returned.
        if (newBytes == 0)
        {
            deallocate(ptr);
            return nullptr;
        }

        //if the 'newByte' size is smaller than previous 'ptr' memory block,then shrink size
        if (newBytes <= allocBlockMap[index])
        {
            allocBlockMap[index] = newBytes;
        }

        //if the newSize is bigger than before:

        //firstly, try to scale memory at same position:
        auto currEntry=allocBlockMap.find(index);
        auto nextEntry = std::next(currEntry);

        int currBlockSize = allocBlockMap[index];

        int spaceLeft = (*nextEntry).first - (index + currBlockSize);

        if (spaceLeft >= newBytes - currBlockSize)
        {
            allocBlockMap[index] = newBytes;
            return &memoryBlock[index];
        }
        
        //if, can't scale at same position then, try to allocate best fit free block 
        
        //  temporarily deallocate current ptr mem. block
        int prevBlockSize = allocBlockMap[index];
        allocBlockMap.erase(index);
        //  then,calc current best fit for new bigger block
        std::pair<int, int> currBestFit = calcBestFitSpace(newBytes);
        //  if no free space for current size
        if (currBestFit == std::make_pair(INT_MAX, INT_MAX))
        {
            std::cout << "No space for current requested size!!" << std::endl;
            //-reassign the previous memory
            allocBlockMap[index] = prevBlockSize;

            return nullptr;
        }

        //else, allocation is possible then allocate
        allocBlockMap.insert({ currBestFit.first,newBytes });
        return &memoryBlock[currBestFit.first];
    }
};

//initializing static variables to assign memory to them
char MemoryManager::memoryBlock[memBlockSize] = {};
std::map<int, int> MemoryManager::allocBlockMap;
MemoryManager* MemoryManager::memManagerObj = nullptr;

int main()
{
    MemoryManager* mmObj = MemoryManager::getObject();
    //Allocating memories
    void* ptr = mmObj->allocate(400);
    void* ptr1 = mmObj->allocate(200);
    void* ptr2 = mmObj->allocate(100);
    void* ptr3 = mmObj->allocate(300);

    //printing the allocation map
    mmObj->printAllocBlockMap();

    //deallocating memory
    mmObj->deallocate(ptr);
    mmObj->deallocate(ptr2);

    //printing the updated allocation map
    mmObj->printAllocBlockMap();

    //Reallocating the above ptr1 memory
    mmObj->reallocate(ptr1, 300);

    //printing the updated allocation map
    mmObj->printAllocBlockMap();

    //Allocating memory and assigning data
    int* ptr4 = (int*)mmObj->allocate(5 * sizeof(int));
    for (int i = 0; i < 5; i++)
        ptr4[i] = i+1;

    //printing the updated allocation map
    mmObj->printAllocBlockMap();

    getchar();
    return 0;
}
