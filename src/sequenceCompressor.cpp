#include <sequenceCompressor.hpp>
#include <taggedSequenceCombination.hpp>
#include <cachingCompressor.hpp>
#include <sequenceReader.hpp>

using namespace std;

static SequenceCompressor *private_instance;

SequenceCompressor::SequenceCompressor(void)
{
}

SequenceCompressor& SequenceCompressor::instance(void) {
    if (private_instance == 0) {
        private_instance = new SequenceCompressor();
    }
    return *private_instance;
}

uint32_t SequenceCompressor::getCompressedSizeSingle(const std::string &crc)
{
    if (CachingCompressor::instance().hasTag(TaggedSequenceCombinationHelper::formSingleTag(crc)))
    {
        return CachingCompressor::instance().getTagSizeFast(crc);
    }
    TaggedSequenceCombination combo = TaggedSequenceCombinationHelper::formSingle(crc);
    string str = SequenceReader::instance().get_sequence(crc);
    uint32_t csize = CachingCompressor::instance().getTaggedCompressedSize(combo);
    return csize;
}

uint32_t SequenceCompressor::getCompressedSizePair(const std::string &crc1, const std::string &crc2)
{
    string tag = TaggedSequenceCombinationHelper::formPairTag(crc1, crc2);
    string tagInv = TaggedSequenceCombinationHelper::formPairTag(crc2, crc1);
    if (CachingCompressor::instance().hasTag(tag))
    {
        return CachingCompressor::instance().getTagSizeFast(tag);
    }else if(CachingCompressor::instance().hasTag(tagInv))
    {
        return CachingCompressor::instance().getTagSizeFast(tagInv);
    }
    TaggedSequenceCombination combo = TaggedSequenceCombinationHelper::formPair(crc1, crc2);
    uint32_t csize = CachingCompressor::instance().getTaggedCompressedSize(combo);
    return csize;
}
