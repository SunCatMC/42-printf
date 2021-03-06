# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: htryndam <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/05/16 22:52:18 by htryndam          #+#    #+#              #
#    Updated: 2019/07/16 22:04:07 by htryndam         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a

IDIR = includes
SOURCES = ft_printf.c conversions.c buffer.c optionals.c basic_types.c \
		  integers.c doubles.c doubles_init.c doubles_misc.c numlist.c \
		  bignums.c bignums_init.c bignums_math.c bignums_misc.c bignums_g.c \
		  bigldbls.c bigldbls_misc.c colors.c misc.c
SRCDIR = src
SRC = $(addprefix $(SRCDIR)/,$(SOURCES))

LIBNAME = ft
LIBDIR = libft
LIBIDIR = libft/includes
LIBFILE = libft.a
LIBOBJ = ft_memchr.o ft_strchrs.o ft_strchr.o ft_isupper.o ft_memccpy.o \
		 ft_memset.o ft_memcpy.o ft_isdigit.o ft_strsubcmp.o

CFLAGS = -Wall -Werror -Wextra
DFLAGS = -g

all: $(NAME)

$(NAME):
	@$(MAKE) -C $(LIBDIR)
	@gcc $(CFLAGS) -I $(IDIR) -I $(LIBIDIR) -c $(SRC)
	@ar rc $(NAME) $(addprefix $(LIBDIR)/,$(LIBOBJ)) \
			$(addsuffix .o,$(basename $(SOURCES)))

clean:
	@$(MAKE) -C $(LIBDIR) clean
	@/bin/rm -f *.o

fclean: clean
	@$(MAKE) -C $(LIBDIR) fclean
	@/bin/rm -f $(NAME)

re: fclean all

debug: fclean
	@$(MAKE) -C $(LIBDIR) debug
	@gcc $(CFLAGS) $(DFLAGS) -I $(IDIR) -I $(LIBIDIR) -c $(SRC)
	@ar rc $(NAME) $(addprefix $(LIBDIR)/,$(LIBOBJ)) \
			$(addsuffix .o,$(basename $(SOURCES)))
