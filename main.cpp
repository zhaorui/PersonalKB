#include "word.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    try
    {
       Word word;
       word.m_word = "hello";
       word.m_explain = "greetings in English, common use";
       word.m_samples.push_back ("hello there, I'm billzhao");
       word.m_samples.push_back ("hello, this is Sheldon");

       WordBook book;
       book.m_words.push_back(word);
       book.m_words.push_back(word);
       book.save("dictionary.xml");
    
       WordBook load_book;
       load_book.load("dictionary.xml");
       BOOST_FOREACH(Word & w, load_book.m_words)
       {
            std::cout << w.m_word << std::endl;
            std::cout << "\t" << w.m_explain << std::endl;
            for (std::list<std::string>::iterator it = w.m_samples.begin();
                    it != w.m_samples.end(); it++)
            {
                std::cout << "\t\t" <<*it << std::endl;
            }
       }
    }
    catch (std::exception &e)
    {
        std::cout << "Error: "<< e.what() << "\n";
    }
    return 0;
}
