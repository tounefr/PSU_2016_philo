
#ifndef UNTITLED_PHILO_H
#define UNTITLED_PHILO_H

#include "extern.h"

enum PHILO_STATES
{
    UNKNOWN = 0,
    THINK = 1,
    EAT = 2,
    SLEEP = 3
};

enum PHILO_FORKS
{
    LEFT_FORK = 0,
    RIGHT_FORK = 1,
    BOTH_FORK = 2
};

enum THREAD_ACTION
{
    LOCK = 0,
    RELEASE = 1
};

typedef struct s_philosophers t_philosophers;
typedef struct s_philosopher
{
    int max_eat;
    int cur_eat;
    int index;
    char state;
    pthread_t thread;
    void *s_arg_philosopher_thread;
    int forks_index[2];

    void (*init)(struct s_philosophers*, int index, int max_eat);
    void (*change_state)(struct s_philosophers*, int state);
    void (*running_thread)(struct s_philosophers*);
    void (*describe)(struct s_philosophers*);
} t_philosopher;

typedef struct s_forks
{
    void (*init)(struct s_forks*, int i_left, int i_right);
    void (*pick)(struct s_forks*);
    void (*release)(struct s_forks*);
} t_forks;

typedef struct s_arg_philosopher_thread
{
    int index;
    struct s_philosophers *philosophers;
} t_arg_philosopher_thread;

typedef struct s_philosophers
{
    int nbr_philo;
    int max_eat;
    t_philosopher *philosophers;
    pthread_mutex_t *forks;
} t_philosophers;

char parse_args(int argc,
                char **argv,
                int *philo_numbers,
                int *philo_max_eat);
char print_usage();
char here_we_go(t_philosophers *philosophers);


char s_philosopher_init(struct s_philosopher *p,
                        int index,
                        int max_eat,
                        int nbr_philo);
char s_philosophers_init(struct s_philosophers *philosophers);
t_arg_philosopher_thread *s_arg_philosopher_thread_init(
        int index,
        struct s_philosophers *philosophers);
char s_philosophers_free(struct s_philosophers *philosophers);


void forks_action(char action,
                  char chopstick,
                  struct s_arg_philosopher_thread *pharg,
                  struct s_philosopher *ph);
void *s_philosopher_thread(void *arg);
char start_philosophers_threads(struct s_philosophers *philosophers);

void swap_forks(int *a, int *b);



#endif //UNTITLED_PHILO_H
