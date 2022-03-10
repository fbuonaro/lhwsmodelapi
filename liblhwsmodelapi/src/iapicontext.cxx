#include <lhwsmodelapi/iapicontext.h>

#include <stdexcept>

#include <lhmiscutil/singleton.h>

namespace LHWSModelApiNS
{
    IApiContext::IApiContext()
    {
    }

    IApiContext::~IApiContext()
    {
    }

    GetApiContextParams::GetApiContextParams()
    :   checkJwt( CheckJwt::None )
    ,   isGet( false )
    ,   paramName()
    {
    }

    IApiContextFactory::IApiContextFactory()
    {
    }

    IApiContextFactory::~IApiContextFactory()
    {
    }

namespace
{
    bool getBearerAccessToken( cppcms::http::request& cppcmsRequest,
                               std::string& bearerAccessToken )
    {
        bool ret = true;

        const auto& headers( cppcmsRequest.getenv() );

        auto itBearerAccessToken = headers.find( "HTTP_AUTHORIZATION" );
        if( itBearerAccessToken != headers.cend() )
        {
            auto wsIndex = itBearerAccessToken->second.find( ' ' );
            if( ( wsIndex == std::string::npos ) || ( wsIndex == itBearerAccessToken->second.size() - 1 ) )
            {
                ret = false;
            }
            else
            {
                bearerAccessToken = itBearerAccessToken->second.substr( wsIndex + 1 );
            }
        }
        else
        {
            ret = false;
        }

        return ret;
    }
}

    std::unique_ptr< LHWSUtilNS::IValidJwt > GetJwt( cppcms::http::request& cppcmsRequest, CheckJwt checkJwt )
    {
        bool introspect = false;
        std::unique_ptr< LHWSUtilNS::IValidJwt > ret;

        switch( checkJwt )
        {
            case( CheckJwt::IntrospectBearer ):
            {
                introspect = true;
                // fallthrough intentional
            }
            case( CheckJwt::ValidateBearer ):
            {
                std::string bearerAccessToken;
                if( getBearerAccessToken( cppcmsRequest, bearerAccessToken ) )
                {
                    auto jwtValidatorFactory( LHMiscUtilNS::Singleton< LHWSUtilNS::IJwtValidatorFactory >::GetInstance() ); 
                    if( !( jwtValidatorFactory ) )
                    {
                        throw std::runtime_error( "could not get jwt validator factory" );
                    }

                    auto jwtValidator( jwtValidatorFactory->CreateJwtValidator() );
                    if( !( jwtValidator ) )
                    {
                        throw std::runtime_error( "could not create jwt validator" );
                    }

                    if( introspect )
                    {
                        ret = jwtValidator->IntrospectJwt( bearerAccessToken );
                    }
                    else
                    {
                        ret = jwtValidator->ValidateIntoJwt( bearerAccessToken );
                    }
                }

                break;
            }
            case( CheckJwt::None ):
            default:
            {
                break;
            }
        }

        return ret;
    }

    std::unique_ptr< IApiContext > GetApiContext( cppcms::application& cppcmsApplication,
                                                  const GetApiContextParams& params )
    {
        auto apiContextFactory( LHMiscUtilNS::Singleton< IApiContextFactory >::GetInstance() ); 
        if( !( apiContextFactory ) )
        {
            throw std::runtime_error( "could not get api context factory" );
        }

        auto apiContext( apiContextFactory->CreateApiContext( cppcmsApplication, params ) );
        if( !( apiContext ) )
        {
            throw std::runtime_error( "could not create api context" );
        }

        return apiContext;
    }

}
