docker run -v ${PWD}/liblhwsmodelapi/resource/swagger:/lhwsmodelapi --rm lhmodel-scg:main \
  -i /lhwsmodelapi/LHWSModelApi.private.json \
  -c /lhwsmodelapi/LHWSModelApi.private.config.json \
  --additional-properties \
    modelIncludeDir=lhwsmodelapi,\
    namespace=LHWSModelApiNS,\
    lcModelFilename=true,\
  -o /lhwsmodelapi \
  -v
cp ${PWD}/liblhwsmodelapi/resource/swagger/src/io/swagger/server/model/*.h ${PWD}/liblhwsmodelapi/include/lhwsmodelapi/lhwsmodelapi/
cp ${PWD}/liblhwsmodelapi/resource/swagger/src/io/swagger/server/model/*.cxx ${PWD}/liblhwsmodelapi/src/
