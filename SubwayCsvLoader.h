#pragma once

#include <string>
#include <vector>

#include "SubwayTypes.h"

class SubwayCsvLoader
{
public:
    static std::vector<Segment> Load(
        const std::string& csvPath);
};