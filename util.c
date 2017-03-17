/*
** util.c for  in /mnt/PSU_2016_philo
** 
** Made by Thomas HENON
** Login   <henon_t@epitech.net>
** 
** Started on  Fri Mar 17 10:26:46 2017 Thomas HENON
** Last update Fri Mar 17 10:26:46 2017 Thomas HENON
*/

void swap_forks(int *a, int *b)
{
    int c;

    c = *a;
    *a = *b;
    *b = c;
}
