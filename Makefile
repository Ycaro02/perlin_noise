NAME = perlinDisplay

# Path: src/
SRC = main_perlin.c \
	  perlin_noise.c \
		mlx_init.c \

MLX_FLAG 		=	-Lminilibx-linux -lmlx -lX11 -lXext -lm

all: ${NAME}

${NAME}: 
	gcc -o ${NAME} ${SRC} ${MLX_FLAG} libft/libft.a

clean_lib:
	make -C libft fclean
	make -C minilibx-linux clean

fclean :
	rm -f ${NAME}