
#include <lhwsmodelapi/apierror.h>

#include <lhmodel/membermeta.h>

#include <array>
#include <unordered_map>

namespace LHModelNS
{
    constexpr std::array< MemberMeta, 2 > ModelMembersMeta< LHWSModelApiNS::ApiError >::membersMeta;

    const std::unordered_map< std::string, size_t > ModelMembersMeta< LHWSModelApiNS::ApiError >::memberIndex{ {
    { "errorMessage", 0 },
    { "errorCode", 1 } } };

    constexpr ModelMeta ModelModelMeta< LHWSModelApiNS::ApiError >::modelMeta;
}

// Swagger Codegen OpenAPI v3 LHModelServerCodegen v0.0.1

