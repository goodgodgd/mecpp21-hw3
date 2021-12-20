#include "pcpmain.h"


int main(int argc, char **argv)
{
    cout<<"Start programs"<<endl;
    std::string path = argv[0];
    int pos = path.rfind("/build");
    path = path.substr(0, pos);
    cout << "path:" << path << endl;
    PCPMain pcp(path, "color1.png", "depth1.png");
    pcp.main();
}
