#include <iostream>
#include <selectedSequence.hpp>
#include <sequenceReader.hpp>
#include <ncdHelper.hpp>
#include <set>
#include <vector>
#include <entryAndNCD.hpp>
#include <seqNameHelper.hpp>
#include <ncdSortedEntryList.hpp>
#include <humanMito.hpp>
#include <datasource.hpp>
#include <distMatrixWriter.hpp>
#include <treeMaker.hpp>

using namespace std;

int main(int argc, char **argv)
{
    const int experiment_size = 500;
    auto skSelected = SelectedSequence::instance().getSelectedCRC();
    NCDHelper &ncdHelper = NCDHelper::instance();
    set<string> donelist;
    vector<EntryAndNCD> processed;
    NameAndCRC snac;
    SeqNameHelper &snh = SeqNameHelper::instance();
    NCDSortedEntryList sortedList;
    NCDSortedEntryList variantList;
    for (auto it = snh.nameToCRC.begin(); it != snh.nameToCRC.end(); ++it) {
        auto k = (*it).first;
        auto crc = (*it).second;
        if (donelist.find(crc) != donelist.end()) {
            continue;
        }
        auto ncd = ncdHelper.getNCD(skSelected, crc);
        NameAndCRC nameAndCRC;
        nameAndCRC.name = k;
        nameAndCRC.crc = crc;
        if (k.find("MN908947") != string::npos || nameAndCRC.crc == skSelected) {
            sortedList.addEntry(nameAndCRC);
            donelist.insert(crc);
        }
    }
    cout<<"primer bucle"<<endl;
    for (auto it = snh.nameToCRC.begin(); it != snh.nameToCRC.end(); ++it)
    {
        auto k = (*it).first;
        auto crc = (*it).second;
        if (donelist.find(crc) != donelist.end())
        {
            continue;
        }
        NameAndCRC nameAndCRC;
        nameAndCRC.crc = crc;
        nameAndCRC.name = k;
        if (classifyFAIorFastaFilename(nameAndCRC.name) == Gisaid &&
        nameAndCRC.crc != skSelected) {
            continue;
        }
        //sortedList.addEntry(nameAndCRC);
        //donelist.insert(crc);
        //se carga(elimina) los covid
        if (nameAndCRC.name.find("Vari") == string::npos &&
            nameAndCRC.name.find("hCoV-19/Australia/NSW870/2020") == string::npos &&
            nameAndCRC.name.find("hCoV-19/Chile/LI-93379/2021") == string::npos && 
            nameAndCRC.name.find("hCoV-19/India/KA-INSACOG_JNC_330/2022") == string::npos &&
            nameAndCRC.name.find("hCoV-19/India/un-LNHD8/2021") == string::npos &&
            nameAndCRC.name.find("hCoV-19/Australia/NSW1045/2020") == string::npos) {
            sortedList.addEntry(nameAndCRC);
            donelist.insert(crc);
        }

    }
    cout<<"segundo bucle"<<endl;
    //auto elist = sortedList.getLinspacedList(experiment_size);
    auto elist = sortedList.getTruncatedSortedList(1);
    cout<<elist.size()<<endl;
    for (auto it = snh.nameToCRC.begin(); it != snh.nameToCRC.end(); ++it)
    {
        auto k = (*it).first;
        auto crc = (*it).second;
        if (donelist.find(crc) != donelist.end())
        {
            continue;
        }
        NameAndCRC nameAndCRC;
        nameAndCRC.crc = crc;
        nameAndCRC.name = k;
        if (classifyFAIorFastaFilename(nameAndCRC.name) == Gisaid &&
        nameAndCRC.crc != skSelected) {
            continue;
        }
        //sortedList.addEntry(nameAndCRC);
        //donelist.insert(crc);
        //se carga(elimina) los covid
        if (nameAndCRC.name.find("Vari") != string::npos) {
            variantList.addEntry(nameAndCRC);
            donelist.insert(crc);
        }
    }

    auto vlist = variantList.getTruncatedSortedList(experiment_size);
    elist.insert(elist.end(), vlist.begin(), vlist.end());
    cout<<"size"<<elist.size()<<endl;

    EntryAndNCD human;
    human.entry = humanNameAndCRC;
    const clock_t begin_time = clock();
// do something
    human.ncd = ncdHelper.getNCD(skSelected, humanMitoCRCString);
    std::cout << "time: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << std::endl;
    elist.insert(elist.begin(), human);
    vector<NameAndCRC> list;
    DistMatrixWriter distMatrixWriter;
    for (auto e: elist) {
        distMatrixWriter.addNameAndCRC(e.entry);
    }
    std::string distMatrixName = "firsttreematrix.txt";
    distMatrixWriter.writeDistanceMatrix(distMatrixName);
    cerr << "Wrote " << distMatrixName << '\n';
    TreeMaker::makeTreeFile(distMatrixName, "firsttree.newick");
    //TreeMaker::makeDiagramTree("firsttree.newick", "firsttree.ps");
    return 0;
}