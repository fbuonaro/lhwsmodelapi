#include <lhwsmodelapi_impl/apicontext.h>

#include <stdexcept>

namespace LHWSModelApiImplNS
{
    ApiContext::ApiContext()
    :   LHWSModelApiNS::IApiContext()
    ,   jwt()
    {
    }

    ApiContext::~ApiContext()
    {
    }

    bool ApiContext::HasValidJwt() const
    {
        return ( jwt ? true : false );
    }

    const LHWSUtilNS::IValidJwt& GetValidJwt() const
    {
        if( !( jwt ) )
        {
            throw std::runtime_error( "jwt is not null" );
        }

        return *jwt;
    }

    void SetValidJwt( std::unique_ptr< LHWSUtilNS::IValidJwt > _jwt )
    {
        if( jwt )
        {
            throw std::runtime_error( "jwt is already set" );
        }

        if( !( _jwt ) )
        {
            return;
        }

        jwt = std::move( _jwt );
    }

    ApiContextFactory::ApiContextFactory()
    :   LHWSModelApiNS::IApiContextFactory()
    {
    }

    ApiContextFactory::~ApiContextFactory()
    {
    }

    const std::unique_ptr< LHWSModelApiNS::IApiContext > CreateApiContext(
        cppcms::application& cppcmsApplication,
        const LHWSModelApiNS::GetApiContextParams& params ) const
    {
        std::unique_ptr< ApiContext > apiContext( new ApiContext() );
        if( !( apiContext ) )
        {
            throw std::runtime_error( "failed to create api context" );
        }
        
        apiContext->SetValidJwt( GetJwt( cppcmsApplication.request(), params.checkJwt ) );

        return apiContext;
    }
}

namespace LHWSModelApiNS
{
    std::shared_ptr< IApiContextFactory > GetStandardApiContextFactory()
    {
        return std::make_shared< LHWSModelApiImplNS::ApiContextFactory >();
    }
}
