#include "module_manager.h"

ModuleManager::ModuleManager()
{
}

Status ModuleManager::loop()
{
    for (BaseModule *module : this->modules)
    {
        module->loop();
    }
    return Status::OK;
}

Status ModuleManager::setupModule(BaseModule *module)
{
    module->setup();
    modules.push_back(module);
    return Status::OK;
}