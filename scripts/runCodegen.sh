docker run -v ${PWD}/liblhwsmodelapi/resource/swagger/lhwsmodelapi:/lhwsmodelapi --rm lhmodel-scg:main \
  -i /lhwsmodelapi/LHWSModelApi.json \
  -c /lhwsmodelapi/LHWSModelApi.config.json \
  --additional-properties \
    modelIncludeDir=lhwsmodelapi,\
    namespace=LHWSModelApiNS,\
    lcModelFilename=true,\
  -o /lhwsmodelapi \
  -v
cp ${PWD}/liblhwsmodelapi/resource/swagger/lhwsmodelapi/src/io/swagger/server/model/*.h ${PWD}/liblhwsmodelapi/include/lhwsmodelapi/lhwsmodelapi/
cp ${PWD}/liblhwsmodelapi/resource/swagger/lhwsmodelapi/src/io/swagger/server/model/*.cxx ${PWD}/liblhwsmodelapi/src/
