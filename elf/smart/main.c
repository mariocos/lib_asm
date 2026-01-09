#include "packer.h"


/*

*/
void	getReleventPointers(void* map, t_ptrs* releventPtrs, t_ranges* ranges)
{
	// ELF header
	Elf64_Ehdr* eHdr = map;
	releventPtrs->ehdr = eHdr;

	Elf64_Shdr* sHdrTable = map + eHdr->e_shoff;
	char* strTable = map + sHdrTable[eHdr->e_shstrndx].sh_offset;
	ranges->shnum = eHdr->e_shnum;

	for (int i = 0 ; i < ranges->shnum ; i++)
	{
		if (ft_strcmp(".text", &strTable[sHdrTable[i].sh_name]) == 0)
		{
			ranges->textIndex = i;
			break ;
		}
	}
	Elf64_Shdr* textShdr = &sHdrTable[ranges->textIndex];
	releventPtrs->textSectionHdr = textShdr;

	
	//Program headers
	Elf64_Phdr* pHdrTable = map + eHdr->e_phoff;
	releventPtrs->pHdrTable = pHdrTable;

	for (int i = 0 ; i < eHdr->e_phnum ; i++)
	{
		if (pHdrTable[i].p_offset <= textShdr->sh_offset
			&& textShdr->sh_offset + textShdr->sh_size <= pHdrTable[i].p_offset + pHdrTable[i].p_filesz
			&& pHdrTable[i].p_vaddr <= textShdr->sh_addr
			&& textShdr->sh_addr + textShdr->sh_size <= pHdrTable[i].p_vaddr + pHdrTable[i].p_filesz)
		{
			ranges->targetSegment = i;
			break ;
		}
	}
	releventPtrs->targetSegment = &pHdrTable[ranges->targetSegment];
}


int main(int ac, char** av)
{
	if (ac != 2)
	{
		printf("Error: wrong number of args\n");
		return (-1);
	}

	void* map = get_map(av[1]);
	if (!map)
	{
		printf("Error: Couldnt map file\n");
		return (-1);
	}

	int newFd = get_new_file_fd("");
	if (newFd == -1)
	{
		printf("Error: couldnt open new file\n");
		return (-1);
	}

	t_ptrs releventPtrs = {0};
	t_ranges ranges = {0};
	getReleventPointers(map, &releventPtrs, &ranges);

	getSegmentRange();
}
