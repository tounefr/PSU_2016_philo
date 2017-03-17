/*
** main.c for  in /mnt/PSU_2016_philo
** 
** Made by Thomas HENON
** Login   <henon_t@epitech.net>
** 
** Started on  Fri Mar 17 10:26:12 2017 Thomas HENON
** Last update Fri Mar 17 10:26:18 2017 Thomas HENON
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "philo.h"

char parse_args(int argc,
                char **argv,
                int *philo_numbers,
                int *philo_max_eat)
{
    int i;

    i = 1;
    if (argc != 5)
        return 0;
    while (i < argc - 1) {
        if (!strcmp(argv[i], "-p"))
            *philo_numbers = atoi(argv[i + 1]);
        if (!strcmp(argv[i], "-e"))
            *philo_max_eat = atoi(argv[i + 1]);
        i += 2;
    }
    if (*philo_numbers < 2)
        return 0;
    return 1;
}

char print_usage() {
    fprintf(stderr, "Usage: ./philo -p [philo_numbers] -e [max_eat]\n");
    return 1;
}

char here_we_go(t_philosophers *philosophers) {
    int i;
    void *thread_status;

    if (!s_philosophers_init(philosophers))
        return 0;
    if (!start_philosophers_threads(philosophers))
        return 0;
    i = -1;
    while (++i < philosophers->nbr_philo) {
        pthread_join(philosophers->philosophers[i].thread, &thread_status);
        free(philosophers->philosophers[i].s_arg_philosopher_thread);
    }
    s_philosophers_free(philosophers);
    return 1;
}

int main(int argc, char **argv) {
    t_philosophers philosophers;
    int returnv;

    if (!parse_args(argc, argv, &philosophers.nbr_philo,
                    &philosophers.max_eat))
        return print_usage();
    RCFStartup(argc, argv);
    returnv = !here_we_go(&philosophers);
    RCFCleanup();
    return returnv;
}
