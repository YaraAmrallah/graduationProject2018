################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
GPIO.obj: ../GPIO.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="F:/self/Embedded/2018/graduation2019/I2Cbasic" --include_path="F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="GPIO.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

GPIO_Cfg.obj: ../GPIO_Cfg.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="F:/self/Embedded/2018/graduation2019/I2Cbasic" --include_path="F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="GPIO_Cfg.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

I2C.obj: ../I2C.C $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="F:/self/Embedded/2018/graduation2019/I2Cbasic" --include_path="F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="I2C.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

I2C_CFG.obj: ../I2C_CFG.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="F:/self/Embedded/2018/graduation2019/I2Cbasic" --include_path="F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="I2C_CFG.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MPU.obj: ../MPU.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="F:/self/Embedded/2018/graduation2019/I2Cbasic" --include_path="F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="MPU.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MPU_Manager.obj: ../MPU_Manager.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="F:/self/Embedded/2018/graduation2019/I2Cbasic" --include_path="F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="MPU_Manager.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MPU_Manager_Cfg.obj: ../MPU_Manager_Cfg.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="F:/self/Embedded/2018/graduation2019/I2Cbasic" --include_path="F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="MPU_Manager_Cfg.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="F:/self/Embedded/2018/graduation2019/I2Cbasic" --include_path="F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

tm4c123gh6pm_startup_ccs.obj: ../tm4c123gh6pm_startup_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="F:/self/Embedded/2018/graduation2019/I2Cbasic" --include_path="F:/self/Embedded/2018/RTOS_SSCS/ccsv7/tools/compiler/ti-cgt-arm_16.9.11.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="tm4c123gh6pm_startup_ccs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


