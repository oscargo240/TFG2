#include <treeMaker.hpp>

using namespace std;
void TreeMaker::makeTreeFile(const std::string& inputFilename, const std::string& outFilename) {
    string firstCmd = "docker run --rm --name ncd-app-instance -v /home/oscargo2/repos/ncd-covid-data/datosgisaideurope/intermediate:/expdir  ncd bash -c 'cd /expdir && maketree " + inputFilename + "'";
    //system(firstCmd.c_str());
    string secondCmd = "cd /home/oscargo2/repos/ncd-covid-data/datosgisaideurope/intermediate && ruby /home/oscargo2/repos/TFG2/tree_converter/convert_tree.rb >" + outFilename;
    //system(secondCmd.c_str());
    string thirdCmd = "rm -f /home/oscargo2/repos/ncd-covid-data/datosgisaideurope/intermediate/treefile.dot";
//    system(thirdCmd.c_str());
}

void TreeMaker::makeDiagramTree(const std::string& inputFilename, const std::string& outFilename) {
    string cmd = "cp /home/oscargo2/repos/ncd-covid-data/datosgisaideurope/intermediate/"+inputFilename + " intree && rm -f plotfile && (echo \"F\\nHershey\\nY\\n\" | /usr/lib/phylip/bin/drawgram) && mv plotfile /home/oscargo2/repos/ncd-covid-data/datosgisaideurope/intermediate/" + outFilename + " && rm intree";
    system(cmd.c_str());
}