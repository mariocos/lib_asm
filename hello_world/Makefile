NAME = Hello_world
SRCS = hello_world.asm
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