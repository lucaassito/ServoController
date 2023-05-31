EXEC = exec.elf

# Répertoire d'installation des outils de développement
# pour Raspberry Pi (à adapter, si besoin).
RASPBERRY_TOOLS = /home/lucas/Logiciels/RaspberryPi/Tools/

# Compilateur croisé à utiliser (à adapter)
RASPBERRY_GCC = $(RASPBERRY_TOOLS)/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc

# Sysroot (à adapter)
#RASPBERRY_SYSROOT = /<path>/<to>/<raspberry>/<rootfs>/
RASPBERRY_SYSROOT = $(RASPBERRY_TOOLS)/rootfs_bplus/

# Compilateur.
CC = $(RASPBERRY_GCC)
LD = $(RASPBERRY_GCC)
CCFLAGS += --sysroot=$(RASPBERRY_SYSROOT)
CCFLAGS += -std=c99 -Wall -pedantic
CCFLAGS += -D_POSIX_C_SOURCE=199309L  # Ajout du flag de compilation
LDFLAGS += --sysroot=$(RASPBERRY_SYSROOT)
LDFLAGS += -lgpiod -pthread

# Organisation des sources
SRC = $(wildcard *.c) 
OBJ = $(SRC:.c=.o)


# Règles de compilations
all: $(EXEC)

$(EXEC): $(OBJ)
	$(LD) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

# Nettoyage du projet
clean:
	$(RM) *.o $(EXEC)