cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=gcc-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=true

https://forums.freertos.org/t/how-to-build-with-opencm3-freertos-cmake-to-arm-cm4/13948

https://github.com/richard-damon/FreeRTOScpp/blob/master/TaskCPP.h

not enough ram -> check #define configTOTAL_HEAP_SIZE ( ( size_t ) ( 8 * 1024 ) )
