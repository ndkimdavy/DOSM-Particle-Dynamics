#include "dosmMetric.hpp"

int main(int argc, char** argv)
{
    dosm::DosmMetric dosmMetric("doc/particule");
    dosmMetric.run();
    return 0;
}

