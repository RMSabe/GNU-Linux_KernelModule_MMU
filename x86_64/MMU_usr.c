//Memory Mapping Utility (x86_64)

#include "MMU_usr.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#define MMU_WAIT_KERNEL_RESPONSE

#define MMU_PROC_FILE_DIR "/proc/MMU"
#define MMU_WAIT_TIME_US 1

#define MMU_DATAIO_SIZE_BYTES 9

#define MMU_CMD_GET_PHYSICAL_ADDR 1
#define MMU_CMD_GET_VIRTUAL_ADDR 2

#define MMU_CMD_KERNEL_RESPONSE 0xFF

int mmu_proc_fd = -1;
void *mmu_data_io = NULL;

void mmu_wait(void)
{
	clock_t start_time = clock();
	while(clock() < (start_time + MMU_WAIT_TIME_US));
	return;
}

bool mmu_is_active(void)
{
	return (mmu_proc_fd >= 0);
}

bool mmu_init(void)
{
	if(mmu_is_active()) return true;

	mmu_proc_fd = open(MMU_PROC_FILE_DIR, O_RDWR);
	if(mmu_proc_fd < 0) return false;

	mmu_data_io = malloc(MMU_DATAIO_SIZE_BYTES);
	return true;
}

#ifdef MMU_WAIT_KERNEL_RESPONSE
void mmu_call_kernel(void)
{
	uint8_t *pbyte = (uint8_t*) mmu_data_io;
	write(mmu_proc_fd, mmu_data_io, MMU_DATAIO_SIZE_BYTES);

	do{
		read(mmu_proc_fd, mmu_data_io, MMU_DATAIO_SIZE_BYTES);
	}while(pbyte[0] != MMU_CMD_KERNEL_RESPONSE);

	return;
}
#else
void mmu_call_kernel(void)
{
	write(mmu_proc_fd, mmu_data_io, MMU_DATAIO_SIZE_BYTES);
	mmu_wait();
	read(mmu_proc_fd, mmu_data_io, MMU_DATAIO_SIZE_BYTES);
	return;
}
#endif

uint64_t mmu_get_phys_from_virt(void *virtaddr)
{
	uint8_t *pbyte = (uint8_t*) mmu_data_io;
	uint64_t *pulong = (uint64_t*) &pbyte[1];
	pbyte[0] = MMU_CMD_GET_PHYSICAL_ADDR;
	pulong[0] = (uint64_t) virtaddr;

	mmu_call_kernel();
	return pulong[0];
}

void *mmu_get_virt_from_phys(uint64_t physaddr)
{
	uint8_t *pbyte = (uint8_t*) mmu_data_io;
	uint64_t *pulong = (uint64_t*) &pbyte[1];
	pbyte[0] = MMU_CMD_GET_VIRTUAL_ADDR;
	pulong[0] = physaddr;

	mmu_call_kernel();
	return (void*) pulong[0];
}

