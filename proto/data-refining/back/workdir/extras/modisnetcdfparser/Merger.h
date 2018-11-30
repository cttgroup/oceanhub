#ifndef MERGER_H
#define MERGER_H

#include <vector>
#include <map>
#include <string>

#include "ComponentNT.h"
#include "floatops.h"

static bool getMergeIndices(const std::vector<component2f> &__restrict coords,
                            std::vector<int> *__restrict indices,
                            const size_t checklenght,
                            const float delta)
{
    if (coords.empty())
        return false;
    indices->clear();
    indices->resize(coords.size());
    size_t i;
    for (i = 0; i < indices->size(); ++i)
        (*indices)[i] = i;
    i = 0;
    while (i < coords.size())
    {
        const component2f ci = coords[i];
        size_t first_unsetj = i + 1;
        const size_t lim = (i + checklenght) < coords.size() ? (i + checklenght) : coords.size();
        for (size_t j = i+1; j < lim; ++j)
        {
            if ((*indices)[j] == j)
            {
                const component2f cj = coords[j];
                if ((abs1f(cj.x-ci.x) < delta)&(abs1f(cj.y-ci.y) < delta))
                {
                    (*indices)[j] = i;
                }
                else
                {
                    first_unsetj = first_unsetj != (i + 1) ? j : first_unsetj;
                }
            }
        }
        i = first_unsetj;
        if (!(i&0xFF))
            std::cout << "\r" << i+1 << " of " << coords.size(); std::cout.flush();
    }
    std::cout << "\n";
    return true;
}

static bool merge(std::vector<component2f> *__restrict coords,
                  std::vector<double> *__restrict values,
                  const size_t checklenght,
                  const float delta)
{
    if (coords->size()!=values->size())
        false;
    std::vector<int> indices;
    if (!getMergeIndices(*coords, &indices, checklenght, delta))
        return false;
    std::vector<int> cnt; cnt.assign(coords->size(), 0);
    std::vector<double> vbuf; vbuf.assign(coords->size(), 0.0);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        const int idx = indices[i];
        ++cnt[idx];
        vbuf[idx] += (*values)[i];
    }
    values->clear();
    values->reserve(cnt.size());
    for (size_t i = 0; i < cnt.size(); ++i)
    {
        if (cnt[i])
            values->push_back(vbuf[i] / (double)cnt[i]);
    }
    vbuf.clear(); vbuf.shrink_to_fit();
    std::vector<component2f> cbuf; cbuf.reserve(cnt.size());
    for (size_t i = 0; i < cnt.size(); ++i)
    {
        if (cnt[i])
            cbuf.push_back((*coords)[i]);
    }
    *coords = std::move(cbuf);
    return true;
}

#endif // MERGER_H
