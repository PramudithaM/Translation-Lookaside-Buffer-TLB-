
#include "MemSim.h"

// Constants
const int TLB_size = 16;
const int Cache_size = 64;
const int Page_table_size = 256;
const int Block_size = 4;

// TLB entry structure
struct TLB_entry
{
    bool valid;
    int page_number;
    int frame_number;
};

// Page table entry structure
struct Page_table_entry
{
    bool valid;
    int frame_number;
};

// Cache line structure
struct Cache_line
{
    bool valid;
    char data;
    int tag;
    int offset;
};

// Data structures
TLB_entry tlb[TLB_size];
Page_table_entry pageTable[Page_table_size];
Cache_line cache[Cache_size];
char main_memory[Page_table_size * Block_size];

// Function to handle TLB miss
void TLB_miss(int page_number, int offset)
{
    // Check page table
    if (pageTable[page_number].valid)
    {
        // Update TLB with page table entry
        int TLB_index = page_number % TLB_size;
        tlb[TLB_index].valid = true;
        tlb[TLB_index].page_number = page_number;
        tlb[TLB_index].frame_number = pageTable[page_number].frame_number;

        // Read data from main memory into cache
        int Cache_index = pageTable[page_number].frame_number * Block_size;
        for (int i = 0; i < Block_size; i++)
        {
            cache[Cache_index + i].valid = true;
            cache[Cache_index + i].tag = page_number;
            cache[Cache_index + i].offset = i;
            cache[Cache_index + i].data = main_memory[(page_number * Block_size) + i];
        }
    }
    else
    {
        // Page fault, handle accordingly
        std::cout << "Page fault occurred for page " << page_number << std::endl;
    }
}

// Function to simulate memory read
char Read_memory(int virtual_address)
{
    // Extract page number and offset from virtual address
    int page_number = virtual_address / Block_size;
    int offset = virtual_address % Block_size;

    // Check TLB for translation
    int TLB_index = page_number % TLB_size;
    if (tlb[TLB_index].valid && tlb[TLB_index].page_number == page_number)
    {
        // Translation found in TLB, retrieve data from cache
        int Cache_index = tlb[TLB_index].frame_number * Block_size;
        return cache[Cache_index + offset].data;
    }
    else
    {
        // TLB miss, handle accordingly
        TLB_miss(page_number, offset);

        // Check TLB again
        if (tlb[TLB_index].valid && tlb[TLB_index].page_number == page_number)
        {
            // Translation found in TLB, retrieve data from cache
            int Cache_index = tlb[TLB_index].frame_number * Block_size;
            return cache[Cache_index + offset].data;
        }
        else
        {
            // Page mapping not found in TLB and page table, handle accordingly
            std::cout << "Page mapping not found for virtual address " << virtual_address << std::endl;
            return '\0';
        }
    }
}

int main()
{
    // Perform memory reads
    char data_1 = Read_memory(0);
    char data_2 = Read_memory(100);
    char data_3 = Read_memory(500);

    // Print the retrieved data
    std::cout << "Data_1: " << data_1 << std::endl;
    std::cout << "Data_2: " << data_2 << std::endl;
    std::cout << "Data_3: " << data_3 << std::endl;

    return 0;
}