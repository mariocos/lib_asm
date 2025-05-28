#include "packer.h"

int main(int argc, char **argv)
{
	if (argc != 2 || !argv || !argv[0] || !argv[0][0]){
		write(2, "bad arguments\n", 14);
		return (1);
	}
	//initializations
	t_offsets		ptrs_to_update;
	t_ptrs			ptrs_to_old;
	int 			new_file_fd;
	
	void *map = get_map(argv[0]);
	if (map == NULL) {
		write(2, "problem creating map\n", 21);
	}

	new_file_fd = get_new_file_fd(argv[0]);
	printf("got fd [%d]\n", new_file_fd);
	if (new_file_fd < 0){
		write(2, "problem creating new file\n", 27);
		return (-1);
	}

	//parse header
	parse_header(&ptrs_to_old, &ptrs_to_update, map, new_file_fd);
	
	ptrs_to_update.bites_written = write(new_file_fd, (char*)(map + sizeof(Elf64_Ehdr)), (ptrs_to_old.E_shoff - 64));

}