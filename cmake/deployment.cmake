# Set the package name
set(CPACK_PACKAGE_NAME "jmbde")

# Include the custom CPack configuration
include(jmbde-cpack-config)

# Set package metadata
set(CPACK_PACKAGE_VENDOR "Jürgen Mülbert")
set(CPACK_PACKAGE_VERSION ${VERSION_STRING})
set(CPACK_PACKAGE_DESCRIPTION "A tool to collect the infrastructure of a company")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/jmuelbert/jmbde-QT")
set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/assets/icons/jmbde-icon.ico")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

# Common files to ignore during packaging
set(COMMON_IGNORE_FILES
    "/CMakeFiles/"
    "_CPack_Packages/"
    "/Testing/"
    ".cmake$"
    ".directory$"
    "CMakeCache.txt"
    "/.svn/"
    "/CVS/"
    "~$"
    ".swp$"
    ".log$"
    ".gz$"
    "/src/config.h$")

# Set the files to ignore for the package
set(CPACK_PACKAGE_IGNORE_FILES ${COMMON_IGNORE_FILES})

# Windows-specific packaging
if(WIN32)
    # Include all dynamically linked runtime libraries such as MSVCRxxx.dll
    include(InstallRequiredSystemLibraries)

    if(RUN_IN_PLACE)
        # Set package file name based on architecture
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-win64")
        else()
            set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-win32")
        endif()
        set(CPACK_GENERATOR ZIP)
    else()
        # Include all dynamically linked runtime libraries
        include(InstallRequiredSystemLibraries)

        # Set up NSIS and WIX for installer generation
        set(CPACK_GENERATOR NSIS64 WIX ZIP)
        set(CPACK_PACKAGE_NAME "${PROJECT_NAME_CAPITALIZED}")
        set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME_CAPITALIZED}")
        set(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME} "${PROJECT_NAME_CAPITALIZED}")
        set(CPACK_CREATE_DESKTOP_LINKS ${PROJECT_NAME})

        # WIX installer settings
        set(CPACK_WIX_PRODUCT_ICON "${CMAKE_SOURCE_DIR}/assets/icons/jmbde.ico")
        set(CPACK_WIX_CULTURES "en-US", "de-DE")
        set(CPACK_WIX_UI_BANNER "${CMAKE_SOURCE_DIR}/assets/icons/CPACK_WIX_UI_BANNER.BMP")
        set(CPACK_WIX_UI_DIALOG "${CMAKE_SOURCE_DIR}/assets/icons/CPACK_WIX_UI_DIALOG.BMP")

        # Set properties for the WIX installer
        set(CPACK_WIX_PROPERTY_ARPHELPLINK "${CPACK_PACKAGE_HOMEPAGE_URL}")
        set(CPACK_WIX_PROPERTY_ARPURLINFOABOUT "${CPACK_PACKAGE_HOMEPAGE_URL}")
        set(CPACK_WIX_ROOT_FEATURE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
        set(CPACK_WIX_LIGHT_EXTRA_FLAGS "-dcl:high") # Set high compression

        # License and README files
        set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/packaging/win/License.rtf")
        set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

        # Disable DESTDIR for WiX
        set(CPACK_SET_DESTDIR OFF)
        set(CPACK_WIX_UPGRADE_GUID "7ACB456E-18F2-452E-8D12-464FCCF0DCB0")

        # Set upgrade GUID based on architecture
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(CPACK_WIX_UPGRADE_GUID "477B5F99-2A01-4D4A-A0F2-7E82705D7B25") # x64
        else()
            set(CPACK_WIX_UPGRADE_GUID "A6B2BF29-1F7D-4CC5-A7F0-3AC10F0B62CD") # x86
        endif()
    endif()

    # macOS-specific packaging
elseif(APPLE)
    set(CPACK_SET_DESTDIR ON)
    set(JMBDE_QT5_RUNTIME_INSTALL_DIRECTORY "lib/jmbde")
    set(JMBDE_COMMON_RUNTIME_INSTALL_DIRECTORY "lib/jmbde")
    set(CPACK_PACKAGE_FILE_NAME jmbde)
    set(MACOS_BUNDLE_STARTUP_COMMAND jmbde)
    set(MACOS_BUNDLE_BUNDLE_NAME jmbde)
    set(CPACK_BINARY_DRAGNDROP ON)

    # Linux-specific packaging
elseif(UNIX)
    set(CPACK_DEB_COMPONENT_INSTALL 1)
    set(CPACK_GENERATOR
        "

  set(CPACK_GENERATOR "DEB")
  set(CPACK_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER
      "Jürgen
        Mülbert
        <juergen.muelbert@gmail.com>")
  set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
      "${CMAKE_CURRENT_BINARY_DIR}/postinst;${CMAKE_CURRENT_BINARY_DIR}/prerm")
  set(CPACK_DEBIAN_PACKAGE_DESCRIPTION
      "A
        tool
        to
        collect
        the
        infrastructure
        of
        a
        company")

  set(INSTALL_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib)
  configure_file(${CMAKE_SOURCE_DIR}/cmake/modules/postinst.in
                 ${CMAKE_CURRENT_BINARY_DIR}/postinst @ONLY)
  configure_file(${CMAKE_SOURCE_DIR}/cmake/modules/prerm.in
                 ${CMAKE_CURRENT_BINARY_DIR}/prerm @ONLY)

  set(CPACK_SET_DESTDIR ON)
  set(CPACK_GENERATOR "DEB")
  set(CPACK_DEBIAN_PACKAGE_NAME "jmbde")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jürgen
        Mülbert")
  set(CPACK_DEBIAN_PACKAGE_BUILD_DEPENDS
      "cmake
        (>= 3.13)
        ,
        qt5-default
        (>= 5.12)
        ,
        qtbase5-dev-tools
        (>= 5.12)
        ,
        qttools5-dev-tools
        (>= 5.12)
        ,
        qttools5-dev
        (>= 5.12)
        ,
        bison
        (>=3.0)
        "
  )
  set(CPACK_DEBIAN_PACKAGE_DEPENDS
      "libqt5core5a
        (>= 5.12)
        ,
        libqt5gui5
        (>= 5.12)
        ,
        libqt5network5
        (>= 5.12)
        ,
        libqt5printsupport5
        (>= 5.12)
        ,
        libqt5widgets5
        (>= 5.12)
        ,
        libqt5xml5
        (>= 5.12)
        "
  )
  set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${CMAKE_PROJECT_HOMEPAGE_URL})
  set(CPACK_DEBIAN_PACKAGE_SECTION "business")
  set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

  if(BUILD_RPM)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/packaging/linux/jmbde.spec.in"
                   "${CMAKE_CURRENT_BINARY_DIR}/jmbde.spec" @ONLY IMMEDIATE)
    # set(CPACK_RPM_USER_BINARY_SPECFILE
    # "${CMAKE_CURRENT_BINARY_DIR}/jmbde.spec")
    set(CPACK_RPM_FILE_NAME RPM-DEFAULT)
    set(CPACK_GENERATOR "RPM")
    set(CPACK_RPM_PACKAGE_NAME "jmbde")
    set(CPACK_PACKAGE_CONTACT "juergen.muelbert@gmail.com")
    set(CPACK_RPM_PACKAGE_VENDOR "Jürgen
        Mülbert")
    if(LSB_RELEASE_ID_SHORT EQUAL "openSUSE")
      set(CPACK_RPM_BUILDREQUIRES
          "cmake
        >=
        3.13,
        libqt5-qtbase-common-devel
        >=
        5.12,
        libQt5Core-devel
        >=
        5.12,
        libQt5Gui-devel
        >=
        5.12,
        libQt5Network-devel
        >=
        5.12,
        libQt5Widgets-devel
        >=
        5.12,
        libQt5Concurrent-devel
        >=
        5.12,
        libqt5-linguist-devel
        >=
        5.12,
        libqt5-qtsvg-devel
        >=
        5.12,
        update-desktop-files,
        ninja"
      )
    else()
      set(CPACK_RPM_BUILDREQUIRES
          "cmake
        >=
        3.13,
        libqt5-qtbase-common-devel
        >=
        5.12,
        libQt5Core-devel
        >=
        5.12,
        libQt5Gui-devel
        >=
        5.12,
        libQt5Network-devel
        >=
        5.12,
        libQt5Widgets-devel
        >=
        5.12,
        libQt5Concurrent-devel
        >=
        5.12,
        libqt5-linguist-devel
        >=
        5.12,
        libqt5-qtsvg-devel
        >=
        5.12,
        update-desktop-files,
        ninja-build"
      )
    endif()
    set(CPACK_RPM_PACKAGE_LICENSE "GPL3
        ")
endif()
endif()

# Must Here
include(CPack)
