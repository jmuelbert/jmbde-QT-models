# -------------------------------------------------------------------------------------------------------------------- #
# Define directory path
# -------------------------------------------------------------------------------------------------------------------- #
set(MAIN_DIR ${CMAKE_SOURCE_DIR})
set(INCLUDE_DIR ${MAIN_DIR}/include)
set(SOURCE_DIR ${MAIN_DIR}/src)

# -------------------------------------------------------------------------------------------------------------------- #
# Define headers files
# -------------------------------------------------------------------------------------------------------------------- #
set(HEADERS
  ${INCLUDE_DIR}/jmbde/models/core/commondata.h
  ${INCLUDE_DIR}/jmbde/models/database/datacontext.h
  ${INCLUDE_DIR}/jmbde/models/account.h
  ${INCLUDE_DIR}/jmbde/models/chipcard.h
  ${INCLUDE_DIR}/jmbde/models/chipcarddoor.h
  ${INCLUDE_DIR}/jmbde/models/chipcardprofile.h
  ${INCLUDE_DIR}/jmbde/models/chipcardprofiledoor.h
  ${INCLUDE_DIR}/jmbde/models/device/computer.h
  ${INCLUDE_DIR}/jmbde/models/device/computersoftware.h
  ${INCLUDE_DIR}/jmbde/models/device/devicename.h
  ${INCLUDE_DIR}/jmbde/models/device/devicetype.h
  ${INCLUDE_DIR}/jmbde/models/device/fax.h
  ${INCLUDE_DIR}/jmbde/models/device/manufacturer.h
  ${INCLUDE_DIR}/jmbde/models/device/mobile.h
  ${INCLUDE_DIR}/jmbde/models/device/os.h
  ${INCLUDE_DIR}/jmbde/models/device/phone.h
  ${INCLUDE_DIR}/jmbde/models/device/printer.h
  ${INCLUDE_DIR}/jmbde/models/device/processor.h
  ${INCLUDE_DIR}/jmbde/models/device/software.h
  ${INCLUDE_DIR}/jmbde/models/employee/employeeaccount.h
  ${INCLUDE_DIR}/jmbde/models/employee/employee.h
  ${INCLUDE_DIR}/jmbde/models/employee/employeedocument.h
  ${INCLUDE_DIR}/jmbde/models/employee/function.h
  ${INCLUDE_DIR}/jmbde/models/employee/title.h
  ${INCLUDE_DIR}/jmbde/models/cityname.h
  ${INCLUDE_DIR}/jmbde/models/company.h
  ${INCLUDE_DIR}/jmbde/models/department.h
  ${INCLUDE_DIR}/jmbde/models/document.h
  ${INCLUDE_DIR}/jmbde/models/inventory.h
  ${INCLUDE_DIR}/jmbde/models/place.h
  ${INCLUDE_DIR}/jmbde/models/systemdata.h
  ${INCLUDE_DIR}/jmbde/models/zipcity.h
  ${INCLUDE_DIR}/jmbde/models/zipcode.h
  ${INCLUDE_DIR}//jmbde/models/version.h



)

# Group files in IDE project
source_group("include/${TARGET_ID_LOWER}" FILES ${HEADERS})

# -------------------------------------------------------------------------------------------------------------------- #
# Define sources files
# -------------------------------------------------------------------------------------------------------------------- #
set(SOURCES
    # core
    ${SOURCE_DIR}/core/commondata.cpp
    # datebase
    ${SOURCE_DIR}/database/datacontext.cpp
    # device
    ${SOURCE_DIR}/device/computer.cpp
    ${SOURCE_DIR}/device/computersoftware.cpp
    ${SOURCE_DIR}/device/devicename.cpp
    ${SOURCE_DIR}/device/devicetype.cpp
    ${SOURCE_DIR}/device/fax.cpp
    ${SOURCE_DIR}/device/manufacturer.cpp
    ${SOURCE_DIR}/device/mobile.cpp
    ${SOURCE_DIR}/device/os.cpp
    ${SOURCE_DIR}/device/phone.cpp
    ${SOURCE_DIR}/device/printer.cpp
    ${SOURCE_DIR}/device/processor.cpp
    ${SOURCE_DIR}/device/software.cpp
    # employee
    ${SOURCE_DIR}/employee/employee.cpp
    ${SOURCE_DIR}/employee/employeeaccount.cpp
    ${SOURCE_DIR}/employee/employeedocument.cpp
    ${SOURCE_DIR}/employee/function.cpp
    ${SOURCE_DIR}/employee/title.cpp
    # models
    ${SOURCE_DIR}/models/account.cpp
    ${SOURCE_DIR}/models/chipcard.cpp
    ${SOURCE_DIR}/models/chipcarddoor.cpp
    ${SOURCE_DIR}/models/chipcardprofile.cpp
    ${SOURCE_DIR}/models/chipcardprofiledoor.cpp
    ${SOURCE_DIR}/models/cityname.cpp
    ${SOURCE_DIR}/models/company.cpp
    ${SOURCE_DIR}/models/department.cpp
    ${SOURCE_DIR}/models/document.cpp
    ${SOURCE_DIR}/models/inventory.cpp
    ${SOURCE_DIR}/models/place.cpp
    ${SOURCE_DIR}/models/systemdata.cpp
    ${SOURCE_DIR}/models/zipcity.cpp
    ${SOURCE_DIR}/models/zipcode.cpp
    ${SOURCE_DIR}/resources/res.qrc)


# Group files in IDE project
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
