#ifndef __LHWSMODELAPI_IAPICONTEXT_H__
#define __LHWSMODELAPI_IAPICONTEXT_H__

#include <memory>
#include <string>

#include <cppcms/application.h>
#include <cppcms/http_request.h>

#include <lhmiscutil/singleton.h>
#include <lhwsutil/ijwtvalidator.h>

namespace LHWSModelApiNS
{
    class IApiContext
    {
        public:
            IApiContext();
            virtual ~IApiContext();

            virtual bool HasValidJwt() const = 0;
            virtual const LHWSUtilNS::IValidJwt& GetValidJwt() const = 0;
    };

    enum class CheckJwt
    {
        None,
        ValidateBearer,
        IntrospectBearer
    };

    struct GetApiContextParams
    {
        GetApiContextParams();

        CheckJwt checkJwt;
        bool isGet;
        std::string paramName;
    };

    class IApiContextFactory
    {
        public:
            IApiContextFactory();
            virtual ~IApiContextFactory();

            virtual std::unique_ptr< IApiContext > CreateApiContext( cppcms::application& cppcmsApplication,
                                                                     const GetApiContextParams& params ) const = 0;
    };

    std::unique_ptr< LHWSUtilNS::IValidJwt > GetJwt( cppcms::http::request& cppcmsRequest, CheckJwt checkJwt );

    std::unique_ptr< IApiContext > GetApiContext( cppcms::application& cppcmsApplication,
                                                  const GetApiContextParams& params );

    std::shared_ptr< IApiContextFactory > GetStandardApiContextFactory();
}

namespace LHMiscUtilNS
{
    EnableClassAsSingleton( LHWSModelApiNS::IApiContextFactory, SingletonCanBeSet::WhenEmpty );
}

#endif
