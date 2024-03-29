/*
** philo_algo.c for  in /mnt/PSU_2016_philo
** 
** Made by Thomas HENON
** Login   <henon_t@epitech.net>
** 
** Started on  Fri Mar 17 10:26:33 2017 Thomas HENON
** Last update Fri Mar 17 10:26:34 2017 Thomas HENON
*/

#include <unistd.h>
#include "philo.h"

char                forks_action(
                        char action,
                        char chopstick,
                        struct s_arg_philosopher_thread *pharg,
                        struct s_philosopher *ph)
{
    pthread_mutex_t *mutex[2];

    mutex[0] = &pharg->philosophers->forks[ph->forks_index[LEFT_FORK]];
    mutex[1] = &pharg->philosophers->forks[ph->forks_index[RIGHT_FORK]];
    if (action == LOCK) {
        if (chopstick == LEFT_FORK || chopstick == BOTH_FORK) {
            pthread_mutex_lock(mutex[0]);
            lphilo_take_chopstick(mutex[0]);
        }
        if (chopstick == RIGHT_FORK || chopstick == BOTH_FORK) {
            pthread_mutex_lock(mutex[1]);
            lphilo_take_chopstick(mutex[1]);
        }
    }
    else if (action == RELEASE) {
        if (chopstick == LEFT_FORK  || chopstick == BOTH_FORK) {
            pthread_mutex_unlock(mutex[0]);
            lphilo_release_chopstick(mutex[0]);
        }
        if (chopstick == RIGHT_FORK || chopstick == BOTH_FORK) {
            pthread_mutex_unlock(mutex[1]);
            lphilo_release_chopstick(mutex[1]);
        }
    }
    return 1;
}

char    philo_eat(struct s_arg_philosopher_thread *ph_arg,
               struct s_philosopher *philosopher)
{
    int i;

    i = -1;
    while (++i < ph_arg->philosophers->nbr_philo) {
        if (ph_arg->philosophers->philosophers[i].cur_eat >=
                ph_arg->philosophers->max_eat) {
            forks_action(RELEASE, BOTH_FORK, ph_arg, philosopher);
            return 0;
        }
    }
    philosopher->state = EAT;
    lphilo_eat();
    philosopher->cur_eat++;
    return 1;
}

void                                    *s_philosopher_thread(void *arg)
{
    int                                 i;
    struct s_arg_philosopher_thread     *ph_arg;
    struct s_philosopher                *philosopher;

    ph_arg = (struct s_arg_philosopher_thread*) arg;
    philosopher = &ph_arg->philosophers->philosophers[ph_arg->index];
    i = 0;
    while (1) {
        if (philosopher->state == THINK) {
            forks_action(LOCK, BOTH_FORK, ph_arg, philosopher);
            if (!philo_eat(ph_arg, philosopher))
                return 0;
            forks_action(RELEASE, BOTH_FORK, ph_arg, philosopher);
            usleep(1400);
        }
        else if (philosopher->state == UNKNOWN ||
                 philosopher->state == SLEEP) {
            forks_action(LOCK, LEFT_FORK, ph_arg, philosopher);
            philosopher->state = THINK;
            lphilo_think();
            forks_action(RELEASE, LEFT_FORK, ph_arg, philosopher);
        }
        else if (philosopher->state == EAT && (philosopher->state = SLEEP))
            lphilo_sleep();
    }
}

char                            start_philosophers_threads(
        struct s_philosophers *philosophers)
{
    int                         i;
    t_arg_philosopher_thread    *arg_p;

    i = -1;
    while (++i < philosophers->nbr_philo) {
        if (!(arg_p = s_arg_philosopher_thread_init(i, philosophers)))
            return 0;
        philosophers->philosophers[i].s_arg_philosopher_thread = arg_p;
        if (pthread_create(&philosophers->philosophers[i].thread,
                           NULL,
                           s_philosopher_thread,
                           arg_p) != 0)
            return 0;
    }
    return 1;
}
