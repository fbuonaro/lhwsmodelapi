#ifndef __LHWSModelApiNS_ApiError_lhmodel_H__
#define __LHWSModelApiNS_ApiError_lhmodel_H__

#include <cstdint>
#include <string>

#include <lhmodel/membermeta.h>
#include <lhmodel/modelmeta.h>
#include <lhmodel/visitor.h>

#include <array>
#include <unordered_map>

namespace LHWSModelApiNS
{
    class ApiError;
}

namespace LHModelNS
{
    template<>
    class MemberType< LHWSModelApiNS::ApiError >
    {
        public:
            typedef ModelMemberType valueType;
    };

    template<>
    class ModelMembersMeta< LHWSModelApiNS::ApiError >
    {
        public:
            static constexpr std::array< MemberMeta, 2 > membersMeta{ {
                MemberMeta( "errorMessage", "std::string", "" ),
                MemberMeta( "errorCode", "std::int64_t", "" ) } };

            static const std::unordered_map< std::string, size_t > memberIndex;
    };

    template<>
    class ModelModelMeta< LHWSModelApiNS::ApiError >
    {
        public:
            static constexpr ModelMeta modelMeta{ "ApiError" };
    };

}

namespace LHWSModelApiNS
{
    class ApiError
    {
        public:
            template< typename V >
            void Accept( V& visitor ) const
            {
                visitor._VisitMember( LHModelNS::GetMemberMeta< ApiError, 0 >(), errorMessage );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ApiError, 1 >(), errorCode );
            }


            template< typename V >
            void Accept( V& visitor )
            {
                visitor._VisitMember( LHModelNS::GetMemberMeta< ApiError, 0 >(), errorMessage );
                visitor._VisitMember( LHModelNS::GetMemberMeta< ApiError, 1 >(), errorCode );
            }

            template< typename V >
            void Accept( V& visitor, const std::string& member ) const
            {
                size_t memberIndex = LHModelNS::GetMemberIndex< ApiError >( member );
                switch( memberIndex )
                {
                    default:
                    {
                        break;
                    }
                    case( 0 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ApiError, 0 >(), errorMessage );
                        break;
                    }
                    case( 1 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ApiError, 1 >(), errorCode );
                        break;
                    }
                }
            }

            template< typename V >
            void Accept( V& visitor, const std::string& member )
            {
                size_t memberIndex = LHModelNS::GetMemberIndex< ApiError >( member );
                switch( memberIndex )
                {
                    default:
                    {
                        break;
                    }
                    case( 0 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ApiError, 0 >(), errorMessage );
                        break;
                    }
                    case( 1 ):
                    {
                        visitor._VisitMember( LHModelNS::GetMemberMeta< ApiError, 1 >(), errorCode );
                        break;
                    }
                }
            }

            std::string errorMessage;
            std::int64_t errorCode;
    };
}

// Swagger Codegen OpenAPI v3 LHModelServerCodegen v0.0.1

#endif
