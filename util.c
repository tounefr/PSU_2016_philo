/*
** util.c for  in /mnt/PSU_2016_philo
** 
** Made by Thomas HENON
** Login   <henon_t@epitech.net>
** 
** Started on  Fri Mar 17 10:26:46 2017 Thomas HENON
** Last update Fri Mar 17 10:26:46 2017 Thomas HENON
*/

#include <string.h>

void swap_forks(int *a, int *b)
{
    int c;

    c = *a;
    *a = *b;
    *b = c;
}

char is_integer(char *s)
{
    int i;
    int len;

    i = -1;
    len = strlen(s);
    while (++i < len) {
        if (s[i] < '0' || s[i] > '9')
            return 0;
    }
    return 1;
}