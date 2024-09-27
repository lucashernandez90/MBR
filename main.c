#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MBR_SIZE 512
#define PARTITION_ENTRY_OFFSET 446
#define PARTITION_ENTRY_SIZE 16
#define MAX_PARTITIONS 4

typedef struct {
    uint8_t boot_indicator;   // Boot indicator (0x80 for bootable)
    uint8_t start_chs[3];    // Starting CHS address
    uint8_t partition_type;    // Partition type
    uint8_t end_chs[3];      // Ending CHS address
    uint32_t start_lba;      // Starting LBA (Logical Block Addressing)
    uint32_t num_sectors;     // Number of sectors in the partition
} __attribute__((packed)) PartitionEntry;

void print_partition_info(PartitionEntry *entry, int index) {
    if (entry->boot_indicator == 0x80) {
        printf("Partition %d: Bootable\n", index + 1);
    } else {
        printf("Partition %d: Not bootable\n", index + 1);
    }
    printf("  Partition Type: 0x%02X\n", entry->partition_type);
    printf("  Start LBA: %u\n", entry->start_lba);
    printf("  Number of Sectors: %u\n", entry->num_sectors);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s/home/lucas/MBR/mbr.bin\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *mbr_file = fopen(argv[1], "rb");
    if (!mbr_file) {
        perror("Erro ao abrir o arquivo MBR");
        return EXIT_FAILURE;
    }

    uint8_t mbr[MBR_SIZE];
    fread(mbr, sizeof(uint8_t), MBR_SIZE, mbr_file);
    fclose(mbr_file);

    printf("Análise do MBR:\n");

    for (int i = 0; i < MAX_PARTITIONS; i++) {
        PartitionEntry *entry = (PartitionEntry *)(mbr + PARTITION_ENTRY_OFFSET + i * PARTITION_ENTRY_SIZE);
        print_partition_info(entry, i);
    }

    return EXIT_SUCCESS;
}

