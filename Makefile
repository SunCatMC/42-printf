# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: htryndam <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/05/16 22:52:18 by htryndam          #+#    #+#              #
#    Updated: 2019/06/07 21:19:25 by htryndam         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a

IDIR = .
SOURCES = ft_printf.c buffer.c optionals.c basic_types.c

SRC = $(SOURCES)

LIBNAME = ft
LIBDIR = libft
LIBFILE = libft.a
LIBOBJ = *.o

CFLAGS = -Wall -Werror -Wextra
DFLAGS = -g

all: $(NAME)

$(NAME):
	@$(MAKE) -C $(LIBDIR)
	@gcc $(CFLAGS) -I $(IDIR) -I $(LIBDIR) -c $(SRC)
	@ar rc $(NAME) $(addprefix $(LIBDIR)/,$(LIBOBJ)) \
			$(addsuffix .o,$(basename $(SRC)))

clean:
	@$(MAKE) -C $(LIBDIR) clean
	@/bin/rm -f *.o

fclean: clean
	@$(MAKE) -C $(LIBDIR) fclean
	@/bin/rm -f $(NAME)

re: fclean all

debug: fclean
	@$(MAKE) -C $(LIBDIR) debug
	@gcc $(CFLAGS) $(DFLAGS) -I $(IDIR) -I $(LIBDIR) -c $(SRC)
	@ar rc $(NAME) $(addprefix $(LIBDIR)/,$(LIBOBJ)) \
			$(addsuffix .o,$(basename $(SRC)))
