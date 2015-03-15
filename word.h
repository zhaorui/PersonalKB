#ifndef WORD_H
#define WORD_H

#include <string>
#include <list>

class Word;
class WordBook;

std::istream& operator>> (std::istream&, Word&);
std::ostream& operator<< (std::ostream&, const Word&);

struct Word
{
    std::string m_word;
    std::string m_explain;
    std::list<std::string> m_samples;
};

class WordBook
{
public:
    WordBook(std::string file);
    void addWord(const Word&);
    bool isWordExist(const std::string&);
    void load();
    void load(const std::string &filename);
    void save();
    void save(const std::string &filename);
    int  format();

    std::list<Word> m_words;
    std::string filename;
};

#endif
