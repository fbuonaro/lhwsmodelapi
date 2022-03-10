#ifndef __LHWSMODELAPI_APIEXCEPTION_H__
#define __LHWSMODELAPI_APIEXCEPTION_H__

#include <stdexcept>
#include <string>

#include <lhwsmodelapi/apierror.h>

namespace LHWSModelApiNS
{
    class ApiException : public std::exception
    {
        public:
            ApiException( ApiError _apiError );
            ApiException( int errorCode, std::string errorMessage );
            const char* what() const throw();
            const ApiError& GetApiError() const throw();

        private:
            ApiError apiError;
    };
}

#endif

