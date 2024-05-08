NAME = perlinDisplay

# Path: src/
SRC = main_perlin.c \
	  perlin_noise.c \
		mlx_init.c \

MLX_FLAG 		=	-Lminilibx-linux -lmlx -lX11 -lXext -lm

all: ${NAME}

${NAME}:
	make -s -C libft
	make -s -C minilibx-linux
	gcc -o ${NAME} ${SRC} ${MLX_FLAG} libft/libft.a

debug:
	make -s -C libft
	make -s -C minilibx-linux
	gcc -o ${NAME} ${SRC} ${MLX_FLAG} libft/libft.a -D PERLIN_NOISE_DEBUG -g

clean_lib:
	make -s -C libft fclean
	make -s -C minilibx-linux clean

test: all
	./${NAME} 42 8 1.0 2.0

fclean :
	rm -f ${NAME}