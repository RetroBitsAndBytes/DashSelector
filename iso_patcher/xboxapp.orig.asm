;;;
;;; xboxapp.asm
;;;
;;; Copyright 2004 rmenhal 
;;;    modifided by xman954
;;;    for use as a cdrom patcher (kernel patcher for rmemhal's ISO loader)
;;;
;;; Licensed under GNU General Public License version 2. 
;;;
;;;
;;; Compile using the command:
;;; 	nasm -o default.xbe xboxapp.asm
;;;
;;;
;;; Supports kernels 3944, 4034, 4817, 5101, 5530, 5713, 5838 and evox M8+.


%define BOOT_TO_DASH       ; uncomment to boot the 2nd Dashboard when done (only needed if Quick IGR works)

%define DASH_PATH   '\Device\Harddisk0\Partition1\default.xbe'

;;;

%include "header.asm"
 
	align 4
kernel_thunk:	
AvGetSavedDataAddress:		dd	0x80000000 + 1
AvSetSavedDataAddress:		dd	0x80000000 + 4
HalReadSMBusValue:		dd	0x80000000 + 45
HalReturnToFirmware:		dd	0x80000000 + 49
HalWriteSMBusValue:		dd	0x80000000 + 50
KeQuerySystemTime:		dd	0x80000000 + 128
KeRaiseIrqlToDpcLevel:		dd	0x80000000 + 129
KfLowerIrql:			dd	0x80000000 + 161
LaunchDataPage:			dd	0x80000000 + 164
MmAllocateContiguousMemory:	dd	0x80000000 + 165
MmAllocateContiguousMemoryEx:	dd	0x80000000 + 166
MmFreeContiguousMemory:		dd	0x80000000 + 171
MmGetPhysicalAddress:		dd	0x80000000 + 173
MmPersistContiguousMemory:	dd	0x80000000 + 178
MmQueryAllocationSize:		dd	0x80000000 + 180
NtClose:				dd	0x80000000 + 187
NtCreateFile:			dd	0x80000000 + 190
NtDeviceIoControlFile:		dd	0x80000000 + 196
NtOpenFile:				dd	0x80000000 + 202
NtReadFile:				dd	0x80000000 + 219
NtSetInformationFile		dd	0x80000000 + 226
NtSetSystemTime:			dd 	0x80000000 + 228
NtWriteFile:			dd	0x80000000 + 236
RtlTimeFieldsToTime:		dd 	0x80000000 + 304
XboxKrnlVersion:			dd	0x80000000 + 324
XePublicKeyData:			dd	0x80000000 + 355
					dd	0 ; end of table


	align 4

;-------------------------------------
patched_adr:
	dd	3944,	80027743h+5    ;  is put at 80027748h when patch is applied
	dd	4034,	80027763h+5
	dd	4817,	800264E7h+5
	dd	4981,	80029D33h+5
	dd	5003,	8002B4B2h+5
	dd	5035,	80030EE1h+5
	dd	5101,	80026515h+5
	dd	5530,	80026919h+5
	dd	5713,	80026919h+5
	dd	5838,	80026919h+5
.last	dd	0000
;-------------------------------------

	align 	16
buffer:  db    0
		 times 256 db 0
	align 16
	
start:
	;cld

	mov  	eax, 0x00000000		; led off
	push	eax			; must be dword size
	push	byte 0
 	push	byte 8 			; led_register
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]

 	push	byte 1			; manual mode
 	push	byte 0
 	push	byte 7 			; led_mode
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]

	mov	eax,0x0FFFFFFF
dly_98	dec	eax
	jnz     short dly_98		; ~ 1 sec 



mov     esi, [LaunchDataPage]
mov 	esi, [esi]
mov     al, [esi + 1024]
cmp     al, 0x5c
jne      near exit

	mov  	eax, 0x000000F0		; red
	push	eax			; must be dword size
	push	byte 0
 	push	byte 8 			; led_register
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]

 	push	byte 1			; manual mode
 	push	byte 0
 	push	byte 7 			; led_mode
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]

	mov	eax,0x0FFFFFFF
dly_95	dec	eax
	jnz     short dly_95		; ~ 1 sec 




	; check launch data
	mov esi, [LaunchDataPage]
	mov esi, [esi]
	; test 	esi, esi
	; jz near exit

	cld
	; mov esi, source      ; put address into the source index
    ; mov esi, [esi + 1024]
	add esi, 1024
	mov edi, buffer        ; put address into the destination index
    mov ecx, dword 256          ; put the number of bytes to copy in ecx
  ; --------------------------------------------------
  ; repeat copying bytes from ESI to EDI until ecx = 0
  ; --------------------------------------------------
    rep movsb  
	; len = 57
	;mov al, byte [esi + 1024]
	;mov [buffer], al
	
	mov  	eax, 0x00000000		; led off
	push	eax			; must be dword size
	push	byte 0
 	push	byte 8 			; led_register
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]

 	push	byte 1			; manual mode
 	push	byte 0
 	push	byte 7 			; led_mode
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]
	mov	eax,0x0FFFFFFF
dly_94	dec	eax
	jnz     short dly_94		; ~ 1 sec 
	
	
	
	mov 	al, byte [buffer]
	cmp 	al, 0x5c
	jne 		near exit		
	;mov 	al, [buffer+1]
	;cmp 	al, 0x44
	;jne 		near exit		
		
	mov  	eax, 0x000000F0		; red
	push	eax			; must be dword size
	push	byte 0
 	push	byte 8 			; led_register
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]

 	push	byte 1			; manual mode
 	push	byte 0
 	push	byte 7 			; led_mode
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]

	mov	eax,0x0FFFFFFF
dly_99	dec	eax
	jnz     short dly_99		; ~ 1 sec 
		
		
	jmp near exit
	
	
chkkernelversion:
	mov	esi,[XboxKrnlVersion]
	test	esi,esi
	jz	near exit

	cmp	dword [esi],byte 1
	jne	near exit
;	cmp	word [esi+6],byte 1	; keeps .67 or .07 bios's from passing test
;	jne	near exit

	push	80010000h
	call	nkpatcher_get_kernel_info
	test	eax,eax
	jz	near exit


	
;--------------------------------
;       both ecx and edx are Kernel Version
;	both ecx and edx can be used (trashed)

test_patch:
	mov	ecx,patched_adr
.nxt_a	cmp     [ecx],edx		; edx is kernel version
	je	short .found
	add	ecx, byte 8
	cmp	ecx,patched_adr.last	; end of table
	jne	short .nxt_a		; next
	jmp     near exit		; done
	
.found  mov	ecx,[ecx+4]
	cmp     byte [ecx], 0x99 	; was it patched
	je	near exit		; yes done

;-------------------------------

	mov	[xa_kernel_info_addr],eax
	mov	ebx,eax
	push	ebx
	call	expand_kernel

patchkernel:		
	call	dword [KeRaiseIrqlToDpcLevel]
	push	eax

	cli
	mov	eax,cr0
	push	eax
	and	eax,0FFFEFFFFh
	mov	cr0,eax

	mov	eax,cr3
	mov	cr3,eax
	wbinvd

	call	expand_code_segment
	push	ebx
	call	patchheaders
	push	xa_feature_param
	push	ebx
	push	80010000h
	call	dword [ebx + KERNEL_INFO.patcher]	;<<<<<<<<<<<<<<<<<< here we go...

	mov	eax,cr3
	mov	cr3,eax
	wbinvd

	pop	eax
	mov	cr0,eax
	sti

	pop	ecx
	call	dword [KfLowerIrql]

	jmp	short exit          ; skip led RED

;---------------------------------------
;;;    set led to RED if patch was sucessfull
;---------------------------------------
	mov	eax,[test_val]
	and   eax,eax
	jz    short exit
	
	mov  	eax, 0x000000F0		; red
	push	eax			; must be dword size
	push	byte 0
 	push	byte 8 			; led_register
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]

 	push	byte 1			; manual mode
 	push	byte 0
 	push	byte 7 			; led_mode
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]

	mov	eax,0x0FFFFFFF
dly_0	dec	eax
	jnz     short dly_0		; ~ 1 sec 
	

;----------------------------------------
;
;	reboot
;
exit:
%ifndef BOOT_TO_DASH

	mov     esi, [LaunchDataPage]
	mov	edi, 0x00001000
        mov     ebx, [esi]
        test    ebx, ebx
        jnz     m_ok

        push    edi
        call    dword [MmAllocateContiguousMemory]
        mov     ebx, eax
        mov     [esi], eax

m_ok:   push    byte 0x01
        push    edi
        push    ebx
        call    dword [MmPersistContiguousMemory]

        cld
        mov     edi, ebx
        xor     eax, eax
        mov     ecx, dword 0x00000400	 ; zero all of LDP
        rep     stosd

;;        or      dword [ebx], byte -0x01 ; ?????? boot dash only ????

        push    byte 0x02		 ; quick reboot
        call    dword [HalReturnToFirmware]
	jmp	short $

%else
;-------------------------------------------------------------------------------
; This routine executes the XBE specified by APP_PATH and APP_NAME.
;-------------------------------------------------------------------------------
        mov     esi, [LaunchDataPage]
		mov	edi, 0x00001000
        mov     ebx, [esi]
        test    ebx, ebx
        jnz     mem_ok

        push    edi
        call    dword [MmAllocateContiguousMemory]
        mov     ebx, eax
        mov     [esi], eax

mem_ok: push    byte 0x01
        push    edi
        push    ebx
        call    dword [MmPersistContiguousMemory]

        cld
        mov     edi, ebx
        xor     eax, eax
        mov     ecx, dword 0x00000400
        rep     stosd

		
        mov     [ebx+4], eax
        lea     edi, [ebx+8]
        lea     esi, [buffer]
        ;push    byte 127
		;pop     ecx
		mov ecx, dword 256
        rep     movsb

        push    byte 0x02
        call    dword [HalReturnToFirmware]
	jmp	short $

;----------------------------------------------------------
D_PATH: db      DASH_PATH, 0
		times 20 db 0
DASHSZ: equ     $-D_PATH
;----------------------------------------------------------
%endif
	align	4
;--------------------- subs -------------------

nkpatcher_get_kernel_info:
	push	dword [esp+4]
	call	.get_kernel_version

	mov	edx,eax
	mov	eax,kernel_info_table
.sl:
	mov	ecx,[eax]
	jecxz	.notfound
	cmp	ecx,edx
	je	.done
	add	eax,byte KERNEL_INFO_size
	jmp	short .sl
.notfound:
	xor	eax,eax
.done:
	ret	4


.get_kernel_version:
	push	ebx
	push	esi
	
	mov	esi,[esp+4+8]
	
	mov	eax,[esi+3Ch]
	mov	ebx,[esi+eax+78h]
	add	ebx,esi
	mov	edx,[ebx+1Ch]
	add	edx,esi

	mov	ecx,324
	sub	ecx,[ebx+10h]
	mov	eax,[edx+4*ecx]
	add	eax,esi

	movzx	eax,word [eax+4]

	pop	esi
	pop	ebx
	ret	4


expand_kernel:
.top_addr		equ 0-4
.expansion_end_addr_ex	equ .top_addr-4
.local_var_size		equ -.expansion_end_addr_ex
	
	push	ebp
	mov	ebp,esp
	sub	esp,byte .local_var_size
	
	push	ebx
	mov	ebx,[ebp+8]
	mov	eax,[ebx + KERNEL_INFO.expansion_size]
	test	eax,eax
	jz	near .done

	push	esi
	push	edi
	
	mov	edx,[ebx + KERNEL_INFO.top_var_addr]
	mov	edx,[edx]
	mov	[ebp + .top_addr],edx
	add	eax,edx
	mov	[ebp + .expansion_end_addr_ex],eax
	
	call	dword [AvGetSavedDataAddress]
	mov	edi,eax
	test	eax,eax
	jz	.expand

	push	edi
	call	dword [MmQueryAllocationSize]
	mov	esi,eax

	push	edi
	call	dword [MmGetPhysicalAddress]

	lea	edx,[eax+esi]
	cmp	[ebp + .expansion_end_addr_ex],eax
	jbe	.expand
	cmp	edx,[ebp + .top_addr]
	jbe	.expand
	
	push	404h
	push	byte 0
	push	byte -1
	push	byte 0
	push	esi
	call	dword [MmAllocateContiguousMemoryEx]
	mov	edi,eax
	test	eax,eax
	jz	.error

	push	edi
	call	dword [MmGetPhysicalAddress]

	lea	edx,[eax+esi]
	cmp	[ebp + .expansion_end_addr_ex],eax
	jbe	.movedata
	cmp	edx,[ebp + .top_addr]
	ja	.error

.movedata:	
	push	esi
	push	edi
	call	dword [ebx + KERNEL_INFO.kernel_move_saved_data]

.expand:
	mov	eax,[ebp + .expansion_end_addr_ex]
	dec	eax
	push	byte 4
	push	byte 0
	push	eax
	push	dword [ebp + .top_addr]
	push	dword [ebx + KERNEL_INFO.expansion_size]
	call	dword [MmAllocateContiguousMemoryEx]
	test	eax,eax
	jz	.error

	mov	edx,[ebx + KERNEL_INFO.top_var_addr]
	mov	ecx,[ebx + KERNEL_INFO.expansion_size]
	add	[edx],ecx

	mov	[ebx + KERNEL_INFO.expd_space],eax
		
	push	byte 1
	push	ecx
	push	eax
	call	dword [MmPersistContiguousMemory]

.error:	
	pop	edi	
	pop	esi

.done:	
	pop	ebx
	leave
	ret	4


;;; Expand code segment size too. This is required for kernels 5530 and above,
;;; because those kernels downsize code segment when they discard the INIT
;;; section. It doesn't do really anything for kernels 5101 or less.
;;;
;;; This will also make Phoenix Bios Loader 1.3 - 1.4.1 work with 5530 and 5713
;;; kernel boxes.
	
expand_code_segment:
	mov	eax,.fjmp
	push	eax
	push	eax
	push	eax
	push	eax
	sgdt	[esp+2]
	pop	eax
	pop	eax
	mov	edx,cs
	add	eax,edx
	mov	[esp+4],edx
	mov	word [eax],0FFFFh
	or	byte [eax+6],0Bh
	jmp	far [esp]
.fjmp:
	pop	eax
	pop	eax
	ret
	
	
;;; If we expanded the kernel, xboxkrnl.exe headers must be made consistent.

patchheaders:	
	mov	ecx,[esp+4]

	cmp	dword [ecx + KERNEL_INFO.expd_space],byte 0
	je	.done

	push	esi
	mov	esi,80010000h

	add	esi,[esi+3Ch]

	movzx	eax,word [esi+6]			; NumberOfSections
	movzx	edx,word [esi+14h]		; SizeOfOptionalHeader
	dec	eax
	dec	eax
	imul	eax,byte 28h
	add	esi,edx
	add	esi,eax
	add	esi,byte 18h			; Second to last section

	mov	eax,[ecx + KERNEL_INFO.expansion_size]

	add	[esi+8],eax			; VirtualSize
	add	[esi+10h],eax		; SizeOfRawData

	sub	[esi+28h+8],eax		; INIT section VirtualSize
	sub	[esi+28h+10h],eax		; INIT section SizeOfRawData
	add	[esi+28h+0Ch],eax		; INIT section VirtualAddress
	add	[esi+28h+14h],eax		; INIT section PointerToRawData

	pop	esi
.done:	
	ret	4
	

;;; --------------------------------------------------------------------------
;;; Include patcher source
;;; --------------------------------------------------------------------------

%define INCLUDE_MODE
%define CODE_SECTION
%define DATA_SECTION


;;; --------------------------------------------------------------------------
;;; Helper macros and functions for patch installers
;;; --------------------------------------------------------------------------

%macro	VCALL 1
	db 0E8h
.var_%1:db 0,0,0,0
%endmacro

	
%macro	VJMP 1
	db 0E9h
.var_%1:db 0,0,0,0
%endmacro


%macro	VPUSH 1
	db 68h
.var_%1:db 0,0,0,0
%endmacro


%macro	STOS_CONST 1
%if ((%1) / 4) < 6
	times ((%1) / 4) stosd
%else
  %if ((%1) / 4) > 127
	mov	ecx,(%1) / 4
  %else
	push	byte ((%1) / 4)
	pop	ecx
  %endif
	rep	stosd
%endif
	times ((%1) % 4) stosb
%endmacro


%macro	MOVS_CONST 1
%if ((%1) / 4) < 6
	times ((%1) / 4) movsd
%else
  %if ((%1) / 4) > 127
	mov	ecx,(%1) / 4
  %else
	push	byte ((%1) / 4)
	pop	ecx
  %endif
	rep	movsd
%endif
	times ((%1) % 4) movsb
%endmacro




%macro	setcalljmptarget 4
	sub	%1,%4
	sub	%1,%3-%2+4
	mov	[%3],%1
%endmacro


%macro	setcalljmptarget 3
	sub	%1,%2
	sub	%1,byte 5
	mov	[%2+%3+1],%1
%endmacro

%macro	setjcondneartarget 3
	sub	%1,%2
	sub	%1,byte 6
	mov	[%2+%3+2],%1
%endmacro



%define TUNE_GEN		0
%define TUNE_REL		1
%define TUNE_CALL		1
%define TUNE_JMP		1
%define TUNE_ABS		2
%define TUNE_PUSH		2
%define TUNE_MOVREG	2
%define TUNE_END		0xffffffff


%macro	set_exp_entry 4
	dd %1, KERNEL_EXPORTS.%3, %2.var_%3_%4
%endmacro

	
%macro	set_exp_entry_last 0
	dd TUNE_END, 0, 0
%endmacro


tune_exp_refs:
.loop:	
	mov	eax,[esi]
	cmp	eax,byte -1
	je	.done
	cmp	al,TUNE_REL
	je	.tune_rel
	cmp	al,TUNE_ABS
	je	.tune_abs
.next:	
	add	esi,byte 12
	jmp	short .loop
.done:
	ret

.tune_abs:	
	mov	eax,[esi+4]
	mov	edx,[esi+8]
	mov	eax,[ebx+eax]
	add	eax,80010000h
	mov	[edx],eax
	jmp	short .next

.tune_rel:
	mov	eax,[esi+4]
	mov	edx,[esi+8]
	mov	eax,[ebx+eax]
	add	eax,80010000h
	add	eax,ecx
	sub	eax,edx
	sub	eax,edi
	sub	eax,byte 4
	mov	[edx],eax
	jmp	short .next


;;; --------------------------------------------------------------------------
;;; Helpers end
;;; --------------------------------------------------------------------------



;;; --------------------------------------------------------------------------
;;; Virtual CD/DVD drive patch
;;; --------------------------------------------------------------------------
%define  VIRTUAL_CDROM_DRIVE
%include "kexports.inc"
%include "cdromimagedrv.asm"

;;; --------------------------------------------------------------------------
;;; Virtual CD/DVD end
;;; --------------------------------------------------------------------------


;;; --------------------------------------------------------------------------
;;; 
;;; --------------------------------------------------------------------------

%define USEDSPACE_EXPD_USUAL	(CDROM_IMAGE_PATCH_SIZE)
%define USEDSPACE_EXPD_3944	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_4034	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_4817	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_4981	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_5003	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_5035	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_5101	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_5530	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_5713	USEDSPACE_EXPD_USUAL
%define USEDSPACE_EXPD_5838	USEDSPACE_EXPD_5713


	
;;; --------------------------------------------------------------------------
;;; Helper macros for patchers
;;; --------------------------------------------------------------------------


%macro	patcherinit 0
	pushad
	mov	eax,[esp+32+12]
	mov	[caller_param],eax
	mov	edx,[esp+32+8]
	push	edx
	mov	ecx,[esp+36+4]
	call	init_patcher_vars
%endmacro


%macro	patcherfinish 0
	pop	edx
	popad
	ret	12
%endmacro


	CODE_SECTION

init_patcher_vars:
	push	ecx
	sub	ecx,80010000h
	mov	[memdiff],ecx
	mov	eax,[edx + KERNEL_INFO.expd_space] 	; <<<<< initspace
	mov	[expd_space],eax
	pop	ecx
	mov	eax,[ecx+3Ch]				; <<<<< get_export_table
	mov	eax,[ecx+eax+78h]
	mov	eax,[ecx+eax+1Ch]
	add	eax,ecx
	sub	eax,[memdiff]
	mov	[kexports],eax
	ret
	

;;; --------------------------------------------------------------------------
;;; Helpers end
;;; --------------------------------------------------------------------------


	DATA_SECTION
	
align 4

expd_space		dd 0
kexports		dd 0
memdiff			dd 0
caller_param		dd 0		; feature parameters
xa_kernel_info_addr	dd 0
xa_feature_param	dd 0 
marker_loc		dd 0			; adr to put 0x99
test_val		dd 0


struc		KERNEL_INFO
.version:			resd 1
.patcher:			resd 1
.bootflags_addr:		resd 1
.top_var_addr:			resd 1
.expansion_size:		resd 1
.kernel_move_saved_data:	resd 1
.expd_space:			resd 1
.expd_space_end:		resd 1
endstruc





%macro	kinfo_entry 4
	dd %1, patcher_%1, %2, %3, (USEDSPACE_EXPD_%1 + 0xFFF) & ~0xFFF, %4, 0, 0
%endmacro

%macro	kinfo_entry_last 0
	dd 0
%endmacro
	
	
kernel_info_table:
	kinfo_entry 3944, 8003BE58h, 8003bff0h, 80031656h
	kinfo_entry 4034, 8003BF18h, 8003c0b0h, 80031293h
	kinfo_entry 4817, 8003B198h, 8003b330h, 8002fd41h
	kinfo_entry 4981, 8004F5F8h, 8004F790h, 80034584h
	kinfo_entry 5003, 80042D48h, 80042EE0h, 80035902h
	kinfo_entry 5035, 8005D4F0h, 8005D680h, 8003BC5Dh
	kinfo_entry 5101, 8003B298h, 8003B430h, 8002FDD7h
	kinfo_entry 5530, 8003C118h, 8003c2c4h, 8002ff55h
	kinfo_entry 5713, 8003C138h, 8003c2e4h, 8002ff6fh
	kinfo_entry 5838, 8003C138h, 8003c2e4h, 8002ff6fh
	kinfo_entry_last
	



	CODE_SECTION

	
patcher_3944:
	patcherinit
	cdromimagedrv 80027743h,80012390h,800123A4h,8003BA20h
	patcherfinish

patcher_4034:
	patcherinit
	cdromimagedrv 80027763h,80012398h,800123ACh,8003BAE0h
	patcherfinish
	
patcher_4817:
	patcherinit
	cdromimagedrv 800264E7h,800123B8h,800123CCh,8003AD60h
	patcherfinish

patcher_4981:
	patcherinit
	cdromimagedrv 80029D33h,80012780h,80012794h,80042E40h
	patcherfinish

patcher_5003:
	patcherinit
	cdromimagedrv 8002B4B2h,80012EE8h,80012EFCh,80041FE0h
	patcherfinish

patcher_5035:
	patcherinit
	cdromimagedrv 80030EE1h,80013180h,80013194h,80050BE0h
	patcherfinish

patcher_5101:
	patcherinit
	cdromimagedrv 80026515h,800123B8h,800123CCh,8003AE60h
	patcherfinish

patcher_5530:
	patcherinit
	cdromimagedrv 80026919h,800123B0h,800123C4h,8003BCE0h
	patcherfinish

patcher_5713:
patcher_5838:	
	patcherinit
	cdromimagedrv 80026919h,800123B0h,800123C4h,8003BD00h
	patcherfinish



;;; 
;;; END OF CODE
;;; 
%include "footer.asm"
