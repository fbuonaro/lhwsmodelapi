#ifndef __LHWSMODELAPI_IMPL_APICONTEXT_H__
#define __LHWSMODELAPI_IMPL_APICONTEXT_H__

#include <memory>
#include <string>

#include <cppcms/application.h>
#include <cppcms/http_request.h>

#include <lhwsmodelapi/iapicontext.h>

namespace LHWSModelApiImplNS
{
    class ApiContext : public LHWSModelApiNS::IApiContext
    {
        public:
            ApiContext();
            ~ApiContext();

            bool HasValidJwt() const;
            const LHWSUtilNS::IValidJwt& GetValidJwt() const;
            void SetValidJwt( std::unique_ptr< LHWSUtilNS::IValidJwt > _jwt );

        private:
            std::unique_ptr< LHWSUtilNS::IValidJwt > jwt;
    };

    class ApiContextFactory : public LHWSModelApiNS::IApiContextFactory
    {
        public:
            ApiContextFactory();
            ~ApiContextFactory();

            std::unique_ptr< LHWSModelApiNS::IApiContext > CreateApiContext(
                cppcms::application& cppcmsApplication,
                const LHWSModelApiNS::GetApiContextParams& params ) const;
    };
}
#endif
