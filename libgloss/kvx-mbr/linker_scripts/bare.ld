/*
 * Copyright 2019
 * Kalray Inc. All rights reserved.
 *
 * This software is furnished under license and may be used and copied only
 * in accordance with the following terms and conditions.  Subject to these
 * conditions, you may download, copy, install, use, modify and distribute
 * modified or unmodified copies of this software in source and/or binary
 * form. No title or ownership is transferred hereby.
 *
 * 1) Any source code used, modified or distributed must reproduce and
 *    retain this copyright notice and list of conditions as they appear in
 *    the source file.
 *
 * 2) No right is granted to use any trade name, trademark, or logo of
 *    Kalray Inc.  The "Kalray Inc" name may not be
 *    used to endorse or promote products derived from this software
 *    without the prior written permission of Kalray Inc.
 *
 * 3) THIS SOFTWARE IS PROVIDED "AS-IS" AND ANY EXPRESS OR IMPLIED
 *    WARRANTIES, INCLUDING BUT NOT LIMITED TO, ANY IMPLIED WARRANTIES OF
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
 *    NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT SHALL KALRAY BE LIABLE
 *    FOR ANY DAMAGES WHATSOEVER, AND IN PARTICULAR, KALRAY SHALL NOT BE
 *    LIABLE FOR DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *    BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *    OR OTHERWISE), EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

INCLUDE memory_map.ld

/* Entry point */
ENTRY(_start)

INCLUDE bare_extern.ld

KVX_BOOT_ADDRESS      = DEFINED(KVX_BOOT_ADDRESS) ? KVX_BOOT_ADDRESS : 0x0;
KVX_EXCEPTION_ADDRESS = DEFINED(KVX_EXCEPTION_ADDRESS) ? KVX_EXCEPTION_ADDRESS : 0x400;
KVX_DEBUG_ADDRESS     = KVX_EXCEPTION_ADDRESS + 0x00;
KVX_TRAP_ADDRESS      = KVX_EXCEPTION_ADDRESS + 0x40;
KVX_INTERRUPT_ADDRESS = KVX_EXCEPTION_ADDRESS + 0x80;
KVX_SYSCALL_ADDRESS   = KVX_EXCEPTION_ADDRESS + 0xc0;

KVX_DEBUG_RAM_ADDRESS = DEFINED(KVX_DEBUG_RAM_ADDRESS) ? KVX_DEBUG_RAM_ADDRESS : 0xA46000;
KVX_DEBUG_RAM_SIZE    = 8K;

PE_STACK_SIZE        = DEFINED(PE_STACK_SIZE) ? PE_STACK_SIZE : 64K;
RM_STACK_SIZE        = DEFINED(RM_STACK_SIZE) ? RM_STACK_SIZE : 20K;

MPPA_ARGAREA_SIZE    = DEFINED(MPPA_ARGAREA_SIZE) ? MPPA_ARGAREA_SIZE : 4K;
PAGE_SIZE            = DEFINED(PAGE_SIZE)? PAGE_SIZE : 4096;


INTERNAL_RAM_BASE    = DEFINED(INTERNAL_RAM_BASE) ? INTERNAL_RAM_BASE : ORIGIN(internal_mem);
INTERNAL_RAM_SIZE    = DEFINED(INTERNAL_RAM_SIZE) ? INTERNAL_RAM_SIZE : LENGTH(internal_mem);

DEBUG_SIZE           = DEFINED(DEBUG_SIZE) ? DEBUG_SIZE : 32;
DEBUG_BASE           = INTERNAL_RAM_BASE + INTERNAL_RAM_SIZE - DEBUG_SIZE;

RamBase = DEFINED(RamBase) ? RamBase : ORIGIN(ddr);
RamSize = DEFINED(RamSize) ? RamSize : LENGTH(ddr);

REGION_ALIAS("REGION_TLS", internal_mem);

/* Definition of sections */
SECTIONS
{
  /* We assume 1KB for each vector and EV to be set to 1KB at start */
  . = KVX_BOOT_ADDRESS;
  .boot.startup : { KEEP(*(.boot.startup)) } > internal_mem

  .exception.debug KVX_DEBUG_ADDRESS : {
    /* The debug exception handler */
    KEEP(*(.exception.debug))
  } > internal_mem

  .exception.trap KVX_TRAP_ADDRESS : {
    /* The debug exception handler */
    KEEP(*(.exception.trap))
  } > internal_mem

  .exception.interrupt KVX_INTERRUPT_ADDRESS : {
    /* The debug exception handler */
    KEEP(*(.exception.interrupt))
  } > internal_mem

  .exception.syscall KVX_SYSCALL_ADDRESS : {
    /* The debug exception handler */
    KEEP(*(.exception.syscall))
  } > internal_mem
  . = ALIGN(0x40);

  /* The rest of the boot stuff */
  .exception ALIGN(8) : {
    KEEP(*(.exception))
  }  > internal_mem

  /* The rest of the boot stuff */
  .boot : {
    *(.boot)
    . = ALIGN(8);
  }  > internal_mem

  .text   ALIGN(8) : {
    KEEP(*(.init))
    KEEP(*(.fini))
    *(.text)
    *(.text.*)
    . = ALIGN(4096);
  }  > internal_mem

  .rodata   ALIGN(8) : {
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(8);
  }  > internal_mem

  .init_array ALIGN(64) :
  {
    . = ALIGN(64);
    PROVIDE (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE (__preinit_array_end = .);
    PROVIDE (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
    PROVIDE (__init_array_end = .);
    PROVIDE (__fini_array_start = .);
    KEEP (*(.fini_array))
    KEEP (*(SORT(.fini_array.*)))
    PROVIDE (__fini_array_end = .);
  } > internal_mem

  /* The bsp configuration arguments */
  .bsp_config ALIGN(8) : {
    KEEP(*(.bsp_config_desc))
    INTERNAL_RAM_SIZE_VALUE = ABSOLUTE(INTERNAL_RAM_SIZE) ;
    RM_STACK_SIZE_VALUE = ABSOLUTE(RM_STACK_SIZE) ;
    PE_STACK_SIZE_VALUE = ABSOLUTE(PE_STACK_SIZE) ;
    DDR_SIZE_VALUE = ABSOLUTE(RamSize) ;
    *(.bsp_config)
  }  > internal_mem

  /* If there is no space reserved (ie. section is empty and left out), loader will not load device tree */
  .dtb ALIGN(0x100) : {
    __dtb_start = .;
    KEEP(*(.dtb));
    __dtb_end = ABSOLUTE(.);
  } > internal_mem
  /* ASSERT(__dtb_end > __dtb_start, ".dtb (device tree) section wrongly empty") */

  /* This is the value of the GP base */
  _data_start = .;
  .data   ALIGN(8) : {
    *(.data)
    *(.data.*)
    . = ALIGN(8);
  }  > internal_mem

  .bss   ALIGN(8) : {
    _bss_start = .;
    *(.bss)
    *(.bss.*)
    *(COMMON)
    *(.gnu.linkonce.b.*)
    . = ALIGN(8);
    _bss_end = .;
  }  > internal_mem

  /* Stack area for RM */
  .stack ALIGN(0x1000) : {
    _rm_stack_end = ABSOLUTE(.);
    RM_STACK_END = ABSOLUTE(.);
    . += RM_STACK_SIZE;
    _rm_stack_start = ABSOLUTE(.);
    RM_STACK_START = ABSOLUTE(.);
  } > internal_mem

  /* Stack area for PE0 */
  .pe_stack ALIGN(0x1000) : {
    _pe_stack_end = ABSOLUTE(.);
    KVX_PE_STACK_END = ABSOLUTE(.);
    . += PE_STACK_SIZE;
    _pe_stack_start = ABSOLUTE(.);
    KVX_PE_STACK_START = ABSOLUTE(.);
  } > internal_mem

  .mppa_args ALIGN(8) : {
    /* Space reserved for mppa_spawn args */
    _mppa_arg_start = ABSOLUTE(.);
    PROVIDE(MPPA_ARGAREA_START = ABSOLUTE(.));
    . += MPPA_ARGAREA_SIZE;
    _mppa_arg_end = ABSOLUTE(.);
  } > internal_mem =0

  /* TLS support */
  INCLUDE tls.ld

  .heap_start ALIGN(8) : {
    _heap_start = ABSOLUTE(.);
  } > internal_mem

  . = DEBUG_BASE;

  .heap_end ALIGN(8) : {
    _heap_end = ABSOLUTE(.);
    _debug_start = ABSOLUTE(.);
  } > internal_mem

  /* DWARF2 debugging sections */
  .debug_info       0 : { *(.debug_info) }
  .debug_line       0 : { *(.debug_line) }
  .debug_abbrev     0 : { *(.debug_abbrev) }
  .debug_frame      0 : { *(.debug_frame) }
  .debug_aranges    0 : { *(.debug_aranges) }
  .debug_pubnames   0 : { *(.debug_pubnames) }
  .debug_str        0 : { *(.debug_str) }
  .debug_funcnames  0 : { *(.debug_funcnames) }
  .debug_typenames  0 : { *(.debug_typenames) }
  .debug_varnames   0 : { *(.debug_varnames) }
  .debug_weaknames  0 : { *(.debug_weaknames) }
  .debug_macinfo    0 : { *(.debug_macinfo) }
  .debug_loc        0 : { *(.debug_loc) }

  . = 0x80000000;
  .alias_ddr ALIGN(16) : {
    *(.alias_ddr)
  } > alias_ddr

  . = 0x100000000;
  .ddr ALIGN(16) : {
    *(.ddr)
  } > ddr

  __dtb_size = ABSOLUTE(__dtb_end)- ABSOLUTE(__dtb_start);
}
