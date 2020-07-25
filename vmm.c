#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

//Bit mask for masking out lower 8 bits (0-7)
#define MASK_FRAME_NUMER 0x000000FF
//Bit mask for masking out bits 8-15
#define MASK_PAGE_NUMBER 0x0000FF00
//macro to extract bits 8-15 from a number, then shift right by 8 bits
#define get_page_num(num) ((num & MASK_PAGE_NUMBER) >> 8)
//macro to extract bits 0-15 from a number
#define get_frame_offset(num) (num & MASK_FRAME_NUMER)

#define PAGETABLESIZE 256
#define PAGESIZE 256
#define FRAMECOUNT 256
#define FRAMESIZE 256

#define TLBSIZE 16

struct PageTableEntry
{
    int page_number;
    int frame_number;
};
// array of PageTableEntry to make up page table.
struct PageTableEntry page_table[PAGETABLESIZE];

struct Frame
{
    signed char data[FRAMESIZE];
    int last_accessed;
};
//                             Frame # --> Frame data
struct Frame physical_memory[FRAMECOUNT];

struct TLBEntry
{
    int page_number;
    int frame_number;
    int last_accessed;
};
struct TLBEntry tlb[TLBSIZE];

/**
 * THese two variables can be used to make the next available frame 
 * or page table entry.
 */ 
int availableFrameIndex = 0;
int availablePageTableIndex = 0;


/**
 * takes 2 commands line args
 * 1 --> addresses.txt
 * 2 --> BACKING_STORE.bin
 * */
int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Error Invalid Arguments!!!\nCorrect Usage:\n\t./vmm addressfile backstorefile\n\n");
        exit(-1);
    }

    //address translation variables
    unsigned int logical_address;
    int frame_offset; 
    int page_number;
    //data read from "physical memory", must be signed type
    signed char data_read;
    
    char *address_text_filename;
    char *backing_store_filename;
    //statistic variables
    int page_fault = 0;
    int tlb_hits = 0;
    int num_addresses_translated = 0;
    
    address_text_filename = argv[1];
    backing_store_filename = argv[2];

    //init physical memory
    for (int i = 0; i < FRAMECOUNT; i++)
    {
        memset(physical_memory[i].data, 0, FRAMESIZE);
        physical_memory[i].last_accessed = -1;
    }

    //init Page Table
    for (int i = 0; i < PAGETABLESIZE; i++)
    {
        page_table[i].page_number = -1;
        page_table[i].frame_number = -1;
    }

    //init tlb
    for (int i = 0; i < TLBSIZE; i++)
    {
        tlb[i].page_number = -1;
        tlb[i].frame_number = -1;
        tlb[i].last_accessed = -1;
    }

    FILE *addresses_to_translate = fopen(address_text_filename, "r");
    if (addresses_to_translate == NULL)
    {
        perror("could not open address file");
        exit(-2);
    }

    FILE *backing_store_file = fopen(backing_store_filename, "rb");
    if (backing_store_file == NULL)
    {
        perror("could not open backingstore");
        exit(-2);
    }

    while (fscanf(addresses_to_translate, "%d\n", &logical_address) == 1)
    {
        num_addresses_translated++;
        page_number = get_page_num(logical_address);
        frame_offset = get_frame_offset(logical_address);

        printf("Virtual Address %5d, Page Number: %3d, Fame Offset: %3d\n", logical_address, page_number, frame_offset);

        //printf("Virtual address: %d Physical address: %d Value: %d\n", logical_address, (physical_address << 8) | frame_offset, data_read);
    }

    float hit_rate = (float)tlb_hits / num_addresses_translated;
    float fault_rate = (float)page_fault / num_addresses_translated;
    printf("Hit Rate %0.2f\nPage Fault Rate: %0.2f\n", hit_rate, fault_rate);
    fclose(addresses_to_translate);
    fclose(backing_store_file);
}
