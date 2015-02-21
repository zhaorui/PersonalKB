#include "cmd.h"
#include "word.h"
#include <iostream>
#include <string>
#include <boost/property_tree/exceptions.hpp>

const char *file = "dict.xml";
static WordBook book(file);

static int setup (void)
{

    try
    {
        book.load();
    }
    catch (boost::property_tree::ptree_bad_path &e)
    {
        std::cout << "ptree_bad_path" << e.what() << std::endl;
    }
    catch (boost::property_tree::ptree_bad_data &e)
    {
        std::cout << "ptree_bad_data" << e.what() << std::endl;
    }
    catch (boost::property_tree::ptree_error &e)
    {
        std::cout << "ptree_error" << e.what() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout<< "std::exception " << e.what() << std::endl;
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


int cmd_add(int argc, const char** argv, const char* prefix)
{
    Word word;
    setup();

    std::cin >> word;
    book.addWord(word);

    save();

    return 0;
}

int cmd_list(int argc, const char** argv, const char* prefix)
{
    return 0;
}
    
int cmd_search(int argc, const char** argv, const char* prefix)
{
    return 0;
}

int cmd_rm(int argc, const char** argv, const char* prefix)
{
    return 0;
}
