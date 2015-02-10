#ifndef WORD_H
#define WORD_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <list>
#include <iostream>

struct Word
{
    std::string m_word;
    std::string m_explain;
    std::list<std::string> m_samples;
};

struct WordBook
{
    std::list<Word> m_words;
    void load(const std::string &filename);
    void save(const std::string &filename);
};

void WordBook::load(const std::string &filename)
{
    using boost::property_tree::ptree;
    ptree pt_dict;
    read_xml(filename, pt_dict);

    // I don't quite undestand the code here, why it's ptree::value_type??
    BOOST_FOREACH(ptree::value_type &v, pt_dict.get_child("dict"))
    {
            Word w;
            ptree pt_word = v.second;
            w.m_word = pt_word.get<std::string>("word");
            w.m_explain = pt_word.get<std::string>("explain");
            BOOST_FOREACH(ptree::value_type &v, pt_word.get_child("samples"))
            {
                w.m_samples.push_back(v.second.data());
            }
            m_words.push_back(w);
    }
}

void WordBook::save(const std::string &filename)
{
    using boost::property_tree::ptree;
    ptree pt_dict;
    BOOST_FOREACH(const Word &word, m_words)
    {
        ptree pt_word;
        pt_word.put("word", word.m_word);
        pt_word.put("explain", word.m_explain);
        BOOST_FOREACH(const std::string& line, word.m_samples)
        {
            pt_word.add("samples.line", line);
        }
        pt_dict.add_child("dict.item", pt_word);
    }
    write_xml(filename, pt_dict);
}

#endif
