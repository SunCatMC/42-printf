# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: htryndam <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/05/16 22:52:18 by htryndam          #+#    #+#              #
#    Updated: 2019/05/16 23:08:53 by htryndam         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a

IDIR = .
SOURCES = ft_printf.c

SRC = $(SOURCES)

LIBNAME = ft
LIBDIR = libft
LIBFILE = libft.a

CFLAGS = -Wall -Werror -Wextra
DFLAGS = -g

all: $(NAME)

$(NAME):
	@$(MAKE) -C $(LIBDIR)
	@gcc $(CFLAGS) -I $(IDIR) -I $(LIBDIR) -c $(SRC)
	@libtool -static -o $(NAME) $(addprefix $(LIBDIR)/,$(LIBFILE)) $(addsuffix .o,$(basename $(SRC)))

clean:
	@+$(MAKE) -C $(LIBDIR) clean
	@/bin/rm -f *.o

fclean: clean
	@+$(MAKE) -C $(LIBDIR) fclean
	@/bin/rm -f $(NAME)

re: fclean all

debug: fclean
	@$(MAKE) -C $(LIBDIR) debug
	@gcc $(CFLAGS) $(DFLAGS) -I $(IDIR) -I $(LIBDIR) -c $(SRC)
	@gcc $(CFLAGS) $(DFLAGS) $(addsuffix .o,$(basename $(SRC))) -I $(IDIR) \
		-I $(LIBDIR) -L $(LIBDIR) -l$(LIBNAME) -o $(NAME)
