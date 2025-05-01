set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(CMAKE_CXX_FLAGS "/utf-8")
#set(CMAKE_C_FLAGS "/utf-8")

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR STEM project_name)

# набор флагов выесняем согласно инструкции для каждого типа МК свой
set(cross_toolchain_flags
	-march=rv32imacxw
	-mabi=ilp32
	-msmall-data-limit=8  
	-fmax-errors=20	
	#-misa-spec=2.2
	-Os
	-msave-restore
	-fmessage-length=0
	-fsigned-char
	-ffunction-sections
	-fdata-sections
	-fno-common
	-Wunused
	-Wuninitialized
	#-DCMAKE_CXX_FLAGS="/chcp 1251"
	#-DCMAKE_C_FLAGS="/chcp 1251"
)

#if("$ENV{RAM_VAR}" MATCHES "True")
set(RAM_VAR "RAM")		# !!! закоментировать если компилируем не в RAM

if(NOT DEFINED RAM_VAR)
	SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/Ld/Link.ld)
else()
	SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/Ld/Link_ram.ld) 
endif()

message(STATUS "RAM_VAR__ is: $ENV{RAM_VAR}")


set(asm_flags ${cross_toolchain_flags}
	-x assembler-with-cpp
)

set(c_flags ${cross_toolchain_flags}
)

set(cxx_flags ${cross_toolchain_flags}
	-fno-rtti
	-fno-exceptions
	-Wno-volatile
)

add_compile_options(
	"$<$<COMPILE_LANGUAGE:ASM>:${asm_flags}>"
	"$<$<COMPILE_LANGUAGE:C>:${c_flags}>"
	"$<$<COMPILE_LANGUAGE:CXX>:${cxx_flags}>"
)
PROJECT(${project_name} C CXX ASM)

add_definitions(

)

# тут добавляется определения для версии компиляции DEBUG с префиксом -D (-DDEF  =  #define DEF) (-DVAR=5  =  #define VAR 5) 
if((CMAKE_BUILD_TYPE MATCHES "Debug") OR (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo"))
	add_definitions(
		# -DDEBUG
	)
endif()

get_filename_component(parent_dir ../ ABSOLUTE)

# директории со всеми компилируемыми файлами
include_directories(
	Debug/
	Core/
	User/
	Peripheral/inc	
	FreeRTOS/	#
	FreeRTOS/include
	FreeRTOS/portable/Common	#
	FreeRTOS/portable/GCC/RISC-V
	FreeRTOS/portable/GCC/RISC-V/chip_specific_extensions/RV32I_PFIC_no_extensions
	FreeRTOS/portable/MemMang	#
)

file(GLOB_RECURSE SOURCES
	User/*.c
	User/src/*.c
	Core/*.c	
	Debug/*.c
	FreeRTOS/*.c
	FreeRTOS/include/*.c
	FreeRTOS/portable/Common/*.c
	FreeRTOS/portable/GCC/RISC-V/*.c
	FreeRTOS/portable/MemMang/*.c
	Peripheral/src/*.c
	# все ассемблеровские файлы *.S прописыаем ниже 
	Startup/startup_ch32v30x_D8C.S
	FreeRTOS/portable/GCC/RISC-V/portASM.S
)

add_link_options(
	${cross_toolchain_flags}
	-T ${LINKER_SCRIPT}
	-nostartfiles
	-Xlinker --gc-sections
	-Wl,-Map,${project_name}.map
	-Wl,-static --specs=nano.specs --specs=nosys.specs
	-Wl,--print-memory-usage
)

add_executable(${project_name}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${project_name}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${project_name}.bin)
add_custom_command(TARGET ${project_name}.elf POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${project_name}.elf> ${HEX_FILE}
	COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${project_name}.elf> ${BIN_FILE}
)