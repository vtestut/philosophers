#include "philo.h"

int main(int argc, char **argv)
{
    // t_table *d;

    // d = NULL;
    if(argc - 1 >= 4 && argc - 1 <= 5)
    {
        if(parsing(argc, argv))
        {
            // d = data_init(argc, argv);
            // if (!d)
            //     return(EXIT_FAILURE);
            // if (!start_routine(d))
            //     return (EXIT_FAILURE);
            // stop_routine(d);
            // return (EXIT_SUCCESS);
            printf("ok\n");
        }
    }
    else
    {
        printf("an error occured\n");
        return (EXIT_FAILURE);
    }
}