
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"
#include "philo.h"

char parse_args(int argc, char **argv, int *philo_numbers, int *philo_max_eat) {
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

void forks_action(char action,
                  char chopstick,
                  struct s_arg_philosopher_thread *pharg,
                  struct s_philosopher *ph) {
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
}

void *s_philosopher_thread(void *arg) {
    int i;
    struct s_arg_philosopher_thread *ph_arg;
    struct s_philosopher *philosopher;

    ph_arg = (struct s_arg_philosopher_thread*) arg;
    philosopher = &ph_arg->philosophers->philosophers[ph_arg->index];
    i = 0;
    while (i < philosopher->max_eat) {
        if (philosopher->state == THINK) {
            forks_action(LOCK, BOTH_FORK, ph_arg, philosopher);
            philosopher->state = EAT;
            lphilo_eat();
            forks_action(RELEASE, BOTH_FORK, ph_arg, philosopher);
            i++;
        }
        else if (philosopher->state == UNKNOWN || philosopher->state == SLEEP) {
            forks_action(LOCK, LEFT_FORK, ph_arg, philosopher);
            philosopher->state = THINK;
            lphilo_think();
            forks_action(RELEASE, LEFT_FORK, ph_arg, philosopher);
        }
        else if (philosopher->state == EAT && (philosopher->state = SLEEP))
            lphilo_sleep();
    }
}

void swap_forks(int *a, int *b)
{
    int c;

    c = *a;
    *a = *b;
    *b = c;
}

char s_philosopher_init(struct s_philosopher *philosopher, int index, int max_eat, int nbr_philo) {
    philosopher->max_eat = max_eat / nbr_philo;
    philosopher->index = index;
    philosopher->state = UNKNOWN;
    philosopher->forks_index[LEFT_FORK] = index;
    philosopher->forks_index[RIGHT_FORK] = (index + 1) % nbr_philo;
    if (philosopher->forks_index[LEFT_FORK] > philosopher->forks_index[RIGHT_FORK])
        swap_forks(&philosopher->forks_index[LEFT_FORK], &philosopher->forks_index[RIGHT_FORK]);
    return 1;
}

char s_philosophers_init(struct s_philosophers *philosophers)
{
    int i;

    philosophers->philosophers = malloc(sizeof(t_philosopher) * philosophers->nbr_philo);
    if (philosophers->philosophers == NULL)
        return 0;
    philosophers->forks = malloc(sizeof(pthread_mutex_t) * philosophers->nbr_philo);
    if (philosophers->forks == NULL)
        return 0;
    philosophers->cur_eat = 0;
    i = -1;
    while (++i < philosophers->nbr_philo) {
        if (!s_philosopher_init(&philosophers->philosophers[i], i,
                                philosophers->max_eat,
                                philosophers->nbr_philo))
            return 0;
        if (pthread_mutex_init(&philosophers->forks[i], NULL) != 0)
            return 0;
    }
    return 1;
}

t_arg_philosopher_thread *s_arg_philosopher_thread_init(
        int index,
        struct s_philosophers *philosophers) {
    t_arg_philosopher_thread *arg;

    if (NULL == (arg = malloc(sizeof(t_arg_philosopher_thread))))
        return NULL;
    arg->index = index;
    arg->philosophers = philosophers;
    return arg;
}

char start_philosophers_threads(struct s_philosophers *philosophers) {
    int i;
    t_arg_philosopher_thread *arg_philosopher;

    i = -1;
    while (++i < philosophers->nbr_philo) {
        if (!(arg_philosopher = s_arg_philosopher_thread_init(i, philosophers)))
            return 0;
        philosophers->philosophers[i].s_arg_philosopher_thread = arg_philosopher;
        if (pthread_create(&philosophers->philosophers[i].thread,
                           NULL,
                           s_philosopher_thread,
                           arg_philosopher) != 0)
            return 0;
    }
    return 1;
}

char s_philosophers_free(struct s_philosophers *philosophers)
{
    int i;

    i = -1;
    while (++i < philosophers->nbr_philo) {
        pthread_mutex_destroy(&philosophers->forks[i]);
    }
    free(philosophers->forks);
    free(philosophers->philosophers);
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
