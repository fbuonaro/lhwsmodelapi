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

    const char* ApiError::what() const throw()
    {
        return apiError.c_str();
    }

    const ApiError& GetApiError() const throw()
    {
        return apiError;
    }
}
