// Copyright (c) 2012 The Foundry Visionmongers Ltd. All Rights Reserved.


#ifndef FnScenegraphGenerator_H
#define FnScenegraphGenerator_H

#include <FnPluginSystem/FnPluginSystem.h>
#include <FnPluginSystem/FnPlugin.h>
#include <FnScenegraphGenerator/suite/FnScenegraphGeneratorSuite.h>
#include <FnAttribute/FnAttribute.h>

#include <vector>
#include <string>

namespace Foundry
{
    namespace Katana
    {


        /**
         * \defgroup SG Scenegraph Generator (SGG) Plug-In API
         * @{
         *
         * @brief The Scenegraph Generator plug-in API
         * The Scenegraph Generator API can be extended to create arbitrary scenegraph structures
         * through simple and well defined procedures. When designing, for example, a geometry
         * format importer this is the API that should be used.
         *
         * The core idea is that the main entry point for a new plug-in extends ScenegraphGenerator.
         * It can accept arguments and return a context (ScenegraphContext) responsible for
         * the creation of the first location of the portion of Scenegraph generated by the plugin
         * (referred as the root location of the Scenegraph Generator).
         *
         * The root and any other subsequent ScenegraphContexts return their sibling and child context as
         * well as any attributes which define the nodes. This tells the Scenegraph Generator API
         * how to traverse through the user defined contexts, starting from the root, which in turn
         * populates the scenegraph with the desired Locations (with their attributes and values).
         *
         * In order to create a Scenegraph Generator Plugin the ScenegraphGenerator abstract class
         * must be extended and registered using the <i>DEFINE_SGG_PLUGIN(class)</i> and the
         * <i>REGISTER_PLUGIN(class, name, major_version, minor_version)</i> macros.
         *
         * In order to use these plugins a ScenegraphGeneratorSetup node should be used. The
         * plugin will execute with an implicit resolver (triggered by a render, for example)
         * or forced to resolve by using the ScenegraphGeneratorResolve node.
         *
         */

        class ScenegraphContext;

        /**
         * @brief The main entry point for the plug-in. Any new plug-in must extend this interface
         * (once per plug-in). It tells Katana which arguments the plug-in takes and what is the
         * ScenegraphContext responsible for generating the root Location.
         */
        class ScenegraphGenerator
        {
        public:
            ScenegraphGenerator() {}
            virtual ~ScenegraphGenerator() {}
            /**
             * A static function that allows Katana to query the AMP about which arguments
             * it expects. This should return a GroupAttribute with the same structure
             * as the one that is passed to the constructor, in which the Attribute values
             * will be the default values of the correspondent AMP argument.
             */
            static GroupAttribute getArgumentTemplate();
            /**
             * Prior to running the plug-in, this method defines which arguments this plug-in listens for and
             * can therefore be sent in through Katana.
             *
             * Note: Katana reserves the top-level "system" argument for the arguments passed in
             * automatically.  Adding an expected arg beginning with "system." produces an error.
             *
             *
             */
            /* This method is deprecated; the class method getArgumentTemplate() is the thing to use now. */
            //virtual GroupAttribute getExpectedArgs() = 0;
            /**
             * Here the arguments defined by getArgumentTemplate are received from Katana where they can be used
             * at runtime by the plug-in.
             * @param args The arguments defined in getArgumentTemplate
             *
             */
            virtual bool setArgs(GroupAttribute args) = 0;
            /**
             * ...
             */
            virtual bool checkArgs(GroupAttribute args);
            /**
             * Returns a context to the root node in the scenegraph.
             *
             * This is the first step of traversing the contexts in order to create
             * a scenegraph structure where the subsequent steps will involve
             * retrieving the child and sibling nodes.
             *
             */
            virtual ScenegraphContext * getRoot() = 0;

            ///@cond FN_INTERNAL_DEV

            static FnPlugStatus setHost(FnPluginHost *host);
            static FnPluginHost *getHost();

            static FnScenegraphGeneratorPluginSuite_v1 createSuite(FnScenegraphGeneratorHandle (*create)(), FnAttributeHandle (*getArgumentTemplate)());

            static FnScenegraphGeneratorHandle newScenegraphGeneratorHandle(ScenegraphGenerator *sg);

            static unsigned int _apiVersion;
            static const char* _apiName;

        private:
            static FnPluginHost *_host;

            ///@endcond
        };


        /**
         * @brief Class responsible for providing Katana the information needed to generate a Location.
         *
         *  Each context implemented in a custom plug-in must extend this class. Typically, at least two contexts are
         * required where the first context is used for the root node which is appended to the scenegraph, while the
         * second context (and others) is used for a specific node type that is required to create the necessary
         * node types (structures, geometry, etc.).
         */
        class ScenegraphContext
        {
        public:
            ScenegraphContext() {}
            virtual ~ScenegraphContext() {}

            /**
             * Returns a context to the first child node in the scenegraph.
             *
             * Note: Returning a null context is done through 'return 0x0;' such as in
             * the case for the context for a leaf node.
             */
            virtual ScenegraphContext *getFirstChild() const = 0;
            /**
             * Returns a context to the next sibling in the scenegraph.
             *
             * Note: Returning a null context is done through 'return 0x0;' such as in
             * the case for the context for a root node.
             */
            virtual ScenegraphContext *getNextSibling() const = 0;

            /**
             * Prior to using the plug-in, this method defines which attributes this context has
             * defined and will be generated at runtime (through getLocalAttrNames).
             * @param names A reference to the vector that will contain the attributes for this context
             *
             */
            virtual void getLocalAttrNames(std::vector<std::string> *names) const = 0;
            /**
             * Set the attribute values.
             *
             * The API calls this method with the attribute names that were set using
             * getLocalAttrNames (called as many times as the number of defined attributes).
             * The method should return a Attribute object based on what attribute was
             * passed in.
             * @param name An arbitrary attribute name which forms a subset of the attributes
             *             defined using getLocalAttrNames for this context.
             *
             */
            virtual Attribute getLocalAttr(const std::string & name) const = 0;

            ///@cond FN_INTERNAL_DEV

            static FnSGContextHandle newSGContextHandle(ScenegraphContext *context);

            //@endcond
        };

        /// @}

    }//namespace
}//namespace

namespace FnKat = Foundry::Katana;

///@cond FN_INTERNAL_DEV


// Plugin Registering Macro.

#define DEFINE_SGG_PLUGIN(SGG_CLASS)                                                                        \
                                                                                                            \
    FnPlugin SGG_CLASS##_plugin;                                                                            \
                                                                                                            \
    FnScenegraphGeneratorHandle SGG_CLASS##_create(void)                                                    \
    {                                                                                                       \
         return Foundry::Katana::ScenegraphGenerator::newScenegraphGeneratorHandle(SGG_CLASS::create());    \
    }                                                                                                       \
                                                                                                            \
    FnAttributeHandle SGG_CLASS##_getArgumentTemplate()                                                     \
    {                                                                                                       \
        Foundry::Katana::GroupAttribute attr = SGG_CLASS::getArgumentTemplate();                            \
        if (attr.isValid())                                                                                 \
        {                                                                                                   \
            attr.getSuite()->retainAttr(attr.getHandle());                                                  \
        }                                                                                                   \
        return attr.getHandle();                                                                            \
    }                                                                                                       \
    FnScenegraphGeneratorPluginSuite_v1 SGG_CLASS##_suite =                                                 \
        Foundry::Katana::ScenegraphGenerator::createSuite(SGG_CLASS##_create,                               \
                                           SGG_CLASS##_getArgumentTemplate );                               \
                                                                                                            \
    const void* SGG_CLASS##_getSuite()                                                                      \
    {                                                                                                       \
        return &SGG_CLASS##_suite;                                                                          \
    }                                                                                                       \

///@endcond

#endif // FnScenegraphGenerator_H
