#include <lhwsmodelapi/apiexception.h>

namespace LHWSModelApiNS
{
    ApiException::ApiException( ApiError _apiError )
    :   apiError( std::move( _apiError ) )
    {
    }

    ApiException::ApiException( int errorCode, std::string errorMessage )
    :   apiError()
    {
        apiError.errorCode = errorCode;
        apiError.errorMessage = std::move( errorMessage );
    }

    const char* ApiException::what() const throw()
    {
        return apiError.errorMessage.c_str();
    }

    const ApiError& ApiException::GetApiError() const throw()
    {
        return apiError;
    }
}
