#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

#include "student_code.h"



bool is_entry_valid(PageTableEntry pte) {
   return (is_bit_set(pte, VALID_BIT)); 
   //good
}

bool is_read_enabled(PageTableEntry pte) {
  // todo
   return (is_bit_set(pte,READ_BIT)); 
  // Question: Implement this function.  Hint: it can take as little as 1 line.
  //good
}

bool is_write_enabled(PageTableEntry pte) {
  // todo
   return (is_bit_set(pte,WRITE_BIT)); 
   //good
}

bool is_execute_enabled(PageTableEntry pte) {
  // todo
   return (is_bit_set(pte,EXEC_BIT)); 
  //good
}


PFN find_free_page(MMU m) {
  // todo
  for(int i = 0; i < NUM_FRAMES; i++){
    if(!m.page_used[i]){
      return i;
    }
  }
  //good
}

PFN convert_PageTableEntry_to_PFN(PageTableEntry pte) {
  // todo
  //need to get masks to get size
  //by using & it will separate the bits that we want, so if its relevant it will stay
  //lots of research to understand that from the book^^ but makes sense, why use more than we need?
  PFN pfn;
  AddressMask mask = get_mask(NUM_PFN_BITS);
  pfn = pte & mask;
  return pfn;
  //probably can make this one line, but easier to read
  //can use this style for other things
  //good
}


/*
 * Page Table Functions
 */
// Search the page table for the entry related to the given virtual address.
PageTableEntry get_pagetableentry__MMU_pagetable(MMU m, VPN vpn) {
  // todo
  for (int i = 0; i < PAGE_DIRECTORY_ENTRIES; i++){
      if(m.page_pointer[i] == vpn){
        return m.page_pointer[i];
      }
  }
  //good
}
//Map a new page into the page page table by finding a free page frame and setting the appropriate metadata.
PFN map_page__MMU_pagetable(MMU* m, VPN vpn, bool can_read, bool can_write, bool can_exec) {
  // todo
  PFN free_pfn = find_free_page(*m);
  PageTableEntry pte = get_pagetableentry__MMU_pagetable(*m, vpn);

  // can_read |= READ_BIT;
  // can_write |= WRITE_BIT;
  // can_exec |= EXEC_BIT;
  //how dio i set the bits correctly, the bitwise doesnt make sense on this one?

  // return free_pfn;
  //free_pfn = (pte)(*m, vpn, can_read= READ_BIT, can_write = WRITE_BIT, can_exec = EXEC_BIT);

//could not get
}


/*
 * Page Directory Functions
 */
//Given an MMU and a virtual address search for the corresponding page in physical memory.  Note that checks should be done for permissions.
Page* get_page(MMU m, VirtualAddress va, bool for_read, bool for_write, bool for_execute) {
  // todo
  //VPN = (VirtualAddress & VPN_MASK) >> SHIFT - this is offfset according to book
  AddressMask vpn_mask = get_mask(NUM_VPN_BITS);
  VPN vpn = (va & vpn_mask) >> NUM_OFFSET_BITS;

  PageTableEntry pte = get_pagetableentry__MMU_pagetable(m, vpn);

  if(!for_read ){
    printf("Error!!!! for_read\n");
  }else if(!for_write){
    printf("Error!!!! for_write\n");
  }else if(!for_execute){
    printf("Error!!!! for_exec\n");
  }
  PFN pfn = convert_PageTableEntry_to_PFN(pte);

  //pass by reference to return correctly
  Page * page = &(m.physical_memory[pfn * PAGE_SIZE]);
  //size is important!! this happened on last assignment, multiply by size.
  return page;
}

//Read a byte from the given virtual address
char read_byte(MMU m, VirtualAddress va) {
  // todo
  AddressMask vpn_mask = get_mask(NUM_VPN_BITS);
  VPN vpn = (va & vpn_mask) >> NUM_OFFSET_BITS;

  PageTableEntry pte = get_pagetableentry__MMU_pagetable(m, vpn);

  //testing purposes
  if(!is_bit_set(pte, READ_BIT)){
    printf("error read_byte\n");
  }
  PFN pfn = convert_PageTableEntry_to_PFN(pte);
  AddressMask offset_mask = get_mask(NUM_OFFSET_BITS);
  OFFSET offset = va  & offset_mask;
  PhysicalAddress pa = (pfn << NUM_OFFSET_BITS) | offset;
  //this will help from book
  //offset = VirtualAddress & OFFSET_MASK
  //PhysAddr = (PFN << SHIFT) | offset
  char read_byte = m.physical_memory[pa];
  return read_byte;

}
//Write a byte to the given virtual address

void write_byte(MMU m, VirtualAddress va, char val) {
  // todo
  AddressMask vpn_mask = get_mask(NUM_VPN_BITS);
  VPN vpn = (va & vpn_mask) >> NUM_OFFSET_BITS;

  PageTableEntry pte = get_pagetableentry__MMU_pagetable(m, vpn);

  //testing purposes
  if(!is_bit_set(pte, WRITE_BIT)){
    printf("error write_byte\n");
  }
  PFN pfn = convert_PageTableEntry_to_PFN(pte);
  AddressMask offset_mask = get_mask(NUM_OFFSET_BITS);
  OFFSET offset = va  & offset_mask;
  PhysicalAddress pa = (pfn << NUM_OFFSET_BITS) | offset;
  //offset = VirtualAddress & OFFSET_MASK
  //PhysAddr = (PFN << SHIFT) | offset
  //same as before except we set equal to the val
  m.physical_memory[pa] = val;

}
