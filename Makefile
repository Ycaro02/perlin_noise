NAME = perlinDisplay

SRC =	src/main_perlin.c\
		src/mlx_display_init.c\
		src/noise_image.c\
		src/noise_interpolate_utils.c\
		src/noise_utils.c\
		src/perlin_noise.c\

MLX_FLAG 		=	-Lminilibx-linux -lmlx -lX11 -lXext -lm

MAKE_SILENCE = make -s -C

all: ${NAME}

${NAME}:
	${MAKE_SILENCE} libft
	${MAKE_SILENCE} minilibx-linux
	gcc -o ${NAME} ${SRC} ${MLX_FLAG} libft/libft.a

debug:
	${MAKE_SILENCE} libft
	${MAKE_SILENCE} minilibx-linux
	gcc -o ${NAME} ${SRC} ${MLX_FLAG} libft/libft.a -D PERLIN_NOISE_DEBUG -g

clean_lib:
	${MAKE_SILENCE} libft fclean
	${MAKE_SILENCE} minilibx-linux clean

test: all
	./${NAME} 42 8 1.0 2.0

clean:
	rm -f ${NAME}

fclean : clean_lib clean
	@echo fclean done