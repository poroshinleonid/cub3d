NAME = libft.a 

CC = cc

CFLAGS = -Wall -Wextra -Werror 

CFILES = ./ft_char/ft_char_change_case.c \
./ft_char/ft_char_info.c \
./ft_conv/ft_atoi.c \
./ft_conv/ft_atoi_base.c \
./ft_conv/ft_atoi_ui.c \
./ft_conv/ft_itoa.c \
./ft_conv/ft_itoa_base.c \
./ft_lst/ft_lstadd_back.c \
./ft_lst/ft_lstadd_front.c \
./ft_lst/ft_lstclear.c \
./ft_lst/ft_lstdelone.c \
./ft_lst/ft_lstiter.c \
./ft_lst/ft_lstlast.c \
./ft_lst/ft_lstmap.c \
./ft_lst/ft_lstnew.c \
./ft_lst/ft_lstsize.c \
./ft_mem/ft_bzero.c \
./ft_mem/ft_calloc.c \
./ft_mem/ft_memchr.c \
./ft_mem/ft_memcmp.c \
./ft_mem/ft_memcpy.c \
./ft_mem/ft_memmove.c \
./ft_mem/ft_memset.c \
./ft_mem/ft_realloc.c \
./ft_num/ft_minmax.c \
./ft_read/gnl.c \
./ft_read/gnl_utils.c \
./ft_read/gnl_utils_2.c \
./ft_stack/stack.c \
./ft_stack/stack_methods.c \
./ft_stack/stack_methods_2.c \
./ft_stack/stack_methods_3.c \
./ft_string/ft_in.c \
./ft_string/ft_split.c \
./ft_string/ft_strchr.c \
./ft_string/ft_strdup.c \
./ft_string/ft_striteri.c \
./ft_string/ft_strjoin.c \
./ft_string/ft_strlen.c \
./ft_string/ft_strmapi.c \
./ft_string/ft_strncat.c \
./ft_string/ft_strncmp.c \
./ft_string/ft_strncpy.c \
./ft_string/ft_strndup.c \
./ft_string/ft_strnstr.c \
./ft_string/ft_strrchr.c \
./ft_string/ft_strrev.c \
./ft_string/ft_strtrim.c \
./ft_string/ft_substr.c \
./ft_string/string_array_utils.c \
./ft_write/ft_putchar_fd.c \
./ft_write/ft_putendl_fd.c \
./ft_write/ft_putnbr_fd.c \
./ft_write/ft_putstr_fd.c \
./ft_write/ft_printf/conv_writers_1.c \
./ft_write/ft_printf/conv_writers_1_bonus.c \
./ft_write/ft_printf/conv_writers_2.c \
./ft_write/ft_printf/conv_writers_2_bonus.c \
./ft_write/ft_printf/ft_printf.c \
./ft_write/ft_printf/ft_printf_bonus.c \
./ft_write/ft_printf/ft_printf_dec_utils_1_bonus.c \
./ft_write/ft_printf/ft_printf_dec_utils_2_bonus.c \
./ft_write/ft_printf/ft_printf_dec_utils_3_bonus.c \
./ft_write/ft_printf/ft_printf_dec_utils_4_bonus.c \
./ft_write/ft_printf/ft_printf_string_utils_1_bonus.c \
./ft_write/ft_printf/ft_printf_string_utils_2_bonus.c \
./ft_write/ft_printf/ft_printf_utils.c \
./ft_write/ft_printf/ft_printf_utils_2.c \
./ft_write/ft_printf/ft_printf_utils_2_bonus.c \
./ft_write/ft_printf/ft_printf_utils_bonus.c \
./ft_write/ft_printf/hex_utils_2_bonus.c \
./ft_write/ft_printf/hex_utils_3_bonus.c \
./ft_write/ft_printf/hex_utils_bonus.c \
./ft_write/ft_printf/width_utils_bonus.c \
./ft_clst/ft_clst.c

OBJECTS = $(CFILES:.c=.o)

$(NAME): $(OBJECTS)
	ar -rc $(NAME) $(OBJECTS)

all: $(NAME) 

clean: 
	rm -f $(OBJECTS)

fclean: clean 
	rm -f $(NAME)
	
re: fclean all 