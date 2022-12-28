//Memory Mapping Utility (AARCH64)

#ifndef MMU_USR_H
#define MMU_USR_H

#include <stdbool.h>
#include <stdint.h>

bool mmu_is_active(void);
bool mmu_init(void);
uint64_t mmu_get_phys_from_virt(void *virtaddr);
void *mmu_get_virt_from_phys(uint64_t physaddr);

#endif
