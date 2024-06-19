include ./libft/rsc/mk/color.mk

NAME = perlinDisplay

CC		=	gcc

CFLAGS 	=	-Wall -Wextra -Werror

SRC =	src/main_perlin.c\
		src/mlx_display_init.c\


LIB_SRC =	src/perlin/noise_image.c\
			src/perlin/noise_interpolate_utils.c\
			src/perlin/noise_utils.c\
			src/perlin/perlin_noise.c\
			src/perlin/snake_perlin.c\

MLX_FLAG 		=	-Lminilibx-linux -lmlx -lX11 -lXext -lm -fsanitize=address -g

MAKE_SILENCE 	=	make -s -C

MAKE_LIBFT		=	$(MAKE_SILENCE) libft -j

MAKE_LIST		=	$(MAKE_SILENCE) libft/list -j

LIBFT			= 	libft/libft.a

LIST			= 	libft/list/linked_list.a

PERLIN_LIB		= 	perlin_noise.a

MLX_LIB 		= 	minilibx-linux/libmlx.a

OBJS = $(SRC:.c=.o)

LIB_OBJS = $(LIB_SRC:.c=.o)

all:		$(NAME)

%.o: %.c
	@printf "$(YELLOW)Compile $<$(RESET) $(BRIGHT_BLACK)-->$(RESET) $(BRIGHT_MAGENTA)$@$(RESET)\n"
	@$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(MLX_LIB) $(LIBFT) $(LIST) $(OBJS) $(LIB_OBJS)
	@printf "$(CYAN)Compiling ${NAME} ...$(RESET)\n"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIB_OBJS) $(LIBFT) $(LIST) $(MLX_FLAG)
	@printf "$(GREEN)Compiling $(NAME) done$(RESET)\n"


$(MLX_LIB) :
ifeq ($(shell [ -f ${MLX_LIB} ] && echo 0 || echo 1), 1)
	@printf "$(CYAN)Compiling minilibx-linux...$(RESET)\n"
	@$(MAKE_SILENCE) minilibx-linux
	@printf "$(GREEN)Compiling minilibx-linux done$(RESET)\n"
endif

$(LIST):
ifeq ($(shell [ -f ${LIST} ] && echo 0 || echo 1), 1)
	@printf "$(CYAN)Compiling list...$(RESET)\n"
	@$(MAKE_LIST)
	@printf "$(GREEN)Compiling list done$(RESET)\n"
endif

$(LIBFT):
ifeq ($(shell [ -f ${LIBFT} ] && echo 0 || echo 1), 1)
	@printf "$(CYAN)Compiling libft...$(RESET)\n"
	@$(MAKE_LIBFT)
	@printf "$(GREEN)Compiling libft done$(RESET)\n"
endif


lib $(PERLIN_LIB): $(LIBFT) $(LIST) $(LIB_OBJS)
	@printf "$(CYAN)Compiling perlin noise library...$(RESET)\n"
	@$(MAKE_SILENCE) minilibx-linux
	@ar rcs $(PERLIN_LIB) $(LIB_OBJS)
	@printf "$(GREEN)Compiling perlin noise library done -> $(PERLIN_LIB) $(RESET)\n"

debug:
	${MAKE_SILENCE} libft
	${MAKE_SILENCE} minilibx-linux
	gcc -o ${NAME} ${SRC} ${MLX_FLAG} libft/libft.a -D PERLIN_NOISE_DEBUG -g

clean_lib:
	${MAKE_SILENCE} libft fclean
	${MAKE_SILENCE} minilibx-linux clean

basic_test: all
	./${NAME} 42 8 1.0 2.0

continental_noise: all
	./${NAME} 42 8 0.6 2.0 1

erosion_noise: all
	./${NAME} 43 8 0.6 1.8 1

humidity_noise: all
	./${NAME} 44 4 1.0 2.0 1

temperature_noise: all
	./${NAME} 45 6 0.4 2.0 1

peaks_valley_noise: all
	./${NAME} 46 10 0.5 3.0 1

2peaks_valley_noise: all
	./${NAME} 46 6 0.6 2.2 1

snake_noise: all 
	./${NAME} 42 6 0.6 2.0 2

clean:
	rm -f ${NAME} ${OBJS} ${LIB_OBJS} ${PERLIN_LIB}

re : clean all

fclean : clean_lib clean
	$(MAKE_SILENCE) minilibx-linux clean
	$(MAKE_SILENCE) libft fclean
	$(MAKE_SILENCE) libft/list fclean
	@echo fclean done
