cmake_minimum_required(VERSION 3.5.1)
project(vm)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main2.c
        vm.c
        ../libft/ft_atoi.c
    ../libft/ft_bzero.c
        ../libft/ft_isdigit.c
        ../libft/ft_strcmp.c
        ../libft/ft_strdup.c
        ../libft/ft_strlen.c)


        add_executable(vm ${SOURCE_FILES})
