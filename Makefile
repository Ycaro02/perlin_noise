NAME = perlinDisplay

SRC =	src/main_perlin.c\
		src/mlx_display_init.c\
		src/noise_image.c\
		src/noise_interpolate_utils.c\
		src/noise_utils.c\
		src/perlin_noise.c\
		src/snake_perlin.c\

MLX_FLAG 		=	-Lminilibx-linux -lmlx -lX11 -lXext -lm -fsanitize=address -g

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

basic_test: all
	./${NAME} 42 8 1.0 2.0

float_test: all
	./${NAME} 42 4 1.0 2.0 1

snake_test: all 
	./${NAME} 42 6 0.6 2.0 2

clean:
	rm -f ${NAME}

re : clean all

fclean : clean_lib clean
	@echo fclean done