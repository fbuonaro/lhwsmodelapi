#ifndef __LHWSMODELAPI_MODELCONTEXT_H__
#define __LHWSMODELAPI_MODELCONTEXT_H__

#include <memory>
#include <stdexcept>

#include <cppcms/application.h>

#include <lhmiscutil/singleton.h>
#include <lhwsmodelapi/iapicontext.h>
#include <lhwsmodelapi/parsemodel.h>

namespace LHWSModelApiNS
{
    template< class T >
    class ApiRequestModelContext
    {
    public:
        ApiRequestModelContext( T _requestModel, std::unique_ptr< IApiContext > _context );

        const T& GetRequestModel() const;

        const IApiContext& GetContext() const;

        static ApiRequestModelContext< T > FromApplication( cppcms::application& cppcmsApplication,
            const GetApiContextParams& params );

    private:

        T requestModel;
        std::unique_ptr< IApiContext > context;
    };

    template< class T >
    const T& ApiRequestModelContext< T >::GetRequestModel() const
    {
        return requestModel;
    }

    template< class T >
    const IApiContext& ApiRequestModelContext< T >::GetContext() const
    {
        if ( !( context ) )
        {
            throw std::runtime_error( "context is null" );
        }

        return *context;
    }

    template< class T >
    ApiRequestModelContext< T >::ApiRequestModelContext( T _requestModel,
        std::unique_ptr< IApiContext > _context )
        : requestModel( std::move( _requestModel ) )
        , context( std::move( _context ) )
    {
        if ( !( context ) )
        {
            throw std::runtime_error( "provided context is null" );
        }
    }

    template< class T >
    ApiRequestModelContext< T > ApiRequestModelContext< T >::FromApplication( cppcms::application& cppcmsApplication,
        const GetApiContextParams& params )
    {
        T requestModel( ParseModel< T >::FromReqParam( cppcmsApplication.request(),
            params.isGet,
            params.paramName ) );

        auto apiContext( GetApiContext( cppcmsApplication, params ) );

        return ApiRequestModelContext< T >( std::move( requestModel ), std::move( apiContext ) );
    }
}
#endif
