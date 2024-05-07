NAME = perlinDisplay

# Path: src/
SRC = main_perlin.c \
	  perlin_noise.c \
		mlx_init.c \

MLX_FLAG 		=	-Lminilibx-linux -lmlx -lX11 -lXext -lm

all: ${NAME}

${NAME}: 
	gcc -o ${NAME} ${SRC} ${MLX_FLAG} libft/libft.a

fclean :
	rm -f ${NAME}