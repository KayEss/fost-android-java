find_library(log-lib log)

add_library(fost-android-java
        fost-android.cpp
        logger.cpp
        messaging.cpp
        setting.cpp
        view-assets.cpp
        webserver.cpp
    )
target_include_directories(fost-android-java PUBLIC .)
target_link_libraries(fost-android-java
        ${log-lib}
        fost-s3
        fost-urlhandler
    )

