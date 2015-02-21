#include "cmd.h"
#include "word.h"
#include "compat-util.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>


struct cmd_struct {
    const char *cmd;
    int (*fn) (int, const char**, const char*);
    int option;
};

static struct cmd_struct commands[] = {
    {"add", cmd_add, 0},
    {"list", cmd_list, 0},
    {"search", cmd_search, 0},
    {"rm", cmd_rm, 0}
};

static struct cmd_struct *get_builtin(const char *s)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(commands); i++) {
        struct cmd_struct *p = commands+i;
        if (!strcmp(s, p->cmd))
            return p;
    }
    return NULL;
}

static int run_builtin(struct cmd_struct *p, int argc, const char **argv)
{
    int status;
    const char *prefix = NULL;
    status = p->fn(argc, argv, prefix);
    return status;
}

int is_builtin(const char *s)
{
    return !!get_builtin(s);
}

int main(int argc, char **av)
{
    const char **argv = (const char **)av;
    const char *cmd;
    struct cmd_struct *builtin;

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <action> [option...]" << std::endl;
        return -1;
    }

    argv++;
    argc--;
    cmd = argv[0];

    if (!is_builtin(cmd))
    {
        std::cout << "action \"" << cmd << "\"" << " not exist"<< std::endl;
        return -1;
    }

    builtin = get_builtin(cmd);
    exit(run_builtin(builtin, argc, argv));

    return 0;
}
