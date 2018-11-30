#ifndef STRINGOPS_H
#define STRINGOPS_H

#include <string>
#include <sstream>
#include <vector>
#include <memory.h>

static std::vector<std::wstring> split(const std::wstring &s, wchar_t delim)
{
    std::wstringstream ss(s);
    std::wstring item;
    std::vector<std::wstring> elems;
    while (std::getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}

static std::vector<std::string> split(const char *s, char delim)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}

static std::vector<std::vector<char> > split(const std::vector<char> &__restrict s, char delim)
{
    std::stringstream ss(s.data());
    std::string item;
    std::vector<std::vector<char> > elems;
    while (std::getline(ss, item, delim))
    {
        std::vector<char> as_vector;
        as_vector.assign(item.size()+1, 0);
        memcpy(as_vector.data(), item.data(), item.size());
        elems.push_back(as_vector);
    }
    return elems;
}

static std::vector<unsigned int> to_uint_vector(const std::vector<std::wstring> &__restrict strs)
{
    std::vector<unsigned int> uivec;
    uivec.reserve(strs.size());
    for (unsigned int i = 0; i < strs.size(); ++i)
    {
        unsigned int range[2] = {0,0};
        int ri = 0;
        unsigned int p = 0;
        for (unsigned int j = 0; j < strs[i].size(); ++j)
        {
            const wchar_t ch = strs[i][j];
            if ((ch==L'-')&(ri==0))
            {
                range[0] = p;
                p = 0;
                ++ri;
            }
            else
            {
                p *= 10;
                if (ch==L'0')
                    p += 0;
                else if (ch==L'1')
                    p += 1;
                else if (ch==L'2')
                    p += 2;
                else if (ch==L'3')
                    p += 3;
                else if (ch==L'4')
                    p += 4;
                else if (ch==L'5')
                    p += 5;
                else if (ch==L'6')
                    p += 6;
                else if (ch==L'7')
                    p += 7;
                else if (ch==L'8')
                    p += 8;
                else if (ch==L'9')
                    p += 9;
                else
                {
                    p = 0;
                    break;
                }
            }
        }
        range[ri] = p;
        if (range[0])
            uivec.push_back(range[0]);
        for (unsigned int j = range[0] + 1; j <= range[1]; ++j)
            uivec.push_back(j);
    }
    return uivec;
}

static std::vector<unsigned int> to_uint_vector(const std::vector<std::string> &__restrict strs)
{
    std::vector<unsigned int> uivec;
    uivec.reserve(strs.size());
    for (unsigned int i = 0; i < strs.size(); ++i)
    {
        unsigned int p = 0;
        for (unsigned int j = 0; j < strs[i].size(); ++j)
        {
            const char ch = strs[i][j];
            if ((ch > '9')&&(ch < '0'))
            {
                p = 0;
                break;
            }
            p *= 10;
            p += ch - '0';
        }
        if (p)
            uivec.push_back(p);
    }
    return uivec;
}

#endif // STRINGOPS_H

