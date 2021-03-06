#ifndef __LHWSMODELAPI_PARSEMODEL_H__
#define __LHWSMODELAPI_PARSEMODEL_H__

#include <stdexcept>
#include <sstream>
#include <string>

#include <cppcms/http_request.h>
#include <cppcms/http_response.h>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include <lhmodelutil/lhmodel_rapidjson.h>

namespace LHWSModelApiNS
{
    template< class T >
    class ParseModel
    {
        public:
            static T FromJsonStr( const std::string& requestParamJsonStr );

            static T FromReqJsonParam( cppcms::http::request& cppcmsRequest,
                                       bool isGet,
                                       const std::string& paramName );

            static void IntoRespAsJsonStr( cppcms::http::response& cppcmsResponse,
                                           const T& responseModel );
    };

    template< class T >
    T ParseModel< T >::FromJsonStr( const std::string& requestParamJsonStr )
    {
        rapidjson::Document requestParamJson;
        rapidjson::ParseResult parsedOkay;
        std::ostringstream oss;

        parsedOkay = requestParamJson.Parse( requestParamJsonStr.c_str() );
        if( !( parsedOkay ) )
        {
            oss << "JSON parse error: " << rapidjson::GetParseError_En( parsedOkay.Code() ) << " " << parsedOkay.Offset();
            throw ApiException( 1, oss.str() );
        }

        T requestModel;
        if( !( LHModelUtilNS::DeserializeValue( requestModel, requestParamJson, &oss, true ) ) )
        {
            throw ApiException( 1, oss.str() );
        }

        return requestModel;
    }

    template< class T >
    T ParseModel< T >::FromReqJsonParam( cppcms::http::request& cppcmsRequest,
                                         bool isGet,
                                         const std::string& paramName )
    {
        std::string requestParamJsonStr;

        if( isGet )
        {
            requestParamJsonStr = cppcmsRequest.get( paramName );
        }
        else
        {
            if( paramName.empty() )
            {
                auto rawPostData( cppcmsRequest.raw_post_data() );

                if( rawPostData.first && rawPostData.second )
                {
                    requestParamJsonStr.assign( static_cast< char* >( rawPostData.first ), rawPostData.second );
                }
            }
            else
            {
                requestParamJsonStr = cppcmsRequest.post( paramName );
            }
        }

        if( requestParamJsonStr.empty() )
        {
            std::ostringstream oss;
            oss << "Missing " << ( isGet ? "GET" : "POST" ) << " parameter[" << paramName << "]";
            throw ApiException( 1, oss.str() );
        }

        return FromJsonStr( requestParamJsonStr );
    }

    template< class T >
    void ParseModel< T >::IntoRespAsJsonStr( cppcms::http::response& cppcmsResponse,
                                             const T& responseModel )
    {
        rapidjson::StringBuffer jsonStrBuffer;
        rapidjson::Writer< rapidjson::StringBuffer > jsonWriter( jsonStrBuffer );
        
        LHModelUtilNS::SerializeModel( jsonWriter, responseModel );

        if( !( jsonStrBuffer.GetString() ) )
        {
            throw ApiException( 2, "Failed to serialize response as JSON" );
        }

        cppcmsResponse.out() << jsonStrBuffer.GetString();
    }
}

#endif
