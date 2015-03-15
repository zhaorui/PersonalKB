#include "cmd.h"
#include "word.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <string.h>

extern WordBook book;

int cmd_add(int argc, const char** argv, const char* prefix)
{
    std::string word;
    char file_ex[128];
    char file_samp[128];
    char cmd[128];

    memset(file_ex, 0, 128);
    memset(file_samp, 0, 128);
    if (!argv[1] || !argv[1][0]) {
        std::cout << "word: "; 
        std::cin >> word;
    }
    else
    {
        word = argv[1];
    }
    if (word.length() > 60 )
    {
        throw std::runtime_error("word is too long");
    }
    strncpy(file_ex, word.c_str(), word.length());
    strncpy(file_samp, word.c_str(), word.length());
    strncpy(file_ex+word.length(), ".ex.XXXXXX", 10);
    strncpy(file_samp+word.length(), ".sa.XXXXXX", 10);

    std::list<Word>::iterator it;
    for (it = book.m_words.begin(); it != book.m_words.end(); it++)
    {
        if (it->m_word == word)
        {
            /*FIXME Security issue - never use mktemp*/
            mkstemp(file_ex);
            mkstemp(file_samp);
            std::ofstream ex_out(file_ex);
            std::ofstream samp_out(file_samp);
            ex_out << "# This is the explain page for word" << std::endl;
            ex_out << it->m_explain;
            samp_out << "# This is the sample page for the word" << std::endl;
            for (auto item = it->m_samples.begin(); item != it->m_samples.end(); item++)
                samp_out << *item << std::endl;
            ex_out.close();
            samp_out.close();

            snprintf(cmd, 128, "vim -O %s %s", file_ex, file_samp);
            system(cmd);

            std::ifstream ex_in(file_ex);
            std::ifstream samp_in(file_samp);
            std::string line;
            it->m_explain.clear();
            while (getline(ex_in, line), !ex_in.eof())
            {
                if (line[0] != '#'){
                    it->m_explain += line;
                    it->m_explain += "\n";
                }
            }

            it->m_samples.clear();
            while(getline(samp_in, line), !samp_in.eof()){
                if (line[0] != '#'){
                    it->m_samples.push_back(line);
                }
            }

            remove(file_ex);
            remove(file_samp);
            break;
        }
    }

    if (it == book.m_words.end())
    {
        Word w;
        std::string tmp;
        int i = 0;
        w.m_word = word;

        //Input explanation of the word
        std::cout << "explain:"<<std::endl;
        putchar('>');
        while (getline(std::cin, tmp), !std::cin.eof())
        {
            w.m_explain += tmp;
            w.m_explain += "\n";
            putchar('>');
        }
        std::cin.clear();
        std::cout << std::endl;

        //Input samples for the word
        std::cout << "samples:"<<std::endl;
        do
        {
            std::cout << ++i << ". ";
            if (!tmp.empty())
                w.m_samples.push_back(tmp);
        }
        while (getline(std::cin, tmp), !std::cin.eof());
        std::cin.clear();
        std::cout << std::endl;

        book.addWord(w);
    }

    return 0;
}

int cmd_list(int argc, const char** argv, const char* prefix)
{
    std::list<Word>::iterator it;
    for (it = book.m_words.begin(); it != book.m_words.end(); it++)
    {
        std::cout << *it << std::endl;
    }
    return 0;
}
    
int cmd_search(int argc, const char** argv, const char* prefix)
{

    std::string word;
    if (!argv[1] || !argv[1][0]) {
        std::cout << "word: "; 
        std::cin >> word;
    }
    else
    {
        word = argv[1];
    }
    
    for (auto it = book.m_words.begin(); it != book.m_words.end(); it++)
    {
        if (it->m_word == word)
        {
            std::cout << *it << std::endl;
            return 0;
        }
    }

    std::cout << "\"" << word << "\" not exist in dictionary" << std::endl;
    return -1;
}

int cmd_rm(int argc, const char** argv, const char* prefix)
{
    std::string word;
    if (!argv[1] || !argv[1][0]) {
        std::cout << "word: "; 
        std::cin >> word;
    }
    else
    {
        word = argv[1];
    }

    std::list<Word>::iterator it;
    for (it = book.m_words.begin(); it != book.m_words.end(); it++)
    {
        if (it->m_word == word)
        {
            book.m_words.erase(it);
            std::cout << "erased successfully" << std::endl;
            return 0;
        }
    }

    std::cout << word << " is not exist" << std::endl;
    return 1;
}
