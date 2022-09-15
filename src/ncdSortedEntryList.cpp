#include <ncdSortedEntryList.hpp>
#include <selectedSequence.hpp>
#include <ncdHelper.hpp>
#include <algorithm>

using namespace std;

NCDSortedEntryList::NCDSortedEntryList(void): sorted(false) {
   skSelected = SelectedSequence::instance().getSelectedCRC();
}

void NCDSortedEntryList::addEntry(const NameAndCRC& nameAndCRC) {
    sorted = false;
    EntryAndNCD entry;
    NCDHelper& ncdHelper = NCDHelper::instance();
    entry.entry = nameAndCRC;
    entry.ncd = ncdHelper.getNCD(skSelected, entry.entry.crc);
    entries.push_back(entry);
}

const std::vector<EntryAndNCD>& NCDSortedEntryList::getSortedList(void) {
    if (!sorted) {
        sort(entries.begin(), entries.end(), CompareByNCD());
        sorted = true;
    }
    return entries;
}

const std::vector<EntryAndNCD> NCDSortedEntryList::getTruncatedSortedList(uint32_t howMany) {
    getSortedList();
    if (howMany >= entries.size()) {
        return entries;
    }
    std::vector<EntryAndNCD> result(entries.begin(), entries.begin() + howMany);
    return result;
}

template<typename T>
std::vector<double> linspace(T start_in, T end_in, int num_in)
{

  std::vector<double> linspaced;

  double start = static_cast<double>(start_in);
  double end = static_cast<double>(end_in);
  double num = static_cast<double>(num_in);

  if (num == 0) { return linspaced; }
  if (num == 1) 
    {
      linspaced.push_back(start);
      return linspaced;
    }

  double delta = (end - start) / (num - 1);

  for(int i=0; i < num-1; ++i)
    {
      linspaced.push_back(start + delta * i);
    }
  linspaced.push_back(end); // I want to ensure that start and end
                            // are exactly the same as the input
  return linspaced;
}

const std::vector<EntryAndNCD> NCDSortedEntryList::getLinspacedList(uint32_t howMany) {
    getSortedList();
    if (howMany >= entries.size()) {
        return entries;
    }
    printf("%d\n", entries.size());
    std::vector<double> linspaced = linspace(0, int(entries.size() - 1), howMany);
    std::vector<EntryAndNCD> result;
    for (auto i = linspaced.begin(); i != linspaced.end(); ++i) {
        result.push_back(entries[int(*i)]);
    }
    return result;
}
