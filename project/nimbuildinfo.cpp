#include "nimbuildinfo.h"
#include "nimbuildconfigurationfactory.h"

namespace NimPlugin {

NimBuildInfo::NimBuildInfo(const NimBuildConfigurationFactory *factory)
    : ProjectExplorer::BuildInfo(factory)
{}

}

