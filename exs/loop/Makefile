NAME = skip
SRCS = ex01.asm
OBJ = $(SRCS).o

ASM = nasm
ASM_FLAGS = -f elf64
LD = ld

all: $(NAME)

$(NAME): $(OBJ)
	$(LD) $(OBJ) -o $(NAME)

$(OBJ): $(SRCS)
	$(ASM) $(ASM_FLAGS) $(SRCS) -o $(OBJ)

clean:
	rm -f $(OBJ) $(NAME)