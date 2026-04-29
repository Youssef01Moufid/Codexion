#include "../includes/codexion.h"

bool ft_atoi(char const *str, int *out)
{
    int i;
    int sign;
    long result;

    i = 0;
    sign = 1;
    result = 0;
    if (!str || str[0] == '\0')
        return (false);
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    if (str[i] < '0' || str[i] > '9')
        return (false);
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        if ((sign == 1 && result > INT_MAX) || (sign == -1 && -result < INT_MIN))
            return (false);
        i++;
    }
    if (str[i] != '\0')
        return (false);
    *out = result * sign;
    return (true);
}

bool    parse_positive(int val, char *name)
{
    if (val <= 0)
    {
        fprintf(stderr,"Error: %s must be a positive integer", name);
        return (false);
    }
    return (true);
}

bool    parse_non_negative(int val, char *name)
{
    if (val < 0)
    {
        fprintf(stderr,"Error: %s must be > or = zero", name);
        return (false);
    }
    return (true);
}

bool    parse_scheduler(char *s)
{
    if (strcmp(s, "fifo") != 0 && strcmp(s, "edf") != 0)
    {
        fprintf(stderr, "%s must be fifo or edf", s);
        return (false);
    }
    return (true);
}

bool    parse_arg(char *str, int *out, int min_value, char *name)
{
    if (!ft_atoi(str, out))
    {
        fprintf(stderr, "Error: invalid input");
        return (false);
    }
    if (min_value == 0)
        return (parse_non_negative(*out, name));
    else
        return (parse_positive(*out, name));
}

bool    parsing(int argc, char **argv, t_sim *sim)
{
    int val;

    val = 0;
    if (argc != 9)
    {
        fprintf(stderr, "number of argument must be 8");
        return (false);
    }
    if (!parse_arg(argv[1], &val, 1, "number_of_coders"))
        return (false);
    sim->number_of_coders = val;
    if (!parse_arg(argv[2], &val, 1, "time_to_burnout"))
        return (false);
    sim->time_to_burnout = val;
    if (!parse_arg(argv[3], &val, 1, "time_to_compile"))
        return (false);
    sim->time_to_compile = val;
    if (!parse_arg(argv[4], &val, 0, "time_to_debug"))
        return (false);
    sim->time_to_debug = val;
    if (!parse_arg(argv[5], &val, 0, "time_to_refactor"))
        return (false);
    sim->time_to_refactor = val;
    if (!parse_arg(argv[6], &val, 1, "number_of_compiles_required"))
        return (false);
    sim->number_of_compiles_required = val;
    if (!parse_arg(argv[7], &val, 0, "dongle_cooldown"))
        return (false);
    sim->dongle_cooldown = val;
    if (!parse_scheduler(argv[8]))
        return (false);
    if (strcmp(argv[8], "fifo") == 0)
        sim->scheduler = FIFO;
    else
        sim->scheduler = EDF;
    return (true);
}
