TARGETNAME=ndasbus
TARGETTYPE=DRIVER
TARGETPATH=obj

MSC_WARNING_LEVEL=-W3 -WX -Wp64

NTTARGETFILE0=mofcomp
NTTARGETFILES=cdfcopy

#
# Set INI File Version
#
INFFILENAME=$(TARGETNAME)

!IF "$(_BUILDARCH)" == "AMD64"
INFSRCNAME=$(INFFILENAME).amd64
!ELSE
INFSRCNAME=$(INFFILENAME)
!ENDIF
CDFSRCFILE=$(TARGETNAME).cdf
CDFFILE=$(O)\$(TARGETNAME).cdf

POST_BUILD_CMD=$(NDAS_TOOLS)\setinfver $(O)\$(TARGETNAME).sys $(INFSRCNAME).inf $(O)\$(INFFILENAME).inf
BINPLACE_FLAGS=$(O)\$(INFFILENAME).inf $(CDFFILE)

C_DEFINES=/DUNICODE /D_UNICODE
INCLUDES=$(NDAS_DRIVER_INC_PATH);$(NDAS_DRIVER_INC_PATH)\kernel;$(NDAS_INC_PATH)
RCOPTIONS=/i $(NDAS_INC_PATH)

BUILD_CONSUMES=hash lpxtdi ndasklib scrc32

TARGETLIBS=$(TARGETLIBS) \
	$(DDK_LIB_PATH)\ntstrsafe.lib \
	$(DDK_LIB_PATH)\tdi.lib \
	$(NDAS_LIB_PATH)\scrc32.lib \
	$(NDAS_LIB_PATH)\hash.lib \
	$(NDAS_DRIVER_LIB_PATH)\lpxtdi.lib \
	$(NDAS_DRIVER_LIB_PATH)\ndask.lib

SOURCES= busenum.rc \
	busenum.c \
	pnp.c \
	power.c \
	buspdo.c \
	wmi.c \
	ndasbus.c \
	register.c \
	utils.c
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 #include <ntddk.h>
#include <scsi.h>
#include <ntddscsi.h>
#define NTSTRSAFE_LIB
#include <ntstrsafe.h>

#include "ndasbus.h"
#include "ndasbusioctl.h"
#include "ndasscsiioctl.h"
#include "ndas/ndasdib.h"
#include "lsutils.h"
#include "lurdesc.h"
#include "binparams.h"
#include "lslurn.h"

#include "busenum.h"
#include "stdio.h"
#include "hdreg.h"
#include "ndasbuspriv.h"



#ifdef __MODULE__
#undef __MODULE__
#endif // __MODULE__
#define __MODULE__ "NdasComm"

#define NCOMM_POOLTAG_LSS	'lnSL'

NTSTATUS
NCommGetDIBV1(
	OUT	PNDAS_DIB				DiskInformationBlock,
	IN	PLSSLOGIN_INFO			LoginInfo,
	IN	UINT64					DIBAddress,
	IN	PTA_LSTRANS_ADDRESS		NodeAddress,
	IN	PTA_LSTRANS_ADDRESS		BindingAddress
) {
	PLANSCSI_SESSION	LSS;
	NTSTATUS			status;
	LSTRANS_TYPE		LstransType;
	ULONG				pduFlags;
	BOOLEAN				dma;

	LSS = (PLANSCSI_SESSION)ExAllocatePoolWithTag(NonPagedPool, sizeof(LANSCSI_SESSION), NCOMM_POOLTAG_LSS);
	if(!LSS) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("ExAllocatePoolWithTag() failed.\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}


	//
	//	Connect and log in.
	//

	status = LsuConnectLogin(	LSS,
								NodeAddress,
								BindingAddress,
								LoginInfo,
								&LstransType);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("LsuConnectLogin() failed. NTSTATUS: %08lx.\n", status));
		ExFreePool(LSS);
		return status;
	}

	status = LsuConfigureIdeDisk(LSS, &pduFlags, &dma);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("IdeConfigure() failed. NTSTATUS: %08lx.\n", status));
		return status;
	}
	if(dma == FALSE) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("Disk does not support DMA. DMA required.\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}


	//
	//	Read informaition block
	//

	Bus_KdPrint_Def(BUS_DBG_SS_INFO, ("DIB addr=%I64u\n", DIBAddress));
	status = LsuGetDiskInfoBlockV1(
					LSS,
					DiskInformationBlock,
					DIBAddress,
					pduFlags
				);

	LspLogout(LSS);
	LspDisconnect(LSS);
	ExFreePool(LSS);

	return status;
}

NTSTATUS
NCommGetDIBV2(
	OUT	PNDAS_DIB_V2			DiskInformationBlock,
	IN	PLSSLOGIN_INFO			LoginInfo,
	IN	UINT64					DIBAddress,
	IN	PTA_LSTRANS_ADDRESS		NodeAddress,
	IN	PTA_LSTRANS_ADDRESS		BindingAddress
) {
	PLANSCSI_SESSION	LSS;
	NTSTATUS			status;
	LSTRANS_TYPE		LstransType;
	ULONG				pduFlags;
	BOOLEAN				dma;

	LSS = (PLANSCSI_SESSION)ExAllocatePoolWithTag(NonPagedPool, sizeof(LANSCSI_SESSION), NCOMM_POOLTAG_LSS);
	if(!LSS) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("ExAllocatePoolWithTag() failed.\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}


	//
	//	Connect and log in.
	//

	status = LsuConnectLogin(	LSS,
								NodeAddress,
								BindingAddress,
								LoginInfo,
								&LstransType);
	if(!NT_SUCCESS(status)) {
		ExFreePool(LSS);
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("LsuConnectLogin() failed. NTSTATUS: %08lx.\n", status));
		return status;
	}

	status = LsuConfigureIdeDisk(LSS, &pduFlags, &dma);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("IdeConfigure() failed. NTSTATUS: %08lx.\n", status));
		return status;
	}
	if(dma == FALSE) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("Disk does not support DMA. DMA required.\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}


	//
	//	Read information block
	//

	Bus_KdPrint_Def(BUS_DBG_SS_INFO, ("DIB addr=%I64u\n", DIBAddress));
	status = LsuGetDiskInfoBlockV2(
					LSS,
					DiskInformationBlock,
					DIBAddress,
					pduFlags
				);

	LspLogout(LSS);
	LspDisconnect(LSS);
	ExFreePool(LSS);

	return status;
}


ULONG
LagacyMirrAggrType2SeqNo(
	ULONG DiskType
) {
	switch(DiskType) {
	case NDAS_DIB_DISK_TYPE_MIRROR_MASTER:
		return 0;
	case NDAS_DIB_DISK_TYPE_MIRROR_SLAVE:
		return 1;
	case NDAS_DIB_DISK_TYPE_AGGREGATION_FIRST:
		return 0;
	case NDAS_DIB_DISK_TYPE_AGGREGATION_SECOND:
		return 1;
	case NDAS_DIB_DISK_TYPE_AGGREGATION_THIRD:
		return 2;
	case NDAS_DIB_DISK_TYPE_AGGREGATION_FOURTH:
		return 3;
	default:
		return -1;
	}
}

LagacyMirrAggrType2TargetType(
	ULONG DiskType
) {
	switch(DiskType) {

	case NDAS_DIB_DISK_TYPE_MIRROR_MASTER:
		return NDASSCSI_TYPE_DISK_MIRROR;

	case NDAS_DIB_DISK_TYPE_MIRROR_SLAVE:
		return NDASSCSI_TYPE_DISK_MIRROR;

	case NDAS_DIB_DISK_TYPE_AGGREGATION_FIRST:
		return NDASSCSI_TYPE_DISK_AGGREGATION;

	case NDAS_DIB_DISK_TYPE_AGGREGATION_SECOND:
		return NDASSCSI_TYPE_DISK_AGGREGATION;

	case NDAS_DIB_DISK_TYPE_AGGREGATION_THIRD:
		return NDASSCSI_TYPE_DISK_AGGREGATION;

	case NDAS_DIB_DISK_TYPE_AGGREGATION_FOURTH:
		return NDASSCSI_TYPE_DISK_AGGREGATION;

	default:
		return -1;
	}
}

ULONG
NdasDevType2DIBType(
	ULONG	NdasDevType
){
	switch(NdasDevType) {
	case NDASSCSI_TYPE_DISK_NORMAL:
		return NMT_SINGLE;
	case NDASSCSI_TYPE_DISK_MIRROR:
		return NMT_MIRROR;
	case NDASSCSI_TYPE_DISK_AGGREGATION:
		return NMT_AGGREGATE;
	case NDASSCSI_TYPE_DVD:
		return NMT_CDROM;
	case NDASSCSI_TYPE_VDVD:
		return NMT_VDVD;
	case NDASSCSI_TYPE_MO:
		return NMT_OPMEM;
	case NDASSCSI_TYPE_AOD:
		return NMT_AOD;
	case NDASSCSI_TYPE_DISK_RAID0:
		return NMT_RAID0;
	case NDASSCSI_TYPE_DISK_RAID1R:
		return NMT_RAID1R;
	case NDASSCSI_TYPE_DISK_RAID4R:
		return NMT_RAID4R;
	default:
		return NMT_INVALID;
	}
}




#define BUILD_LOGININFO(LOGININFO_POINTER, UNIT_POINTER) {												\
	(LOGININFO_POINTER)->LoginType				= LOGIN_TYPE_NORMAL;									\
	RtlCopyMemory(&(LOGININFO_POINTER)->UserID, &(UNIT_POINTER)->iUserID, LSPROTO_USERID_LENGTH);		\
	RtlCopyMemory(&(LOGININFO_POINTER)->Password, &(UNIT_POINTER)->iPassword, LSPROTO_PASSWORD_LENGTH);	\
	RtlCopyMemory((PVOID)(LOGININFO_POINTER)->Password_v2, (PVOID)(UNIT_POINTER)->iPassword_v2, LSPROTO_PASSWORD_V2_LENGTH);	\
	(LOGININFO_POINTER)->MaxBlocksPerRequest	= (UNIT_POINTER)->UnitMaxRequestBlocks;					\
	(LOGININFO_POINTER)->LanscsiTargetID		= (UNIT_POINTER)->ucUnitNumber;							\
	(LOGININFO_POINTER)->LanscsiLU				= 0;													\
	(LOGININFO_POINTER)->HWType					= (UNIT_POINTER)->ucHWType;								\
	(LOGININFO_POINTER)->HWVersion				= (UNIT_POINTER)->ucHWVersion;							\
	(LOGININFO_POINTER)->IsEncryptBuffer		= TRUE;													\
	(LOGININFO_POINTER)->BlockInBytes			= BLOCK_SIZE;	}



NTSTATUS
VerifyAddTargetData(
	IN PLANSCSI_ADD_TARGET_DATA	AddTargetData
){
	switch(AddTargetData->ucTargetType) {
	case NDASSCSI_TYPE_DISK_NORMAL:
	case NDASSCSI_TYPE_DVD:
	case NDASSCSI_TYPE_VDVD:
	case NDASSCSI_TYPE_MO: {
		if(AddTargetData->ulNumberOfUnitDiskList != 1) {
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("NORMAL/ODD: Too many Unit devices\n"));
			return STATUS_TOO_MANY_NODES;
		}
		break;
	}
	case NDASSCSI_TYPE_DISK_RAID0:
	case NDASSCSI_TYPE_DISK_RAID1R:
	case NDASSCSI_TYPE_DISK_RAID4R:
	case NDASSCSI_TYPE_DISK_MIRROR:
	case NDASSCSI_TYPE_DISK_AGGREGATION: {
		if(AddTargetData->ulNumberOfUnitDiskList <= 1) {
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("RAID: Too few Unit devices\n"));
			return STATUS_INVALID_PARAMETER;
		}
		break;
	}
	case NDASSCSI_TYPE_AOD: {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("AOD: Not supported\n"));
		return STATUS_NOT_SUPPORTED;
	}
	default:
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("Invalid target type.\n"));
		return STATUS_OBJECT_TYPE_MISMATCH;
	}

	return STATUS_SUCCESS;
}

NTSTATUS
NCommVerifyNdasDevWithDIB(
		IN OUT PLANSCSI_ADD_TARGET_DATA	AddTargetData,
		IN PTA_LSTRANS_ADDRESS		SecondaryAddress
	) {
	NTSTATUS			status;
	LSSLOGIN_INFO		loginInfo;
	PLSBUS_UNITDISK		unit;
	TA_LSTRANS_ADDRESS	targetAddr;
	TA_LSTRANS_ADDRESS	bindingAddr;
	TA_LSTRANS_ADDRESS	bindingAddr2;
	TA_LSTRANS_ADDRESS	boundAddr;
	ULONG				idx_unit;
	ULONG				fault_cnt;
	BOOLEAN				oriDibValid;
	UINT32				crcunit;
	BOOLEAN				exitImm;


	//
	//	check AddTargetData sanity
	//
	status = VerifyAddTargetData(AddTargetData);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("VerifyAddTargetData() failed. STATUS=%08lx\n", status));
		return status;
	}


	//
	//	Start verifying
	//

	fault_cnt = 0;
	oriDibValid = FALSE;
	exitImm = FALSE;
	for(idx_unit = 0; idx_unit < AddTargetData->ulNumberOfUnitDiskList; idx_unit++) {
		Bus_KdPrint_Def(BUS_DBG_SS_INFO, ("== UNIT #%u ==\n", idx_unit));

		unit = AddTargetData->UnitDiskList + idx_unit;
		LSTRANS_COPY_LPXADDRESS(&targetAddr, &unit->Address);
		LSTRANS_COPY_LPXADDRESS(&bindingAddr, &unit->NICAddr);
		if(SecondaryAddress)
			LSTRANS_COPY_LPXADDRESS(&bindingAddr2, &SecondaryAddress->Address[0].Address);

		BUILD_LOGININFO(&loginInfo, unit);

		if(unit->ucHWVersion <= LANSCSIIDE_VERSION_2_0) {
			loginInfo.UserID = CONVERT_TO_ROUSERID(loginInfo.UserID);
		} else if (unit->ucHWVersion == LANSCSIIDE_VERSION_2_5) {
			loginInfo.UserID = MAKE_USER_ID(DEFAULT_USER_NUM, USER_PERMISSION_RO);
		}
		//
		//	Query binding address
		//

		if(SecondaryAddress)
			status = LsuQueryBindingAddress(&boundAddr, &targetAddr, &bindingAddr, &bindingAddr2, TRUE);
		else
			status = LsuQueryBindingAddress(&boundAddr, &targetAddr, &bindingAddr, NULL, TRUE);

		if(!NT_SUCCESS(status)) {
			unit->LurnOptions |= LURNOPTION_MISSING;
			fault_cnt ++;
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("LsuQueryBindingAddress() failed. STATUS=%08lx\n", status));
			continue;
		}


		//
		//	Set the actual binding address.
		//

		LSTRANS_COPY_TO_LPXADDRESS(&unit->NICAddr, &boundAddr);
		RtlCopyMemory(&bindingAddr, &boundAddr, sizeof(TA_LSTRANS_ADDRESS));


		//
		//	Read and verify DIB
		//

		switch(AddTargetData->ucTargetType) {
		case NDASSCSI_TYPE_DISK_NORMAL: {
			union {
				NDAS_DIB	V1;
				NDAS_DIB_V2	V2;
			}	DIB;


			status = NCommGetDIBV2(	&DIB.V2,
									&loginInfo,
									unit->ulPhysicalBlocks-2,
									&targetAddr,
									&bindingAddr);
			if(NT_SUCCESS(status)) {
				//
				//	Disk information block Version 2
				//
				if(DIB.V2.iMediaType != NMT_SINGLE) {
					Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("NORMAL: DIB's disktype mismatch. MediaType:%x\n", DIB.V2.iMediaType));
					status = STATUS_OBJECT_TYPE_MISMATCH;
					break;
				}
				status = STATUS_SUCCESS;
			} else if(status == STATUS_REVISION_MISMATCH) {
				//
				//	Disk information block Version 1
				//
				Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("NORMAL: DIBV2 doesn't exist. try DIBV1. NTSTATUS:%08lx\n", status));
				status = NCommGetDIBV1(	&DIB.V1,
									&loginInfo,
									unit->ulPhysicalBlocks-1,
									&targetAddr,
									&bindingAddr);

				if(status == STATUS_REVISION_MISMATCH) {
					Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("NORMAL: DIBV1 doesn't exist. Take it Single. NTSTATUS:%08lx\n", status));
					status = STATUS_SUCCESS;
					break;
				} else if(!NT_SUCCESS(status)) {
					Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("NORMAL: NCommGetDIBV1() failed. NTSTATUS:%08lx\n", status));
					break;
				}
				if(DIB.V1.DiskType != NDAS_DIB_DISK_TYPE_SINGLE) {
					Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("NORMAL: DIB's disktype mismatch. DiskType:%x\n", DIB.V1.DiskType));
					status = STATUS_OBJECT_TYPE_MISMATCH;
					break;
				}
				status = STATUS_SUCCESS;

			} else {
				Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("NORMAL: NCommGetDIBV2() failed. NTSTATUS:%08lx\n", status));
			}
			break;
		}

		case NDASSCSI_TYPE_DISK_RAID1R:	{
			NDAS_DIB_V2	DIBV2;

			//
			//	Disk information block Version 2
			//

			status = NCommGetDIBV2(	&DIBV2,
									&loginInfo,
									unit->ulPhysicalBlocks-2,
									&targetAddr,
									&bindingAddr);
			if(!NT_SUCCESS(status)) {
				Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("RAID1: NCommGetDIBV2() failed. NTSTATUS:%08lx\n", status));
				exitImm = TRUE;
			}

			if(DIBV2.iMediaType != NdasDevType2DIBType(AddTargetData->ucTargetType)) {
				Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("RAID1: DIBv2's disktype mismatch. MediaType:%x\n", DIBV2.iMediaType));
				status = STATUS_OBJECT_TYPE_MISMATCH;
				exitImm = TRUE;
				break;
			}

			if(DIBV2.nDiskCount != AddTargetData->ulNumberOfUnitDiskList) {
				Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("RAID1: DIBv2's Diskcount mismatch."
					" numberofunitdisklist=%u ndiskcount=%u\n", AddTarget					NDBUS_REG_TARGETID,
						REG_DWORD,
						&tempUlong,
						sizeof(tempUlong)
					);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_TARGETID));
		return	status;
	}
	//	Target type
	tempUlong = AddTargetData->ucTargetType;
	status = DrWriteKeyValue(
						NdasTargetKey,
						NDBUS_REG_TARGETTYPE,
						REG_DWORD,
						&tempUlong,
						sizeof(tempUlong)
						);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_TARGETTYPE));
		return	status;
	}
	// AccessRight
	status = DrWriteKeyValue(
						NdasTargetKey,
						NDBUS_REG_DESIREACC,
						REG_DWORD,
						&AddTargetData->DesiredAccess,
						sizeof(AddTargetData->DesiredAccess)
					);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_DESIREACC));
		return	status;
	}
	// Target blocks
	status = DrWriteKeyValue(
						NdasTargetKey,
						NDBUS_REG_TARGETBLKS,
						REG_BINARY,
						&AddTargetData->ulTargetBlocks,
						sizeof(AddTargetData->ulTargetBlocks)
					);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_TARGETBLKS));
		return	status;
	}
	// Content encryption method
	tempUlong = AddTargetData->CntEcrMethod;
	status = DrWriteKeyValue(
						NdasTargetKey,
						NDBUS_REG_CNTECR,
						REG_DWORD,
						&tempUlong,
						sizeof(tempUlong)
					);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_CNTECR));
		return	status;
	}
	// Content encryption method
	tempUlong = AddTargetData->CntEcrKeyLength;
	status = DrWriteKeyValue(
						NdasTargetKey,
						NDBUS_REG_CNTECRKEYLEN,
						REG_DWORD,
						&tempUlong,
						sizeof(tempUlong)
		);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_CNTECRKEYLEN));
		return	status;
	}
	// Content encryption key
	status = DrWriteKeyValue(
						NdasTargetKey,
						NDBUS_REG_CNTECRKEY,
						REG_BINARY,
						&AddTargetData->CntEcrKey,
						sizeof(AddTargetData->CntEcrKey)
		);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_CNTECRKEY));
		return	status;
	}
	//	LUR flags
	status = DrWriteKeyValue(
		NdasTargetKey,
		NDBUS_REG_LURFLAGS,
		REG_DWORD,
		&AddTargetData->LurFlags,
		sizeof(AddTargetData->LurFlags)
		);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_LURFLAGS));
		return	status;
	}
	//	LUR options
	status = DrWriteKeyValue(
						NdasTargetKey,
						NDBUS_REG_LUROPTIONS,
						REG_DWORD,
						&AddTargetData->LurOptions,
						sizeof(AddTargetData->LurOptions)
					);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_LUROPTIONS));
		return	status;
	}
	//	RAID: sectors per bit
	status = DrWriteKeyValue(
		NdasTargetKey,
		NDBUS_REG_RAID_SPB,
		REG_DWORD,
		&AddTargetData->RAID_Info.SectorsPerBit,
		sizeof(AddTargetData->RAID_Info.SectorsPerBit)
		);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_RAID_SPB));
		return	status;
	}

	//	Spare disk count
	status = DrWriteKeyValue(
		NdasTargetKey,
		NDBUS_REG_RAID_SPARE,
		REG_DWORD,
		&AddTargetData->RAID_Info.nSpareDisk,
		sizeof(AddTargetData->RAID_Info.nSpareDisk)
		);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_RAID_SPARE));
		return	status;
	}	


	//	NDAS unit count
	status = DrWriteKeyValue(
						NdasTargetKey,
						NDBUS_REG_UNITCNT,
						REG_DWORD,
						&AddTargetData->ulNumberOfUnitDiskList,
						sizeof(AddTargetData->ulNumberOfUnitDiskList)
					);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_UNITCNT));
		return	status;
	}

	//
	//	Write NDAS units
	//
	nameBuffer = (PWSTR)ExAllocatePoolWithTag(PagedPool, 512, NDBUSREG_POOLTAG_NAMEBUFF);
	if(!nameBuffer) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("ExAllocatePoolWithTag() failed.\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	objectName.Length = 0;
	objectName.MaximumLength = 512;
	objectName.Buffer = nameBuffer;

	for(idxKey = 0 ; idxKey < AddTargetData->ulNumberOfUnitDiskList; idxKey ++) {
		unit = AddTargetData->UnitDiskList + idxKey;

		status = Reg_OpenUnit(	&unitKey,
								idxKey,
								TRUE,
								NdasTargetKey);
		if(!NT_SUCCESS(status)) {
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("Reg_OpenUnit() failed. NTSTATUS:%08lx\n", status));
			break;
		}

		status = WriteUnitToRegistry(unitKey, unit);

		ZwClose(unitKey);

		if(!NT_SUCCESS(status)) {
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("WriteUnitToRegistry() failed. NTSTATUS:%08lx\n", status));
			break;
		}

	}

	ExFreePool(nameBuffer);

	return status;
}


//
//	Write an NDAS device to the registry.
//	NOTE: It does not write any target information.
//

NTSTATUS
WriteNDASDevToRegistry(
	HANDLE							NdasDevInst,
	PBUSENUM_PLUGIN_HARDWARE_EX2	Plugin
){

	NTSTATUS					status;


	//	Slot number
	status = DrWriteKeyValue(
					NdasDevInst,
					NDBUS_REG_SLOTNO,
					REG_DWORD,
					&Plugin->SlotNo,
					sizeof(Plugin->SlotNo)
				);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_SLOTNO));
		goto cleanup;
	}

	//	Max request blocks
	status = DrWriteKeyValue(
					NdasDevInst,
					NDBUS_REG_DEVMAXBPR,
					REG_DWORD,
					&Plugin->MaxRequestBlocks,
					sizeof(Plugin->MaxRequestBlocks)
					);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_DEVMAXBPR));
		goto cleanup;
	}

	//	Hardware ID length
	status = DrWriteKeyValue(
					NdasDevInst,
					NDBUS_REG_HWIDLEN,
					REG_DWORD,
					&Plugin->HardwareIDLen,
					sizeof(Plugin->HardwareIDLen)
					);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_HWIDLEN));
		goto cleanup;
	}

	//	HardwareIDs
	status = DrWriteKeyValue(
					NdasDevInst,
					NDBUS_REG_HWIDS,
					REG_MULTI_SZ,
					&Plugin->HardwareIDs,
					Plugin->HardwareIDLen * sizeof(WCHAR)
		);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_HWIDS));
		goto cleanup;
	}

	//	Flags
	status = DrWriteKeyValue(
					NdasDevInst,
					NDBUS_REG_FLAGS,
					REG_DWORD,
					&Plugin->Flags,
					sizeof(Plugin->Flags)
				);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrWriteKeyValue() failed. ValueKeyName:%ws\n", NDBUS_REG_FLAGS));
		goto cleanup;
	}


cleanup:
	return status;
}


//
//	Allocate AddTargetData and read the registry.
//

NTSTATUS
ReadTargetInstantly(
	HANDLE						NdasDevInst,
	ULONG						TargetId,
	PLANSCSI_ADD_TARGET_DATA	*AddTargetData
){
	NTSTATUS					status;
	HANDLE						targetKey;
	ULONG						outLength;
	PLANSCSI_ADD_TARGET_DATA	addTargetData;

	do {

		status =Reg_OpenTarget(&targetKey, TargetId, FALSE, NdasDevInst);
		if(!NT_SUCCESS(status)) {
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("Reg_OpenTarget() failed. NTSTATUS:%08lx\n", status));
			break;
		}

		status = ReadTargetAndUnitFromRegistry(targetKey, 0, NULL, &outLength);
		if(status != STATUS_BUFFER_TOO_SMALL) {
			ZwClose(targetKey);
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("ReadTargetAndUnitFromRegistry() failed. NTSTATUS:%08lx\n", status));
			break;
		}
		addTargetData = ExAllocatePoolWithTag(PagedPool, outLength, NDBUSREG_POOLTAG_ADDTARGET);
		if(addTargetData == NULL) {
			ZwClose(targetKey);
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("ExAllocatePoolWithTag() failed. NTSTATUS:%08lx\n", status));
			break;
		}
		status = ReadTargetAndUnitFromRegistry(targetKey, outLength, addTargetData,  &outLength);
		if(!NT_SUCCESS(status)) {
			ExFreePool(addTargetData);
			ZwClose(targetKey);
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("ReadTargetAndUnitFromRegistry() failed. NTSTATUS:%08lx\n", status));
			break;
		}


		//
		//	Set return values
		//

		*AddTargetData = addTargetData;

	} while(FALSE);

	return status;
}

//
//	Allocate AddTargetData and read the registry.
//

NTSTATUS
DeleteTarget(
	HANDLE						NdasDevInst,
	ULONG						TargetId
){
	NTSTATUS					status;
	HANDLE						targetKey;

	status =Reg_OpenTarget(&targetKey, TargetId, TRUE, NdasDevInst);
	if(!NT_SUCCESS(status)) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("Reg_OpenTarget() for deletion failed. NTSTATUS:%08lx\n", status));
	} else do {

		status = DrDeleteAllSubKeys(targetKey);
		if(!NT_SUCCESS(status)) {
			ZwClose(targetKey);
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("DrDeleteAllSubKeys() for subkey deletion failed. NTSTATUS:%08lx\n", status));
			break;
		}

		status = ZwDeleteKey(targetKey);
#if DBG
		if(!NT_SUCCESS(status)) {
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("ZwDeleteKey() for target deletion failed. NTSTATUS:%08lx\n", status));
		}
#endif

		ZwClose(targetKey);
	} while(0);

	return status;
}

NTSTATUS
RewriteTargetInstantly(
	HANDLE						NdasDevInst,
	ULONG						TargetId,
	PLANSCSI_ADD_TARGET_DATA	AddTargetData
){
	NTSTATUS status;
	HANDLE	targetKey;

	do {

		//
		//	Clean the target registry to rewrite AddTargetData
		//
		status = Reg_OpenTarget(&targetKey, TargetId, FALSE, NdasDevInst);
		if(!NT_SUCCESS(status)) {
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("Reg_OpenTarget() failed. NTSTATUS:%08lx\n", status));
			break;
		}
		DrDeleteAllSubKeys(targetKey);


		//
		//	Rewrite AddTargetData
		//
		status = WriteTargetToRegistry(targetKey, AddTargetData);
		if(!NT_SUCCESS(status)) {
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("WriteTargetToRegistry() failed. NTSTATUS:%08lx\n", status));
			break;
		}

		ZwClose(targetKey);
	} while(FALSE);

	return status;
}


//////////////////////////////////////////////////////////////////////////
//
//	Clean up NDAS devices' registry keys
//


//
//	Clean up NDAS devices by reading registry.
//

static
NTSTATUS
CleanupNDASEnumReg(
		PFDO_DEVICE_DATA	FdoData,
		HANDLE				NdasDeviceReg
	) {

	NTSTATUS					status;
	PKEY_BASIC_INFORMATION		keyInfo;
	ULONG						outLength;
	LONG						idxKey;
	OBJECT_ATTRIBUTES			objectAttributes;
	UNICODE_STRING				objectName;
	HANDLE						ndasDevInst;
	PPDO_DEVICE_DATA			pdoData;
	PBUSENUM_PLUGIN_HARDWARE_EX2	plugIn;
	PLANSCSI_ADD_TARGET_DATA		addTargetData;


	keyInfo = (PKEY_BASIC_INFORMATION)ExAllocatePoolWithTag(PagedPool, 512, NDBUSREG_POOLTAG_KEYINFO);
	if(!keyInfo) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("ExAllocatePoolWithTag(KEY_BASIC_INFORMATION) failed.\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	status = STATUS_SUCCESS;
	for(idxKey = 0 ; idxKey < MAX_DEVICES_IN_NDAS_REGISTRY; idxKey ++) {

		//
		//	Enumerate subkeys under the NDAS device root
		//

		status = ZwEnumerateKey(
						NdasDeviceReg,
						idxKey,
						KeyBasicInformation,
						keyInfo,
						512,
						&outLength
						);

		if(status == STATUS_NO_MORE_ENTRIES) {
			status = STATUS_SUCCESS;
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("No more entry\n"));
			break;
		}
		if(status != STATUS_SUCCESS) {
			ASSERT(status != STATUS_BUFFER_OVERFLOW && status != STATUS_BUFFER_TOO_SMALL);
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("ZwEnumerateKey() failed. NTSTATUS:%08lx\n", status));
			ExFreePool(keyInfo);
			return STATUS_SUCCESS;
		}

		//
		//	Name verification
		//
		//	TODO
		//


		//
		//	Open a sub key (NdasDevices\Devxx) and plug in device with the registry key.
		//

		objectName.Length = objectName.MaximumLength = (USHORT)keyInfo->NameLength;
		objectName.Buffer = keyInfo->Name;
		InitializeObjectAttributes(		&objectAttributes,
										&objectName,
										OBJ_KERNEL_HANDLE,
										NdasDeviceReg,
										NULL
								);
		status = ZwOpenKey(&ndasDevInst, KEY_READ, &objectAttributes);
		if(!NT_SUCCESS(status)) {
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("ZwOpenKey() failed. NTSTATUS:%08lx\n", status));
			continue;
		}

		Bus_KdPrint_Def(BUS_DBG_SS_INFO, ("'%wZ' opened.\n", &objectName));

		//
		//	Read NDAS device instance.
		//

		status = ReadNDASDevInstFromRegistry(ndasDevInst, 0, NULL, &outLength);
		if(status != STATUS_BUFFER_TOO_SMALL) {
			ZwClose(ndasDevInst);
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("ReadNDASDevInstFromRegistry() failed. NTSTATUS:%08lx\n", status));
			continue;
		}
		plugIn = ExAllocatePoolWithTag(PagedPool, outLength, NDBUSREG_POOLTAG_PLUGIN);
		if(plugIn == NULL) {
			ZwClose(ndasDevInst);
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("ExAllocatePoolWithTag() failed. NTSTATUS:%08lx\n", status));
			continue;
		}
		status = ReadNDASDevInstFromRegistry(ndasDevInst, outLength, plugIn, &outLength);
		if(!NT_SUCCESS(status)) {
			ExFreePool(plugIn);
			ZwClose(ndasDevInst);
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("ReadNDASDevInstFromRegistry() failed. NTSTATUS:%08lx\n", status));
			continue;
		}


		//
		//	Allocate AddTarget Data and read target and unit devices.
		//

		status = ReadTargetInstantly(ndasDevInst, 0, &addTargetData);
		if(!NT_SUCCESS(status)) {
			ExFreePool(plugIn);
			ZwClose(ndasDevInst);
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("Reg_OpenTarget() failed. NTSTATUS:%08lx\n", status));
			continue;
		}
		addTargetData->ulSlotNo = plugIn->SlotNo;


		//
		//	Check to see if the NDAS device already plug-ined
		//

		pdoData = LookupPdoData(FdoData, plugIn->SlotNo);
		if(pdoData) {
			ExFreePool(addTargetData);
			ExFreePool(plugIn);
			ZwClose(ndasDevInst);
			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("PDO already exists\n"));
			continue;
		}


		//
		//	Delete AddTargetData structure with DIBs in NDAS devices.
		// Do not delete the NDAS device entry itself because
		// We are going to adapt one-adapter and multiple targets in the future version.
		//

		DeleteTarget(ndasDevInst,0);


		//
		//	Close handle here
		//

		ZwClose(ndasDevInst);


		//
		//	Free resources
		//

		ExFreePool(addTargetData);
		ExFreePool(plugIn);
	}

	ExFreePool(keyInfo);

	Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("Enumerating NDAS devices from registry is completed. NTSTATUS:%08lx\n", status));

	return STATUS_SUCCESS;
}


//
//	Clean up NDAS devices' registry
//	Acquire FDO's mutext before calling
//

NTSTATUS
LSBus_CleanupNDASDeviceRegistryUnsafe(
		PFDO_DEVICE_DATA	FdoData
) {
	NTSTATUS			status;
	HANDLE				DeviceReg;
	HANDLE				NdasDeviceReg;

	ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);


	DeviceReg = NULL;
	NdasDeviceReg = NULL;

	//
	//	Parameter check
	//
	if(!FdoData) {
		Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("FdoData NULL!\n"));
		return STATUS_INVALID_PARAMETER;
	}

	//
	//	Open the bus registry and NDAS device root.
	//

	do {

		status = Reg_OpenDeviceControlRoot(&DeviceReg, KEY_READ|KEY_WRITE);
		if(!NT_SUCCESS(status)) {

			Bus_KdPrint_Def(BUS_DBG_SS_ERROR, ("OpenServiceRegistry() failed.\n"));
			break;
		}
		status = Reg_OpenNdasDeviceRoot(&NdasDeviceReg, KEY