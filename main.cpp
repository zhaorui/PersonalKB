#include "cmd.h"
#include "word.h"
#include "compat-util.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <boost/property_tree/exceptions.hpp>

const char *file = "dict.xml";
WordBook book(file);

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

static int setup (void)
{
    try
    {
        book.load();
    }
    catch (boost::property_tree::ptree_bad_path &e)
    {
        std::cout << "ptree_bad_path: " << e.what() << std::endl;
    }
    catch (boost::property_tree::ptree_bad_data &e)
    {
        std::cout << "ptree_bad_data: " << e.what() << std::endl;
    }
    catch (boost::property_tree::ptree_error &e)
    {
        std::cout << "ptree_error: " << e.what() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout<< "std::exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "other exception" << std::endl;
    }

    return 0;
}

static int save (void)
{
    try
    {
        book.save();
        book.format();
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << "\n";
    }
}

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

    setup();
    builtin = get_builtin(cmd);
    run_builtin(builtin, argc, argv);
    save();
    
    return 0;
}
