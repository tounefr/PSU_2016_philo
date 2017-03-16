
#include <stdlib.h>
#include "philo.h"

char s_philosopher_init(struct s_philosopher *p,
                        int index,
                        int max_eat,
                        int nbr_philo)
{
    p->max_eat = max_eat;
    p->index = index;
    p->state = UNKNOWN;
    p->cur_eat = 0;
    p->forks_index[LEFT_FORK] = index;
    p->forks_index[RIGHT_FORK] = (index + 1) % nbr_philo;
    if (p->forks_index[LEFT_FORK] > p->forks_index[RIGHT_FORK])
        swap_forks(&p->forks_index[LEFT_FORK],
                   &p->forks_index[RIGHT_FORK]);
    return 1;
}

char s_philosophers_init(struct s_philosophers *philosophers)
{
    int i;
    int m_size;

    m_size = sizeof(t_philosopher) * philosophers->nbr_philo;
    philosophers->philosophers = malloc(m_size);
    if (philosophers->philosophers == NULL)
        return 0;
    m_size = sizeof(pthread_mutex_t) * philosophers->nbr_philo;
    philosophers->forks = malloc(m_size);
    if (philosophers->forks == NULL)
        return 0;
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