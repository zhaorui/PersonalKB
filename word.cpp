#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <vector>
#include <cstddef>  //std::size_t
#include <string>
#include "compat-util.h"
#include "word.h"

struct tag_format
{
    const char* name;
    const char* prefix;
    const char* suffix;
};

static struct tag_format tag_format_table[] = {
    {"<dict>",      NULL,           "\n"},
    {"</dict>",     NULL,           "\n"},
    {"<item>",      "    ",         "\n"},
    {"</item>",     "    ",         "\n"},
    {"<word>",      "        ",     NULL},
    {"</word>",     NULL,           "\n"},
    {"<explain>",   "        ",     NULL},
    {"</explain>",  NULL,           "\n"},
    {"<samples>",   "        ",     "\n"},
    {"</samples>",  "        ",     "\n"},
    {"<line>",      "            ", NULL},
    {"</line>",     NULL,           "\n"},
};

static struct tag_format *get_format(const char *s)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(tag_format_table); i++) {
        if (!strcmp(s, tag_format_table[i].name))
            return &tag_format_table[i];
    }
    return NULL;
}

int fix_tags(std::string &s)
{
    //unsigned int l,r,start_pos; should not use unsigned int here, size_t is correct
    std::size_t l=0 ,r=0, start_pos = 0;
    std::string tag;

    while ( (l = s.find_first_of('<', start_pos)) != std::string::npos)
    {
        r = s.find_first_of('>', l+1);
        if ( r == std::string::npos ) {
            return -1;
        }

        tag = s.substr(l, r-l+1);

        struct tag_format *f = get_format(tag.c_str());
        if (f)
        {
            if (f->prefix)
            {
                s.insert(l, f->prefix);
                l += strlen(f->prefix);
                r += strlen(f->prefix);
                start_pos = r + 1;
            }

            if (f->suffix)
            {
                s.insert(r+1, f->suffix);
                start_pos = r + strlen(f->suffix);
            }

        }
        else
        {
            start_pos = r + 1;
            continue;
        }
    }

    return 0;
}

std::istream& operator>> (std::istream& in, Word& word)
{
    std::string tmp;
    std::cout << "word: ";
    std::getline(in, word.m_word);

    std::cout << "explain:"<<std::endl;
    while (getline(in, tmp), !in.eof())
    {
        tmp.append("\n");
        word.m_explain += tmp;
    }

    in.clear();

    std::cout << "samples:"<<std::endl;
    while (getline(in, tmp), !in.eof())
    {
        word.m_samples.push_back(tmp);
    }
    
    in.clear();
}


// ------ Member function of class WordBook -----

WordBook::WordBook(std::string name)
    :filename(name)
{
}

void WordBook::addWord(const Word& word)
{
    m_words.push_back(word);
}

void WordBook::load()
{
    load(filename);
}

void WordBook::load(const std::string &filename)
{
    using boost::property_tree::ptree;
    ptree pt_dict;

    if (this->filename != filename)
        this->filename = filename;

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

void WordBook::save()
{
    save(filename);
}

void WordBook::save(const std::string &filename)
{
    using boost::property_tree::ptree;
    ptree pt_dict;

    if (this->filename != filename)
        this->filename = filename;

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

int WordBook::format()
{
    std::string line;
    std::ifstream infile(filename.c_str());
    std::vector<std::string> lines;
    if (!infile)
    {
        //need to throw exception here
        return -1;
    }
    while (getline(infile,line))
    {
        struct tag_locate *p = NULL;
        struct tag_format *f = NULL;
        fix_tags(line);
        lines.push_back(line);
    }
    infile.close();

    //resave the file
    std::ofstream outfile(filename.c_str());
    if (!outfile)
    {
        //need to throw exception here
        return -1;
    }
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
        outfile << *it << std::endl;
    return 0;
}
