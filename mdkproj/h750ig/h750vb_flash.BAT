SET PATH=C:\Keil_v5\ARM\ARMCLANG\Bin;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\windows;C:\windows\system32;C:\windows\System32\Wbem;C:\windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\Python38\Scripts\;C:\Program Files\Python38\;C:\Program Files (x86)\AMD APP\bin\x86_64;C:\Program Files (x86)\ATI Technologies\ATI.ACE\Core-Static;C:\Program Files\Microsoft VS Code\bin;C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility;C:\Program Files\TortoiseSVN\bin;C:\MyProgramFiles\codeblocks\MinGW\bin;C:\MyProgramFiles\jre\bin;C:\MyProgramFiles\gcc-arm-none-eabi\bin;C:\MinGW\msys\1.0\bin;D:\Program Files\TortoiseGit\bin;C:\Program Files\Git\cmd;C:\Program Files (x86)\Atmel\sam-ba_2.18\drv\;C:\Program Files (x86)\Atmel\sam-ba_2.18
SET ARMCC6_ASMOPT=--diag_suppress=9931
SET ARMCC6_CCOPT=--diag_suppress=9931
SET ARMCC6_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32H750VBTx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=h750vb_flash
SET CPU_CLOCK=0x007A1200
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\myh750_main.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\retarget.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\cmdline.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\task.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\mcu_isr.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\stm32h7xx_ll_usart.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\stm32h7xx_ll_utils.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\stm32h7xx_ll_rcc.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\stm32h7xx_ll_gpio.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmClang.exe" @".\obj\system_stm32h7xx.__i"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmAsm" --Via ".\obj\startup_stm32h750xx._ia"
"C:\Keil_v5\ARM\ARMCLANG\Bin\ArmLink" --Via ".\obj\h750vb_flash.lnp"
"C:\Keil_v5\ARM\ARMCLANG\Bin\fromelf.exe" ".\obj\h750vb_flash.axf" --i32combined --output ".\obj\h750vb_flash.hex"
