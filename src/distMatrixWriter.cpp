#include <distMatrixWriter.hpp>
#include <envutil.hpp>
#include <fstream>
#include <displayName.hpp>
#include <ncdHelper.hpp>
#include <iostream>
#include <chrono>

using namespace std;

static std::string forMatrix(const std::string& name) {
    std::string result;
    for (char c: name) {
        if (c == '/') {
            c = '_';
            result.push_back(c);
            result.push_back(c);
        }
        if (c == '-') {
            c = '_';
            result.push_back(c);
        }
        result.push_back(c);
    }
    return result;
}

DistMatrixWriter::DistMatrixWriter(void) {
}

void DistMatrixWriter::addNameAndCRC(const NameAndCRC& nameAndCRC) {
    list[nameAndCRC.crc] = nameAndCRC;
}

void DistMatrixWriter::writeDistanceMatrix(const std::string& shortFilename) {
    NCDHelper& ncdHelper = NCDHelper::instance();
    auto filename = getDataFilename("intermediate/" + shortFilename);
    ofstream f(filename);
    for (auto i = list.begin(); i != list.end(); ++i) {
        auto ancd = (*i).second;
        

        auto t_start = std::chrono::high_resolution_clock::now();
// the work...
       // do something
        f << forMatrix(smartName(ancd));
        int contDB = 0;
        for (auto j = list.begin(); j != list.end(); ++j) {
            auto bncd = (*j).second;
            auto t_start2 = std::chrono::high_resolution_clock::now();
            auto ncd = ncdHelper.getNCD(ancd.crc, bncd.crc);
            auto t_end2 = std::chrono::high_resolution_clock::now();
            double elapsed_time_ms2 = std::chrono::duration<double, std::milli>(t_end2-t_start2).count();
            //cout << "elapsed_time_one_s: " << elapsed_time_ms2 / 1000 << endl;
            if(elapsed_time_ms2/1000 < 0.01) {
                contDB++;
            }
            f << ' ' << ncd;
        }
        auto t_end = std::chrono::high_resolution_clock::now();

        double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();
        cout << endl <<endl << "elapsed_time_line_s: " << elapsed_time_ms / 1000 << endl;
        cout << "contDB: " << contDB << endl;
        contDB = 0;
         f << '\n';
    }
    f.close();
}