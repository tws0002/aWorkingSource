// Copyright (c) 2013 The Foundry Visionmongers Ltd. All Rights Reserved.

#include <FnGeolibServices/FnArgsFile.h>

#include <FnPluginManager/FnPluginManager.h>

FNGEOLIBSERVICES_NAMESPACE_ENTER
{
    FnAttribute::Attribute FnArgsFile::parseArgsFile(const std::string & filePath)
    {
        const FnArgsFileHostSuite_v1 * suite = _getSuite();
        if (!suite) return FnAttribute::Attribute();

        return FnAttribute::Attribute::CreateAndSteal(
                suite->parseArgsFile(filePath.c_str()));
    }

    void FnArgsFile::flushCache()
    {
        const FnArgsFileHostSuite_v1 * suite = _getSuite();
        if (!suite) return;

        suite->flushCache();
    }

    FnAttribute::Attribute FnArgsFile::getProceduralArgs(
            const std::string & assetId)
    {
        const FnArgsFileHostSuite_v1 * suite = _getSuite();
        if (!suite) return FnAttribute::Attribute();

        return FnAttribute::Attribute::CreateAndSteal(
                suite->getProceduralArgs(assetId.c_str()));

    }


    ////////////////////////////////////////////////////////////////////////////

    const FnArgsFileHostSuite_v1 *FnArgsFile::_getSuite()
    {
        static const FnArgsFileHostSuite_v1 * suite = 0x0;

        if (!suite)
        {
            suite = reinterpret_cast<const FnArgsFileHostSuite_v1 *>(
                    FnPluginManager::PluginManager::getHostSuite(
                            "ArgsFileHost", 1));
        }

        return suite;
    }
}
FNGEOLIBSERVICES_NAMESPACE_EXIT
