#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "SubwayCsvLoader.h"

std::vector<Segment> SubwayCsvLoader::Load(
    const std::string& csvPath)
{
    std::ifstream file(csvPath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "CSV 파일을 열 수 없습니다."
        );
    }

    std::vector<Segment> segments;

    std::string line;
    std::getline(file, line);

    if (!line.empty() && line.back() == '\r')
    {
        line.pop_back();
    }

    if (line != "line,from_station,to_station,time_seconds")
    {
        throw std::runtime_error(
            "CSV 헤더 형식이 올바르지 않습니다."
        );
    }

    int lineNumber = 1;

    while (std::getline(file, line))
    {
        lineNumber++;

        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (line.empty())
        {
            continue;
        }

        std::stringstream stream(line);

        std::string lineText;
        std::string fromStation;
        std::string toStation;
        std::string timeText;

        if (!std::getline(stream, lineText, ',') ||
            !std::getline(stream, fromStation, ',') ||
            !std::getline(stream, toStation, ',') ||
            !std::getline(stream, timeText, ','))
        {
            throw std::runtime_error(
                "CSV 데이터 형식이 올바르지 않습니다."
            );
        }

        if (fromStation.empty() || toStation.empty())
        {
            throw std::runtime_error(
                "역 이름이 비어 있습니다."
            );
        }

        int subwayLine = 0;
        int timeSeconds = 0;

        try
        {
            subwayLine = std::stoi(lineText);
            timeSeconds = std::stoi(timeText);
        }
        catch (...)
        {
            throw std::runtime_error(
                "호선 또는 시간이 숫자가 아닙니다."
            );
        }

        if (timeSeconds <= 0)
        {
            throw std::runtime_error(
                "이동 시간은 0보다 커야 합니다."
            );
        }

        Segment segment;

        segment.Line = subwayLine;
        segment.FromStation = fromStation;
        segment.ToStation = toStation;
        segment.TimeSeconds = timeSeconds;

        segments.push_back(segment);
    }

    return segments;
}