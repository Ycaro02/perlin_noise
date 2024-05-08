NAME = perlinDisplay

# Path: src/
SRC = main_perlin.c \
	  perlin_noise.c \
		mlx_init.c \

MLX_FLAG 		=	-Lminilibx-linux -lmlx -lX11 -lXext -lm

all: ${NAME}

${NAME}:
	make -C libft
	make -C minilibx-linux
	gcc -o ${NAME} ${SRC} ${MLX_FLAG} libft/libft.a -g

clean_lib:
	make -C libft fclean
	make -C minilibx-linux clean

test: all
	./${NAME} 42 8 1.0 2.0

fclean :
	rm -f ${NAME}